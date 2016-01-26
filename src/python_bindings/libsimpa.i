/* File : _libsimpa.i
SWIG Python lib interface module declaration
 */
%module libsimpa
%{
#include "../lib_interface/input_output/bin.h"
#include "../lib_interface/input_output/importExportMaillage/mbin.h"
%}

%include "std_vector.i"
// Instantiate templates used by libinterface
namespace std {
   %template(ioFaceVector) vector<formatCoreBIN::ioFace>;
   %template(t_posVector) vector<formatCoreBIN::t_pos>;

   %template(bintetrahedreVector) vector<formatMBIN::bintetrahedre>;
   %template(t_binNodeVector) vector<formatMBIN::t_binNode>;
}

// Exposed into python module directly

namespace formatCoreBIN
{
	class t_pos
	{
	public:
		t_pos(float _x,float _y,float _z);
		t_pos();
	};

	struct ioFace {
		indiceV a;
		indiceV b;
		indiceV c;
		indiceMat idMat;
		indiceRS idRs;
		indiceEN idEn;
	};

	struct ioModel {
		std::vector<ioFace> faces;
		std::vector<t_pos> vertices;
	};

	class CformatBIN
	{
	public:
		CformatBIN();
		~CformatBIN();
		bool ImportBIN(ioModel &modelImport, const char *strFileName);
		bool ExportBIN(const char *strFileName,ioModel &modelExport);
		float GetSurfAire( const ioModel& model, const int& faceIndex);
	};

} //namespace


namespace formatMBIN
{
	struct t_binNode
	{
		t_binNode();
		t_binNode(const formatMBIN::Floatb& x,const formatMBIN::Floatb& y,const formatMBIN::Floatb& z);
		formatMBIN::Floatb node[3];
	};

	struct bintetraface
	{
		formatMBIN::Intb sommets[3];
		formatMBIN::Intb marker;	
		formatMBIN::Intb neighboor;
	};

	struct bintetrahedre
	{
		formatMBIN::Intb sommets[4];
		formatMBIN::Intb idVolume;
		bintetraface tetrafaces[4];
	};

	struct trimeshmodel
	{
		std::vector<bintetrahedre> tetrahedres;
		std::vector<t_binNode> nodes;
	};
	
	class CMBIN
	{
	public:
		CMBIN();
		~CMBIN();
		bool ImportBIN(const char *strFileName,bintetrahedre **tabTetra,t_binNode **tabNodes,unsigned int &sizeTetra,unsigned int &sizeNodes);
		bool ExportBIN(const char *strFileName,bintetrahedre **tabTetra,t_binNode **tabNodes,unsigned int sizeTetra,unsigned int sizeNodes);
		trimeshmodel LoadMesh(const char *strFileName);
		void SaveMesh(const char *strFileName,trimeshmodel& trimesh);
		static double ComputeVolume(trimeshmodel& trimesh);
	};


} //namespace