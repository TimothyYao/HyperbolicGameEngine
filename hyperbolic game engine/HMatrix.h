#ifndef HMATRIX_H
#define HMATRIX_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include "HPoint.h"
#include "HVector.h"
#include "DMatrix.h"
// struct HVector;
struct HMatrix {
    HPoint mat[3][3];
    HPoint inv[3][3];
    HMatrix();
    void operator=(HMatrix that);
    bool operator==(HMatrix that);
    bool operator!=(HMatrix that);
    //    HMatrix(const HVector a, const HVector& b, const HVector& c);
    
    bool sameCell(HMatrix cell);
    
    DMatrix toDouble();
    HVector toVector();
    
    void translateUp();
    void translateDown();
    void translateRight();
    void translateLeft();
    
    HMatrix up();
    HMatrix down();
    HMatrix left();
    HMatrix right();
    
    HMatrix rotateCW();
    HMatrix rotateCC();
    
    // HMatrix rotateCW(const HMatrix& m);
    // HMatrix rotateCCW(const HMatrix& m);
    // HMatrix flip(const HMatrix& m);
    // bool hasInverse();
    // HMatrix(HPoint** m);
    // HVector operator*(const HVector& v);
    HMatrix operator*(HMatrix m);
    void operator*=(HMatrix m);
    HPoint* operator[](const int index);
    
    
    HMatrix inverse();
    bool isAdjacent(HMatrix cell);
    
    static HMatrix upMatrix();
    static HMatrix downMatrix();
    static HMatrix rightMatrix();
    static HMatrix leftMatrix();
    
    double distance(HMatrix m);
    
    std::string toString();
    std::string toStringInv();
    void print();
    void printInv();
};

#endif
