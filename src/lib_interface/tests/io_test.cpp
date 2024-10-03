#define BOOST_TEST_MODULE lib_interface modelio tests
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <input_output/bin.h>
#include <input_output/gabe/stdgabe.h>
#include <input_output/poly/poly.h>
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





BOOST_AUTO_TEST_CASE(read_poly_test1)
{
	using namespace formatPOLY;
	t_model model;
	CPoly reader;	

	BOOST_REQUIRE(boost::filesystem::exists("test_import1.poly"));

	BOOST_REQUIRE(reader.ImportPOLY(model, "test_import1.poly"));

	BOOST_REQUIRE(model.modelVertices.size() == 19);
	
	BOOST_CHECK(model.modelVertices[0] == dvec3(5.5, 0, 0));
	BOOST_CHECK(model.modelVertices[1] == dvec3(0, -0, 0));
	BOOST_CHECK(model.modelVertices[2] == dvec3(0, 8.1000004, 0));
	BOOST_CHECK(model.modelVertices[3] == dvec3(5.5, 8.1000004, 0));
	BOOST_CHECK(model.modelVertices[4] == dvec3(0, 8.1000004, 3.2));
	BOOST_CHECK(model.modelVertices[5] == dvec3(5.5, 8.1000004, 3.2));
	BOOST_CHECK(model.modelVertices[6] == dvec3(0, -0, 3.2));
	BOOST_CHECK(model.modelVertices[7] == dvec3(5.5, -0, 3.2));
	BOOST_CHECK(model.modelVertices[8] == dvec3(3.7532663, 2.4895077, 0));
	BOOST_CHECK(model.modelVertices[9] == dvec3(1.7356972, 2.4895077, 0));
	BOOST_CHECK(model.modelVertices[10] == dvec3(1.7356972, 5.795608, 0));
	BOOST_CHECK(model.modelVertices[11] == dvec3(3.7532663, 5.795608, 0));
	BOOST_CHECK(model.modelVertices[12] == dvec3(1.7356972, 5.795608, 1.1));
	BOOST_CHECK(model.modelVertices[13] == dvec3(3.7532663, 5.795608, 1.1));
	BOOST_CHECK(model.modelVertices[14] == dvec3(1.7356972, 2.4895077, 1.1));
	BOOST_CHECK(model.modelVertices[15] == dvec3(3.7532663, 2.4895077, 1.1));
	BOOST_CHECK(model.modelVertices[16] == dvec3(3.7532661, 2.5724626, 0));
	BOOST_CHECK(model.modelVertices[17] == dvec3(1.7356973, 5.5437918, 0));
	BOOST_CHECK(model.modelVertices[18] == dvec3(1.7356972, 5.5054293, 0));

	BOOST_REQUIRE(model.modelFaces.size() == 39);

	BOOST_CHECK(model.modelFaces[0] == t_face(0, 1, 9, 0));
	BOOST_CHECK(model.modelFaces[1] == t_face(0, 16, 3, 1));
	BOOST_CHECK(model.modelFaces[2] == t_face(2, 4, 5, 2));
	BOOST_CHECK(model.modelFaces[3] == t_face(2, 5, 3, 3));
	BOOST_CHECK(model.modelFaces[4] == t_face(2, 6, 4, 4));
	BOOST_CHECK(model.modelFaces[5] == t_face(2, 1, 6, 5));
	BOOST_CHECK(model.modelFaces[6] == t_face(1, 0, 7, 6));
	BOOST_CHECK(model.modelFaces[7] == t_face(6, 1, 7, 7));
	BOOST_CHECK(model.modelFaces[8] == t_face(0, 3, 5, 8));
	BOOST_CHECK(model.modelFaces[9] == t_face(7, 0, 5, 9));
	BOOST_CHECK(model.modelFaces[10] == t_face(7, 5, 4, 10));
	BOOST_CHECK(model.modelFaces[11] == t_face(6, 7, 4, 11));
	BOOST_CHECK(model.modelFaces[12] == t_face(10, 12, 13, 14));
	BOOST_CHECK(model.modelFaces[13] == t_face(10, 13, 11, 15));
	BOOST_CHECK(model.modelFaces[14] == t_face(10, 14, 12, 16));
	BOOST_CHECK(model.modelFaces[15] == t_face(10, 17, 14, 17));
	BOOST_CHECK(model.modelFaces[16] == t_face(9, 8, 15, 18));
	BOOST_CHECK(model.modelFaces[17] == t_face(14, 9, 15, 19));
	BOOST_CHECK(model.modelFaces[18] == t_face(8, 16, 13, 20));
	BOOST_CHECK(model.modelFaces[19] == t_face(15, 8, 13, 21));
	BOOST_CHECK(model.modelFaces[20] == t_face(15, 13, 12, 22));
	BOOST_CHECK(model.modelFaces[21] == t_face(14, 15, 12, 23));
	BOOST_CHECK(model.modelFaces[22] == t_face(0, 9, 8, 0));
	BOOST_CHECK(model.modelFaces[23] == t_face(9, 1, 2, 0));
	BOOST_CHECK(model.modelFaces[24] == t_face(16, 17, 10, 1));
	BOOST_CHECK(model.modelFaces[25] == t_face(16, 10, 11, 1));
	BOOST_CHECK(model.modelFaces[26] == t_face(10, 2, 3, 1));
	BOOST_CHECK(model.modelFaces[27] == t_face(16, 11, 3, 1));
	BOOST_CHECK(model.modelFaces[28] == t_face(11, 10, 3, 1));
	BOOST_CHECK(model.modelFaces[29] == t_face(17, 18, 14, 17));
	BOOST_CHECK(model.modelFaces[30] == t_face(0, 8, 16, 0));
	BOOST_CHECK(model.modelFaces[31] == t_face(8, 9, 18, 0));
	BOOST_CHECK(model.modelFaces[32] == t_face(16, 11, 13, 20));
	BOOST_CHECK(model.modelFaces[33] == t_face(16, 8, 17, 0));
	BOOST_CHECK(model.modelFaces[34] == t_face(17, 2, 10, 1));
	BOOST_CHECK(model.modelFaces[35] == t_face(17, 8, 18, 0));
	BOOST_CHECK(model.modelFaces[36] == t_face(18, 9, 14, 17));
	BOOST_CHECK(model.modelFaces[37] == t_face(18, 9, 2, 0));
	BOOST_CHECK(model.modelFaces[38] == t_face(18, 2, 17, 0));

	BOOST_REQUIRE(model.modelRegions.size() == 1);

	BOOST_CHECK(model.modelRegions[0].dotInRegion == vec3(3.7530646, 5.7952776, 1.09989));
	BOOST_CHECK(model.modelRegions[0].regionIndex == 3119);
	BOOST_CHECK(model.modelRegions[0].regionRefinement == -1);

}




BOOST_AUTO_TEST_CASE(write_read_poly_test1)
{
	using namespace formatPOLY;
	t_model model;

	model.modelVertices.push_back(dvec3(5.5, 0, 0));
	model.modelVertices.push_back(dvec3(0, -0, 0));
	model.modelVertices.push_back(dvec3(0, 8.1000004, 0));
	model.modelVertices.push_back(dvec3(5.5, 8.1000004, 0));
	model.modelVertices.push_back(dvec3(0, 8.1000004, 3.2));
	model.modelVertices.push_back(dvec3(5.5, 8.1000004, 3.2));
	model.modelVertices.push_back(dvec3(0, -0, 3.2));
	model.modelVertices.push_back(dvec3(5.5, -0, 3.2));
	model.modelVertices.push_back(dvec3(3.7532663, 2.4895077, 0));
	model.modelVertices.push_back(dvec3(1.7356972, 2.4895077, 0));
	model.modelVertices.push_back(dvec3(1.7356972, 5.795608, 0));
	model.modelVertices.push_back(dvec3(3.7532663, 5.795608, 0));
	model.modelVertices.push_back(dvec3(1.7356972, 5.795608, 1.1));
	model.modelVertices.push_back(dvec3(3.7532663, 5.795608, 1.1));
	model.modelVertices.push_back(dvec3(1.7356972, 2.4895077, 1.1));
	model.modelVertices.push_back(dvec3(3.7532663, 2.4895077, 1.1));
	model.modelVertices.push_back(dvec3(3.7532661, 2.5724626, 0));
	model.modelVertices.push_back(dvec3(1.7356973, 5.5437918, 0));
	model.modelVertices.push_back(dvec3(1.7356972, 5.5054293, 0));



	model.modelFaces.push_back(t_face(0, 1, 9, 0));
	model.modelFaces.push_back(t_face(0, 16, 3, 1));
	model.modelFaces.push_back(t_face(2, 4, 5, 2));
	model.modelFaces.push_back(t_face(2, 5, 3, 3));
	model.modelFaces.push_back(t_face(2, 6, 4, 4));
	model.modelFaces.push_back(t_face(2, 1, 6, 5));
	model.modelFaces.push_back(t_face(1, 0, 7, 6));
	model.modelFaces.push_back(t_face(6, 1, 7, 7));
	model.modelFaces.push_back(t_face(0, 3, 5, 8));
	model.modelFaces.push_back(t_face(7, 0, 5, 9));
	model.modelFaces.push_back(t_face(7, 5, 4, 10));
	model.modelFaces.push_back(t_face(6, 7, 4, 11));
	model.modelFaces.push_back(t_face(10, 12, 13, 14));
	model.modelFaces.push_back(t_face(10, 13, 11, 15));
	model.modelFaces.push_back(t_face(10, 14, 12, 16));
	model.modelFaces.push_back(t_face(10, 17, 14, 17));
	model.modelFaces.push_back(t_face(9, 8, 15, 18));
	model.modelFaces.push_back(t_face(14, 9, 15, 19));
	model.modelFaces.push_back(t_face(8, 16, 13, 20));
	model.modelFaces.push_back(t_face(15, 8, 13, 21));
	model.modelFaces.push_back(t_face(15, 13, 12, 22));
	model.modelFaces.push_back(t_face(14, 15, 12, 23));
	model.modelFaces.push_back(t_face(0, 9, 8, 0));
	model.modelFaces.push_back(t_face(9, 1, 2, 0));
	model.modelFaces.push_back(t_face(16, 17, 10, 1));
	model.modelFaces.push_back(t_face(16, 10, 11, 1));
	model.modelFaces.push_back(t_face(10, 2, 3, 1));
	model.modelFaces.push_back(t_face(16, 11, 3, 1));
	model.modelFaces.push_back(t_face(11, 10, 3, 1));
	model.modelFaces.push_back(t_face(17, 18, 14, 17));
	model.modelFaces.push_back(t_face(0, 8, 16, 0));
	model.modelFaces.push_back(t_face(8, 9, 18, 0));
	model.modelFaces.push_back(t_face(16, 11, 13, 20));
	model.modelFaces.push_back(t_face(16, 8, 17, 0));
	model.modelFaces.push_back(t_face(17, 2, 10, 1));
	model.modelFaces.push_back(t_face(17, 8, 18, 0));
	model.modelFaces.push_back(t_face(18, 9, 14, 17));
	model.modelFaces.push_back(t_face(18, 9, 2, 0));
	model.modelFaces.push_back(t_face(18, 2, 17, 0));

	t_region region;
	region.dotInRegion = vec3(3.7530646, 5.7952776, 1.09989);
	region.regionIndex = 3119;
	region.regionRefinement = -1;
	model.modelRegions.push_back(region);

	CPoly driver;

	BOOST_REQUIRE(driver.ExportPOLY(model, "test_write1.poly"));
	
	BOOST_REQUIRE(boost::filesystem::exists("test_write1.poly"));

	model.modelVertices.clear();
	model.modelFaces.clear();
	model.modelRegions.clear();
	model.saveFaceIndex = false;
	model.userDefinedFaces.clear();



	BOOST_REQUIRE(driver.ImportPOLY(model, "test_write1.poly"));

	BOOST_REQUIRE(model.modelVertices.size() == 19);

	BOOST_CHECK(model.modelVertices[0] == dvec3(5.5, 0, 0));
	BOOST_CHECK(model.modelVertices[1] == dvec3(0, -0, 0));
	BOOST_CHECK(model.modelVertices[2] == dvec3(0, 8.1000004, 0));
	BOOST_CHECK(model.modelVertices[3] == dvec3(5.5, 8.1000004, 0));
	BOOST_CHECK(model.modelVertices[4] == dvec3(0, 8.1000004, 3.2));
	BOOST_CHECK(model.modelVertices[5] == dvec3(5.5, 8.1000004, 3.2));
	BOOST_CHECK(model.modelVertices[6] == dvec3(0, -0, 3.2));
	BOOST_CHECK(model.modelVertices[7] == dvec3(5.5, -0, 3.2));
	BOOST_CHECK(model.modelVertices[8] == dvec3(3.7532663, 2.4895077, 0));
	BOOST_CHECK(model.modelVertices[9] == dvec3(1.7356972, 2.4895077, 0));
	BOOST_CHECK(model.modelVertices[10] == dvec3(1.7356972, 5.795608, 0));
	BOOST_CHECK(model.modelVertices[11] == dvec3(3.7532663, 5.795608, 0));
	BOOST_CHECK(model.modelVertices[12] == dvec3(1.7356972, 5.795608, 1.1));
	BOOST_CHECK(model.modelVertices[13] == dvec3(3.7532663, 5.795608, 1.1));
	BOOST_CHECK(model.modelVertices[14] == dvec3(1.7356972, 2.4895077, 1.1));
	BOOST_CHECK(model.modelVertices[15] == dvec3(3.7532663, 2.4895077, 1.1));
	BOOST_CHECK(model.modelVertices[16] == dvec3(3.7532661, 2.5724626, 0));
	BOOST_CHECK(model.modelVertices[17] == dvec3(1.7356973, 5.5437918, 0));
	BOOST_CHECK(model.modelVertices[18] == dvec3(1.7356972, 5.5054293, 0));

	BOOST_REQUIRE(model.modelFaces.size() == 39);

	BOOST_CHECK(model.modelFaces[0] == t_face(0, 1, 9, 0));
	BOOST_CHECK(model.modelFaces[1] == t_face(0, 16, 3, 1));
	BOOST_CHECK(model.modelFaces[2] == t_face(2, 4, 5, 2));
	BOOST_CHECK(model.modelFaces[3] == t_face(2, 5, 3, 3));
	BOOST_CHECK(model.modelFaces[4] == t_face(2, 6, 4, 4));
	BOOST_CHECK(model.modelFaces[5] == t_face(2, 1, 6, 5));
	BOOST_CHECK(model.modelFaces[6] == t_face(1, 0, 7, 6));
	BOOST_CHECK(model.modelFaces[7] == t_face(6, 1, 7, 7));
	BOOST_CHECK(model.modelFaces[8] == t_face(0, 3, 5, 8));
	BOOST_CHECK(model.modelFaces[9] == t_face(7, 0, 5, 9));
	BOOST_CHECK(model.modelFaces[10] == t_face(7, 5, 4, 10));
	BOOST_CHECK(model.modelFaces[11] == t_face(6, 7, 4, 11));
	BOOST_CHECK(model.modelFaces[12] == t_face(10, 12, 13, 14));
	BOOST_CHECK(model.modelFaces[13] == t_face(10, 13, 11, 15));
	BOOST_CHECK(model.modelFaces[14] == t_face(10, 14, 12, 16));
	BOOST_CHECK(model.modelFaces[15] == t_face(10, 17, 14, 17));
	BOOST_CHECK(model.modelFaces[16] == t_face(9, 8, 15, 18));
	BOOST_CHECK(model.modelFaces[17] == t_face(14, 9, 15, 19));
	BOOST_CHECK(model.modelFaces[18] == t_face(8, 16, 13, 20));
	BOOST_CHECK(model.modelFaces[19] == t_face(15, 8, 13, 21));
	BOOST_CHECK(model.modelFaces[20] == t_face(15, 13, 12, 22));
	BOOST_CHECK(model.modelFaces[21] == t_face(14, 15, 12, 23));
	BOOST_CHECK(model.modelFaces[22] == t_face(0, 9, 8, 0));
	BOOST_CHECK(model.modelFaces[23] == t_face(9, 1, 2, 0));
	BOOST_CHECK(model.modelFaces[24] == t_face(16, 17, 10, 1));
	BOOST_CHECK(model.modelFaces[25] == t_face(16, 10, 11, 1));
	BOOST_CHECK(model.modelFaces[26] == t_face(10, 2, 3, 1));
	BOOST_CHECK(model.modelFaces[27] == t_face(16, 11, 3, 1));
	BOOST_CHECK(model.modelFaces[28] == t_face(11, 10, 3, 1));
	BOOST_CHECK(model.modelFaces[29] == t_face(17, 18, 14, 17));
	BOOST_CHECK(model.modelFaces[30] == t_face(0, 8, 16, 0));
	BOOST_CHECK(model.modelFaces[31] == t_face(8, 9, 18, 0));
	BOOST_CHECK(model.modelFaces[32] == t_face(16, 11, 13, 20));
	BOOST_CHECK(model.modelFaces[33] == t_face(16, 8, 17, 0));
	BOOST_CHECK(model.modelFaces[34] == t_face(17, 2, 10, 1));
	BOOST_CHECK(model.modelFaces[35] == t_face(17, 8, 18, 0));
	BOOST_CHECK(model.modelFaces[36] == t_face(18, 9, 14, 17));
	BOOST_CHECK(model.modelFaces[37] == t_face(18, 9, 2, 0));
	BOOST_CHECK(model.modelFaces[38] == t_face(18, 2, 17, 0));

	BOOST_REQUIRE(model.modelRegions.size() == 1);

	BOOST_CHECK(model.modelRegions[0].dotInRegion == vec3(3.7530646, 5.7952776, 1.09989));
	BOOST_CHECK(model.modelRegions[0].regionIndex == 3119);
	BOOST_CHECK(model.modelRegions[0].regionRefinement == -1);
}




BOOST_AUTO_TEST_CASE(write_read_gabe_test1)
{
	using namespace gabe_io;

	// Create new file
	Gabe_rw driver;

	stringarray expectedCol0({ "first row", "second row", "third row" });
	intarray expectedCol1({ 1, 2, 3 });
	floatarray expectedCol2({ 1.1f, 2.2f, 3.3f });
	floatarray expectedCol3;

	driver.AppendStrCol(expectedCol0, "first column");
	driver.AppendIntCol(expectedCol1, "second column");
	driver.AppendFloatCol(expectedCol2, "third column", 4);
	driver.AppendFloatCol(expectedCol3, "fourth column", 1);

	driver.SetReadOnly(true);

	BOOST_REQUIRE(driver.Save("write_read_gabe_test1.gabe"));

	Gabe_rw driverLoad;

	BOOST_REQUIRE(driverLoad.Load("write_read_gabe_test1.gabe"));

	BOOST_REQUIRE(driverLoad.size() == 4);

	BOOST_CHECK(driverLoad.GetColTitle(0) == "first column");
	BOOST_CHECK(driverLoad.GetColTitle(1) == "second column");
	BOOST_CHECK(driverLoad.GetColTitle(2) == "third column");
	BOOST_CHECK(driverLoad.GetColTitle(3) == "fourth column");
	
	BOOST_REQUIRE(driverLoad.GetTabTypes() == typearray({ gabe_io::COLTYPE::GABE_OBJECTTYPE_SHORTSTRING, gabe_io::COLTYPE::GABE_OBJECTTYPE_INT, gabe_io::COLTYPE::GABE_OBJECTTYPE_FLOAT,gabe_io::COLTYPE::GABE_OBJECTTYPE_FLOAT }));


	stringarray resultCol0;
	intarray resultCol1;
	floatarray resultCol2;
	floatarray resultCol3;

	driverLoad.ReadColStr(0, resultCol0);
	driverLoad.ReadColInt(1, resultCol1);
	driverLoad.ReadColFloat(2, resultCol2);
	driverLoad.ReadColFloat(3, resultCol3);

	BOOST_CHECK(resultCol0 == expectedCol0);
	BOOST_CHECK(resultCol1 == expectedCol1);
	BOOST_CHECK(resultCol2 == expectedCol2);
	BOOST_CHECK(resultCol3 == expectedCol3);

}