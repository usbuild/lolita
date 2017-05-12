CMAKE_MINIMUM_REQUIRED (VERSION 3.0 FATAL_ERROR)
PROJECT (lolita CXX)
SET_PROPERTY( GLOBAL PROPERTY USE_FOLDERS ON)
SET(STRICT_CXX_FLAGS " -Wall -Wextra -Werror -Wno-unused-function -Wno-unused-parameter -Woverloaded-virtual -Wpointer-arith -Wswitch-default -Wold-style-cast -Wnon-virtual-dtor -Wformat -Winit-self")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3 -g3 -std=c++14 -fdiagnostics-color=auto ${STRICT_CXX_FLAGS}")

IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()
IF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D_DEBUG -fstack-protector-all")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math")
ENDIF()

FILE (GLOB src_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

ADD_EXECUTABLE (loli ${src_files})
