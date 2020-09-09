CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)

SET(PROJECT_NAME chinet)
PROJECT(${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

## Check for Conda
###########################
IF(DEFINED ENV{CONDA_PREFIX})
    MESSAGE(STATUS "CONDA env seen: --[$ENV{CONDA_PREFIX}]--")
    SET(ENV{PATH} "$ENV{CONDA_PREFIX}/bin;$ENV{PATH}")
    # Define python_command that is used to call python and locate the python libraries
    if (MSVC)
        INCLUDE_DIRECTORIES($ENV{CONDA_PREFIX}/Library/include)
        LINK_DIRECTORIES($ENV{CONDA_PREFIX}/Library/lib)
        SET(python_command $ENV{CONDA_PREFIX}/python.exe)
    ELSE()
        INCLUDE_DIRECTORIES($ENV{CONDA_PREFIX}/include)
        LINK_DIRECTORIES($ENV{CONDA_PREFIX}/lib)
        SET(python_command $ENV{CONDA_PREFIX}/bin/python)
    ENDIF()
ELSE()
    MESSAGE(STATUS "No conda environment defined")
ENDIF()
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Conda
#######
if(UNIX)
    if(DEFINED ENV{CONDA_PREFIX})
        LINK_DIRECTORIES("$ENV{CONDA_PREFIX}/lib")
        INCLUDE_DIRECTORIES("$ENV{CONDA_PREFIX}/include")
    else()
        LINK_DIRECTORIES(/usr/local/lib)
        INCLUDE_DIRECTORIES(/usr/local/include)
    endif()
endif()


# Compiler flags
#################
if(LINUX)
    STRING(APPEND CMAKE_CXX_FLAGS " -march=native -Ofast -mavx2 -ffast-math -funroll-loops")
    STRING(APPEND CMAKE_C_FLAGS " -march=native -Ofast -mavx2 -ffast-math -funroll-loops")
endif()
if(APPLE)
    FIND_PACKAGE(Threads)
    # explicit link to libc++ for recent osx versions
    MESSAGE(STATUS "Explicitly link to libc++ on modern osx versions")
    ADD_DEFINITIONS(-stdlib=libc++)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    # Requires AVX and FMA
    STRING(APPEND CMAKE_CXX_FLAGS " -march=native -Ofast -mavx2 -ffast-math -funroll-loops -mfma")
    STRING(APPEND CMAKE_C_FLAGS " -march=native -Ofast -mavx2 -ffast-math -funroll-loops -mfma")
endif()
if (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++11")
    ADD_DEFINITIONS(-DMS_WIN64)
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
    # https://devblogs.microsoft.com/cppblog/simd-extension-to-c-openmp-in-visual-studio/
    # /Oi is for intrinsics
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 -openmp:experimental /Oi /O2")
endif (MSVC)


# MACOSX
##############
if (APPLE)
    # Don't set MACOSX_RPATH by default
    # https://github.com/conda/conda/issues/3624
    if(DEFINED ENV{CONDA_PREFIX})
        set(MACOSX_RPATH OFF)
        set(CMAKE_SKIP_RPATH TRUE)
        set(CMAKE_SKIP_INSTALL_RPATH TRUE)
    endif()
endif (APPLE)

# Debug build detection
###########################
IF (CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE("Release build")
    ADD_DEFINITIONS("-DVERBOSE_FIT2X=0")
ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE("Debug build")
    ADD_DEFINITIONS("-DVERBOSE_FIT2X=1")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Release")


################################################################################
###### INCLUDES AND LIBRARIES
################################################################################

## OpenMP
###########################
if (APPLE)
    # When using conda use CONDA_PREFIX
    if(DEFINED ENV{CONDA_PREFIX})
        SET(OMP_INCLUDE_DIR "$ENV{CONDA_PREFIX}/include")
        SET(OMP_LIB_DIR "$ENV{CONDA_PREFIX}/lib")
    else()
        SET(OMP_INCLUDE_DIR "/usr/local/opt/libomp/include")
        SET(OMP_LIB_DIR "/usr/local/opt/libomp/lib")
    endif()
    # dirty hack to make OpenMP work see:
    # https://gitlab.kitware.com/cmake/cmake/issues/18098
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${OMP_INCLUDE_DIR}" CACHE INTERNAL "OpenMP flags for Xcode toolchain.")
    set(OpenMP_CXX_LIB_NAMES "omp" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I${OMP_INCLUDE_DIR}" CACHE INTERNAL "OpenMP flags for Xcode toolchain.")
    set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
    set(OpenMP_omp_LIBRARY "${OMP_LIB_DIR}/libomp.dylib" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
    set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
    set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
    set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
endif ()
FIND_PACKAGE(OpenMP REQUIRED)
LINK_LIBRARIES(OpenMP::OpenMP_CXX)


# Cpp-taskflow
###########################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/cpp-taskflow/)

## Boost
###########################
if (WIN32)
    # use static Boost in Windows
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC ON)
    set(Boost_USE_MULTITHREAD)
endif (WIN32)
FIND_PACKAGE(Boost 1.36 REQUIRED COMPONENTS date_time filesystem iostreams)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})
MESSAGE(${Boost_INCLUDE_DIRS})


# Python
###########################
FIND_PACKAGE(Python)
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Python_NumPy_PATH})


# Eigen3
###########################
FIND_PACKAGE(Eigen3 3.2 QUIET NO_MODULE)
IF(NOT TARGET Eigen3::Eigen)
    MESSAGE(STATUS "Using thirdparty/eigen3")
    SET(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/eigen3)
ENDIF (NOT TARGET Eigen3::Eigen)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})


# IMP
###########################
FIND_PACKAGE(IMP REQUIRED)
INCLUDE(${IMP_USE_FILE})
INCLUDE_DIRECTORIES(${IMP_INCLUDE_DIR})
MESSAGE(STATUS "IMP_INCLUDE_DIR='${IMP_INCLUDE_DIR}'")
MESSAGE(STATUS "IMP_kernel_LIBRARY='${IMP_kernel_LIBRARY}'")
MESSAGE(STATUS "IMP_core_LIBRARY='${IMP_core_LIBRARY}'")
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


################################################################################
###### Targets
################################################################################
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
