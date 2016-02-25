include_directories (./ "${PROJECT_SOURCE_DIR}/src/lib_interface")

set(CT_SOURCE_FILES
		"TC_CalculationCore.cpp"
		"data_manager/core_configuration.cpp"
        "input_output/reportmanager.cpp"
        "main_tc.cpp")

# Set Properties->General->Configuration Type to Application(.exe)
# Creates spps.exe with the listed sources
# Adds sources to the Solution Explorer
add_executable(ct ${CT_SOURCE_FILES})

# Properties->Linker->Input->Additional Dependencies
target_link_libraries (ct lib_interface)

# Creates a folder "executables" and adds target
set_property(TARGET ct PROPERTY FOLDER "executables")

# Adds logic to INSTALL.vcproj to copy spps.exe to destination directory
install (TARGETS ct
        RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/core/ct)
