#ifndef DMATRIX_H
#define DMATRIX_H
#include <iostream>
#include <sstream>
#include <string>
#include "DVector.h"

struct DMatrix {
    double mat[3][3];
    DMatrix();
    DMatrix(double a, double b, double c, double d, double e, double f, double g, double h, double i);
    DMatrix(DVector v);
    void operator=(DMatrix that);
    DMatrix operator*(const DMatrix& m);
    void operator*=(const DMatrix& m);
    double* operator[](const int index);
    DMatrix inverse();
    DMatrix rotate(double t);
    DMatrix scale(double s);
    DMatrix toNormal();
    void normalize();
    
    DVector toVector();
    DMatrix getRotation();
    double distance(DMatrix m);
    
//    double distance(const DMatrix& m);
    
    std::string toString();
    void print();
    
    static DMatrix translateMatX(double d);
    static DMatrix translateMatY(double d);
    static DMatrix scalarMat(double s);
    static DMatrix rotateMat(double t, DMatrix p);
};

#endif
