//
// Created by j4wai on 14/01/2024.
//

#ifndef COURSEWORK2_BASEVECTOR_H
#define COURSEWORK2_BASEVECTOR_H
#include <cmath>
#include "Vector.h"
#include <ostream>
using std::ostream;

namespace VecMaths {

template<class T>
class Vector;

template<class T>
class BaseVector {
protected:
    //Number of elements in vector
    unsigned short size;

public:
    // Size constructor
    explicit BaseVector(unsigned short size) : size(size) {};

    // Copy Operator
    virtual BaseVector& operator=(const BaseVector<T>& other) {
        for (int i = 0; i < this->size; i++) {
            (*this)[i] = other[i];
        }
        return *this;
    }

    // Access an element
    virtual T& operator[](unsigned short i) = 0;

    // Access an element read-only
    virtual const T& operator[](unsigned short i) const = 0;

    // Printing
    friend ostream& operator<<(ostream& os, const BaseVector& vector)
    {
        os << "{";
        for (int i = 0; i < vector.size; i++) {
            os << vector[i];
            if (i != vector.size-1) {
                os << " ";
            }
        }
        os << "}";
        return os;
    }

    // Returns number of elements
    [[nodiscard]] unsigned short length() const {
        return this->size;
    }

    // Returns magnitude
    [[nodiscard]] T magnitude() const {
        T total = 0;
        for (int i = 0; i < size; i++) {
            total += std::pow((*this)[i],2);
        }
        return std::sqrt(total);
    }

    // Inner product
    [[nodiscard]] T innerProduct(const BaseVector<T>& other) const {
         T result = 0;
         for (int i = 0; i < size; i++) {
             result += (*this)[i] * other[i];
         }
         return result;
    }

    // Addition assignment
    void operator+=(const BaseVector<T>& other) {
        for (int i = 0; i < size; i++) {
            (*this)[i] += other[i];
        }
    }

    // Addition
    Vector<T> operator+(const BaseVector<T>& other) const {
        Vector<T> result(*this);
        result += other;
        return result;
    }

    // Subtraction assignment
    void operator-=(const BaseVector<T>& other) {
        for (int i = 0; i < size; i++) {
            (*this)[i] -= other[i];
        }
    }

    // Subtraction
    Vector<T> operator-(const BaseVector<T>& other) const {
        Vector<T> result(*this);
        result -= other;
        return result;
    }

    // Scalar multiplication assignment
    void operator*=(const T c) {
        for (int i = 0; i < size; i++) {
            (*this)[i] *= c;
        }
    }

    // Scalar multiplication
     Vector<T> operator*(const T c) const {
         Vector<T> result(*this);
         result *= c;
         return result;
     }

    // Scalar multiplication
    friend Vector<T> operator*(const T c, const BaseVector<T>& vector) {
        return vector * c;
    }

    // Scalar division assignment
    void operator/=(const T c) {
        for (int i = 0; i < size; i++) {
            (*this)[i] /= c;
        }
    }

    // Scalar divison
    Vector<T> operator/(const T c) const {
        Vector<T> result(*this);
        result /= c;
        return result;
    }

    // Projects this vector onto the input vector
    void project(const BaseVector<T>& other) {
        *this = other * (innerProduct(other)/other.innerProduct(other));
    }

    // Normalises the vector
    void normalise() {
         T mag = magnitude();
         (*this) *= 1/mag;
     }
};

}
#endif //COURSEWORK2_BASEVECTOR_H