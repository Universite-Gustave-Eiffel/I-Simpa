
# Add an executable to the project using the specified source files.
add_executable(tetgen tetgen.cxx predicates.cxx)

#--------------#
#    INSTALL
#--------------#

# Adds logic to INSTALL.vcproj to copy tetgen to destination directory
install (TARGETS tetgen
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/meshing/tetgen)

install (FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/README
        ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
        DESTINATION  ${PROJECT_BINARY_DIR}/bin/meshing/tetgen)