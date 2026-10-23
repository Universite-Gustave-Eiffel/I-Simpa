#define BOOST_TEST_MODULE lib_interface modelio tests
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <input_output/bin.h>
#include <input_output/gabe/stdgabe.h>
#include <input_output/poly/poly.h>
#include <input_output/importExportMaillage/mbin.h>
#include <input_output/exportRecepteurSurf/rsbin.h>
#include <input_output/exportRecepteurSurf/std_rsbin.hpp>
#include <input_output/gpl_io.hpp>
#include <input_output/baseReportManager.h>
#include <std_tools.hpp>
#include <cmath>
#include <fstream>
#include <stdexcept>

using namespace std;
namespace utf = boost::unit_test;

std::string GetDataDirectory() {
	auto& suite = boost::unit_test::framework::master_test_suite();
	BOOST_REQUIRE_MESSAGE(suite.argc >= 2, "Data directory path must be provided as the first command line argument.");
	return suite.argv[1];
}

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
	std::string data_dir = GetDataDirectory();

	BOOST_REQUIRE(driver.ImportBIN(modelTest, (data_dir + st_path_separator() + "cube.cbin").c_str()));

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
	std::string data_dir = GetDataDirectory();
	using namespace formatMBIN;

	CMBIN driver;

	bintetrahedre *tabTetra = NULL;
	t_binNode *tabNodes = NULL;
	unsigned int sizeTetra = 0;
	unsigned int sizeNodes = 0;

	BOOST_REQUIRE(driver.ImportBIN((data_dir + st_path_separator() + "cube_mesh.mbin").c_str(), &tabTetra, &tabNodes, sizeTetra, sizeNodes));

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
	std::string data_dir = GetDataDirectory();
	using namespace formatPOLY;
	t_model model;
	CPoly reader;	

	BOOST_REQUIRE(boost::filesystem::exists(data_dir + st_path_separator() + "test_import1.poly"));

	BOOST_REQUIRE(reader.ImportPOLY(model, data_dir + st_path_separator() + "test_import1.poly"));

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

// Check import of surface receiver file
BOOST_AUTO_TEST_CASE(surfaceReceiverTest1) {
	using namespace formatRSBIN;
	std::string data_dir = GetDataDirectory();
	t_ExchangeData data;
	RSBIN rsbin;
	string path = data_dir + st_path_separator() + "rs_cut.csbin";
	BOOST_REQUIRE(boost::filesystem::exists(path));
	BOOST_CHECK(rsbin.ImportBIN(path.c_str(), data));
	BOOST_CHECK_EQUAL(5, data.nbTimeStep);
	BOOST_CHECK_EQUAL(data.recordType, RECEPTEURS_RECORD_TYPE_SPL_STANDART);
}

// Check that a bare relative palette path (issue #420) is resolved against the
// application resources folder instead of the process' current working directory.
BOOST_AUTO_TEST_CASE(gplRelativePathResolutionTest) {
	using namespace GPL_IO;
	namespace fs = boost::filesystem;

	// Work in a private, newly created temporary directory: create_directory fails if
	// the path already exists, so nothing planted beforehand (e.g. a symbolic link in a
	// shared temp folder) can be written through. Removed when the test ends.
	struct TemporaryDirectory {
		fs::path path;
		TemporaryDirectory() : path(fs::temp_directory_path() / fs::unique_path("isimpa_gpl_test_%%%%-%%%%-%%%%-%%%%")) {
			if (!fs::create_directory(path))
				throw std::runtime_error("temporary directory already exists: " + path.string());
			fs::permissions(path, fs::owner_all);
		}
		~TemporaryDirectory() { boost::system::error_code ec; fs::remove_all(path, ec); }
	};
	TemporaryDirectory testRoot;

	const std::string resourcesFolder = (testRoot.path / "resources").string();
	const std::string relativePalette = "Bitmaps" + st_path_separator() + "iso" + st_path_separator() + "jet.gpl";
	fs::create_directories(fs::path(resourcesFolder) / "Bitmaps" / "iso");
	{
		std::ofstream palette((fs::path(resourcesFolder) / "Bitmaps" / "iso" / "jet.gpl").string());
		palette << "GIMP Palette\nName: Test palette\nColumns: 16\n#\n255   0   0\tUntitled\n";
	}

	// A relative path is resolved against the resources folder, not the cwd
	std::string resolved = ResolvePalettePath(relativePalette, resourcesFolder);
	BOOST_CHECK(resolved == resourcesFolder + st_path_separator() + relativePalette);

	// An absolute path is left untouched
	BOOST_CHECK(ResolvePalettePath(resolved, resourcesFolder) == resolved);

	// The working directory is not the resources folder: the raw relative path
	// is not found there, the resolved one is.
	struct WorkingDirectoryGuard {
		fs::path previous;
		explicit WorkingDirectoryGuard(const fs::path& p) : previous(fs::current_path()) { fs::current_path(p); }
		~WorkingDirectoryGuard() { fs::current_path(previous); }
	};
	fs::path otherWorkingDirectory = testRoot.path / "cwd";
	fs::create_directories(otherWorkingDirectory);
	WorkingDirectoryGuard guard(otherWorkingDirectory);
	GPL_FileExchange reader;
	BOOST_CHECK_EQUAL(-1, reader.Read(relativePalette));
	BOOST_CHECK_EQUAL(0, reader.Read(resolved));
	BOOST_CHECK_EQUAL(1u, reader.GetColorsCount());
}

// A vertex whose surrounding faces recorded no energy has quantFacesLinked == 0.
// Dividing the accumulated energy by zero produced NaN, which broke the smooth
// color shading of every triangle sharing that vertex (issue #425).
BOOST_AUTO_TEST_CASE(surfaceReceiverNodeEnergyAverageGuardTest) {
	using namespace formatRSBIN;
	BOOST_CHECK_EQUAL(AverageNodeEnergy(6.f, 3), 2.f);
	float averaged = AverageNodeEnergy(0.f, 0);
	BOOST_CHECK(std::isfinite(averaged));
	BOOST_CHECK_EQUAL(averaged, 0.f);
}

// The min/max range used by the iso-contouring must include the first vertex of
// every triangle: the original macro only tested the 2nd and 3rd ones (issue #425).
BOOST_AUTO_TEST_CASE(isoContouringFirstVertexMinMaxTest) {
	using namespace formatRSBIN;

	// A range already narrowed to [50, 60]: only the first vertex reaches 100 and 10
	float min(50.f), max(60.f);
	UpdateMinMax(100.f, 10.f, 10.f, min, max);
	BOOST_CHECK_EQUAL(max, 100.f);
	BOOST_CHECK_EQUAL(min, 10.f);

	// End to end. The extreme values sit on the first vertex of a middle face, so
	// the value used to seed the range (the last face processed) does not hide them.
	rsurf_data data;
	data.Make(8, 1, 1, 0.1f, "TR");
	data.MakeRs(0, 4, "iso test", 1);
	for (std::size_t idnode = 0; idnode < 8; idnode++)
		data.SetNodeValue(idnode, (float)idnode, 0.f, 0.f);

	// face 0: vertex 0 = 20, vertices 1 and 2 diluted by the other faces
	data.SetFaceInfo(0, 0, 0, 1, 2, 1);
	data.SetFaceEnergy(0, 0, 0, 0, 20.f);
	// face 1: vertex 3 carries the max (100), the 2 other vertices are 60
	data.SetFaceInfo(0, 1, 3, 1, 2, 1);
	data.SetFaceEnergy(0, 1, 0, 0, 100.f);
	// face 2: vertex 4 carries the min (1), the 2 other vertices are ~40
	data.SetFaceInfo(0, 2, 4, 1, 2, 1);
	data.SetFaceEnergy(0, 2, 0, 0, 1.f);
	// face 3 (last): seeds the range with a mid value (30)
	data.SetFaceInfo(0, 3, 5, 6, 7, 1);
	data.SetFaceEnergy(0, 3, 0, 0, 30.f);

	rsurf_iso_contouring contouring(data, 0);
	float reportedMin(0.f), reportedMax(0.f);
	contouring.GetMinMax(reportedMin, reportedMax);
	BOOST_CHECK_CLOSE(reportedMin, 1.f, 1e-6);
	BOOST_CHECK_CLOSE(reportedMax, 100.f, 1e-6);
}

// t_FaceRS::sommetsIndex is int32 but ivec3 stores long (8 bytes on LP64 platforms).
// memcpy(..., sizeof(ivec3)) wrote 24 bytes into the 12-byte field, so every exported
// face ended up with vertices (a, 0, b) instead of (a, b, c) (issue #425).
BOOST_AUTO_TEST_CASE(surfaceReceiverVertexIndexTest) {
	using namespace formatRSBIN;

	const int expected[2][3] = {{0, 1, 2}, {3, 4, 5}};

	r_Surf receiver;
	receiver.InitFaces(2, 1);
	receiver.idRecepteurS = 1;
	for (uentier idface = 0; idface < 2; idface++) {
		receiver.faces[idface].InitFreq(0, 3);
		receiver.faces[idface].indiceSommets.set(expected[idface][0], expected[idface][1], expected[idface][2]);
		// A non degenerate triangle, needed when the value is divided by the surface
		receiver.faces[idface].sommets[0] = vec3(0.f, 0.f, 0.f);
		receiver.faces[idface].sommets[1] = vec3(1.f, 0.f, 0.f);
		receiver.faces[idface].sommets[2] = vec3(0.f, 1.f, 0.f);
		receiver.faces[idface].energieRecu[0][0] = 1.f;
	}

	t_Mesh mesh;
	mesh.pvert_size = 6;
	mesh.pvertices = new vec3[mesh.pvert_size];
	for (uentier idnode = 0; idnode < mesh.pvert_size; idnode++)
		mesh.pvertices[idnode] = vec3((decimal)idnode, 1.f, 0.f);

	std::vector<r_Surf*> receivers;
	receivers.push_back(&receiver);

	// Written in the working directory, like the other io tests
	std::string exportedFile = "test_rs_vertex_wr.csbin";
	BaseReportManager::SauveRecepteursSurfaciques(CoreString(exportedFile), 0, receivers, mesh, 0.1f, true);

	t_ExchangeData data;
	RSBIN rsbin;
	BOOST_CHECK(rsbin.ImportBIN(exportedFile.c_str(), data));
	BOOST_CHECK_EQUAL(1, data.tabRsSize);
	BOOST_CHECK_EQUAL(2, data.tabRs[0].dataRec.quantFaces);
	for (int idface = 0; idface < 2; idface++) {
		for (int idvert = 0; idvert < 3; idvert++)
			BOOST_CHECK_EQUAL(expected[idface][idvert], data.tabRs[0].dataFaces[idface].dataFace.sommetsIndex[idvert]);
	}
}
