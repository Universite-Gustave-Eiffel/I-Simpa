#define BOOST_TEST_MODULE spps core test
#include <boost/test/included/unit_test.hpp>
#include <tools/dotreflection.h>
#include <iostream>
#include <math.h>

using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(test_dot_reflection_specular)
{
    vec3 vecDir(0, .56, 0);
    t_Material_BFreq material = t_Material_BFreq();
    material.absorption = 0.1f;
    material.affaiblissement = 0;
    material.diffusion = 0;
    material.dotransmission = false;
    material.dotransmission = false;
    material.reflectionLaw = REFLECTION_LAW_SPECULAR;
    material.tau = 0;
    vec3 result = ReflectionLaws::SolveReflection(vecDir, material, vec3(0, -1, 0));
    BOOST_CHECK_EQUAL(1., result.length());
}


BOOST_AUTO_TEST_CASE(test_dot_reflection_lambert)
{
    vec3 vecDir(.56, .56, 0.56);
    t_Material_BFreq material = t_Material_BFreq();
    material.absorption = 0.1f;
    material.affaiblissement = 0;
    material.diffusion = 0.95f;
    material.dotransmission = false;
    material.dotransmission = false;
    material.reflectionLaw = REFLECTION_LAW_LAMBERT;
    material.tau = 0;
    vec3 faceNorm(-0.5, -0.5, -0.5);
    faceNorm.normalize();
    vec3 result = ReflectionLaws::SolveReflection(vecDir, material, faceNorm);
    BOOST_CHECK_CLOSE(1., result.length(), EPSILON);
}
/*
Disabled as execution of spps core is requested

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
*/