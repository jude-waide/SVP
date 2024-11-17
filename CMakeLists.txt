cmake_minimum_required(VERSION 3.26)
project(coursework2)

set(CMAKE_CXX_STANDARD 17)

add_executable(coursework2 main.cpp
        BaseVector.h
        Matrix.h
        Vector.h
        RowWindowVec.h
        ColumnWindowVec.h
        algorithms.h)

add_executable(test test.cpp
        BaseVector.h
        Matrix.h
        Vector.h
        RowWindowVec.h
        ColumnWindowVec.h
        algorithms.h)