
# Add an executable to the project using the specified source files.
add_executable(tetgen tetgen.cxx predicates.cxx)

# Creates a folder "executables" and adds target
set_property(TARGET tetgen PROPERTY FOLDER "executables")

#--------------#
#    INSTALL
#--------------#

# Adds logic to INSTALL.vcproj to copy tetgen to destination directory
install (TARGETS tetgen
        RUNTIME DESTINATION meshing/tetgen)

install (FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/README
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
        DESTINATION meshing/tetgen)