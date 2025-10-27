macro(SetupBoost)
    find_package(Boost 1.89.0 COMPONENTS system random filesystem regex thread chrono date_time python unit_test_framework)

    if(NOT Boost_FOUND)
        find_package (Threads)
        CPMAddPackage(
                NAME Boost
                VERSION 1.89.0
                URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.7z
                URL_HASH SHA256=252149f583054515b10688855b24ca1c16072b5118e603b8fe017894d319c262
                OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_ENABLE_PYTHON ON" "BOOST_INCLUDE_LIBRARIES system\\\;algorithm\\\;random\\\;filesystem\\\;regex\\\;thread\\\;chrono\\\;test\\\;date_time\\\;python\\\;numeric/conversion" # Note the escapes!
        )
        message(STATUS "Boost downloaded and configured via CPM")
        message(STATUS "Boost_SOURCE_DIR=${Boost_SOURCE_DIR}")
        message(STATUS "Boost_BINARY_DIR=${Boost_BINARY_DIR}")
    else()
        message(STATUS "Boost found")
        message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
        message(STATUS "Boost_LIBRARIES=${Boost_LIBRARIES}")
        message(STATUS "Boost_VERSION=${Boost_VERSION}")
    endif()

endmacro(SetupBoost)
