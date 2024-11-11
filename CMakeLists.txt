cmake_minimum_required(VERSION 3.28)
project(Reflection)

set(CMAKE_CXX_STANDARD 23)

add_executable(Reflection main.cpp GetTypeName.h Reflection.h MemberFunction.h)
