
//链接boost单元测试库，链接版本，可以使用更多功能，比如传递命令行参数
#define BOOST_TEST_MODULE MyTestModule
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_case1) {
	BOOST_CHECK(1 + 1 == 2);
}

BOOST_AUTO_TEST_CASE(test_case2) {
	BOOST_REQUIRE(2 * 2 == 4);
}