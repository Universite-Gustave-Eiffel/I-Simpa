#define BOOST_TEST_MODULE spps directivity test
#include <boost/test/included/unit_test.hpp>
#include <input_output/gabe/gabe.h>
#include <iostream>
#include <math.h>

using namespace formatGABE;
using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(directivity_test_scene_result, *utf::tolerance(boost::test_tools::fpc::percent_tolerance(10.0)))
{
	GABE r_front("Punctual receivers/r_front/Sound level per source.recp");
	GABE r_back("Punctual receivers/r_back/Sound level per source.recp");
	GABE r_left("Punctual receivers/r_left/Sound level per source.recp");
	GABE r_right("Punctual receivers/r_right/Sound level per source.recp");
	GABE r_top("Punctual receivers/r_top/Sound level per source.recp");
	GABE r_bot("Punctual receivers/r_bot/Sound level per source.recp");

	BOOST_TEST(r_front.GetCols() > 0);
	BOOST_TEST(r_back.GetCols() > 0);
	BOOST_TEST(r_left.GetCols() > 0);
	BOOST_TEST(r_right.GetCols() > 0);
	BOOST_TEST(r_top.GetCols() > 0);
	BOOST_TEST(r_bot.GetCols() > 0);

	// 100hz
	GABE_Data_Float* front = dynamic_cast<GABE_Data_Float*>(r_front.GetCol(1));
	GABE_Data_Float* back = dynamic_cast<GABE_Data_Float*>(r_back.GetCol(1));
	GABE_Data_Float* left = dynamic_cast<GABE_Data_Float*>(r_left.GetCol(1));
	GABE_Data_Float* right = dynamic_cast<GABE_Data_Float*>(r_right.GetCol(1));
	GABE_Data_Float* top = dynamic_cast<GABE_Data_Float*>(r_top.GetCol(1));
	GABE_Data_Float* bot = dynamic_cast<GABE_Data_Float*>(r_bot.GetCol(1));
	
	// attenuation order
	BOOST_TEST(front->GetValue(0) > left->GetValue(0));
	BOOST_TEST(left->GetValue(0) > bot->GetValue(0));
	BOOST_TEST(bot->GetValue(0) > right->GetValue(0));
	BOOST_TEST(right->GetValue(0) > back->GetValue(0));
	BOOST_TEST(back->GetValue(0) > top->GetValue(0));

	auto lref = (10 * log(front->GetValue(0))) / log(10);

	auto lback = (10 * log(back->GetValue(0))) / log(10);
	auto lleft = (10 * log(left->GetValue(0))) / log(10);
	auto lright = (10 * log(right->GetValue(0))) / log(10);
	auto ltop = (10 * log(top->GetValue(0))) / log(10);
	auto lbot = (10 * log(bot->GetValue(0)) ) / log(10);

	// attenuation value
	BOOST_TEST((lref - lback) == 20.0);
	BOOST_TEST((lref - lleft) == 5.0);
	BOOST_TEST((lref - lright) == 15.0);
	BOOST_TEST((lref - ltop) == 25.0);
	BOOST_TEST((lref - lbot) == 10.0);
}
