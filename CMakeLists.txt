cmake_minimum_required(VERSION 3.12)
project(seminar_assignment)

set(CMAKE_CXX_STANDARD 17)

add_executable(seminar_assignment main.cpp model/student.cpp model/student.h model/seminar.cpp model/seminar.h)