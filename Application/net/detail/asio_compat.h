#pragma once

#ifdef ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ssl.hpp>
#else
#include <boost/version.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#endif

namespace net {

#ifdef ASIO_STANDALONE

    using io_context = asio::io_context;
    using executor_type = asio::io_context::executor_type;
    using strand = asio::strand<executor_type>;
    template <typename Executor>
    using work_guard = asio::executor_work_guard<Executor>;
    inline auto make_work_guard(io_context& io) { return asio::make_work_guard(io); }

    template <typename CompletionToken>
    inline auto post(io_context& io, CompletionToken&& token) {
        return asio::post(io, std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    inline auto dispatch(io_context& io, CompletionToken&& token) {
        return asio::dispatch(io, std::forward<CompletionToken>(token));
    }

    namespace ip = asio::ip;
    using tcp = asio::ip::tcp;
    using steady_timer = asio::steady_timer;

    namespace ssl = asio::ssl;
    using ssl_context = asio::ssl::context;
    template <typename Stream>
    using ssl_stream = asio::ssl::stream<Stream>;

    using streambuf = asio::streambuf;
    using asio::buffer;
    using asio::buffers_iterator;
    using asio::socket_base;

    // 错误码相关
    using system_error = asio::system_error;
    using error_code = asio::error_code;
    namespace error = asio::error;

    using asio::async_connect;
    using asio::async_read;
    using asio::async_read_until;
    using asio::async_write;
    using asio::transfer_at_least;


#else // Boost.Asio

#if BOOST_VERSION >= 106600
    using io_context = boost::asio::io_context;
    using executor_type = boost::asio::io_context::executor_type;
    using strand = boost::asio::strand<executor_type>;
    template <typename Executor>
    using work_guard = boost::asio::executor_work_guard<Executor>;
    inline auto make_work_guard(io_context& io) { return boost::asio::make_work_guard(io); }
#else
    using io_context = boost::asio::io_service;
    using executor_type = io_context::executor_type; // 旧版本可能不可用
    using strand = boost::asio::io_service::strand;
    using work_guard = boost::asio::io_service::work;
    inline auto make_work_guard(io_context& io) { return std::make_unique<work_guard>(io); }
#endif

    template <typename CompletionToken>
    inline auto post(io_context& io, CompletionToken&& token)
    {
        return boost::asio::post(io, std::forward<CompletionToken>(token));
    }

    template <typename CompletionToken>
    inline auto dispatch(io_context& io, CompletionToken&& token)
    {
        return boost::asio::dispatch(io, std::forward<CompletionToken>(token));
    }

    namespace ip = boost::asio::ip;
    using tcp = boost::asio::ip::tcp;
    using steady_timer = boost::asio::steady_timer;

    namespace ssl = boost::asio::ssl;
    using ssl_context = boost::asio::ssl::context;
    template <typename Stream>
    using ssl_stream = boost::asio::ssl::stream<Stream>;

    using streambuf = boost::asio::streambuf;
    using boost::asio::streambuf;
    using boost::asio::buffers_iterator;
    using boost::asio::buffer;

    using boost::asio::socket_base;

    // 错误码相关
    using system_error = boost::system::system_error;
    using error_code = boost::system::error_code;
    namespace error = boost::asio::error;

    using boost::asio::async_connect;
    using boost::asio::async_read;
    using boost::asio::async_read_until;
    using boost::asio::async_write;
    using boost::asio::transfer_at_least;

#endif

    inline net::ip::address make_address_compat(const std::string& s, net::error_code& ec)
    {
#if BOOST_VERSION >= 108700 || defined(ASIO_STANDALONE)
        return net::ip::make_address(s, ec);
#else
        return boost::asio::ip::address::from_string(s, ec);
#endif
    }

    template <typename Timer, typename Duration>
    inline void set_timer_expires_from_now(Timer& t, Duration d)
    {
#if BOOST_VERSION >= 107000 || defined(ASIO_STANDALONE)
        t.expires_after(d);   // 新版本
#else
        t.expires_from_now(d); // 旧版本
#endif
    }

    template <typename PodType, typename ConstBufferSequence>
    inline const PodType* compat_buffer_cast(const ConstBufferSequence& buffers)
    {
#if defined(ASIO_STANDALONE)
        return reinterpret_cast<const PodType*>(&*asio::buffers_begin(buffers));
#  else
   #  if BOOST_VERSION < 106600
        return boost::asio::buffer_cast<const PodType*>(*buffers.begin());
   #  else
        return reinterpret_cast<const PodType*>(&*boost::asio::buffers_begin(buffers));
   #  endif

#endif
    }

} // namespace net
