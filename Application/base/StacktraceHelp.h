#pragma once


// BOOST_ENABLE_ASSERT_DEBUG_HANDLER is defined for the whole project
#include <boost/stacktrace.hpp>
#include <iostream>  // std::cerr
#include <stdexcept> // std::logic_error
namespace boost
{
inline void assertion_failed_msg(char const *expr, char const *msg, char const *function, char const * /*file*/,
                                 long /*line*/)
{
    std::cerr << "Expression '" << expr << "' is false in function '" << function << "': " << (msg ? msg : "<...>")
              << ".\n"
              << "Backtrace:\n"
              << boost::stacktrace::stacktrace() << '\n';
    std::abort();
}
inline void assertion_failed(char const *expr, char const *function, char const *file, long line)
{
    ::boost::assertion_failed_msg(expr, 0 /*nullptr*/, function, file, line);
}
} // namespace boost

/*
我们已经为整个项目定义了BOOST_ENABLE_ASSERT_DEBUG_HANDLER宏。
现在所有的BOOST_ASSERT和BOOST_ASSERT_MSG在失败的情况下都将调用我们的函数assertion_failed和assertion_failed_msg。
在assertion_failed_msg中，我们输出了由断言宏和boost::stacktrace::stacktrace提供的信息。
*/

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
// 声明一个 boost::error_info  typedef，用于保存堆栈跟踪信息。
typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;
// 编写一个帮助类，用于抛出带有堆栈跟踪的任何异常。
template <class E> void throw_with_trace(const E &e)
{
    throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}
// 请使用throw_with_trace(E);而不是只使用throw E"
/*
  if (i >= 4)
    throw_with_trace(std::out_of_range("'i' must be less than 4 in oops()"));
  if (i <= 0)
    throw_with_trace(std::logic_error("'i' must not be greater than zero in oops()"));
*/
// 处理异常：
/*
try {
    foo(5); // testing assert handler
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
    if (st) {
        std::cerr << *st << '\n';
    }
}
*/

