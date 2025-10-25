#pragma once
#include <atomic>
#include <memory>
#include "asio_compat.h"

namespace net::detail
{
template <class derived_t, class args_t> class RecvTimerCp
{
  public:
    RecvTimerCp()
    {
    }
    ~RecvTimerCp()
    {
    }

  public:
    void SetRecvTimeOut(uint32_t timeout_seconds)
    {
        recv_timeout_seconds_ = timeout_seconds;
    }


  protected:
    void DoExpiresRecvTimer(std::shared_ptr<derived_t> this_ptr)
    {
     
        recv_timer_finished_.store(false, std::memory_order_relaxed);
        recv_timer_was_canceled_.store(false, std::memory_order_relaxed);


        uint8_t current_generation = ++recv_timer_generation_;

        if (recv_timeout_seconds_ == 0)
            return;

        if (check_recv_timeout_timer_ == nullptr)
        {
            auto& derived = static_cast<derived_t&>(*this);
            check_recv_timeout_timer_ = std::make_shared<boost::asio::steady_timer>(derived.GetIoService());
        }

        set_timer_expires_from_now(*check_recv_timeout_timer_, std::chrono::seconds(recv_timeout_seconds_));

        check_recv_timeout_timer_->async_wait(
            [this, this_ptr = std::move(this_ptr),current_generation](boost::system::error_code const& ec) { 
                if (current_generation != recv_timer_generation_.load(std::memory_order_relaxed))
                {
                    return; // 已失效
                }
                HandleRecvTimerTimeout(ec);
            });
    }

    size_t DoCancelRecvTimer()
    {

        recv_timer_was_canceled_.store(true, std::memory_order_relaxed);
        ++recv_timer_generation_;

        size_t size = 0;

        if (check_recv_timeout_timer_!=nullptr)
        {
            try
            {
                size = check_recv_timeout_timer_->cancel();
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
    void HandleRecvTimerTimeout(net::error_code const &ec)
    {
        // printf("FILE:%s,FUNCTION:%s,LINE:%d,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, ec.value(),
        //        boost::system::system_error(ec).what());

        if (!ec) // 0 操作成功
        {
            recv_timer_finished_.store(true, std::memory_order_relaxed);
            auto &derived = static_cast<derived_t &>(*this);
            derived._DoneRecvTimerTimeout();
        }
        else
        {
            BOOST_ASSERT(ec == net::error::operation_aborted);
        }
    }

    bool IsRecvTimerFinished()
    {
        return recv_timer_finished_.load(std::memory_order_relaxed);
    }
    bool IsRecvTimerCanceled()
    {
        return recv_timer_was_canceled_.load(std::memory_order_relaxed);
    }

    std::shared_ptr<net::steady_timer> check_recv_timeout_timer_;
    std::atomic<uint32_t> recv_timeout_seconds_{ 0 };
    std::atomic<bool> recv_timer_finished_{ false };
    std::atomic<bool> recv_timer_was_canceled_{ false };
    std::atomic<uint8_t> recv_timer_generation_{ 0 };   
};

} // namespace net::detail