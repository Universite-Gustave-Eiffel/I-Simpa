set(CMAKE_INSTALL_MFC_LIBRARIES TRUE) 
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE )
include( InstallRequiredSystemLibraries )

# Add an executable to the project using the specified source files.
add_executable(tetgen tetgen.cxx predicates.cxx)

# Creates a folder "executables" and adds target
set_property(TARGET tetgen PROPERTY FOLDER "executables")
