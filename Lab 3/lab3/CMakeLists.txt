# CMakeList.txt : CMake project for lab3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("lab3")

# Add source to this project's executable.
add_executable (lab3 "quiz2.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab3 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
