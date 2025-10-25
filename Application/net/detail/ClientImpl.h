#pragma once
#include "CloseCp.hpp"
#include "ConnectCp.hpp"
#include "ConnectTimeoutCp.hpp"
#include "HeartbeatTimerCp.hpp"
#include "IoServiceCp.hpp"
#include "Listener.hpp"
#include "NetCommon.hpp"
#include "Object.hpp"
#include "ReconnectTimerCp.hpp"
#include "RecvTimerCp.hpp"
#include "ShutDownCp.hpp"
#include "SocketCp.hpp"
#include "TcpRecvOp.hpp"
#include "TcpSendOp.hpp"
#include "ThisThreadIdCp.hpp"
#include "UserDataCp.hpp"

//#include "boost/asio.hpp"

namespace net::detail
{

struct template_args_tcp_client
{
    // static constexpr bool is_session = false;
    // static constexpr bool is_client = true;
    // static constexpr bool is_server = false;

    using socket_t = net::ip::tcp::socket;
    using buffer_t = net::streambuf;
    // using send_data_t = std::string_view;
    // using recv_data_t = std::string_view;
};

template <class derived_t, class args_t = template_args_tcp_client>
class ClientImpl : public Object_t<derived_t>,
                   public IoServiceCp<derived_t, args_t>,
                   public ThisThreadIdCp<derived_t, args_t>,
                   public SocketCp<derived_t, args_t>,
                   public ConnectCp<derived_t, args_t>,
                   public ConnectTimeoutCp<derived_t, args_t>,
                   public ReconnectTimerCp<derived_t, args_t>,
                   public HeartbeatTimerCp<derived_t, args_t>,
                   public RecvTimerCp<derived_t, args_t>,
                   public TcpRecvOp<derived_t, args_t>,
                   public TcpSendOp<derived_t, args_t>,
                   public ShutDownCp<derived_t, args_t>,
                   public CloseCp<derived_t, args_t>,
                   public UserDataCp<derived_t, args_t>
{
    friend class ConnectCp<derived_t, args_t>;
    friend class IoServiceCp<derived_t, args_t>;
    friend class SocketCp<derived_t, args_t>;
    friend class TcpRecvOp<derived_t, args_t>;
    friend class TcpSendOp<derived_t, args_t>;
    friend class ConnectTimeoutCp<derived_t, args_t>;
    friend class HeartbeatTimerCp<derived_t, args_t>;
    friend class ReconnectTimerCp<derived_t, args_t>;
    friend class RecvTimerCp<derived_t, args_t>;

  public:
    using super = Object_t<derived_t>;
    using self = ClientImpl<derived_t, args_t>;

    using args_type = args_t;
    using key_type = std::size_t;
    using buffer_type = typename args_t::buffer_t;

    ClientImpl(net::io_context &ios)
        : Object_t<derived_t>(), IoServiceCp<derived_t, args_t>(ios),
          SocketCp<derived_t, args_t>(IoServiceCp<derived_t, args_t>::GetIoService()), ConnectCp<derived_t, args_t>(),
          ConnectTimeoutCp<derived_t, args_t>(), ReconnectTimerCp<derived_t, args_t>(),
          HeartbeatTimerCp<derived_t, args_t>(), RecvTimerCp<derived_t, args_t>(), TcpRecvOp<derived_t, args_t>(),
          TcpSendOp<derived_t, args_t>(), ShutDownCp<derived_t, args_t>(), CloseCp<derived_t, args_t>(),
          UserDataCp<derived_t, args_t>(), listener_(), buffer_()
    {
    }

    ~ClientImpl()
    {
    }

  public:
    bool Connect(const std::string &ip, unsigned short port)
    {
        boost::system::error_code ec;
        auto ip_address = net::make_address_compat(ip, ec);
        if (ec)
        {
            return false;
        }

        net::ip::tcp::endpoint connect_endpoint(ip_address, port);

        auto &derived = this->Derived();
        State_t expected = State_t::stopped;
        if (!derived.state_.compare_exchange_strong(expected, State_t::connecting))
        {
            return false;
        }

        auto self_ptr = this->Derived().SelfPtr();
        auto &ios = derived.GetIoService();
        net::post(ios,[this, self_ptr]() {
            this->DoInitThisThreadId();
            this->stopped_ = false;
            this->Listener().Notify(EventType::init);
        });

        net::post(ios,[this, self_ptr, connect_endpoint = std::move(connect_endpoint)]() {
            State_t expected = State_t::connecting;
            if (this->state_.compare_exchange_strong(expected, State_t::connecting))
            {
                this->Listener().Notify(EventType::start);
                this->DoConnect(connect_endpoint);
                this->DoExpiresConnectTimeoutTimer(std::move(self_ptr));
            }
            else
            {
                BOOST_ASSERT(this->state_ == State_t::stopping);
                // this->DoShutdown();
                // this->Listener().Notify(EventType::stop);
                // this->DoClose();
                // this->state_ = State_t::stopped;
                // this->stopped_ = true;
            }
        });

        return true;
    }

    inline bool IsConnected() const
    {
        return (this->state_ == State_t::connected && this->Socket().is_open());
    }

    inline bool IsClosed() const
    {
        return (this->state_ == State_t::stopped && !this->Socket().is_open() && this->stopped_);
    }

    inline void Close(bool only_close=false)
    {
        if (this->state_ == State_t::stopping || this->state_ == State_t::stopped)
            return;

        auto self_ptr = this->Derived().SelfPtr();

        if (only_close)
        {
            net::post(this->Derived().GetService(),[this, self_ptr]() mutable {
                if (this->state_ == State_t::stopping || this->state_ == State_t::stopped)
                    return;
                this->DoShutdown(); 
                });
            return;
        }

        net::post(this->Derived().GetService(),[this, self_ptr]() mutable {
            if (this->state_ == State_t::stopping || this->state_ == State_t::stopped)
                return;

            if (this->state_ == State_t::connecting)
            {
                this->DoShutdown(); 
                this->state_ = State_t::stopping;
            }
            else if (this->state_ == State_t::connected)
            {
                this->DoShutdown();
                this->state_ = State_t::stopping;
            }
            else if (this->state_ == State_t::disconnect)
            {
                this->DoCancelReconnectTimer();
                this->DoShutdown();
                this->Listener().Notify(EventType::stop);
                this->DoClose();
                this->state_ = State_t::stopped;
                this->stopped_ = true;
                return;
            }
        });
    }

  public:
    template <class F, class... C> inline derived_t &BindRecv(F &&fun, C &&...obj)
    {
        this->listener_.Bind(EventType::recv,
                             Observer_t<std::string_view>(std::forward<F>(fun), std::forward<C>(obj)...));
        return (this->Derived());
    }

    template <class F, class... C> inline derived_t &BindConnect(F &&fun, C &&...obj)
    {
        this->listener_.Bind(EventType::connect, Observer_t<>(std::forward<F>(fun), std::forward<C>(obj)...));
        return (this->Derived());
    }
    template <class F, class... C> inline derived_t &BindConnectFailed(F &&fun, C &&...obj)
    {
        this->listener_.Bind(EventType::connect_failed, Observer_t<>(std::forward<F>(fun), std::forward<C>(obj)...));
        return (this->Derived());
    }

    template <class F, class... C> inline derived_t &BindDisconnect(F &&fun, C &&...obj)
    {
        this->listener_.Bind(EventType::disconnect,
                             Observer_t<boost::system::error_code & >(std::forward<F>(fun), std::forward<C>(obj)...));
        return (this->Derived());
    }

    template <class F, class... C> inline derived_t &BindInit(F &&fun, C &&...obj)
    {
        this->listener_.Bind(EventType::init, Observer_t<>(std::forward<F>(fun), std::forward<C>(obj)...));
        return (this->Derived());
    }

  protected:
    bool _DoneConnectionSuccessful()
    {
        try
        {
            auto &derived = this->Derived();
            State_t expected = State_t::connecting;
            if (!derived.state_.compare_exchange_strong(expected, State_t::connected))
            {
                BOOST_ASSERT(derived.state_ == State_t::stopping);

                this->Listener().Notify(EventType::stop);
                this->DoClose();
                derived.state_ = State_t::stopped;
                derived.stopped_ = true;
                return false;
            }

            if (!this->Socket().is_open())
            {
                BOOST_ASSERT(false);
                return false;
            }

            this->DoCancelConnectTimeoutTimer();

            // clear recv buffer
            this->Buffer().consume(this->Buffer().size());
            this->ClearSendQueue();

            this->SetNoDelay(true);

            this->Listener().Notify(EventType::connect);

            auto self_ptr = this->Derived().SelfPtr();
            this->DoTcpAsyncRead(self_ptr);
            this->DoExpiresRecvTimer(self_ptr);
            this->DoExpiresHeartbeatTimer(self_ptr);
        }
        catch (std::exception &e)
        {
            printf("%s,%d,%s\n", __FUNCTION__, __LINE__, e.what());

            return false;
        }

        return true;
    }

    void _DoneConnectionFailure(boost::system::error_code ec)
    {
        auto &derived = this->Derived();
        State_t expected = State_t::connecting;
        if (!derived.state_.compare_exchange_strong(expected, State_t::disconnect))
        {
            BOOST_ASSERT(derived.state_ == State_t::stopping);
            this->Listener().Notify(EventType::stop);
            this->DoClose();
            derived.state_ = State_t::stopped;
            derived.stopped_ = true;
            return;
        }

        if (this->IsConnectTimeoutTimerFinished())
        {
            ec = net::error::timed_out;
        }

        this->DoCancelConnectTimeoutTimer();

        this->DoShutdown();
        this->Listener().Notify(EventType::connect_failed, ec);
        this->DoClose();

        if (this->GetReconnectTimeout())
        {
            this->DoExpiresReconnectTimer(this->Derived().SelfPtr());
        }
        else
        {
            this->Listener().Notify(EventType::stop);
            derived.state_ = State_t::stopped;
            derived.stopped_ = true;
        }
    }

    void _DoneConnectionTimeout()
    {
        this->DoShutdown();
        this->DoClose();
    }

    void _DoneReconnectTimerTimeout()
    {
        if (this->state_ != State_t::disconnect)
        {
            BOOST_ASSERT(this->state_ == State_t::stopping);
        }
        this->state_ = State_t::connecting;
      
        net::post(this->Derived().GetIoService(),[this, self_ptr = this->Derived().SelfPtr()]() mutable {
            auto ep = this->GetConnectEndpoint();
            this->DoConnect(ep);
        });
    }

    void _DoneRecvTimerTimeout()
    {
        _DoneDisConnect(net::error::timed_out);
    }

    void _DoneDisConnect(net::error_code ec)
    {
        auto &derived = this->Derived();

        State_t expected = State_t::connected;
        if (derived.state_.compare_exchange_strong(expected, State_t::disconnect))
        {
            this->DoCancelReconnectTimer();
            this->DoCancelConnectTimeoutTimer();
            this->DoCancelHeartbeatTimer();
            this->DoCancelRecvTimer();

            this->DoShutdown();
            this->Listener().Notify(EventType::disconnect, ec);
            this->DoClose();

            if (this->GetReconnectTimeout())
            {
                this->DoExpiresReconnectTimer(this->Derived().SelfPtr());
            }
            else
            {
                this->Listener().Notify(EventType::stop);
                derived.state_ = State_t::stopped;
                derived.stopped_ = true;
            }
        }
        else
        {
            if (derived.state_ == State_t::stopping)
            {
                this->DoCancelReconnectTimer();
                this->DoCancelConnectTimeoutTimer();
                this->DoCancelHeartbeatTimer();
                this->DoCancelRecvTimer();

                this->DoShutdown();
                this->Listener().Notify(EventType::disconnect, ec);

                this->Listener().Notify(EventType::stop);

                this->DoClose();

                derived.state_ = State_t::stopped;
                derived.stopped_ = true;
            }
            else
            {
                BOOST_ASSERT(!derived.Socket().is_open());
            }
        }
    }

  protected:
    buffer_type &Buffer()
    {
        return buffer_;
    }

    inline Listener_t &Listener() noexcept
    {
        return this->listener_;
    }

  private:
    /// listener
    Listener_t listener_;

    buffer_type buffer_;

    std::atomic<State_t> state_ = State_t::stopped;
    bool stopped_ = true;
};

} // namespace net::detail