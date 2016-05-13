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

	BOOST_TEST(balloon.asValue(200, 45, 90)		== false); // undefined frequency
	BOOST_TEST(balloon.asValue(100, 48, 90)		== false); // undefined phi
	BOOST_TEST(balloon.asValue(100, 400, 170)	== false); // phi out of bound
	BOOST_TEST(balloon.asValue(100, 45, 2)		== false); // undefined theta
	BOOST_TEST(balloon.asValue(100, 355, 190)	== false); // theta out of bound

	BOOST_TEST(balloon.asInterpolatedValue(100, 42, 154) == true);
	BOOST_TEST(balloon.asInterpolatedValue(200, 42, 154) == false); // undefined frequency
	BOOST_TEST(balloon.asInterpolatedValue(100, 400, 154) == false); // phi out of bound
	BOOST_TEST(balloon.asInterpolatedValue(100, 42, 200) == false); // theta out of bound
}

BOOST_AUTO_TEST_CASE(constructor_test2)
{
	t_DirectivityBalloon balloon("speaker-test2.txt");

	BOOST_TEST(balloon.asDataForFrequency(40)		== true);
	BOOST_TEST(balloon.asDataForFrequency(2000)		== true);
	BOOST_TEST(balloon.asDataForFrequency(20000)	== false);

	BOOST_TEST(balloon.asValue(100, 45, 90)		== true);
	BOOST_TEST(balloon.asValue(100, 355, 170)	== true);

	BOOST_TEST(balloon.asValue(205, 45, 90) == false); // undefined frequency
	BOOST_TEST(balloon.asValue(100, 48, 90) == false); // undefined phi
	BOOST_TEST(balloon.asValue(100, 400, 170) == false); // phi out of bound
	BOOST_TEST(balloon.asValue(100, 45, 2) == false); // undefined theta
	BOOST_TEST(balloon.asValue(100, 355, 190) == false); // theta out of bound

	BOOST_TEST(balloon.asInterpolatedValue(100, 42, 154) == true);
	BOOST_TEST(balloon.asInterpolatedValue(205, 42, 154) == false); // undefined frequency
	BOOST_TEST(balloon.asInterpolatedValue(100, 400, 154) == false); // phi out of bound
	BOOST_TEST(balloon.asInterpolatedValue(100, 42, 200) == false); // theta out of bound
}

BOOST_AUTO_TEST_CASE(getValue_test)
{
	t_DirectivityBalloon balloon("speaker-test2.txt");

	BOOST_TEST(balloon.getValue(50, 115, 30)	== -1.51);
	BOOST_TEST(balloon.getValue(50, 115, 35)	== -1.71);
	BOOST_TEST(balloon.getValue(2500, 260, 175)	== -6.08);

	BOOST_TEST(balloon.getValue(40, 90, 0) == balloon.getValue(40, 180, 0));
	BOOST_TEST(balloon.getValue(40, 90, 180) == balloon.getValue(40, 180, 180));
}

BOOST_AUTO_TEST_CASE(getInterpolatedValue_test, *boost::unit_test::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
{
	t_DirectivityBalloon balloon("speaker-test2.txt");

	BOOST_TEST(balloon.getInterpolatedValue(40, 62, 29) == -1.48);
	BOOST_TEST(balloon.getInterpolatedValue(40, 60, 27.5) == -1.45);

	BOOST_TEST(balloon.getInterpolatedValue(50, 115, 30) == -1.51);
	BOOST_TEST(balloon.getInterpolatedValue(50, 115, 35) == -1.71);
	BOOST_TEST(balloon.getInterpolatedValue(2500, 260, 175) == -6.08);

	BOOST_TEST(balloon.getInterpolatedValue(40, 90, 0) == balloon.getInterpolatedValue(40, 180, 0));
	BOOST_TEST(balloon.getInterpolatedValue(40, 90, 180) == balloon.getInterpolatedValue(40, 180, 180));
}
