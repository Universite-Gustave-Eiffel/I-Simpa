#define BOOST_TEST_MODULE lib_interface balloon tests
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <directivityBalloon.h>

using namespace std;

BOOST_AUTO_TEST_CASE(constructor_test1)
{
	t_DirectivityBalloon balloon("speaker-test1.txt");

	BOOST_TEST(balloon.asDataForFrequency(100)	== true);
	BOOST_TEST(balloon.asDataForFrequency(200)	== false);

	BOOST_TEST(balloon.asValue(100, 45, 90)		== true);
	BOOST_TEST(balloon.asValue(100, 355, 170)	== true);

	BOOST_TEST(balloon.asValue(200, 45, 90)		== false);
	BOOST_TEST(balloon.asValue(200, 355, 170)	== false);
	BOOST_TEST(balloon.asValue(100, 48, 90)		== false);
	BOOST_TEST(balloon.asValue(100, 400, 170)	== false);
	BOOST_TEST(balloon.asValue(100, 45, 2)		== false);
	BOOST_TEST(balloon.asValue(100, 355, 190)	== false);
}

BOOST_AUTO_TEST_CASE(constructor_test2)
{
	t_DirectivityBalloon balloon("speaker-test2.txt");

	BOOST_TEST(balloon.asDataForFrequency(40)		== true);
	BOOST_TEST(balloon.asDataForFrequency(2000)		== true);
	BOOST_TEST(balloon.asDataForFrequency(20000)	== false);

	BOOST_TEST(balloon.asValue(100, 45, 90)		== true);
	BOOST_TEST(balloon.asValue(100, 355, 170)	== true);

	BOOST_TEST(balloon.asValue(205, 45, 90)		== false);
	BOOST_TEST(balloon.asValue(205, 355, 170)	== false);
	BOOST_TEST(balloon.asValue(100, 48, 90)		== false);
	BOOST_TEST(balloon.asValue(100, 400, 170)	== false);
	BOOST_TEST(balloon.asValue(100, 45, 2)		== false);
	BOOST_TEST(balloon.asValue(100, 355, 190)	== false);
}

BOOST_AUTO_TEST_CASE(getValue_test)
{
	t_DirectivityBalloon balloon("speaker-test2.txt");

	BOOST_TEST(balloon.getValue(50, 115, 30)	== -1.51);
	BOOST_TEST(balloon.getValue(50, 115, 35)	== -1.71);
	BOOST_TEST(balloon.getValue(2500, 260, 175) == -6.08);
}

BOOST_AUTO_TEST_CASE(getInterpolatedValue_test, *boost::unit_test::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
{
	t_DirectivityBalloon balloon("speaker-test2.txt");

	BOOST_TEST(balloon.getInterpolatedValue(40, 62, 29) == -1.48);
	BOOST_TEST(balloon.getInterpolatedValue(40, 60, 27.5) == -1.45);
}
