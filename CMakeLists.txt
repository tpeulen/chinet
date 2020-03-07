CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)

SET(PROJECT_NAME chinet)
PROJECT(${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")


# Debug build detection
###########################
IF(CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE("Release build")
    ADD_DEFINITIONS("-DVERBOSE=0")
ELSE(CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE("Debug build")
    ADD_DEFINITIONS("-DVERBOSE=1")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")


# 32/64 bit detection
###########################
IF(${CMAKE_SIZEOF_VOID_P} MATCHES "8")
    # If in 64bit mode, we have to define the _OCC64 variable
    ADD_DEFINITIONS(-D_OCC64)
    SET(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -D_OCC64)
    SET(BIT 64)
ELSE(${CMAKE_SIZEOF_VOID_P} MATCHES "8")
    SET(BIT 32)
ENDIF(${CMAKE_SIZEOF_VOID_P} MATCHES "8")


# Windows
###########################
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++11")
    if(BIT MATCHES "64")
        add_definitions(-DMS_WIN64)
    ENDIF(BIT MATCHES "64")
ENDIF(MSVC)

# MACOSX_DEPLOYEMENT_TARGET
###########################
# explicit link to libc++ for recent osx versions
IF(APPLE)
    MESSAGE(STATUS "link to libc++ on modern osx versions")
    ADD_DEFINITIONS(-stdlib=libc++)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    # Don't set MACOSX_RPATH by default
    set(CMAKE_MACOSX_RPATH 0)
ENDIF(APPLE)

# OpenMP
###########################
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ENDIF()
ENDIF()

# Cpp-taskflow
###########################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/cpp-taskflow/)

# Boost
###########################
FIND_PACKAGE(Boost 1.50 COMPONENTS system REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} )
LINK_LIBRARIES(${Boost_LIBRARIES})

# Eigen3
###########################
FIND_PACKAGE(Eigen3 3.2 QUIET NO_MODULE)
IF(NOT TARGET Eigen3::Eigen)
    MESSAGE(STATUS "Using thirdparty/eigen3")
    SET(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/eigen3)
ENDIF (NOT TARGET Eigen3::Eigen)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

# Python
###########################
FIND_PACKAGE(Python)
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Python_NumPy_PATH})

# IMP
###########################
FIND_PACKAGE(IMP REQUIRED)
INCLUDE(${IMP_USE_FILE})
INCLUDE_DIRECTORIES(SYSTEM ${IMP_INCLUDE_DIR})
LINK_LIBRARIES(${IMP_kernel_LIBRARY} ${IMP_core_LIBRARY})
MESSAGE("-- Found IMP")

# RTTR
###########################
FIND_PACKAGE(RTTR CONFIG REQUIRED Core)
LINK_LIBRARIES(RTTR::Core)
MESSAGE ("-- Found librttr version \"${PACKAGE_VERSION}\"")

# libmongoc
###########################
FIND_PACKAGE (libmongoc-1.0)
MESSAGE ("-- Found mongoc version \"${MONGOC_VERSION}\"")
MESSAGE ("-- include path \"${MONGOC_INCLUDE_DIRS}\"")
MESSAGE ("-- libraries \"${MONGOC_LIBRARIES}\"")
INCLUDE_DIRECTORIES (${MONGOC_INCLUDE_DIRS})
LINK_LIBRARIES (${MONGOC_LIBRARIES})
ADD_COMPILE_DEFINITIONS(${MONGOC_DEFINITIONS})

# LabelLib
###########################
#FIND_PACKAGE (FlexLabel REQUIRED)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/labellib/FlexLabel/include/)

# JSON
###########################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/json/single_include/)

# Project
###########################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(pyext)

INSTALL(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )
