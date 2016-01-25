/* File : _libsimpa.i
SWIG Python lib interface module declaration
 */
%module libsimpa
%{
#include "../lib_interface/dummy.hpp"
%}

%include "std_vector.i"
// Instantiate templates used by libinterface
namespace std {
   %template(ioFaceVector) vector<formatCoreBIN::ioFace>;
   %template(t_posVector) vector<formatCoreBIN::t_pos>;

   %template(bintetrahedreVector) vector<formatMBIN::bintetrahedre>;
   %template(t_binNodeVector) vector<formatMBIN::t_binNode>;
}


// Include the header file with above prototypes

%include "../lib_interface/input_output/bin.h"
%include "../lib_interface/input_output/importExportMaillage/mbin.h"