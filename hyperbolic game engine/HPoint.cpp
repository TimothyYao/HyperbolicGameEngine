#include "HPoint.h"

HPoint::HPoint() {
    for (int i = 0; i < 4; i++) {
        this->p[i] = 0;
    }
}

HPoint::HPoint(int* x) {
    for (int i = 0; i < 4; i++) {
        this->p[i] = x[i];
    }
}

HPoint::HPoint(int a, int b, int c, int d) {
    this->p[0] = a;
    this->p[1] = b;
    this->p[2] = c;
    this->p[3] = d;
}

HPoint HPoint::operator+(const HPoint& r) {
    int x[4];
    for (int i = 0; i < 4; i++) {
        x[i] = this->p[i] + r[i];
    }
    return HPoint(x);
}

void HPoint::operator+=(const HPoint& r) {
    *this = *this+r;
}

HPoint HPoint::operator-(const HPoint& r) {
    int x[4];
    for (int i = 0; i < 4; i++) {
        x[i] = this->p[i] - r[i];
    }
    return HPoint(x);
}

void HPoint::operator-=(const HPoint& r) {
    *this = *this-r;
}

HPoint HPoint::operator*(const HPoint& r) {
    int a = this->p[0];
    int b = this->p[1];
    int c = this->p[2];
    int d = this->p[3];
    
    int x = r[0];
    int y = r[1];
    int z = r[2];
    int w = r[3];
    
    int l = a*x;
    int m = a*y+b*x;
    int n = a*z+b*y+c*x;
    int o = a*w+b*z+c*y+d*x;
    
    l += c*z+b*w+d*y+d*w;
    m += c*w+d*z;
    n += c*z+b*w+d*y+2*d*w;
    o += c*w+d*z;
    return HPoint(l, m, n, o);
}

void HPoint::operator*=(const HPoint& r) {
    *this = *this*r; // equivalent to *this = this->operator*(p);
}

bool HPoint::operator==(HPoint r) {
    for (int i = 0; i < 4; i++) {
        if (this->p[i] != r[i]) { return false; }
    }
    return true;
}

bool HPoint::operator!=(HPoint r) {
    return !this->operator==(r);
}

int& HPoint::operator[](const int index) {
    return this->p[index];
}

const int& HPoint::operator[](const int index) const{
    return this->p[index];
}

HPoint HPoint::operator!() {
    HPoint p;
    for (int i = 0; i < 4; i++) {
        p[i] = -1 * this->p[i];
    }
    return p;
}

double HPoint::computeDouble() {
    double c = (1 + sqrt(5)) / 2;
    double s = sqrt(c);
    return this->p[0] + s*this->p[1] + c*this->p[2] + s*c*this->p[3];
}

HPoint HPoint::o() {
    return HPoint(1, 0, 0, 0);
}

HPoint HPoint::s() {
    return HPoint(0, 1, 0, 0);
}

HPoint HPoint::c() {
    return HPoint(0, 0, 1, 0);
}

HPoint HPoint::sc() {
    return HPoint(0, 0, 0, 1);
}

// creates string representation of HPoint
std::string HPoint::toString() {
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < 3; i++) {
        ss << this->p[i] <<", ";
    }
    ss << this->p[3] << ")";
    return ss.str();
}

void HPoint::print() {
    std::cout << this->toString() << std::endl;
}

void HPoint::printDouble() {
    std::cout << this->computeDouble() << std::endl;
}

