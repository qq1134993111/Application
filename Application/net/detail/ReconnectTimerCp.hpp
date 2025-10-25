#pragma once
#include <atomic>
#include <memory>
#include "asio_compat.h"

namespace net::detail
{
template <class derived_t, class args_t> class ReconnectTimerCp
{
  public:
    ReconnectTimerCp()
    {
    }
    ~ReconnectTimerCp()
    {
    }

  public:
    void SetReconnectIntervals(uint32_t timeout_seconds)
    {
        reconnect_seconds_ = timeout_seconds;
    }
    uint32_t GetReconnectTimeout()
    {
        return reconnect_seconds_;
    }

  protected:
    void DoExpiresReconnectTimer(std::shared_ptr<derived_t> this_ptr)
    {
        reconnect_timer_finished_ = false;
        reconnect_timer_canceled_ = false;

        if (reconnect_seconds_ == 0)
            return;

        // printf("FILE:%s,FUNCTION:%s,LINE:%d,connect_timeout_seconds_:%d\n", __FILE__, __FUNCTION__, __LINE__,
        //        connect_timeout_seconds_.load());

        if (check_reconnect_timer_ == nullptr)
        {
            auto &derived = static_cast<derived_t &>(*this);
            check_reconnect_timer_ = std::make_shared<net::steady_timer>(derived.GetIoService());
        }

        set_timer_expires_from_now(*check_reconnect_timer_, std::chrono::seconds(reconnect_seconds_));

        check_reconnect_timer_->async_wait([this, this_ptr = std::move(this_ptr)](net::error_code const &ec) {
            if (reconnect_timer_canceled_)
            {
                return;
            }
            HandleReconnectTimer(ec);
        });
    }

    size_t DoCancelReconnectTimer()
    {
        reconnect_timer_canceled_ = true;

        size_t size = 0;
        if (check_reconnect_timer_!=nullptr)
        {
            try
            {
                size = check_reconnect_timer_->cancel();
            }
            catch (const boost::system::system_error& e)
            {

            }
            // printf("FILE:%s,FUNCTION:%s,LINE:%d, %d canceled,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, size,
            //        ignored_ec.value(), boost::system::system_error(ignored_ec).what());
        }
        return size;
    }

  protected:
    void HandleReconnectTimer(net::error_code const &ec)
    {
        // printf("FILE:%s,FUNCTION:%s,LINE:%d,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, ec.value(),
        //        boost::system::system_error(ec).what());

        if (!ec) // 0 ²Ù×÷³É¹¦
        {
            reconnect_timer_finished_ = true;
            auto &derived = static_cast<derived_t &>(*this);
            derived._DoneReconnectTimerTimeout();
        }
        else
        {
            BOOST_ASSERT(ec == net::error::connection_aborted);
        }
    }

    bool IsReconnectTimerFinished()
    {
        return reconnect_timer_finished_;
    }
    bool IsReconnectTimerCanceled()
    {
        return reconnect_timer_canceled_;
    }

    std::atomic<uint32_t> reconnect_seconds_{0};
    std::atomic<int64_t> total_reconnect_times_{0};

    std::shared_ptr<net::steady_timer> check_reconnect_timer_;
    bool reconnect_timer_finished_;
    bool reconnect_timer_canceled_;
};

} // namespace net::detail