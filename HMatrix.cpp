#include "HMatrix.h"

HMatrix::HMatrix() {
    HPoint p;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->mat[i][j] = p;
            this->inv[i][j] = p;
        }
    }
}

//HMatrix::HMatrix(const HVector a, const HVector& b, const HVector& c) {
//    for (int i = 0; i < 3; i++) {
//        this->mat[0][i] = a.v[i];
//        this->mat[1][i] = b.v[i];
//        this->mat[2][i] = c.v[i];
//    }
//}

HMatrix HMatrix::operator*(HMatrix m) {
    HMatrix n;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            n.mat[i][j] = this->mat[i][0]*m.mat[0][j]
            + this->mat[i][1]*m.mat[1][j]
            + this->mat[i][2]*m.mat[2][j];
            n.inv[i][j] = this->inv[0][j]*m.inv[i][0]
            + this->inv[1][j]*m.inv[i][1]
            + this->inv[2][j]*m.inv[i][2];
        }
    }
    return n;
}

void HMatrix::operator*=(HMatrix m) {
    *this = *this*m;
}

// bool HMatris::hasInverse() {
//
//   HMatrix m = this->mat*this->inv;
// }


HMatrix HMatrix::upMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::o();
    m.mat[1][1] = HPoint::c();
    m.mat[1][2] = HPoint::s();
    m.mat[2][1] = HPoint::s();
    m.mat[2][2] = HPoint::c();
    return m;
}

HMatrix HMatrix::downMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::o();
    m.mat[1][1] = HPoint::c();
    m.mat[1][2] = !HPoint::s();
    m.mat[2][1] = !HPoint::s();
    m.mat[2][2] = HPoint::c();
    return m;
}

HMatrix HMatrix::rightMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::c();
    m.mat[0][2] = HPoint::s();
    m.mat[2][0] = HPoint::s();
    m.mat[1][1] = HPoint::o();
    m.mat[2][2] = HPoint::c();
    return m;
}

HMatrix HMatrix::leftMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::c();
    m.mat[0][2] = !HPoint::s();
    m.mat[2][0] = !HPoint::s();
    m.mat[1][1] = HPoint::o();
    m.mat[2][2] = HPoint::c();
    return m;
}

DMatrix HMatrix::toDouble() {
    DMatrix retMatr;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            retMatr.mat[i][j] = this->mat[i][j].computeDouble();
        }
    }
    return retMatr;
}

std::string HMatrix::toString() {
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < 2; i++) {
        ss << '(';
        for (int j = 0; j < 2; j++) {
            ss << this->mat[i][j].toString() << ", ";
        }
        ss << this->mat[i][2].toString() << ')' << std::endl;
    }
    for (int j = 0; j < 2; j++) {
        ss << this->mat[2][j].toString() << ", ";
    }
    ss << this->mat[2][2].toString() << "))";
    return ss.str();
}

void HMatrix::print() {
    std::cout << this->toString() << std::endl;
}
