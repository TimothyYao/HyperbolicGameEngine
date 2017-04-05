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
    DVector operator*(const double s);
    void operator*=(const double s);
    DVector operator/(const double s);
    void operator/=(const double s);
    DVector operator+(DVector v);
    void operator+=(DVector v);
    DVector operator-(DVector v);
    void operator-=(DVector v);
    double& operator[](const int index);
    const double& operator[](const int index) const;
    
    bool isNormal();
    DVector toNormal();
    void normalize();
    double innerProd(DVector v);
    double distance(DVector v);
    double distancePlane(DVector plane);
    DVector midpoint(DVector v);
    DVector planeBetween(DVector v);
    DVector hlerp(DVector v, double t);
    DVector inverse();
    
    double* klein();
    double* poincare();
    
    std::string toString();
    void print();
};

#endif
