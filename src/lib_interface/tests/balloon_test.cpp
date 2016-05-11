#define BOOST_TEST_MODULE lib_interface test
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <directivityBalloon.h>

using namespace std;

BOOST_AUTO_TEST_CASE( constructor_test1 )
{
	t_DirectivityBalloon balloon1("speaker-test1.txt");
	BOOST_TEST(balloon1.asDataForFrequency(100) == true);
	BOOST_TEST(balloon1.asDataForFrequency(200) == false );
}

BOOST_AUTO_TEST_CASE(constructor_test2)
{
	t_DirectivityBalloon balloon1("speaker-test2.txt");
}


