#pragma once
#include <atomic>
#include <memory>

#include "asio_compat.h"

namespace net::detail
{
template <class derived_t, class args_t> class ConnectTimeoutCp
{
  public:
    ConnectTimeoutCp()
    {
    }
    ~ConnectTimeoutCp()
    {
    }

  public:
    void SetConnectTimeout(uint32_t timeout_seconds)
    {
        connect_timeout_seconds_ = timeout_seconds;
    }

  protected:
    void DoExpiresConnectTimeoutTimer(std::shared_ptr<derived_t> this_ptr)
    {
        connect_timeout_finished_ = false;
        connect_timeout_canceled_ = false;

        if (connect_timeout_seconds_ == 0)
            return;

        // printf("FILE:%s,FUNCTION:%s,LINE:%d,connect_timeout_seconds_:%d\n", __FILE__, __FUNCTION__, __LINE__,
        //        connect_timeout_seconds_.load());

        if (check_connect_timeout_timer_ == nullptr)
        {
            auto &derived = static_cast<derived_t &>(*this);
            check_connect_timeout_timer_ = std::make_shared<net::steady_timer>(derived.GetIoService());
        }

        set_timer_expires_from_now(*check_connect_timeout_timer_, std::chrono::seconds(connect_timeout_seconds_));

        check_connect_timeout_timer_->async_wait(
            [this, this_ptr = std::move(this_ptr)](net::error_code const &ec) {
                if (connect_timeout_canceled_)
                    return;
                HandleConnectTimeoutTimer(ec);
            });
    }

    size_t DoCancelConnectTimeoutTimer()
    {
        connect_timeout_canceled_ = true;

        size_t size = 0;
        if (check_connect_timeout_timer_!=nullptr)
        {
            try
            {
                size = check_connect_timeout_timer_->cancel();
            }
            catch (const net::system_error& e)
            {

            }

            // printf("FILE:%s,FUNCTION:%s,LINE:%d, %d canceled,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, size,
            //        ignored_ec.value(), boost::system::system_error(ignored_ec).what());
        }
        return size;
    }

  protected:
    void HandleConnectTimeoutTimer(net::error_code const &ec)
    {
        // printf("FILE:%s,FUNCTION:%s,LINE:%d,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, ec.value(),
        //        boost::system::system_error(ec).what());

        if (!ec) // 0 ²Ù×÷³É¹¦
        {
            connect_timeout_finished_ = true;
            auto &derived = static_cast<derived_t &>(*this);
            derived._DoneConnectionTimeout();
        }
        else
        {
            BOOST_ASSERT(ec == net::error::operation_aborted);
        }
    }

    bool IsConnectTimeoutTimerFinished()
    {
        return connect_timeout_finished_;
    }
    bool IsConnectTimeoutTimerCanceled()
    {
        return connect_timeout_canceled_;
    }

    std::atomic<uint32_t> connect_timeout_seconds_ = {0};
    std::shared_ptr<net::steady_timer> check_connect_timeout_timer_;
    std::atomic<bool> connect_timeout_finished_;
    std::atomic<bool> connect_timeout_canceled_;
};

} // namespace net::detail