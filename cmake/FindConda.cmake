IF(DEFINED ENV{PREFIX})
    SET($ENV{CONDA_PREFIX} ENV{PREFIX})
ENDIF()
IF(DEFINED ENV{CONDA_PREFIX})
    FILE(TO_CMAKE_PATH "$ENV{CONDA_PREFIX}" CONDA_PATH)
    MESSAGE(STATUS "PREFIX/CONDA_PREFIX env seen: ${CONDA_PATH}")
    SET(CMAKE_PREFIX_PATH "${CONDA_PATH}")
    SET($ENV{PATH} "${CONDA_PATH}/bin;$ENV{PATH}")
    IF(MSVC)	
        FILE(TO_CMAKE_PATH "$ENV{LIBRARY_PREFIX}" ENV_LIBRARY_PREFIX)
        FILE(TO_CMAKE_PATH "$ENV{LIBRARY_INC}" ENV_LIBRARY_INC)
        FILE(TO_CMAKE_PATH "$ENV{LIBRARY_LIB}" ENV_LIBRARY_LIB)
        FILE(TO_CMAKE_PATH "$ENV{LIBRARY_BIN}" ENV_LIBRARY_BIN)
        SET(CMAKE_MODULE_PATH "${ENV_LIBRARY_PREFIX}/cmake;${CMAKE_MODULE_PATH}")
        INCLUDE_DIRECTORIES(${ENV_LIBRARY_INC})
        LINK_DIRECTORIES(${ENV_LIBRARY_LIB})
    ELSE()
        SET(CMAKE_MODULE_PATH "${CONDA_PATH}/include/cmake;${CMAKE_MODULE_PATH}")
        INCLUDE_DIRECTORIES(${CONDA_PATH}/include)
        LINK_DIRECTORIES(${CONDA_PATH}/lib)
    ENDIF()
    MESSAGE(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")
ELSE()
    MESSAGE(STATUS "No conda environment defined")
ENDIF()
