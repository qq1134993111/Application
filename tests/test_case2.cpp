
//不链接单元测试库
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

// 测试用例
BOOST_AUTO_TEST_CASE(test_case2) {
	BOOST_CHECK(1 + 2 == 3);       // 简单检查
	BOOST_REQUIRE(4 / 2 == 2);     // 强制检查
	//BOOST_CHECK_CLOSE(2.71, 2.718, 0.1); // 近似值检查
}


