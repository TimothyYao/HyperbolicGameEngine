#ifndef DMATRIX_H
#define DMATRIX_H
#include <iostream>
#include <sstream>
#include <string>

struct DMatrix {
    double mat[3][3];
    DMatrix();
    DMatrix(double a, double b, double c, double d, double e, double f, double g, double h, double i);
    DMatrix operator*(DMatrix m);
    void operator*=(DMatrix m);
    
    std::string toString();
    void print();
};

#endif
