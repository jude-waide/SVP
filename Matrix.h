//
// Created by j4wai on 14/01/2024.
//

#ifndef COURSEWORK2_MATRIX_H
#define COURSEWORK2_MATRIX_H

#include <vector>
#include "Vector.h"
#include "ColumnWindowVec.h"
#include "RowWindowVec.h"
#include <initializer_list>

using std::initializer_list;

namespace VecMaths {

template<class T>
class Matrix {
private:
    T* values;  //Stores elements of the matrix
    unsigned short rows;
    unsigned short columns;

public:
    // Size Constructor
    Matrix(unsigned short rows, unsigned short columns) : rows(rows), columns(columns), values(new T[rows*columns]) {}

    // Constructor from vector list
    Matrix(std::vector<Vector<T>>& vectors) : rows(vectors[0].length()), columns(vectors.size()){
        values = new T[rows*columns];
        for (int i = 0; i < columns; i++) {
            if (vectors[i].length() != rows) {
                throw(std::invalid_argument("Passed a column of size " + std::to_string(vectors[i].length()) + ", expected size " + std::to_string(rows)));
            }
            this->column(i) = vectors[i];
        }
    }

    // Constructor from initializer list
    Matrix(initializer_list<initializer_list<T>> list) : columns(list.begin()->size()), rows(list.size()){
        values = new T[rows*columns];

        int i = 0;
        for (initializer_list<T> row : list) {
            if (row.size() != columns) {
                throw(std::invalid_argument("Passed a row of size " + std::to_string(row.size()) + ", expected size " + std::to_string(columns)));
            }
            int j = 0;
            for (T value : row) {
                (*this)[j*getRowCount() + i] = value;
                j++;
            }
            i++;
        }
    }

    // Copy Constructor
    Matrix(const Matrix<T>& other) : rows(other.getRowCount()), columns(other.getColumnCount()){
        values = new T[rows*columns];
        std::copy(other.values, other.values + (rows*columns), values);
    }

    // Copy Operator
    Matrix& operator=(const Matrix& other) {
        delete[] values;
        rows = other.getRowCount();
        columns = other.getColumnCount();
        values = new T[rows*columns];
        std::copy(other.values, other.values + (rows*columns), values);
        return *this;
    }

    // Deconstructor
    ~Matrix() {
        delete[] values;
    }

    // Equality operator
    bool operator==(Matrix<T>& other) {
        if (getColumnCount() != other.getColumnCount()) return false;
        if (getRowCount() != other.getRowCount()) return false;
        for (int i = 0; i < getRowCount()* getColumnCount(); i++) {
            if ((*this)[i] != other[i]) return false;
        }
        return true;
    }

    // Printing
    friend ostream& operator<<(ostream& os, const Matrix& matrix)
    {
        os << matrix.getRowCount() << "x" << matrix.getColumnCount() << " " <<  "Matrix:\n";
        for (int i = 0; i < matrix.getRowCount(); i++) {
            os << matrix.row(i) << "\n";
        }
        return os;
    }

    // Get number of row
    [[nodiscard]] unsigned short getRowCount() const {
        return rows;
    }

    // Get number of columns
    [[nodiscard]] unsigned short getColumnCount() const {
        return columns;
    }

    // Access an element
    T& operator[](const unsigned short i) {
        return values[i];
    };

    // Access an element read-only
    const T& operator[](const unsigned short i) const {
        return values[i];
    };

    // Returns a column as a vector
    ColumnWindowVec<T> column(const unsigned short i) {
        return ColumnWindowVec(*this, i);
    }

    // Returns a column as a read-only vector
    const ColumnWindowVec<T> column(const unsigned short i) const {
        return ColumnWindowVec(*this, i);
    }

    // Returns a row as a vector
    RowWindowVec<T> row(const unsigned short i) {
        return RowWindowVec(*this, i);
    }

    // Returns a row as a read-only vector
    const RowWindowVec<T> row(const unsigned short i) const {
        return RowWindowVec<T>(*this, i);
    }

    // Matrix multiplication
    Vector<T> operator*(const BaseVector<T>& vector) {
        Vector<T> result(vector.length());

        for (int row = 0; row < getRowCount(); row++) {
            T value = 0;
            for (int column = 0; column < getColumnCount(); column++) {
                value += (*this).row(row)[column] * vector[column];
            }
            result[row] = value;
        }

        return result;
    }

    // In place gram-schmidt
    void orthoganalise() {
        for (int i = 1; i < columns; i++) {
            for (int j = 0; j < i; j++) {
                Vector<T> copy(column(i));
                copy.project(column(j));
                column(i) -= copy;
            }
        }
    }
};
}
#endif //COURSEWORK2_MATRIX_H
