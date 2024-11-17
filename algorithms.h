//
// Created by j4wai on 15/01/2024.
//

#ifndef COURSEWORK2_ALGORITHMS_H
#define COURSEWORK2_ALGORITHMS_H

#include <array>
#include <string>
#include <stack>
#include <vector>
#include "Matrix.h"
#include "Vector.h"

using namespace std;
using namespace VecMaths;

// Calculates gram-schmidt coefficients
template<class T>
void calculateCoefs(Vector<T>& coefs, const Matrix<T>& mat, const Matrix<T>& orthogonal) {
    for (int i = 1; i < mat.getColumnCount(); i++) {
        for (int j = 0; j < i; j++) {
            coefs[(i*(i-1))/2+j] = mat.column(i).innerProduct(orthogonal.column(j))/orthogonal.column(j).innerProduct(orthogonal.column(j));
        }
    }
}

// LLL algorithm to find reduced lattice basis
template<class T>
void LLL(Matrix<T>& mat, float delta) {
    // Calculate orthogonal matrix using gram-schmidt
    auto orthogonal(mat);
    orthogonal.orthoganalise();

    // Find the schmidt coefficients
    Vector<T> schmidtCoefs((mat.getColumnCount()*(mat.getColumnCount()-1))/2);
    calculateCoefs(schmidtCoefs, mat, orthogonal);


    int k = 1;
    while (k < mat.getColumnCount()) {
        for (int j = k-1; j >= 0; j--) {
            T coef = schmidtCoefs[(k*(k-1))/2+j];
            if (abs(coef) > 0.5) {
                mat.column(k) -= round(coef) * mat.column(j);
                // Recalculate orthogonal matrix and schmidt coefficients
                orthogonal = Matrix(mat);
                orthogonal.orthoganalise();
                calculateCoefs(schmidtCoefs, mat, orthogonal);
            }
        }
        if (orthogonal.column(k).innerProduct(orthogonal.column(k)) > (delta - pow(schmidtCoefs[(k*(k-1))/2+k-1], 2))*orthogonal.column(k-1).innerProduct(orthogonal.column(k-1))) {
            k += 1;
        } else {
            // Swap columns
            swap(mat.column(k), mat.column(k-1));

            // Recalculate orthogonal matrix and schmidt coefficients
            orthogonal = Matrix<T>(mat);
            orthogonal.orthoganalise();
            calculateCoefs(schmidtCoefs, mat, orthogonal);
            if (k - 1 > 1) {
                k = k-1;
            } else {
                k = 1;
            }
        }
    }
}

// kfp enumeration algorithm
template<class T>
Vector<T> kfpEnum(const Matrix<T>& matrix) {
    // Find orthogonal matrix
    auto orthogonal(matrix);
    orthogonal.orthoganalise();

    // Get Gram-Schmidt coefficients
    Vector<T> schmidtCoefs((matrix.getColumnCount()*(matrix.getColumnCount()-1))/2);
    calculateCoefs<T>(schmidtCoefs , matrix, orthogonal);

    // Caluclate the inner products of the orthogonal matrix
    Vector<T> orthogonalProducts(matrix.getColumnCount());
    for (int i = 0; i < orthogonalProducts.length(); i++) {
        orthogonalProducts[i] = orthogonal.column(i).innerProduct(orthogonal.column(i));
    }

    // Set initial values
    auto x = Vector<T>(orthogonalProducts.length(), 0);   // The current coefficients being looked at
    auto delta = Vector<T>(orthogonalProducts.length(), 0);   // How x should change
    auto bigDelta = Vector<T>(orthogonalProducts.length(), -1);
    auto c = Vector<T>(orthogonalProducts.length(), 0);
    auto l = Vector<T>(orthogonalProducts.length() + 1, 0);
    auto y = Vector<T>(orthogonalProducts.length(), 0);
    int i = orthogonalProducts.length() - 1;    // Iterated variable
    Vector<T> solution = Vector<T>(orthogonalProducts.length(), 0);   // Column coefficients of the solution

    T bound = orthogonalProducts[0];    // The bound on the searchspace

    while (true) {
        y[i] = abs(x[i]-c[i]);
        l[i] = l[i+1] + orthogonalProducts[i] * pow(y[i], 2);
        if (l[i] <= bound && i == 0) {
            if (l[i] != 0) {
                solution = x;   // Set new minimum
                bound = l[i];   // Set new bound
            }
        }
        if (l[i] <= bound && i > 0) {
            i -= 1;     // Move a layer down the tree
            c[i] = 0;
            for (int j = i + 1; j < orthogonalProducts.length(); j++) {
                c[i] -= x[j]*schmidtCoefs[(j*(j-1))/2 + i];
            }
            x[i] = round(c[i]);
            delta[i] = 0;
            if (c[i] < x[i]) {
                bigDelta[i] = 1;
            } else {
                bigDelta[i] = -1;
            }
        } else {
            i += 1;     // Move a layer up the tree
            if (i >= orthogonalProducts.length()) return solution;
            bigDelta[i] = -bigDelta[i];
            delta[i] = (-delta[i]) + bigDelta[i];
            x[i] += delta[i];
        }
    }
}

double svp(Matrix<double>& M) {
    Vector<double> solution(M.getColumnCount());

    // Use LLL to find a reduced basis
    LLL(M, 0.75);


    // Find an approximate solution by finding the shortest basis vector of the reduced lattice
    double mag = M.column(0).magnitude();
    solution = M.column(0);
    for (int i = 1; i < M.getColumnCount(); i++) {
        double candidate = M.column(i).magnitude();
        if (candidate < mag) {
            mag = candidate;
            solution = M.column(i);
        }
    }

    // Find coefficients of answer using kfp enumeration
    auto coefficients = kfpEnum(M);
    if (coefficients.magnitude() != 0) {   // Coefficients will be all 0 if LLL already found the shortest vector
        solution = M * coefficients;
    }
    return solution.magnitude();
}

#endif //COURSEWORK2_ALGORITHMS_H
