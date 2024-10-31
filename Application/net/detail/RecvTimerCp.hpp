#pragma once
#include <atomic>
#include <memory>

#include "boost/asio.hpp"

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
        recv_timer_finished_ = false;
        if (recv_timer_canceled_ptr_ != nullptr)
        {
            *recv_timer_canceled_ptr_ = true;
        }
        recv_timer_canceled_ptr_ = std::make_shared<bool>(false);

        if (recv_timeout_seconds_ == 0)
            return;

        // printf("FILE:%s,FUNCTION:%s,LINE:%d,connect_timeout_seconds_:%d\n", __FILE__, __FUNCTION__, __LINE__,
        //        connect_timeout_seconds_.load());

        if (check_recv_timeout_timer_ == nullptr)
        {
            auto &derived = static_cast<derived_t &>(*this);
            check_recv_timeout_timer_ = std::make_shared<boost::asio::steady_timer>(derived.GetIoService());
        }

        check_recv_timeout_timer_->expires_from_now(std::chrono::seconds(recv_timeout_seconds_));

        check_recv_timeout_timer_->async_wait(
            [this, this_ptr = std::move(this_ptr),
             this_canceld = recv_timer_canceled_ptr_](boost::system::error_code const &ec) {
                if (*this_canceld)
                {
                    return;
                }
                HandleRecvTimerTimeout(ec);
            });
    }

    size_t DoCancelRecvTimer()
    {

        if (recv_timer_canceled_ptr_!=nullptr)
        {
            *recv_timer_canceled_ptr_ = true;
        }

        size_t size = 0;

        boost::system::error_code ignored_ec;

        if (check_recv_timeout_timer_!=nullptr)
        {
            check_recv_timeout_timer_->cancel(ignored_ec);

            // printf("FILE:%s,FUNCTION:%s,LINE:%d, %d canceled,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, size,
            //        ignored_ec.value(), boost::system::system_error(ignored_ec).what());
        }

        return size;
    }

  protected:
    void HandleRecvTimerTimeout(boost::system::error_code const &ec)
    {
        // printf("FILE:%s,FUNCTION:%s,LINE:%d,%d,%s\n", __FILE__, __FUNCTION__, __LINE__, ec.value(),
        //        boost::system::system_error(ec).what());

        if (!ec) // 0 ²Ù×÷³É¹¦
        {
            recv_timer_finished_ = true;
            auto &derived = static_cast<derived_t &>(*this);
            derived._DoneRecvTimerTimeout();
        }
        else
        {
            BOOST_ASSERT(ec == boost::asio::error::operation_aborted);
        }
    }

    bool IsRecvTimerFinished()
    {
        return recv_timer_finished_;
    }
    bool IsRecvTimerCanceled()
    {
        if (recv_timer_canceled_ptr_ != nullptr)
        {
            return *recv_timer_canceled_ptr_;
        }
        return false;
    }

    std::atomic<uint32_t> recv_timeout_seconds_{0};
    std::shared_ptr<boost::asio::steady_timer> check_recv_timeout_timer_;
    bool recv_timer_finished_;
    std::shared_ptr<bool> recv_timer_canceled_ptr_;
};

} // namespace net::detail