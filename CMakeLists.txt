cmake_minimum_required(VERSION 3.10)
project(PythonAdapter)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -Wl -Bstatic -lstdc++ -lpthread")

set(PYTHON_INCLUDE_DIR "...")
set(PYTHON_LIBRARY "...")
set(Python_ADDITIONAL_VERSIONS 3.7)
FIND_PACKAGE(PythonLibs 3.7 REQUIRED)

message(${PYTHON_INCLUDE_DIRS})
message(${PYTHON_LIBRARIES})
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARIES})



add_library(PythonAdapter SHARED library.cpp library.h)
target_link_libraries(PythonAdapter python37.lib winpthread)

set_target_properties(PythonAdapter PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")