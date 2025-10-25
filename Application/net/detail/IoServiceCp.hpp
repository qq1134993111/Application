#pragma once
#include "IoServicePool.hpp"

namespace net::detail
{

template <class derived_t, class args_t> class IoServiceCp
{
  public:
    IoServiceCp(net::io_context&ios) : ios_(ios)
    {
    }

  protected:
    net::io_context& GetIoService()
    {
        return ios_;
    }

  protected:
    net::io_context&ios_;
};

} // namespace net::detail