CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

SET(PROJECT_NAME chinet)
PROJECT(${PROJECT_NAME})

# Set minimum C++ to 2011 standards
###############################
ENABLE_LANGUAGE(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)


# OpenMP
###############################
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    ENDIF()
ENDIF()


# Boost
###############################
FIND_PACKAGE(Boost 1.40 COMPONENTS system REQUIRED )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} )
LINK_LIBRARIES(${Boost_LIBRARIES})

# RTTR
###############################
find_package(RTTR CONFIG REQUIRED Core)
link_libraries(RTTR::Core)
message ("-- Found librttr version \"${PACKAGE_VERSION}\"")

# libmongoc
###############################
find_package (libmongoc-1.0 1.7 REQUIRED)
message ("-- Found mongoc version \"${MONGOC_VERSION}\"")
message ("--   include path \"${MONGOC_INCLUDE_DIRS}\"")
message ("--   libraries \"${MONGOC_LIBRARIES}\"")
include_directories (${MONGOC_INCLUDE_DIRS})
link_libraries (${MONGOC_LIBRARIES})
add_compile_definitions(${MONGOC_DEFINITIONS})


#[[
# libbson
###############################
find_package (libbson-1.0 1.7 REQUIRED)
message ("-- Found libbson version \"${BSON_VERSION}\"")
message ("--   include path \"${BSON_INCLUDE_DIRS}\"")
message ("--   libraries \"${BSON_LIBRARIES}\"")
link_libraries(${BSON_LIBRARIES})
include_directories(${BSON_INCLUDE_DIRS})

# libmongocxx
###############################
find_package(libmongocxx REQUIRED)
link_libraries(${LIBMONGOCXX_LIBRARIES})
include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
message("-- libmongocxx libraries \"${LIBMONGOCXX_LIBRARIES}\"")
message("-- libmongocxx include \"${LIBMONGOCXX_INCLUDE_DIRS}\"")

# JSON
###############################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/json/single_include/)

]]

# Project
###############################
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
