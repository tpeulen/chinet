cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)


FIND_PACKAGE(Python COMPONENTS Interpreter Development)
FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})
LINK_LIBRARIES(${Python_LIBRARIES})
#[[
if(NOT DEFINED PYTHON_INCLUDE_DIRS AND NOT IMP_STATIC)
    execute_process(COMMAND ${IMP_PYTHON} -c "import sys; print(sys.executable)"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            OUTPUT_VARIABLE python_full_path
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${IMP_PYTHON} -c "import sys; print('%d.%d.%d' % sys.version_info[:3])"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            OUTPUT_VARIABLE python_full_version
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE "^([0-9]+\\.[0-9]+).*" "\\1" python_version
            ${python_full_version})
    message(STATUS "Python binary is " ${python_full_path} " (version " ${python_full_version} ")")

    FIND_PACKAGE(PythonInterp)
    FIND_PACKAGE(PythonLibs ${python_full_version} EXACT REQUIRED)
endif()
]]#


execute_process(
        COMMAND python -c
        "from __future__ import print_function\ntry: import numpy; print(numpy.get_include(), end='')\nexcept:pass\n"
        RESULT_VARIABLE PYTHON_PREFIX_RESULT
        OUTPUT_VARIABLE Python_NumPy_PATH
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
INCLUDE_DIRECTORIES(${Python_NumPy_PATH})


SET(MODULE_NAME chinet)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")
SET(SWIG_SRC chinet.i)

# The output path is controlled by the setup.py
#SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/pyext)

FILE(GLOB SRC_files "../src/*.cpp")

#[[
FILE(GLOB SRC_files
        "../src/MongoObject.cpp"
        "../src/Functions.cpp"
        "../src/Port.cpp"
        "../src/Node.cpp"
        "../src/NodeCallback.cpp"
        "../src/Session.cpp"
        )
]]#

SET_PROPERTY(
        SOURCE
        chinet.i
        PROPERTY
        CPLUSPLUS ON
)

SWIG_ADD_LIBRARY(
        ${MODULE_NAME}
        LANGUAGE python
        SOURCES
        chinet.i
        ${SRC_files}
)

TARGET_LINK_LIBRARIES(
        ${MODULE_NAME} PRIVATE
        ${HDF5_LIBRARIES}
        ${PYTHON_LIBRARY}
        ${BSON_LIBRARIES}
)

TARGET_COMPILE_DEFINITIONS(
        ${MODULE_NAME} PRIVATE
        ${BSON_DEFINITIONS}
)

