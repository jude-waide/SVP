//
// Created by j4wai on 14/01/2024.
//

#ifndef COURSEWORK2_ROWWINDOWVEC_H
#define COURSEWORK2_ROWWINDOWVEC_H
#include "Matrix.h"
#include "BaseVector.h"

namespace VecMaths {
template<class T>
class RowWindowVec : public BaseVector<T> {
private:
    Matrix<T>* matrix;      // Parent matrix
    unsigned short rowNumber;   // Which row this is in the parent matrix

public:
    // Constructor from parent matrix
    RowWindowVec(const Matrix<T>& matrix, unsigned short rowNumber) : BaseVector<T>(matrix.getColumnCount()), matrix(&const_cast<Matrix<T>&>(matrix)), rowNumber(rowNumber) {}

    // Copy Constructor
    RowWindowVec(RowWindowVec<T>& other) : rowNumber(other.rowNumber), matrix(other.matrix) {}

    // Copy Operator
    RowWindowVec& operator=(const BaseVector<T>& other) override {
        BaseVector<T>::operator=(other);
        return *this;
    }

    // Access the ith element in the row
    T& operator[](unsigned short i) override {
        return (*matrix)[(*matrix).getRowCount() * i + rowNumber];
    };

    // Access the ith element in the row, read-only
    const T& operator[](unsigned short i) const override {
        return (*matrix)[(*matrix).getRowCount() * i + rowNumber];
    };

    // Swap rows in matrix
    friend void swap(RowWindowVec vec1, RowWindowVec<T> vec2)
    {
        using std::swap;
        for (int i = 0; i < vec1.length(); i++) {
            swap(vec1[i], vec2[i]);
        }
    }

};

}
#endif //COURSEWORK2_ROWWINDOWVEC_H
