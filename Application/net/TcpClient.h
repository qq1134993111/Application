#pragma once
#include "detail/ClientImpl.h"

namespace net
{

class TcpClient : public net::detail::ClientImpl<TcpClient>
{
  public:
    using net::detail::ClientImpl<TcpClient>::ClientImpl;
};

} // namespace net