#pragma once
#include "asio_compat.h"

namespace net::detail
{
template <class derived_t, class args_t> class CloseCp
{
  public:
    CloseCp()
    {
    }
    ~CloseCp()
    {
    }

  protected:
    void DoClose()
    {
        auto &derived = static_cast<derived_t &>(*this);

        net::error_code ignored_ec;
        derived.Socket().close(ignored_ec);
        BOOST_ASSERT(!derived.Socket().is_open());
    }
};

} // namespace net::detail