#pragma once
#include <memory>
#include <thread>
#include <exception>
#include <list>
#include <algorithm>
#include "boost/noncopyable.hpp"
#include "boost/thread.hpp"
#include "asio_compat.h"

namespace net::detail
{

class IoServicePool : boost::noncopyable
{
  public:
    class IosWorker
    {
      public:
        IosWorker() : ios_(), worker_()
        {
        }

        void Start()
        {
            worker_ = boost::thread(std::bind(&IosWorker::Run, this));
        }

        void Stop()
        {
            //if (!ios_.stopped())
            //    ios_.stop();
        }

        void Wait()
        {

            if (worker_.joinable())
                worker_.join();
        }

        net::io_context &GetIoService()
        {
            return ios_;
        }

      private:
        void Run()
        {
            net::error_code ec;
            auto guard = net::make_work_guard(ios_);
            ios_.run();
        };

        net::io_context ios_;
        boost::thread worker_;
    };

    using iterator = std::list<IosWorker>::iterator;

  public:
    explicit IoServicePool(size_t pool_size = std::thread::hardware_concurrency())
        : ios_workers_(std::max(pool_size, (size_t)1)), next_io_service_(ios_workers_.begin())
    {
    }

    ~IoServicePool()
    {
        Stop();
    }

    bool Start()
    {
        try
        {
            for (auto &ios_worker : ios_workers_)
                ios_worker.Start();
        }
        catch (std::exception &e)
        {
            (void)e;
            return false;
        }

        return true;
    }

    void Stop()
    {
        for (auto &ios : ios_workers_)
            ios.Stop();

        for (auto &ios : ios_workers_)
            ios.Wait();
    }

    net::io_context &GetIoService()
    {
        auto current = next_io_service_++;
        if (ios_workers_.end() == next_io_service_)
        {
            next_io_service_ = ios_workers_.begin();
        }

        return current->GetIoService();
    }

  private:
    std::list<IosWorker> ios_workers_;
    iterator next_io_service_;
};

} // namespace net