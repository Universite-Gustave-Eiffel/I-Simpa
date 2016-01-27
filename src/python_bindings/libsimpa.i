/* File : _libsimpa.i
SWIG Python lib interface module declaration
 */

%module libsimpa
%{
#include "../lib_interface/input_output/bin.h"
#include "../lib_interface/input_output/importExportMaillage/mbin.h"
%}

%include "std_vector.i"
// Following configuration is for special bindings to Python like array access or ignoring some operator overriding
// Instantiate templates used by libinterface
namespace std {
   %template(ioFaceVector) vector<formatCoreBIN::ioFace>;
   %template(t_posVector) vector<formatCoreBIN::t_pos>;

   %template(bintetrahedreVector) vector<formatMBIN::bintetrahedre>;
   %template(t_binNodeVector) vector<formatMBIN::t_binNode>;
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

%include "../lib_interface/input_output/bin.h"
%include "../lib_interface/input_output/importExportMaillage/mbin.h"



