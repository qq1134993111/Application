#pragma once

namespace net::detail
{
using buffer_iterator =  net::buffers_iterator<net::streambuf::const_buffers_type>;
using MatchRole = std::function<std::pair<buffer_iterator, bool>(buffer_iterator begin, buffer_iterator end)>;

template <class derived_t, class args_t> class TcpRecvOp
{
  public:
    TcpRecvOp()
    {
    }
    ~TcpRecvOp()
    {
    }

    void SetMatchRole(const MatchRole &role)
    {
        match_role_ = role;
    }

  protected:
    void DoTcpAsyncRead(std::shared_ptr<derived_t> this_ptr)
    {
        derived_t &derived = static_cast<derived_t &>(*this);

        if (match_role_)
        {

            net::async_read_until(derived.Socket(), derived.Buffer(), match_role_,
                                          [this, this_ptr = derived.SelfPtr()](net::error_code ec,
                                                                               std::size_t bytes_transferred) mutable {
                                              this->HandleTcpRecv(ec, bytes_transferred, std::move(this_ptr));
                                          });
        }
        else
        {
            net::async_read(derived.Socket(), derived.Buffer(), net::transfer_at_least(1),
                                    [this, this_ptr = std::move(this_ptr)](boost::system::error_code ec,
                                                                           std::size_t bytes_transferred) mutable {
                                        this->HandleTcpRecv(ec, bytes_transferred, std::move(this_ptr));
                                    });
        }
    }

  private:
    void HandleTcpRecv(const net::error_code &ec, std::size_t bytes_transferred,
                       std::shared_ptr<derived_t> this_ptr)
    {
        derived_t &derived = static_cast<derived_t &>(*this);
        if (!ec && derived.IsConnected())
        {
            std::string_view data(reinterpret_cast<std::string_view::const_pointer>(derived.Buffer().data().data()),
                                  derived.Buffer().size());

            derived.Listener().Notify(EventType::recv, data);

            derived.Buffer().consume(bytes_transferred);

            DoTcpAsyncRead(this_ptr);
            derived.DoExpiresRecvTimer(std::move(this_ptr));
        }
        else
        {
            derived._DoneDisConnect(ec);
        }
    }

    MatchRole match_role_;
};

} // namespace net::detail

namespace net
{
using MatchRole = detail::MatchRole;
}