#pragma once
#include "IoServicePool.hpp"

namespace net::detail
{

template <class derived_t, class args_t> class IoServiceCp
{
  public:
    IoServiceCp(boost::asio::io_service &ios) : ios_(ios)
    {
    }

  protected:
    boost::asio::io_service &GetIoService()
    {
        return ios_;
    }

  protected:
    boost::asio::io_service &ios_;
};

} // namespace net::detail