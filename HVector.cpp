#include "HVector.h"

HVector::HVector() {
    this->v[0] = HPoint();
    this->v[1] = HPoint();
    this->v[2] = HPoint(1, 0, 0, 0);
}

HVector::HVector(HPoint* p) {
    for (int i = 0; i < 3; i++) {
        this->v[i] = p[i];
    }
}

HVector::HVector(const HPoint& x, const HPoint& y, const HPoint& w) {
    this->v[0] = x;
    this->v[1] = y;
    this->v[2] = w;
}

HVector HVector::operator+(const HVector& r) {
    HVector n;
    for (int i = 0; i < 3; i++) {
        n[i] = this->v[i] + r[i];
    }
    return n;
}

HPoint& HVector::operator[](const int index) {
    return this->v[index];
}

const HPoint& HVector::operator[](const int index) const {
    return this->v[index];
}

HVector HVector::operator*(const HMatrix& m) {
    HPoint x = this->x()*m.mat[0][0]+this->y()*m.mat[0][1]+this->w()*m.mat[0][2];
    HPoint y = this->x()*m.mat[1][0]+this->y()*m.mat[1][1]+this->w()*m.mat[1][2];
    HPoint w = this->x()*m.mat[2][0]+this->y()*m.mat[2][1]+this->w()*m.mat[2][2];
    return HVector(x, y, w);
}

// transform point up one cell
HVector HVector::up() {
    return *this*HMatrix::upMatrix();
}

HVector HVector::right() {
    
    return *this*HMatrix::rightMatrix();
}

HVector HVector::left() {
    
    return *this*HMatrix::leftMatrix();
}

HVector HVector::down() {
    
    return *this*HMatrix::downMatrix();
}

void HVector::operator+=(const HVector& r) {
    *this = *this+r;
}

HVector HVector::operator-(const HVector& r) {
    HVector n;
    for (int i = 0; i < 3; i++) {
        n[i] = this->v[i] - r[i];
    }
    return n;
}

void HVector::operator-=(const HVector& r) {
    *this = *this-r;
}

// HVector HVector::operator*(const HMatrix& m) {
//   HPoint a, b, c;
//   return HVector(a, b, c);
// }

double* HVector::computeDouble() {
    double* r = new double[3];
    for (int i = 0; i < 3; i++) {
        r[i] = this->v[i].computeDouble();
    }
    return r;
}

DVector HVector::computeDVector() {
    double* d = this->computeDouble();
    return DVector(d);
}

HPoint HVector::x() {
    return this->v[0];
}

HPoint HVector::y() {
    return this->v[1];
}

HPoint HVector::w() {
    return this->v[2];
}

std::string HVector::toString() {
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < 2; i++) {
        ss << this->v[i].toString() << ',' << std::endl;
    }
    ss << this->v[2].toString() << ')';
    return ss.str();
}

void HVector::print() {
    std::cout << this->toString() << std::endl;
}

void HVector::printDouble() {
    double* v = this->computeDouble();
    std::cout << '(';
    for (int i = 0; i < 2; i++) {
        std::cout << v[i] << ", ";
    }
    std::cout << v[2] << ')' << std::endl;
}
