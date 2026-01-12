# Add an executable to the project using the specified source files.
add_executable(tetgen tetgen.cxx predicates.cxx)

# Creates a folder "executables" and adds target
set_property(TARGET tetgen PROPERTY FOLDER "executables")
