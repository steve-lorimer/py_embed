cmake_minimum_required (VERSION 3.2.2)

find_package(Boost COMPONENTS python REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)

add_executable            (py_embed main.cpp)
target_link_libraries     (py_embed ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES})
target_include_directories(py_embed SYSTEM PRIVATE ${PYTHON_INCLUDE_DIRS})
