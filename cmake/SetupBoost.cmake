macro(SetupBoost)
    find_package(Boost 1.86.0 COMPONENTS system algorithm random filesystem regex thread chrono date_time python unit_test_framework)

    if(NOT Boost_FOUND)
        message(STATUS "Boost not found")
        message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
        message(STATUS "Boost_LIBRARIES=${Boost_LIBRARIES}")
        message(STATUS "Boost_VERSION=${Boost_VERSION}")

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            CPMAddPackage(
              NAME Boost
              VERSION 1.86.0
              URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
              URL_HASH SHA256=2c5ec5edcdff47ff55e27ed9560b0a0b94b07bd07ed9928b476150e16b0efc57
              OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_ENABLE_PYTHON ON" "BOOST_INCLUDE_LIBRARIES system\\\;algorithm\\\;random\\\;filesystem\\\;regex\\\;python\\\;numeric/conversion\\\;test" # Note the escapes!
            )
        else()
          find_package (Threads)
            CPMAddPackage(
              NAME Boost
              VERSION 1.86.0
              URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
              URL_HASH SHA256=2c5ec5edcdff47ff55e27ed9560b0a0b94b07bd07ed9928b476150e16b0efc57
              OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_ENABLE_PYTHON ON" "BOOST_INCLUDE_LIBRARIES system\\\;algorithm\\\;random\\\;filesystem\\\;regex\\\;thread\\\;chrono\\\;test\\\;date_time\\\;python\\\;numeric/conversion" # Note the escapes!
            )
        endif()
    else()
        message(STATUS "Boost found")
        message(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
        message(STATUS "Boost_LIBRARIES=${Boost_LIBRARIES}")
        message(STATUS "Boost_VERSION=${Boost_VERSION}")
    endif()
endmacro(SetupBoost)
