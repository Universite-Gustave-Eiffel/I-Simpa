
CPMAddPackage(
  NAME SWIG
  VERSION 4.2.1
  URL https://github.com/swig/swig/archive/refs/tags/v4.3.0.tar.gz
  URL_HASH SHA256=f2136da1137a20dfcec795fe0d17ca1a2465d28e3b307f122526629b6b2f2294
  OPTIONS "WITH_PCRE OFF"
    "SWIG_BUILD_EXAMPLES OFF"
    "SWIG_CSHARP OFF"
    "SWIG_JAVA OFF"
    "SWIG_JAVASCRIPT OFF"
    "SWIG_LUA OFF"
    "SWIG_OCTAVE OFF"
    "SWIG_PERL OFF"
    "SWIG_PHP OFF"
    "SWIG_PYTHON ON"
    "SWIG_R OFF"
    "SWIG_RUBY OFF"
    "SWIG_TCL OFF"
)

if(SWIG_ADDED)
  message(STATUS "SWIG added successfully: ${SWIG_SOURCE_DIR}")
  set(SWIG_EXECUTABLE ${SWIG_SOURCE_DIR}/swig)
else()
  message(FATAL_ERROR "Failed to add SWIG")
endif()
