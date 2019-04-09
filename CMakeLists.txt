CMAKE_MINIMUM_REQUIRED(VERSION 3.13)

SET(PROJECT_NAME fluomodlib)
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
FIND_PACKAGE(Boost 1.40 COMPONENTS system filesystem REQUIRED )
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} )
LINK_LIBRARIES(${Boost_LIBRARIES})


# libBson
###############################
find_package (libbson-1.0 1.7 REQUIRED)
message ("libbson version \"${BSON_VERSION}\"")
message ("libbson include path \"${BSON_INCLUDE_DIRS}\"")
message ("libbson libraries \"${BSON_LIBRARIES}\"")
link_libraries(${BSON_LIBRARIES})
include_directories(${BSON_INCLUDE_DIRS})

# libBsonCxx
###############################
find_package(libbsoncxx REQUIRED)
link_libraries(${LIBBSONCXX_LIBRARIES})
include_directories(${LIBBSONCXX_INCLUDE_DIRS})
message(STATUS "${LIBBSONCXX_LIBRARIES}")
message(STATUS "${LIBBSONCXX_INCLUDE_DIRS}")

# libMongoCxx
###############################
find_package(libmongocxx REQUIRED)
link_libraries(${LIBMONGOCXX_LIBRARIES})
include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
message(STATUS "${LIBMONGOCXX_LIBRARIES}")
message(STATUS "${LIBMONGOCXX_INCLUDE_DIRS}")

# JSON
###############################
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/json/single_include/)

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
