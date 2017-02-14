#define BOOST_TEST_MODULE lib_interface modelio tests
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <input_output/bin.h>
#include <Core/mathlib.h>

using namespace std;
namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(constructor_test1)
{
	using namespace formatCoreBIN;
	ioModel modelRef;
	modelRef.vertices.push_back(t_pos(5,0,0));
	modelRef.vertices.push_back(t_pos(0, 0, 0));
	modelRef.vertices.push_back(t_pos(0, 5, 0));
	modelRef.vertices.push_back(t_pos(5, 5, 0));
	modelRef.vertices.push_back(t_pos(0, 5, 5));
	modelRef.vertices.push_back(t_pos(5, 5, 5));
	modelRef.vertices.push_back(t_pos(0, 0, 5));
	modelRef.vertices.push_back(t_pos(5, 0, 5));

	modelRef.faces.push_back(ioFace(0, 1, 2, -1, 66, -1));
	modelRef.faces.push_back(ioFace(0, 2, 3, -1, 66, -1));
	modelRef.faces.push_back(ioFace(2, 4, 5, -1, 100, -1));
	modelRef.faces.push_back(ioFace(2, 5, 3, -1, 100, -1));
	modelRef.faces.push_back(ioFace(2, 6, 4, -1, 100, -1));
	modelRef.faces.push_back(ioFace(2, 1, 6, -1, 100, -1));
	modelRef.faces.push_back(ioFace(1, 0, 7, -1, 100, -1));
	modelRef.faces.push_back(ioFace(6, 1, 7, -1, 100, -1));
	modelRef.faces.push_back(ioFace(0, 3, 5, -1, 100, -1));
	modelRef.faces.push_back(ioFace(7, 0, 5, -1, 100, -1));
	modelRef.faces.push_back(ioFace(7, 5, 4, -1, 66, -1));
	modelRef.faces.push_back(ioFace(6, 7, 4, -1, 66, -1));

	CformatBIN driver;
	BOOST_TEST(driver.ExportBIN("test_io_wr.cbin", modelRef), "Failed to write .bin file");

	ioModel modelTest;
	
	BOOST_TEST(driver.ImportBIN(modelTest, "test_io_wr.cbin"));

	// Check content
}
