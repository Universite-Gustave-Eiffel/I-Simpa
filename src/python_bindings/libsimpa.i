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
%include "std_wstring.i"
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

%typemap(out) float [ANY] {
   int i;
   $result = PyList_New($1_dim0);
   for (i = 0; i < $1_dim0; i++) {
     PyObject *o = PyFloat_FromDouble((double) $1[i]);
     PyList_SetItem($result,i,o);
   }
} 


%typemap(out) long [ANY] {
   int i;
   $result = PyList_New($1_dim0);
   for (i = 0; i < $1_dim0; i++) {
     PyObject *o = PyFloat_FromLong((long) $1[i]);
     PyList_SetItem($result,i,o);
   }
} 

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
%include "../lib_interface/input_output/importExportMaillage/mbin.h"
%include "../lib_interface/input_output/poly/poly.h"
%include "../lib_interface/input_output/gabe/stdgabe.h"
%include "../lib_interface/input_output/progressionInfo.h"
%include "../lib_interface/input_output/exportRecepteurSurf/std_rsbin.hpp"
%include "../lib_interface/input_output/particles/part_io.hpp"
%include "../lib_interface/tools/vol_splitter.hpp"
%include "../lib_interface/tools/surf_merging.hpp"



%template(vec3) core_mathlib::base_vec3<float>;
%template(dvec3) core_mathlib::base_vec3<double>;
