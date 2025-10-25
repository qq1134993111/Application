#pragma once
#include "boost/asio.hpp"

namespace net::detail
{

    template <class derived_t, class args_t>
    class ConnectCp
    {
    public:
        ConnectCp() = default;
        ~ConnectCp() = default;

    protected:
        // 连接单个 endpoint
        bool DoConnect(boost::asio::ip::tcp::endpoint endpoint)
        {
            auto& derived = static_cast<derived_t&>(*this);
            connect_endpoint_ = endpoint;
            derived.Socket().async_connect(
                endpoint,
                [this, self = derived.SelfPtr()](const boost::system::error_code& ec) {
                    HandleConnect(ec);
                });
            return true;
        }

        // 连接 resolver::results_type（多个候选地址）
        bool DoConnect(boost::asio::ip::tcp::resolver::results_type results)
        {
            auto& derived = static_cast<derived_t&>(*this);
            results_ = std::move(results);
            current_it_ = results_.begin();
            TryNextEndpoint();
            return true;
        }

        boost::asio::ip::tcp::endpoint GetConnectEndpoint() const
        {
            return connect_endpoint_;
        }

    private:
        void TryNextEndpoint()
        {
            auto& derived = static_cast<derived_t&>(*this);

            if (current_it_ == results_.end())
            {
                // 所有地址都失败
                derived._DoneConnectionFailure(
                    boost::asio::error::host_unreachable);
                return;
            }

            connect_endpoint_ = current_it_->endpoint();
            derived.Socket().async_connect(
                connect_endpoint_,
                [this, self = derived.SelfPtr()](const boost::system::error_code& ec) {
                    if (!ec)
                    {
                        HandleConnect(ec);
                    }
                    else
                    {
                        // 尝试下一个地址
                        ++current_it_;
                        TryNextEndpoint();
                    }
                });
        }

        void HandleConnect(const boost::system::error_code& ec)
        {
            auto& derived = static_cast<derived_t&>(*this);
            if (!ec)
            {
                derived._DoneConnectionSuccessful();
            }
            else
            {
                derived._DoneConnectionFailure(ec);
            }
        }

    private:
        boost::asio::ip::tcp::endpoint connect_endpoint_;
        boost::asio::ip::tcp::resolver::results_type results_;
        boost::asio::ip::tcp::resolver::results_type::iterator current_it_;
    };

} // namespace net::detail
