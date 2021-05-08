CMAKE_MINIMUM_REQUIRED(VERSION 3.13)
SET(PROJECT_NAME chinet)
PROJECT(${PROJECT_NAME})

ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)

################################################################################
###### CMAKE
################################################################################
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

################################################################################
###### DEFINITIONS
################################################################################
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

################################################################################
###### OPTIONS
################################################################################
option(INSTALL "Install the library and the include files" ON)
option(BUILD_PYTHON_INTERFACE "Build the python interface" ON)
option(ENABLE_AVX "Build with AVX" ON)

################################################################################
###### Conda
################################################################################

## Check for Conda
###########################
IF(DEFINED ENV{CONDA_PREFIX})
    FILE(TO_CMAKE_PATH "$ENV{CONDA_PREFIX}" CONDA_PATH)
    SET(CMAKE_PREFIX_PATH "${CONDA_PATH}")
    SET(ENV{PATH} "${CONDA_PATH}/bin;$ENV{PATH}")
    # Define python_command that is used to call python and locate the python libraries
    IF(MSVC)
        SET(CMAKE_MODULE_PATH "${CONDA_PATH}/Library/cmake")
        INCLUDE_DIRECTORIES(${CONDA_PATH}/Library/include)
        LINK_DIRECTORIES(${CONDA_PATH}/Library/lib)
        SET(python_command ${CONDA_PATH}/python.exe)
    ELSE()
        SET(CMAKE_MODULE_PATH "${CONDA_PATH}/include/cmake")
        INCLUDE_DIRECTORIES(${CONDA_PATH}/include)
        LINK_DIRECTORIES(${CONDA_PATH}}/lib)
        SET(python_command ${CONDA_PATH}/bin/python)
    ENDIF()
    MESSAGE(STATUS "CONDA env seen: --[${CONDA_PATH}]--")
ELSE()
    MESSAGE(STATUS "No conda environment defined")
ENDIF()
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
MESSAGE(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

################################################################################
###### Compiler flags
################################################################################
if(LINUX)
    STRING(APPEND CMAKE_CXX_FLAGS " -march=native -Ofast -mavx -ffast-math -funroll-loops")
    STRING(APPEND CMAKE_C_FLAGS " -march=native -Ofast -mavx -ffast-math -funroll-loops")
endif()
if(APPLE)
    FIND_PACKAGE(Threads)
    # explicit link to libc++ for recent osx versions
    MESSAGE(STATUS "Explicitly link to libc++ on modern osx versions")
    ADD_DEFINITIONS(-stdlib=libc++)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
#    # Use no AVX on Apple
#    IF(ENABLE_AVX)
#        STRING(APPEND CMAKE_CXX_FLAGS " -march=native -Ofast -mavx -ffast-math -funroll-loops")
#        STRING(APPEND CMAKE_C_FLAGS " -march=native -Ofast -mavx -ffast-math -funroll-loops")
#    ENDIF(ENABLE_AVX)
endif()
if (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++11")
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
IF (CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE("Release build")
    IF(MSVC)
        # see: https://stackoverflow.com/questions/58708772/cmake-project-in-visual-studio-gives-flag-override-warnings-command-line-warnin
        string(REGEX REPLACE "/W[1||3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    ELSE()
        ADD_COMPILE_OPTIONS(-w)
    ENDIF()
ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE("Debug build")
    ## Enable Compiler warnings
    #############################
    ADD_DEFINITIONS("-DCHINET_VERBOSE=1")
    IF(MSVC)
        ADD_COMPILE_OPTIONS(/W4)
    ELSE()
        ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic)
    ENDIF()
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Release")


################################################################################
###### INCLUDES AND LIBRARIES
################################################################################

## OpenMP
###########################
if (APPLE)
    # When using conda use CONDA_PREFIX
    if(DEFINED ENV{CONDA_PREFIX})
        SET(OMP_INCLUDE_DIR "${CONDA_PATH}/include")
        SET(OMP_LIB_DIR "${CONDA_PATH}/lib")
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

## Boost
###########################
IF(DEFINED ENV{CONDA_PREFIX})
    MESSAGE("Using Conda Boost")
    SET(Boost_NO_SYSTEM_PATHS ON)
    SET(Boost_NO_BOOST_CMAKE ON)
    SET(BOOST_ROOT ${CONDA_PATH})
    SET(Boost_INCLUDE_DIR ${CONDA_PATH}/include)
    SET(Boost_LIBRARY_DIR ${CONDA_PATH}/lib)
    SET(Boost_USE_STATIC_LIBS OFF)
    SET(Boost_USE_STATIC OFF)
    SET(Boost_USE_MULTITHREAD)
    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET(Boost_DEBUG ON)
        SET(Boost_DETAILED_FAILURE_MESSAGE ON)
    ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    IF (WIN32)
        # Other root dir location
        SET(BOOST_ROOT ${CONDA_PATH}/Library)
        SET(Boost_INCLUDE_DIR ${CONDA_PATH}/Library/include)
        SET(Boost_LIBRARY_DIR ${CONDA_PATH}/Library/lib)
        # Win Boost Conda (Anaconda) uses static linking
        # Conda-forge dynamic linking?
        # SET(Boost_USE_STATIC_LIBS ON)
        # SET(Boost_USE_STATIC ON)
    ENDIF(WIN32)
ENDIF()
MESSAGE(${BOOST_ROOT})
FIND_PACKAGE(Boost 1.36 REQUIRED COMPONENTS filesystem iostreams locale)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_LIBRARIES(${Boost_LIBRARIES})
MESSAGE(${Boost_INCLUDE_DIRS})


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

################################################################################
###### Targets
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

## Compiler warnings
########################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
