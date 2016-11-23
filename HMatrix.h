#ifndef HMATRIX_H
#define HMATRIX_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "HPoint.h"
//#include "HVector.h"
#include "DMatrix.h"
// struct HVector;
struct HMatrix {
    HPoint mat[3][3];
    HPoint inv[3][3];
    HMatrix();
//    HMatrix(const HVector a, const HVector& b, const HVector& c);
    
    DMatrix toDouble();
    
    // HMatrix translateUp(const HMatrix& m);
    // HMatrix translateDown(const HMatrix& m);
    // HMatrix translateRight(const HMatrix& m);
    // HMatrix translateLeft(const HMatrix& m);
    // HMatrix rotateCW(const HMatrix& m);
    // HMatrix rotateCCW(const HMatrix& m);
    // HMatrix flip(const HMatrix& m);
    // bool hasInverse();
    // HMatrix(HPoint** m);
    // HVector operator*(const HVector& v);
    HMatrix operator*(HMatrix m);
    void operator*=(HMatrix m);
//    HPoint*& operator[](const int index);
//    const HPoint*& operator[](const int index) const;
    
    static HMatrix upMatrix();
    static HMatrix downMatrix();
    static HMatrix rightMatrix();
    static HMatrix leftMatrix();
    //
    std::string toString();
    void print();
};

#endif
