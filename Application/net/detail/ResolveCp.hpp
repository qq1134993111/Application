#pragma once
#include <boost/asio.hpp>
#include <string>

namespace net::detail
{

    template <class derived_t, class args_t>
    class ResolveCp
    {
    public:
        ResolveCp(boost::asio::io_context& io)
            : resolver_(io)
        {
        }

        ~ResolveCp() = default;

    protected:
        // 异步解析 host:port  "www.example.com"  "http"
        bool DoResolve(const std::string& host, const std::string& service)
        {
            auto& derived = static_cast<derived_t&>(*this);

            resolver_.async_resolve(
                host, service,
                [this, self = derived.SelfPtr()](const boost::system::error_code& ec,
                    boost::asio::ip::tcp::resolver::results_type results)
                {
                    HandleResolve(ec, results);
                });

            return true;
        }

    private:
        void HandleResolve(const boost::system::error_code& ec,
            boost::asio::ip::tcp::resolver::results_type results)
        {
            auto& derived = static_cast<derived_t&>(*this);
            if (!ec)
            {
                // 解析成功，交给派生类处理
                derived._DoneResolveSuccessful(results);
            }
            else
            {
                // 解析失败
                derived._DoneResolveFailure(ec);
            }
        }

    protected:
        boost::asio::ip::tcp::resolver resolver_;
    };

} // namespace net::detail
