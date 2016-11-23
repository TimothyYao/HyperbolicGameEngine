#ifndef DVECTOR_H
#define DVECTOR_H
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

struct DVector {
    double v[3];
    DVector();
    DVector(double* d);
    DVector(double x, double y, double w);
    double& operator[](const int index);
    const double& operator[](const int index) const;
    double x();
    double y();
    double w();
    
    double* klein();
    double* poincare();
};

#endif
