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
�����Ѿ�Ϊ������Ŀ������BOOST_ENABLE_ASSERT_DEBUG_HANDLER�ꡣ
�������е�BOOST_ASSERT��BOOST_ASSERT_MSG��ʧ�ܵ�����¶����������ǵĺ���assertion_failed��assertion_failed_msg��
��assertion_failed_msg�У�����������ɶ��Ժ��boost::stacktrace::stacktrace�ṩ����Ϣ��
*/

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
// ����һ�� boost::error_info  typedef�����ڱ����ջ������Ϣ��
typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;
// ��дһ�������࣬�����׳����ж�ջ���ٵ��κ��쳣��
template <class E> void throw_with_trace(const E &e)
{
    throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}
// ��ʹ��throw_with_trace(E);������ֻʹ��throw E"
/*
  if (i >= 4)
    throw_with_trace(std::out_of_range("'i' must be less than 4 in oops()"));
  if (i <= 0)
    throw_with_trace(std::logic_error("'i' must not be greater than zero in oops()"));
*/
// �����쳣��
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

