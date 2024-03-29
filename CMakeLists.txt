CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
ENABLE_LANGUAGE(CXX)
SET(CMAKE_CXX_STANDARD 11)
SET(PROJECT_NAME chinet)
PROJECT(${PROJECT_NAME})

## CMAKE
################################################################################
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

## DEFINITIONS
################################################################################
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

## OPTIONS
################################################################################
option(INSTALL "Install the library and the include files" ON)
option(BUILD_PYTHON_INTERFACE "Build the python interface" ON)
# Turned of because of alignment issues in MVSC 2015
option(ENABLE_AVX "Build with AVX" OFF) 

## Conda
################################################################################

## Check for Conda
###########################
FIND_PACKAGE(Conda)

## Compiler flags
################################################################################
if(LINUX)
    STRING(APPEND CMAKE_CXX_FLAGS " -march=native -O3 -mavx")
    STRING(APPEND CMAKE_C_FLAGS " -march=native -O3 -mavx")
endif()
if(APPLE)
    FIND_PACKAGE(Threads)
    # explicit link to libc++ for recent osx versions
    MESSAGE(STATUS "Explicitly link to libc++ on modern osx versions")
    ADD_DEFINITIONS(-stdlib=libc++)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    # Use no AVX on Apple
    IF(ENABLE_AVX)
        STRING(APPEND CMAKE_CXX_FLAGS " -march=native -O3 -mavx")
        STRING(APPEND CMAKE_C_FLAGS " -march=native -O3 -mavx")
    ENDIF(ENABLE_AVX)
endif()
if (MSVC)
    ADD_DEFINITIONS(-DMS_WIN64)
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
    # https://devblogs.microsoft.com/cppblog/simd-extension-to-c-openmp-in-visual-studio/
    # /Oi is for intrinsics
    IF(ENABLE_AVX)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 -openmp:experimental /Oi /O2")
    ENDIF(ENABLE_AVX)
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
IF(MSVC)
    # see: https://stackoverflow.com/questions/58708772/cmake-project-in-visual-studio-gives-flag-override-warnings-command-line-warnin
    string(REGEX REPLACE "/W[1||3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    if(MSVC AND ${MSVC_VERSION} GREATER_EQUAL 1915)
        add_definitions( "-D_ENABLE_EXTENDED_ALIGNED_STORAGE" )
    endif()
ELSE()
    ADD_COMPILE_OPTIONS(-w)
ENDIF()
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE("Debug build")
    ## Enable Compiler warnings
    #############################
    ADD_DEFINITIONS("-DCHINET_VERBOSE=1")
    IF(MSVC)
        ADD_COMPILE_OPTIONS(/W4)
    ELSE()
        ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic)
    ENDIF()
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")


## INCLUDES AND LIBRARIES
################################################################################

## OpenMP
###########################
FIND_PACKAGE(OpenMP REQUIRED)
LINK_LIBRARIES(OpenMP::OpenMP_CXX)


## Boost
###########################
FIND_PACKAGE(CondaBoost)
FIND_PACKAGE(Boost 1.36 REQUIRED COMPONENTS filesystem iostreams locale)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})
MESSAGE(${Boost_INCLUDE_DIRS})

# RTTR
###########################
FIND_PACKAGE(RTTR CONFIG REQUIRED Core)
LINK_LIBRARIES(RTTR::Core)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
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

## Targets
################################################################################
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

IF(BUILD_PYTHON_INTERFACE)
    ADD_SUBDIRECTORY(pyext)
ENDIF(BUILD_PYTHON_INTERFACE)

IF(INSTALL)
INSTALL(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )
ENDIF(INSTALL)

