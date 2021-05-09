cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

IF(BUILD_PYTHON_INTERFACE)
    # Python
    ###########################
    FIND_PACKAGE(Python)
    INCLUDE_DIRECTORIES(${Python_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${Python_NumPy_PATH})

    FIND_PACKAGE(SWIG REQUIRED)
    INCLUDE(${SWIG_USE_FILE})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
    SET(MODULE_NAME ${PROJECT_NAME})

    FILE(GLOB SRC_files "../src/*.cpp")
    SET_SOURCE_FILES_PROPERTIES(SRC_files PROPERTIES CPLUSPLUS ON)
    SET_PROPERTY(SOURCE chinet.i PROPERTY CPLUSPLUS ON)

    SWIG_ADD_LIBRARY(
            ${MODULE_NAME}
            LANGUAGE python
            SOURCES chinet.i
            ${SRC_files}
    )

    TARGET_COMPILE_DEFINITIONS(
            ${MODULE_NAME} PRIVATE
            ${BSON_DEFINITIONS}
    )

    TARGET_LINK_LIBRARIES(
            ${MODULE_NAME}
            ${PYTHON_LIBRARY}
            ${BSON_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
    )

    IF (NOT PYTHON_LINK_LIBRARIES)
        IF (APPLE)
            SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES LINK_FLAGS "-Wl,-flat_namespace,-undefined,dynamic_lookup")
        ENDIF()
    ENDIF()

    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(dir ${dirs})
        message(STATUS "INCLUDE_DIRECTORY='${dir}'")
    endforeach()

    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_LIBRARIES)
    foreach(dir ${dirs})
        message(STATUS "LINK_LIBRARIES='${dir}'")
    endforeach()

    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
    foreach(dir ${dirs})
        message(STATUS "LINK_DIRECTORIES='${dir}'")
    endforeach()


ENDIF(BUILD_PYTHON_INTERFACE)
