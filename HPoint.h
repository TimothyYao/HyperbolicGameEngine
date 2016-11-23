#ifndef HPOINT_H
#define HPOINT_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

struct HPoint {
    
    int p[4];
    HPoint();
    HPoint(int* x);
    HPoint(int a, int b, int c, int d);
    HPoint operator+(const HPoint& r);
    void operator+=(const HPoint& r);
    HPoint operator-(const HPoint& r);
    void operator-=(const HPoint& r);
    HPoint operator*(const HPoint& r);
    void operator*=(const HPoint& r);
    bool operator==(const HPoint& r);
    int& operator[](const int index);
    const int& operator[](const int index) const;
    HPoint operator!();
    
    static HPoint o();
    static HPoint s();
    static HPoint c();
    static HPoint sc();
    
    double computeDouble();
    
    std::string toString();
    void print();
    void printDouble();
};

#endif
