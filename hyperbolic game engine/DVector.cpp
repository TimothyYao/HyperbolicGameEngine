#include "DVector.h"

DVector::DVector() {
    this->v[0] = 0;
    this->v[1] = 0;
    this->v[2] = 1;
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

DVector DVector::operator*(const double s) {
    return DVector(v[0]*s, v[1]*s, v[2]*s);
}

void DVector::operator*=(const double s) {
    *this = *this*s;
}

DVector operator*(double d, DVector v) {
    return v*d;
}

DVector DVector::operator/(const double s) {
    double d = 1/s;
    return DVector(v[0]*d, v[1]*d, v[2]*d);
}

void DVector::operator/=(const double s) {
    *this = *this/s;
}

DVector DVector::operator+(DVector q) {
    return DVector(v[0] + q[0], v[1] + q[1], v[2] + q[2]);
}

void DVector::operator+=(DVector v) {
    *this = *this+v;
}

DVector DVector::operator-(DVector q) {
    return DVector(v[0] - q[0], v[1] - q[1], v[2] - q[2]);
}

void DVector::operator-=(DVector v) {
    *this = *this-v;
}

double& DVector::operator[](const int index) {
    return this->v[index];
}

const double& DVector::operator[](const int index) const {
    return this->v[index];
}

bool DVector::isNormal() {
    return (std::abs(v[2]*v[2] - v[0]*v[0] - v[1]*v[1]) == 1);
}

DVector DVector::toNormal() {
    if (!isNormal()) {
        return *this/sqrt(std::abs(this->innerProd(*this)));
    }
    return *this;
}

void DVector::normalize() {
    *this = this->toNormal();
}

double DVector::innerProd(DVector q) {
    return v[2]*q[2] - (v[0]*q[0]+v[1]*q[1]);
}

double DVector::distance(DVector v) {
    return acosh(this->toNormal().innerProd(v));
}

double DVector::distancePlane(DVector plane) {
    return asinh(this->innerProd(plane));
}

DVector DVector::midpoint(DVector v) {
    DVector r = *this+v;
    return r.toNormal();
}

DVector DVector::planeBetween(DVector v) {
    DVector r = *this-v;
    return r.toNormal();
}

DVector DVector::hlerp(DVector v, double t) {
    double s = 1-t;
    double d = this->distance(v);
    return (sinh(s*d)*(*this) + sinh(t*d)*v) / sinh(d);
}

DVector DVector::inverse() {
    return DVector(-v[0], -v[1], v[2]);
}

double* DVector::klein() {
    double* xy = new double[2];
    xy[0] = v[0]/v[2];
    xy[1] = v[1]/v[2];
    return xy;
}

double* DVector::poincare() {
    DVector n = this->toNormal();
    double* xy = new double[2];
    xy[0] = n[0]/(1+n[2]);
    xy[1] = n[1]/(1+n[2]);
    return xy;
}

std::string DVector::toString() {
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < 2; i++) {
        ss << this->v[i] << ", ";
    }
    ss << this->v[2] << ")";
    return ss.str();
}

void DVector::print() {
    std::cout << this->toString() << std::endl;
}


