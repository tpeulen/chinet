IF(CONDA_PATH)
    MESSAGE("Using Conda Boost")
    SET(BOOST_ROOT ${CONDA_PATH})
    SET(Boost_NO_SYSTEM_PATHS ON)
    SET(Boost_NO_BOOST_CMAKE ON)
    SET(BOOST_INCLUDEDIR ${CONDA_PATH}/include)
    SET(BOOST_LIBRARYDIR ${CONDA_PATH}/lib)
    IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
        SET(Boost_DEBUG ON)
        SET(Boost_DETAILED_FAILURE_MESSAGE ON)
    ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    IF (WIN32)
        # Other root dir location
        SET(BOOST_ROOT ${CONDA_PATH}/Library)
        SET(Boost_INCLUDE_DIR ${CONDA_PATH}/Library/include)
        SET(Boost_LIBRARY_DIR ${CONDA_PATH}/Library/lib)
        # Win Boost Conda uses static linking
        # SET(Boost_USE_STATIC_LIBS ON)
        # SET(Boost_USE_STATIC ON)
    ELSE()
        SET(Boost_USE_STATIC_LIBS OFF)
        SET(Boost_USE_STATIC OFF)
        SET(Boost_USE_MULTITHREAD)
    ENDIF (WIN32)
ENDIF(CONDA_PATH)
