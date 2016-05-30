#define BOOST_TEST_MODULE lib_interface balloon tests
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <directivityBalloon.h>
#include <Core/mathlib.h>

using namespace std;
namespace utf = boost::unit_test;

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
	BOOST_TEST(balloon.asDataForFrequency(200)		== true);
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

BOOST_AUTO_TEST_CASE(getInterpolatedValue_test, *utf::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
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

//-------------------------------------------------------------------------------------------------------------------------\\

BOOST_AUTO_TEST_CASE(cartesianToSpherical_test, *utf::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
{
	vec3 xaxis(1, 0, 0);
	vec3 yaxis(0, 1, 0);
	vec3 zaxis(0, 0, 1);
	vec3 minus_xaxis(-1, 0, 0);
	vec3 minus_yaxis(0, -1, 0);
	vec3 some_vec(1, 1, 1);

	auto coord = t_DirectivityBalloon::cartesianToSpherical(xaxis);
	BOOST_TEST(get<0>(coord) == 0.0);
	BOOST_TEST(get<1>(coord) == M_PIDIV2);

	coord = t_DirectivityBalloon::cartesianToSpherical(minus_xaxis);
	BOOST_TEST(get<0>(coord) == M_PI);
	BOOST_TEST(get<1>(coord) == M_PIDIV2);

	coord = t_DirectivityBalloon::cartesianToSpherical(yaxis);
	BOOST_TEST(get<0>(coord) == M_PIDIV2);
	BOOST_TEST(get<1>(coord) == M_PIDIV2);

	coord = t_DirectivityBalloon::cartesianToSpherical(minus_yaxis);
	BOOST_TEST(get<0>(coord) == (M_PI + M_PIDIV2));
	BOOST_TEST(get<1>(coord) == M_PIDIV2);

	coord = t_DirectivityBalloon::cartesianToSpherical(zaxis);
	BOOST_TEST(get<1>(coord) == 0);

	coord = t_DirectivityBalloon::cartesianToSpherical(some_vec);
	BOOST_TEST(get<0>(coord) == 0.785);
	BOOST_TEST(get<1>(coord) == 0.9557);
}

BOOST_AUTO_TEST_CASE(sphericalToCartesian_test, *utf::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
{
	vec3 xaxis(1, 0, 0);
	vec3 yaxis(0, 1, 0);
	vec3 zaxis(0, 0, 1);
	vec3 minus_xaxis(-1, 0, 0);
	vec3 minus_yaxis(0, -1, 0);
	
	vec3 xaxis_calc = t_DirectivityBalloon::sphericalToCartesian(1, 0, M_PIDIV2);
	vec3 minus_xaxis_calc = t_DirectivityBalloon::sphericalToCartesian(1, M_PI, M_PIDIV2);
	vec3 yaxis_calc = t_DirectivityBalloon::sphericalToCartesian(1, M_PIDIV2, M_PIDIV2);
	vec3 minus_yaxis_clac = t_DirectivityBalloon::sphericalToCartesian(1, (M_PI + M_PIDIV2), M_PIDIV2);
	vec3 zaxis_calc = t_DirectivityBalloon::sphericalToCartesian(1, 0, 0);

	BOOST_TEST(xaxis.barelyEqual(xaxis_calc));
	BOOST_TEST(yaxis.barelyEqual(yaxis_calc));
	BOOST_TEST(zaxis.barelyEqual(zaxis_calc));
}

BOOST_AUTO_TEST_CASE(vectorRotation_test, *utf::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
{
	vec3 xaxis(1, 0, 0);
	vec3 yaxis(0, 1, 0);
	vec3 zaxis(0, 0, 1);

	vec3 minus_xaxis(-1, 0, 0);
	vec3 somevec1(-1, 1, 1);
	vec3 somevec2(-1, 1, 0);
	vec3 somevec3(-1, -1, 1);
	
	vec3 particleVector(0, 1, 0);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2 );
	BOOST_TEST(particleVector.barelyEqual(zaxis));

	particleVector.set(0, 0, 1);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2);
	BOOST_TEST(particleVector.barelyEqual(yaxis));

	particleVector.set(1, 0, 0);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2);
	BOOST_TEST(particleVector.barelyEqual(minus_xaxis));

	particleVector.set(-1, 0, 0);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2);
	BOOST_TEST(particleVector.barelyEqual(xaxis));

	particleVector.set(1, 1, 1);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2);
	BOOST_TEST(particleVector.barelyEqual(somevec1));

	particleVector.set(1, 0, 1);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2);
	BOOST_TEST(particleVector.barelyEqual(somevec2));

	particleVector.set(1, 1, -1);
	particleVector = particleVector.Rotation(yaxis, M_PI);
	particleVector = particleVector.Rotation(xaxis, M_PIDIV2);
	BOOST_TEST(particleVector.barelyEqual(somevec3));
}

BOOST_AUTO_TEST_CASE(loudspeaker_coordinate_test, *utf::tolerance(boost::test_tools::fpc::percent_tolerance(1.0)))
{
	vec3 source_Direction(1, 0, 0); // first test direction

	vec3 particle(1, 0, 0); // front
	tuple<double, double> spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<1>(spk_coord) == 0.0);

	particle.set(-1, 0, 0); // back
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<1>(spk_coord) == M_PI);

	particle.set(0, 1, 0); // left
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<0>(spk_coord) == 0.0);
	BOOST_TEST(get<1>(spk_coord) == M_PIDIV2);

	particle.set(0, -1, 0); // right
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<0>(spk_coord) == M_PI);
	BOOST_TEST(get<1>(spk_coord) == M_PIDIV2);

	particle.set(0, 0, 1); // top
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<0>(spk_coord) == M_PIDIV2);
	BOOST_TEST(get<1>(spk_coord) == M_PIDIV2);

	particle.set(0, 0, -1); // bottom
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<0>(spk_coord) == (M_PI + M_PIDIV2));
	BOOST_TEST(get<1>(spk_coord) == M_PIDIV2);

	particle.set(1, 0, -1); // bottom front
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<0>(spk_coord) == (M_PI + M_PIDIV2));
	BOOST_TEST(get<1>(spk_coord) == 0.785);

	source_Direction.set(1, 1, -1); // second test direction

	particle.set(1, 1, -1); // front
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<1>(spk_coord) == 0.0);

	particle.set(-1, -1, 1); // back
	spk_coord = t_DirectivityBalloon::loudspeaker_coordinate(source_Direction, particle);
	BOOST_TEST(get<1>(spk_coord) == M_PI);

}

