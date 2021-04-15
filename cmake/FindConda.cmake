
MESSAGE(STATUS "Try to find conda...")
IF(DEFINED "$ENV{CONDA_PREFIX}")
    # If a conda env is set in CONDA_PREFIX use it as path for include and libs
    MESSAGE(STATUS "CONDA env seen: --[$ENV{CONDA_PREFIX}]--")
    SET(CONDA_PATH "$ENV{CONDA_PREFIX}")
ELSE()
    # otherwise try to find the conda command and use the base env.
    MESSAGE(STATUS "No CONDA_PREFIX environment variable")
    MESSAGE(STATUS "Using location of conda bin as reference.")
    FIND_PROGRAM(CONDA_FILENAME conda)
    MESSAGE(STATUS "Location of conda bin: ${CONDA_FILENAME}")
    GET_FILENAME_COMPONENT(CONDA_BIN_PATH ${CONDA_FILENAME} DIRECTORY)
    MESSAGE(STATUS "Path of conda bin: ${CONDA_BIN_PATH}")
    SET(CONDA_PATH ${CONDA_BIN_PATH}/..)
ENDIF(DEFINED "$ENV{CONDA_PREFIX}")

IF(DEFINED CONDA_PATH)
    MESSAGE(STATUS "Using conda path: ${CONDA_PATH}")
    SET(ENV{PATH} "${CONDA_PATH}/bin;$ENV{PATH}")
    # Define python_command that is used to call python and locate the python libraries
    if (MSVC)
        INCLUDE_DIRECTORIES(${CONDA_PATH}/Library/include)
        LINK_DIRECTORIES(${CONDA_PATH}/Library/lib)
        SET(python_command ${CONDA_PATH}/python.exe)
    ELSE()
        INCLUDE_DIRECTORIES(/include)
        LINK_DIRECTORIES(${CONDA_PATH}/lib)
        SET(python_command ${CONDA_PATH}/bin/python)
    ENDIF()
    set(CMAKE_MODULE_PATH "${CONDA_PATH}/lib/cmake;${CMAKE_MODULE_PATH}")
    MESSAGE(STATUS "Setting CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
    SET(CMAKE_PREFIX_PATH "${CONDA_PATH}/lib/;${CONDA_PATH}/bin/;${CONDA_PATH}/include/;${CMAKE_PREFIX_PATH}")

    # Handle potential Boost
    set(Boost_NO_BOOST_CMAKE ON) #  Turn off search, as it may a system FindBoost.cmake
    if (WIN32)
        set(Boost_USE_STATIC_LIBS ON) # conda boost is statically build
        set(Boost_USE_STATIC ON)
        set(Boost_USE_MULTITHREAD)
    endif (WIN32)
    set(BOOST_ROOT ${CONDA_PATH})
ELSE()
    MESSAGE(STATUS "No conda found!")
ENDIF(DEFINED CONDA_PATH)


## Include and lib dir
###########################
if(UNIX)
    if(DEFINED CONDA_PATH)
        LINK_DIRECTORIES("${CONDA_PATH}/lib")
        INCLUDE_DIRECTORIES("${CONDA_PATH}/include")
    else()
        LINK_DIRECTORIES(/usr/local/lib)
        INCLUDE_DIRECTORIES(/usr/local/include)
    endif()
endif()
