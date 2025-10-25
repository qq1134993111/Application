#pragma once
#include <atomic>
#include <memory>
#include "asio_compat.h"

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
        heartbeat_timer_finished_.store(false, std::memory_order_relaxed);
        heartbeat_timer_was_canceled_.store(false, std::memory_order_relaxed);


        uint8_t current_generation = ++heartbeat_timer_generation_;

        if (heartbeat_seconds_ == 0)
            return;

        // printf("FILE:%s,FUNCTION:%s,LINE:%d,connect_timeout_seconds_:%d\n", __FILE__, __FUNCTION__, __LINE__,
        //        connect_timeout_seconds_.load());

        if (check_heartbeat_timer_ == nullptr)
        {
            auto &derived = static_cast<derived_t &>(*this);
            check_heartbeat_timer_ = std::make_shared<net::steady_timer>(derived.GetIoService());
        }

        set_timer_expires_from_now(*check_heartbeat_timer_, std::chrono::seconds(heartbeat_seconds_));

        check_heartbeat_timer_->async_wait(
            [this, this_ptr = std::move(this_ptr),current_generation](net::error_code const &ec) {
                if (current_generation != heartbeat_timer_generation_.load(std::memory_order_relaxed))
                {
                    return; // 已失效
                }
                HandleHeartbeatTimerTimeout(ec);
            });
    }

    size_t DoCancelHeartbeatTimer()
    {
        heartbeat_timer_was_canceled_.store(true, std::memory_order_relaxed);
        ++heartbeat_timer_generation_;
     
        size_t size = 0;
        if (check_heartbeat_timer_ != nullptr)
        {
            try
            {
                size = check_heartbeat_timer_->cancel();
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
    void HandleHeartbeatTimerTimeout(net::error_code const &ec)
    {
        // printf("FILE:%s,FUNCTION:%s,LINE:%d,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, ec.value(),
        //        boost::system::system_error(ec).what());

        if (!ec) // 0 操作成功
        {
            heartbeat_timer_finished_.store(true, std::memory_order_relaxed);
            auto &derived = static_cast<derived_t &>(*this);
            derived._DoneHeartbeatTimerTimeout();
        }
        else
        {
            BOOST_ASSERT(ec == net::error::operation_aborted);
        }
    }

    bool IsHeartbeatTimerFinished()
    {
        return heartbeat_timer_finished_.load(std::memory_order_relaxed);
    }
    bool IsHeartbeatTimerCanceled()
    {
        return heartbeat_timer_was_canceled_.load(std::memory_order_relaxed);
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

    HeartbeartInfoFunc get_heartbeat_info_func_;
    std::shared_ptr<net::steady_timer> check_heartbeat_timer_;
    std::atomic<uint32_t> heartbeat_seconds_{ 0 };
    std::atomic<bool> heartbeat_timer_finished_{false};
    std::atomic<bool> heartbeat_timer_was_canceled_{ false };
    std::atomic<uint8_t> heartbeat_timer_generation_{ 0 };
};

} // namespace net::detail

namespace net
{
using HeartbeartInfoFunc = detail::HeartbeartInfoFunc;
}