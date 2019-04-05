CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

SET(PROJECT_NAME fluomodlib)
PROJECT(${PROJECT_NAME})

# Set minimum C++ to 2011 standards
ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)


OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ENDIF()
ENDIF()

# https://stackoverflow.com/questions/51717207/building-mongo-cxx-driver-using-cmake-externalproject-add
include(ExternalProject)
ExternalProject_Add(libmongoc
        GIT_REPOSITORY  "https://github.com/mongodb/mongo-c-driver.git"
        GIT_TAG         "1.12.0"
        GIT_PROGRESS    1
        GIT_SHALLOW     1
        SOURCE_DIR      "${CMAKE_BINARY_DIR}/libmongoc"
        BINARY_DIR      "${CMAKE_BINARY_DIR}/libmongoc-build"
        INSTALL_DIR     "${CMAKE_BINARY_DIR}/libmongoc-install"
        CMAKE_CACHE_ARGS
        ${common_cmake_cache_args}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/libmongoc-install
        -DENABLE_TESTS:BOOL=OFF
        -DENABLE_STATIC:BOOL=OFF
        -DENABLE_EXAMPLES:BOOL=OFF
        -DENABLE_EXTRA_ALIGNMENT:BOOL=OFF
        INSTALL_COMMAND ""
        )
set(libmongoc-1.0_DIR "${CMAKE_BINARY_DIR}/libmongoc-install/lib/cmake/libmongoc-1.0/")
set(libbson-1.0_DIR "${CMAKE_BINARY_DIR}/libmongoc-install/lib/cmake/libbson-1.0/")


# Specify the minimum version you require.
FIND_PACKAGE (libbson-1.0 1.7 REQUIRED)
message ("--   libbson found version \"${BSON_VERSION}\"")
message ("--   libbson include path \"${BSON_INCLUDE_DIRS}\"")
message ("--   libbson libraries \"${BSON_LIBRARIES}\"")
INCLUDE_DIRECTORIES(${BSON_INCLUDE_DIRS})
LINK_LIBRARIES(${BSON_LIBRARIES})
ADD_COMPILE_DEFINITIONS(${BSON_DEFINITIONS})

# JSON
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/json/single_include/)

# Project
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)


ADD_SUBDIRECTORY(pyext)

INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
        )
