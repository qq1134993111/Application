#pragma once

#include <deque>
#include <string>
#include <string_view>

namespace net::detail
{
template <class derived_t, class args_t> class TcpSendOp
{
  public:
    TcpSendOp()
    {
    }
    ~TcpSendOp()
    {
    }

  public:
    bool AsyncSend(std::string_view data_view)
    {
        auto &derived = static_cast<derived_t &>(*this);
        if (!derived.IsConnected())
            return false;

        std::string data(data_view.data(), data_view.size());

        bool write_in_progress = false;
        {
            std::unique_lock<std::mutex> lc(send_mtx_);
            write_in_progress = !deq_send_messages_.empty();
            deq_send_messages_.push_back(std::move(data));
        }

        if (!write_in_progress)
        {
           net::dispatch(derived.GetIoService(),[this, this_ptr = derived.SelfPtr()]() {
                auto &derived = static_cast<derived_t &>(*this);
                if (!derived.IsConnected())
                    return;

                std::unique_lock<std::mutex> lc(send_mtx_);
                if (!deq_send_messages_.empty())
                {
                    DoAsyncSend(std::move(this_ptr));
                }
            });
        }

        return true;
    }

  protected:
    void DoAsyncSend(std::shared_ptr<derived_t> this_ptr)
    {
        auto &derived = static_cast<derived_t &>(*this);

        net::async_write(
            derived.Socket(), net::buffer(deq_send_messages_.front()),
            [this, this_ptr = std::move(this_ptr)](const net::error_code &ec, std::size_t size) {
                HandleWrite(ec, std::move(this_ptr));
            });
    }

    void HandleWrite(const net::error_code &ec, std::shared_ptr<derived_t> this_ptr)
    {
        auto &derived = static_cast<derived_t &>(*this);

        if (!ec && derived.IsConnected())
        {

            std::unique_lock<std::mutex> lc(send_mtx_);
            deq_send_messages_.pop_front();

            if (!deq_send_messages_.empty())
            {
                DoAsyncSend(std::move(this_ptr));
            }
        }
        else
        {

            // printf("%s,%d,%d,%s\n", __FUNCTION__, __LINE__, ec.value(), boost::system::system_error(ec).what());

            derived._DoneDisConnect(ec);
        }
    }

    void ClearSendQueue()
    {
        std::unique_lock<std::mutex> lc(send_mtx_);
        deq_send_messages_.clear();
    }

    std::deque<std::string> deq_send_messages_;
    std::mutex send_mtx_;

  private:
};

} // namespace net::detail
