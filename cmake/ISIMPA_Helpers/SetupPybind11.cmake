macro(SetupPybind11)
# -------------------------------- PyBind11 -----------------------------

    # necessary for flat-mesh feature
    option(ISIMPA_USE_PYBIND11 "Use pybind11" OFF)
    if (ISIMPA_USE_PYBIND11)
        find_package(pybind11 REQUIRED)
    endif()

endmacro(SetupPybind11)
