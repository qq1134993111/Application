#pragma once

namespace net::detail
{

template <class derived_t, class args_t> class ThisThreadIdCp
{
  public:
    ThisThreadIdCp()
    {
    }

    void DoInitThisThreadId()
    {
        this_thread_id_ = std::this_thread::get_id();
    }

    inline bool RunningInThisThread() const noexcept
    {
        return (std::this_thread::get_id() == this_thread_id_);
    }


    inline std::thread::id GetThreadId() const noexcept
    {
        return this_thread_id_;
    }


  private:
    std::thread::id this_thread_id_{};
};


} // namespace net::detail