#include "DVector.h"

DVector::DVector() {
    for (int i = 0; i < 3; i++) {
        this->v[i] = 0;
    }
}

DVector::DVector(double* d) {
    this->v[0] = d[0];
    this->v[1] = d[1];
    this->v[2] = d[2];
}

DVector::DVector(double x, double y, double w) {
    this->v[0] = x;
    this->v[1] = y;
    this->v[2] = w;
}

double& DVector::operator[](const int index) {
    return this->v[index];
}

const double& DVector::operator[](const int index) const {
    return this->v[index];
}

double DVector::x() {
    return this->v[0];
}

double DVector::y() {
    return this->v[1];
}

double DVector::w() {
    return this->v[2];
}

double* DVector::klein() {
    double* xy = new double[2];
    xy[0] = this->x() / this->w();
    xy[1] = this->y() / this->w();
    return xy;
}

double* DVector::poincare() {
    double* xy = new double[2];
    double x = this->x() / this->w();
    double y = this->y() / this->w();
    xy[0] = x/(1+sqrt(1-x*x-y*y));
    xy[1] = y/(1+sqrt(1-x*x-y*y));
    return xy;
}
