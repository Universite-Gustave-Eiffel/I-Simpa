/* File : _libsimpa.i
SWIG Python lib interface module declaration
 */

%module libsimpa
%{
#include "../lib_interface/Core/mathlib.h"
#include "../lib_interface/input_output/bin.h"
#include "../lib_interface/input_output/poly/poly.h"
#include "../lib_interface/input_output/importExportMaillage/mbin.h"
#include "../lib_interface/input_output/gabe/stdgabe.h"
#include "../lib_interface/input_output/progressionInfo.h"
#include "../lib_interface/input_output/exportRecepteurSurf/std_rsbin.hpp"
#include "../lib_interface/input_output/particles/part_io.hpp"
#include "../lib_interface/tools/vol_splitter.hpp"
#include "../lib_interface/tools/surf_merging.hpp"
#include "../lib_interface/data_manager/data_calculation/calculsPropagation.h"
%}

%include "std_vector.i"
%include "stl.i"
%include "stdint.i"
%include "std_wstring.i"
%include "typemaps.i"
%include "carrays.i"

%array_class(double, doubleArray);
%array_class(float, floatArray);
%array_class(long, longArray);

%array_class(formatMBIN::bintetraface, bintetrafaceArray);


%extend core_mathlib::ivec3 {
	%rename(Index) operator[](int) const;
	%ignore operator long*;
    long __getitem__(int i) {
        return (*($self))[i];
    }
	int __len__() {
		return 3;
	}
    %pythoncode %{
		def __repr__(self):
		  return "[%s,%s,%s]" % (self[0],self[1],self[2])
	%}
}

%extend core_mathlib::ivec4 {
	%rename(Index) operator[](int) const;
	%ignore operator long*;
    long __getitem__(int i) {
        return (*($self))[i];
    }
	int __len__() {
		return 4;
	}
}

%extend core_mathlib::base_vec3 {
	%rename(Index) operator[](int) const;
	%ignore operator double*;
    double __getitem__(int i) {
        return (*($self))[i];
    }
	int __len__() {
		return 3;
	}
    %pythoncode %{
		def __repr__(self):
		  return "[%f,%f,%f]" % (self[0],self[1],self[2])
	%}
}

// Map a Python sequence into any sized C double array
%typemap(in) double[ANY](double temp[$1_dim0]) {
  int i;
  if (!PySequence_Check($input)) {
      PyErr_SetString(PyExc_TypeError,"Expecting a sequence");
      return NULL;
  }
  if (PyObject_Length($input) != $1_dim0) {
      PyErr_SetString(PyExc_ValueError,"Expecting a sequence with $1_dim0 elements");
      return NULL;
  }
  for (i =0; i < $1_dim0; i++) {
      PyObject *o = PySequence_GetItem($input,i);
      if (!PyFloat_Check(o)) {
         Py_XDECREF(o);
         PyErr_SetString(PyExc_ValueError,"Expecting a sequence of floats");
         return NULL;
      }
      temp[i] = PyFloat_AsDouble(o);
      Py_DECREF(o);
  }
  $1 = &temp[0];
}

%extend formatMBIN::t_binNode {
	%rename(Index) operator[](int) const;
	%ignore operator float*;
    float __getitem__(int i) {
        return (*($self))[i];
    }
	int __len__() {
		return 3;
	}
}

%extend formatCoreBIN::t_pos {
	%rename(Index) operator[](int) const;
	%ignore operator float*;
    float __getitem__(int i) {
        return (*($self))[i];
    }
	int __len__() {
		return 3;
	}
}

%extend gabe_io::Gabe_rw {
	%rename(Index) operator[](std::size_t indicecol);
	%ignore ReadColStr(const std::size_t& idcol,stringarray& coldata);
	%ignore ReadColInt(const std::size_t& idcol,intarray& coldata);
	%ignore ReadColFloat(const std::size_t& idcol,floatarray& coldata);

	gabe_io::stringarray ReadColStr(const std::size_t& idcol) {
		gabe_io::stringarray ar;
		(*($self)).ReadColStr(idcol, ar);
		return ar;
	}
	
	gabe_io::intarray ReadColInt(const std::size_t& idcol) {
		gabe_io::intarray ar;
		(*($self)).ReadColInt(idcol, ar);
		return ar;
	}
	
	gabe_io::floatarray ReadColFloat(const std::size_t& idcol) {
		gabe_io::floatarray ar;
		(*($self)).ReadColFloat(idcol, ar);
		return ar;
	}

	int __len__() {
		return (*($self)).size();
	}
    gabe_io::stringarray __getitem__(int i) {
        return (*($self))[i];
    }
}

namespace CalculsGenerauxThermodynamique {
	class CCalculsGenerauxThermodynamique
	{
	public:
		static double c_son(double x);
		static double masse_vol(double P, double K);
		static double Coef_Att_Atmos(double F, double H, double P, double K);
	};
}

%include "../lib_interface/Core/mathlib.h"
%include "../lib_interface/input_output/bin.h"
%include "../lib_interface/input_output/poly/poly.h"
%include "../lib_interface/input_output/gabe/gabe.h"
%include "../lib_interface/input_output/gabe/stdgabe.h"
%include "../lib_interface/input_output/progressionInfo.h"
%include "../lib_interface/input_output/exportRecepteurSurf/std_rsbin.hpp"
%include "../lib_interface/input_output/particles/part_io.hpp"
%include "../lib_interface/tools/vol_splitter.hpp"
%include "../lib_interface/tools/surf_merging.hpp"
%include "std_except.i"

namespace formatMBIN
{

struct t_binNode
{
	t_binNode();
	t_binNode(const formatMBIN::Floatb& x,const formatMBIN::Floatb& y,const formatMBIN::Floatb& z);
	formatMBIN::Floatb node[3];
	int operator==(const formatMBIN::t_binNode &_f);
	operator float*();
	const float &operator[](int _i) const;
};

struct bintetraface {
	bintetraface();
	bintetraface(formatMBIN::Intb a, formatMBIN::Intb b, formatMBIN::Intb c, formatMBIN::Intb _marker, formatMBIN::Intb _neighboor);
	ivec3 vertices;
	int32_t marker;
	int32_t neighbor;
	int operator==(const bintetraface &_f);
};

struct bintetrahedre {
	bintetrahedre(const formatMBIN::Intb& _a, const formatMBIN::Intb& _b, const formatMBIN::Intb& _c, const formatMBIN::Intb& _d, const formatMBIN::Intb& _idVolume, 
		const formatMBIN::bintetraface& faceA, const formatMBIN::bintetraface& faceB, const formatMBIN::bintetraface& faceC, const formatMBIN::bintetraface& faceD);
	bintetrahedre();
	ivec4 vertices;
	formatMBIN::Intb idVolume;
	int operator==(const bintetrahedre &_f);
};

struct trimeshmodel
{
	std::vector<bintetrahedre> tetrahedres;
	std::vector<t_binNode> nodes;
};


/**
 *	\brief Classe de sauvegarde et de chargement du maillage tétrahedrique
 */
class CMBIN
{
public:
	/** Constructeur
	 */
	CMBIN();
	/** Destructeur
	 */
	~CMBIN();
	/// @cond INCLUDE_PYTHON_HIDDEN_MEMBERS
	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @param tabTetra Tableau de tetrahèdre
	 * @param tabNodes Tableau de noeuds
	 * @param sizeTetra Taille du tableau de tetrahèdre
	 * @param sizeNodes Taille du tableau de tetrahèdre
	 * @return Vrai si l'opération a réussi
	 */
    bool ImportBIN(const char *strFileName,bintetrahedre **tabTetra,t_binNode **tabNodes,unsigned int &sizeTetra,unsigned int &sizeNodes);
	/**
	 * Méthode d'exportation d'un modèle 3D
	 * @param strFileName Chemin du fichier
	 * @param tabTetra Tableau de tetrahèdre
	 * @param tabNodes Tableau de noeuds
	 * @param sizeTetra Taille du tableau de tetrahèdre
	 * @param sizeNodes Taille du tableau de tetrahèdre
	 * @return Vrai si l'opération a réussi
	 */
	bool ExportBIN(const char *strFileName,const bintetrahedre* tabTetra,const t_binNode* tabNodes,unsigned int sizeTetra,unsigned int sizeNodes);
	/// @endcond
	/**
	 * Méthode d'importation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @return Le maillage
	 */
	trimeshmodel LoadMesh(const char *strFileName);
	/**
	 * Méthode d'exportation d'un fichier binaire
	 * @param strFileName Chemin du fichier
	 * @param trimesh Le maillage
	 */
	bool SaveMesh(const char *strFileName,trimeshmodel& trimesh);
	/*
	 * Calcul la somme des volumes des tetrahèdre contenu de le modèle
	 * @return Volume m3
	 */
	static double ComputeVolume(trimeshmodel& trimesh);
};


}

%extend formatMBIN::bintetrahedre {
  const formatMBIN::bintetraface& getFace(int i) {  
    if (i >= 4 || i < 0)
      throw std::out_of_range("out of bounds access");
    return $self->tetrafaces[i];
  }
  void setFace(int i, const formatMBIN::bintetraface& face) {
    if (i >= 4 || i < 0)
      throw std::out_of_range("out of bounds access");
	$self->tetrafaces[i] = face;
  }
}

%template(vec3) core_mathlib::base_vec3<float>;
%template(dvec3) core_mathlib::base_vec3<double>;

// Following configuration is for special bindings to Python like array access or ignoring some operator overriding
// Instantiate templates used by libinterface
namespace std {
   %template(ioFaceVector) vector<formatCoreBIN::ioFace>;
   %template(t_posVector) vector<formatCoreBIN::t_pos>;

   %template(bintetrahedreVector) vector<formatMBIN::bintetrahedre>;
   %template(t_binNodeVector) vector<formatMBIN::t_binNode>;

   %template(stringarray) vector<std::string>;
   %template(intarray) vector<int>;
   %template(floatarray) vector<float>;
   %template(typearray) vector<formatGABE::GABE_OBJECTTYPE>;

   
   %template(polyFaceList) vector<formatPOLY::t_face>;
   %template(polyRegionList) vector<formatPOLY::t_region>;
   %template(dvec3List) vector<core_mathlib::base_vec3<double>>;
}