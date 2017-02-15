#define BOOST_TEST_MODULE lib_interface modelio tests
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <input_output/bin.h>
#include <input_output/importExportMaillage/mbin.h>
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
	BOOST_REQUIRE(driver.ExportBIN("test_io_wr.cbin", modelRef));

	ioModel modelTest;
	
	BOOST_REQUIRE(driver.ImportBIN(modelTest, "test_io_wr.cbin"));

	// Check content
	BOOST_REQUIRE(modelTest.faces.size() == 12);
	BOOST_REQUIRE(modelTest.vertices.size() == 8);

	BOOST_TEST((modelTest.vertices[0] == t_pos(5, 0, 0)));
	BOOST_TEST((modelTest.vertices[1] == t_pos(0, 0, 0)));
	BOOST_TEST((modelTest.vertices[2] == t_pos(0, 5, 0)));
	BOOST_TEST((modelTest.vertices[3] == t_pos(5, 5, 0)));
	BOOST_TEST((modelTest.vertices[4] == t_pos(0, 5, 5)));
	BOOST_TEST((modelTest.vertices[5] == t_pos(5, 5, 5)));
	BOOST_TEST((modelTest.vertices[6] == t_pos(0, 0, 5)));
	BOOST_TEST((modelTest.vertices[7] == t_pos(5, 0, 5)));

	
	BOOST_TEST((modelTest.faces[0] == formatCoreBIN::ioFace(0, 1, 2, -1, 66, -1)));
	BOOST_TEST((modelTest.faces[1] == formatCoreBIN::ioFace(0, 2, 3, -1, 66, -1)));
	BOOST_TEST((modelTest.faces[2] == formatCoreBIN::ioFace(2, 4, 5, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[3] == formatCoreBIN::ioFace(2, 5, 3, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[4] == formatCoreBIN::ioFace(2, 6, 4, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[5] == formatCoreBIN::ioFace(2, 1, 6, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[6] == formatCoreBIN::ioFace(1, 0, 7, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[7] == formatCoreBIN::ioFace(6, 1, 7, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[8] == formatCoreBIN::ioFace(0, 3, 5, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[9] == formatCoreBIN::ioFace(7, 0, 5, -1, 100, -1)));
	BOOST_TEST((modelTest.faces[10] == formatCoreBIN::ioFace(7, 5, 4, -1, 66, -1)));
	BOOST_TEST((modelTest.faces[11] == formatCoreBIN::ioFace(6, 7, 4, -1, 66, -1)));
}



BOOST_AUTO_TEST_CASE(retrocompat_test1)
{
	using namespace formatCoreBIN;
	ioModel modelTest;
	CformatBIN driver;

	BOOST_REQUIRE(driver.ImportBIN(modelTest, "cube.cbin"));

	// Check content
	BOOST_REQUIRE(modelTest.faces.size() == 12);
	BOOST_REQUIRE(modelTest.vertices.size() == 8);

	BOOST_TEST((modelTest.vertices[0] == t_pos(5, 0, 0)));
	BOOST_TEST((modelTest.vertices[1] == t_pos(0, 0, 0)));
	BOOST_TEST((modelTest.vertices[2] == t_pos(0, 5, 0)));
	BOOST_TEST((modelTest.vertices[3] == t_pos(5, 5, 0)));
	BOOST_TEST((modelTest.vertices[4] == t_pos(0, 5, 5)));
	BOOST_TEST((modelTest.vertices[5] == t_pos(5, 5, 5)));
	BOOST_TEST((modelTest.vertices[6] == t_pos(0, 0, 5)));
	BOOST_TEST((modelTest.vertices[7] == t_pos(5, 0, 5)));


	BOOST_TEST((modelTest.faces[0] == formatCoreBIN::ioFace(0, 1, 2, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[1] == formatCoreBIN::ioFace(0, 2, 3, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[2] == formatCoreBIN::ioFace(2, 4, 5, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[3] == formatCoreBIN::ioFace(2, 5, 3, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[4] == formatCoreBIN::ioFace(2, 6, 4, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[5] == formatCoreBIN::ioFace(2, 1, 6, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[6] == formatCoreBIN::ioFace(1, 0, 7, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[7] == formatCoreBIN::ioFace(6, 1, 7, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[8] == formatCoreBIN::ioFace(0, 3, 5, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[9] == formatCoreBIN::ioFace(7, 0, 5, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[10] == formatCoreBIN::ioFace(7, 5, 4, 0, -1, -1)));
	BOOST_TEST((modelTest.faces[11] == formatCoreBIN::ioFace(6, 7, 4, 0, -1, -1)));
}




BOOST_AUTO_TEST_CASE(retrocompat_test2)
{
	using namespace formatMBIN;

	CMBIN driver;

	bintetrahedre *tabTetra = NULL;
	t_binNode *tabNodes = NULL;
	unsigned int sizeTetra = 0;
	unsigned int sizeNodes = 0;

	BOOST_REQUIRE(driver.ImportBIN("cube_mesh.mbin", &tabTetra, &tabNodes, sizeTetra, sizeNodes));

	// Check content
	BOOST_REQUIRE(sizeNodes == 8);


	BOOST_TEST((tabNodes[0] == t_binNode(5, 0, 0)));
	BOOST_TEST((tabNodes[1] == t_binNode(0, 0, 0)));
	BOOST_TEST((tabNodes[2] == t_binNode(0, 5, 0)));
	BOOST_TEST((tabNodes[3] == t_binNode(5, 5, 0)));
	BOOST_TEST((tabNodes[4] == t_binNode(0, 5, 5)));
	BOOST_TEST((tabNodes[5] == t_binNode(5, 5, 5)));
	BOOST_TEST((tabNodes[6] == t_binNode(0, 0, 5)));
	BOOST_TEST((tabNodes[7] == t_binNode(5, 0, 5)));
	
	BOOST_REQUIRE(sizeTetra == 6);

	BOOST_TEST((tabTetra[0] == bintetrahedre(4,5,7,2,1,
		bintetraface(5,2,7,-1,5),bintetraface(7,2,4,-1,4), bintetraface(4,2,5,2,-2), bintetraface(5,7,4,10,-2))));
	BOOST_TEST((tabTetra[1] == bintetrahedre(6,2 ,7 ,1 ,1 ,
		bintetraface(2,1 ,7 ,-1 ,2 ), bintetraface(7,1 ,6 ,7 ,-2 ), bintetraface(6,1 ,2,5 ,-2 ), bintetraface(2,7 ,6 , -1, 4))));
	BOOST_TEST((tabTetra[2] == bintetrahedre(0,2 ,1 ,7 ,1 ,
		bintetraface(2, 7, 1, -1, 1), bintetraface(1,7 ,0 ,6 ,-2 ), bintetraface(0,7 ,2 ,-1 ,5 ), bintetraface(2,1 ,0 ,0 ,-2 ))));
	BOOST_TEST((tabTetra[3] == bintetrahedre(0,3 ,2 ,5 ,1 ,
		bintetraface(3,5 ,2 , 3,-2 ), bintetraface(2,5 ,0 ,-1 ,5 ), bintetraface(0, 5, 3, 8, -2), bintetraface(3,2 ,0 ,1 ,-2 ))));
	BOOST_TEST((tabTetra[4] == bintetrahedre(4, 2, 7, 6, 1,
		 bintetraface(2, 6, 7, -1,1 ), bintetraface(7, 6, 4, 11, -2), bintetraface(4, 6, 2, 4, -2), bintetraface(2, 7,4 , -1, 0))));
	BOOST_TEST((tabTetra[5] == bintetrahedre(5,0 ,7 ,2 ,1 ,
		bintetraface(0, 2, 7, -1, 2), bintetraface(7, 2, 5, -1, 0), bintetraface(5, 2, 0, -1, 3), bintetraface(0, 7, 5, 9, -2))));


	delete[] tabTetra;
	delete[] tabNodes;
}


BOOST_AUTO_TEST_CASE(write_read_mbin1_test1)
{
	using namespace formatMBIN;


	CMBIN driver;


	t_binNode tabNodesRef[] = {t_binNode(5, 0, 0),t_binNode(0, 0, 0),t_binNode(0, 5, 0),t_binNode(5, 5, 0),t_binNode(0, 5, 5),t_binNode(5, 5, 5)
		,t_binNode(0, 0, 5), t_binNode(5, 0, 5)};

	bintetrahedre tabTetraRef[] = {bintetrahedre(4, 5, 7, 2, 1,
		bintetraface(5, 2, 7, -1, 5), bintetraface(7, 2, 4, -1, 4), bintetraface(4, 2, 5, 2, -2), bintetraface(5, 7, 4, 10, -2)),
		bintetrahedre(6, 2, 7, 1, 1,
		bintetraface(2, 1, 7, -1, 2), bintetraface(7, 1, 6, 7, -2), bintetraface(6, 1, 2, 5, -2), bintetraface(2, 7, 6, -1, 4)),
		bintetrahedre(0, 2, 1, 7, 1,
		bintetraface(2, 7, 1, -1, 1), bintetraface(1, 7, 0, 6, -2), bintetraface(0, 7, 2, -1, 5), bintetraface(2, 1, 0, 0, -2)),
		bintetrahedre(0, 3, 2, 5, 1,
		bintetraface(3, 5, 2, 3, -2), bintetraface(2, 5, 0, -1, 5), bintetraface(0, 5, 3, 8, -2), bintetraface(3, 2, 0, 1, -2)),
		bintetrahedre(4, 2, 7, 6, 1,
		bintetraface(2, 6, 7, -1, 1), bintetraface(7, 6, 4, 11, -2), bintetraface(4, 6, 2, 4, -2), bintetraface(2, 7, 4, -1, 0)),
		bintetrahedre(5, 0, 7, 2, 1,
		bintetraface(0, 2, 7, -1, 2), bintetraface(7, 2, 5, -1, 0), bintetraface(5, 2, 0, -1, 3), bintetraface(0, 7, 5, 9, -2))};
	

	BOOST_REQUIRE(driver.ExportBIN("output_cube_mesh.mbin", tabTetraRef, tabNodesRef, sizeof(tabTetraRef) / sizeof(bintetrahedre), sizeof(tabNodesRef) / sizeof(t_binNode)));



	bintetrahedre *tabTetra = NULL;
	t_binNode *tabNodes = NULL;
	unsigned int sizeTetra = 0;
	unsigned int sizeNodes = 0;

	BOOST_REQUIRE(driver.ImportBIN("output_cube_mesh.mbin", &tabTetra, &tabNodes, sizeTetra, sizeNodes));

	// Check content
	BOOST_REQUIRE(sizeNodes == 8);


	BOOST_TEST((tabNodes[0] == t_binNode(5, 0, 0)));
	BOOST_TEST((tabNodes[1] == t_binNode(0, 0, 0)));
	BOOST_TEST((tabNodes[2] == t_binNode(0, 5, 0)));
	BOOST_TEST((tabNodes[3] == t_binNode(5, 5, 0)));
	BOOST_TEST((tabNodes[4] == t_binNode(0, 5, 5)));
	BOOST_TEST((tabNodes[5] == t_binNode(5, 5, 5)));
	BOOST_TEST((tabNodes[6] == t_binNode(0, 0, 5)));
	BOOST_TEST((tabNodes[7] == t_binNode(5, 0, 5)));

	BOOST_REQUIRE(sizeTetra == 6);

	BOOST_TEST((tabTetra[0] == bintetrahedre(4, 5, 7, 2, 1,
		bintetraface(5, 2, 7, -1, 5), bintetraface(7, 2, 4, -1, 4), bintetraface(4, 2, 5, 2, -2), bintetraface(5, 7, 4, 10, -2))));
	BOOST_TEST((tabTetra[1] == bintetrahedre(6, 2, 7, 1, 1,
		bintetraface(2, 1, 7, -1, 2), bintetraface(7, 1, 6, 7, -2), bintetraface(6, 1, 2, 5, -2), bintetraface(2, 7, 6, -1, 4))));
	BOOST_TEST((tabTetra[2] == bintetrahedre(0, 2, 1, 7, 1,
		bintetraface(2, 7, 1, -1, 1), bintetraface(1, 7, 0, 6, -2), bintetraface(0, 7, 2, -1, 5), bintetraface(2, 1, 0, 0, -2))));
	BOOST_TEST((tabTetra[3] == bintetrahedre(0, 3, 2, 5, 1,
		bintetraface(3, 5, 2, 3, -2), bintetraface(2, 5, 0, -1, 5), bintetraface(0, 5, 3, 8, -2), bintetraface(3, 2, 0, 1, -2))));
	BOOST_TEST((tabTetra[4] == bintetrahedre(4, 2, 7, 6, 1,
		bintetraface(2, 6, 7, -1, 1), bintetraface(7, 6, 4, 11, -2), bintetraface(4, 6, 2, 4, -2), bintetraface(2, 7, 4, -1, 0))));
	BOOST_TEST((tabTetra[5] == bintetrahedre(5, 0, 7, 2, 1,
		bintetraface(0, 2, 7, -1, 2), bintetraface(7, 2, 5, -1, 0), bintetraface(5, 2, 0, -1, 3), bintetraface(0, 7, 5, 9, -2))));


	delete[] tabTetra;
	delete[] tabNodes;
}