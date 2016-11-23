#ifndef HVECTOR_H
#define HVECTOR_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "HPoint.h"
#include "DVector.h"
#include "HMatrix.h"
struct HVector {
    
    HPoint v[3];
    HVector();
    HVector(HPoint* p);
    HVector(const HPoint& x, const HPoint& y, const HPoint& w);
    HVector operator+(const HVector& r);
    void operator+=(const HVector& r);
    HVector operator-(const HVector& r);
    void operator-=(const HVector& r);
    
    HPoint& operator[](const int index);
    const HPoint& operator[](const int index) const;
    
    HVector operator*(const HMatrix& m);
//    void operator*=(const HMatrix& m);
    
    HVector up();
    HVector right();
    HVector left();
    HVector down();
    
    // HVector operator*(const HMatrix& m);
    
    double* computeDouble();
    DVector computeDVector();
    
    HPoint x();
    HPoint y();
    HPoint w();
    
    std::string toString();
    void print();
    void printDouble();
};

#endif
