//
// Created by j4wai on 14/01/2024.
//

#ifndef COURSEWORK2_COLUMNVIEWVECTOR_H
#define COURSEWORK2_COLUMNVIEWVECTOR_H

#include "Matrix.h"
#include "BaseVector.h"

namespace VecMaths {
template<class T>
class Matrix;

template<class T>
class ColumnWindowVec : public BaseVector<T> {
private:
    Matrix<T>* matrix;      // Stores parent Matrix
    unsigned short columnNumber;    // Stores which column this is in parent matrix

public:
    // Constructor
    ColumnWindowVec(const Matrix<T>& matrix, unsigned short columnNumber) : BaseVector<T>(matrix.getRowCount()), matrix(&const_cast<Matrix<T>&>(matrix)), columnNumber(columnNumber) {}

    // Copy Constructor
    ColumnWindowVec(ColumnWindowVec<T>& other) : columnNumber(other.columnNumber), matrix(other.matrix) {}

    // Copy Operator
    ColumnWindowVec& operator=(const BaseVector<T>& other) override {
        BaseVector<T>::operator=(other);
        return *this;
    }

    // Access ith element in the column
    T& operator[](unsigned short i) override{
        return (*matrix)[columnNumber * (*matrix).getRowCount() + i];
    };

    // Access ith element in the column, read-only
    const T& operator[](unsigned short i) const override {
        return (*matrix)[columnNumber * (*matrix).getRowCount() + i];
    };

    // Swap columns in matrix
    friend void swap(ColumnWindowVec vec1, ColumnWindowVec<T> vec2)
    {
        using std::swap;
        for (int i = 0; i < vec1.length(); i++) {
            swap(vec1[i], vec2[i]);
        }
    }

};

}

#endif //COURSEWORK2_COLUMNVIEWVECTOR_H
