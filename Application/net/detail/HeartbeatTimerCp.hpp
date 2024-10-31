#pragma once
#include <atomic>
#include <memory>

#include "boost/asio.hpp"

namespace net::detail
{
using HeartbeartInfoFunc = std::function<std::string()>;

template <class derived_t, class args_t> class HeartbeatTimerCp
{
  public:
    HeartbeatTimerCp()
    {
    }
    ~HeartbeatTimerCp()
    {
    }

  public:
    void SetHeartbeatIntervals(uint32_t timeout_seconds, HeartbeartInfoFunc func)
    {
        heartbeat_seconds_ = timeout_seconds;
        get_heartbeat_info_func_ = std::move(func);
    }

  protected:
    void DoExpiresHeartbeatTimer()
    {
        auto &derived = static_cast<derived_t &>(*this);
        auto self_ptr = derived.SelfPtr();
        DoExpiresHeartbeatTimer(std::move(self_ptr));
    }
    void DoExpiresHeartbeatTimer(std::shared_ptr<derived_t> this_ptr)
    {
        heartbeat_timer_finished_ = false;
        if (heartbeat_timer_canceled_ptr_ != nullptr)
        {
            *heartbeat_timer_canceled_ptr_ = true;
        }

        heartbeat_timer_canceled_ptr_ = std::make_shared<bool>(false);

        if (heartbeat_seconds_ == 0)
            return;

        // printf("FILE:%s,FUNCTION:%s,LINE:%d,connect_timeout_seconds_:%d\n", __FILE__, __FUNCTION__, __LINE__,
        //        connect_timeout_seconds_.load());

        if (check_heartbeat_timer_ == nullptr)
        {
            auto &derived = static_cast<derived_t &>(*this);
            check_heartbeat_timer_ = std::make_shared<boost::asio::steady_timer>(derived.GetIoService());
        }

        check_heartbeat_timer_->expires_from_now(std::chrono::seconds(heartbeat_seconds_));

        check_heartbeat_timer_->async_wait(
            [this, this_ptr = std::move(this_ptr),
             this_canceled = heartbeat_timer_canceled_ptr_](boost::system::error_code const &ec) {
                if (*this_canceled)
                {
                    return;
                }
                HandleHeartbeatTimerTimeout(ec);
            });
    }

    size_t DoCancelHeartbeatTimer()
    {
        if (heartbeat_timer_canceled_ptr_ != nullptr)
        {
            *heartbeat_timer_canceled_ptr_ = true;
        }

        boost::system::error_code ignored_ec;

        size_t size = 0;
        if (check_heartbeat_timer_ != nullptr)
        {
            check_heartbeat_timer_->cancel(ignored_ec);

            // printf("FILE:%s,FUNCTION:%s,LINE:%d, %d canceled,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, size,
            //        ignored_ec.value(), boost::system::system_error(ignored_ec).what());
        }
        return size;
    }

  protected:
    void HandleHeartbeatTimerTimeout(boost::system::error_code const &ec)
    {
        // printf("FILE:%s,FUNCTION:%s,LINE:%d,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, ec.value(),
        //        boost::system::system_error(ec).what());

        if (!ec) // 0 操作成功
        {
            heartbeat_timer_finished_ = true;
            auto &derived = static_cast<derived_t &>(*this);
            derived._DoneHeartbeatTimerTimeout();
        }
        else
        {
            BOOST_ASSERT(ec == boost::asio::error::operation_aborted);
        }
    }

    bool IsHeartbeatTimerFinished()
    {
        return heartbeat_timer_finished_;
    }
    bool IsHeartbeatTimerCanceled()
    {
        if (heartbeat_timer_canceled_ptr_ != nullptr)
        {
            return *heartbeat_timer_canceled_ptr_;
        }

        return false;
    }

    void _DoneHeartbeatTimerTimeout()
    {
        // 发送心跳
        auto &derived = static_cast<derived_t &>(*this);
        if (derived.deq_send_messages_.empty())
        {
            std::string str_heartbeat=get_heartbeat_info_func_();
            derived.AsyncSend(str_heartbeat);
        }

        this->DoExpiresHeartbeatTimer();
    }

    std::atomic<uint32_t> heartbeat_seconds_{0};
    HeartbeartInfoFunc get_heartbeat_info_func_;
    std::shared_ptr<boost::asio::steady_timer> check_heartbeat_timer_;
    bool heartbeat_timer_finished_;
    std::shared_ptr<bool> heartbeat_timer_canceled_ptr_;
};

} // namespace net::detail

namespace net
{
using HeartbeartInfoFunc = detail::HeartbeartInfoFunc;
}