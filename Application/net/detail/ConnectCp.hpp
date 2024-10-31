#pragma once
#include "boost/asio.hpp"

namespace net::detail
{
template <class derived_t, class args_t> class ConnectCp
{
  public:
    ConnectCp()
    {
    }
    ~ConnectCp()
    {
    }

  protected:
    bool DoConnect(boost::asio::ip::tcp::endpoint endpoint)
    {
        auto &derived = static_cast<derived_t &>(*this);
        connect_endpoint_ = endpoint;
        derived.Socket().async_connect(
            endpoint, [this, this_ptr = derived.SelfPtr()](const boost::system::error_code &ec) { HandleConnect(ec); });

        return true;
    }

    boost::asio::ip::tcp::endpoint GetConnectEndpoint()
    {
        return connect_endpoint_;
    }
  private:
    void HandleConnect(boost::system::error_code ec)
    {
        auto &derived = static_cast<derived_t &>(*this);
        if (!ec)
        {
            derived._DoneConnectionSuccessful();
        }
        else
        {
            derived._DoneConnectionFailure(ec);
        }
    }

    boost::asio::ip::tcp::endpoint connect_endpoint_;
};

} // namespace net::detail