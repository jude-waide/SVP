//
// Created by j4wai on 14/01/2024.
//

#ifndef COURSEWORK2_VECTOR_H
#define COURSEWORK2_VECTOR_H

#include "BaseVector.h"
#include <initializer_list>
using std::initializer_list;

namespace VecMaths {

template<class T>
class Vector : public BaseVector<T> {

private:
    T* values;

public:
    // Size Constructor, elements uninitalised
    explicit Vector(unsigned short size) : BaseVector<T>(size), values(new T[size]) {}

    // Size constructor, elements initialised
    Vector(unsigned short size, T value) : BaseVector<T>(size), values(new T[size]) {
        for (int i = 0; i < size; i++) {
            (*this)[i] = value;
        }
    }

    // Destructor
    ~Vector() {
        delete[] values;
    }

    // Constructor from initializer list
    Vector(initializer_list<T> list) : BaseVector<T>(list.size()) {
        values = new T[this->length()];

        int i = 0;
        for (T value : list) {
            (*this)[i] = value;
            i++;
        }
    }

    //Move Constructor
    Vector(Vector<T>&& other) : BaseVector<T>(other.size) {
        delete[] values;
        values = other.values;
        other.values = nullptr;
        other.size = 0;
    }

    // Copy Constructor
    Vector(const Vector<T>& other) : BaseVector<T>(other.size), values(new T[other.size]) {
        BaseVector<T>::operator=(other);
    }

    // Copy Constructor from ABS
    Vector(const BaseVector<T>& other) : BaseVector<T>(other.length()), values(new T[other.length()]) {
        BaseVector<T>::operator=(other);
    }

    // Move Operator
    Vector& operator=(Vector<T>&& other) noexcept {
        delete[] values;
        this->size = other.size;
        values = other.values;
        other.values = nullptr;
        other.size = 0;
        return *this;
    }

    // Copy Operator
    Vector& operator=(const Vector<T>& other) {
        BaseVector<T>::operator=(other);
        return *this;
    }

    // Copy Operator
    Vector& operator=(const BaseVector<T>& other) override {
        BaseVector<T>::operator=(other);
        return *this;
    }

    // Access element
    T& operator[](unsigned short i) override {
        return values[i];
    };

    // Access element as read-only
    const T& operator[](unsigned short i) const override {
        return values[i];
    };
};
}

#endif //COURSEWORK2_VECTOR_H
