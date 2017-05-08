#include "DMatrix.h"

DMatrix::DMatrix() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                this->mat[i][j] = 1;
            } else {
                this->mat[i][j] = 0;
            }
        }
    }
}

DMatrix::DMatrix(double a, double b, double c, double d, double e, double f, double g, double h, double i) {
    this->mat[0][0] = a;
    this->mat[0][1] = b;
    this->mat[0][2] = c;
    this->mat[1][0] = d;
    this->mat[1][1] = e;
    this->mat[1][2] = f;
    this->mat[2][0] = g;
    this->mat[2][1] = h;
    this->mat[2][2] = i;
}

DMatrix::DMatrix(DVector v) {
    v.normalize();
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            if (i == j) {
                this->mat[i][j] = 1;
            } else {
                this->mat[i][j] = 0;
            }
        }
    }
    this->mat[0][2] = v[0];
    this->mat[1][2] = v[1];
    this->mat[0][0] = sqrt(v[0]*v[0]+1);
    this->mat[1][1] = sqrt(v[1]*v[1]+1);
    this->mat[2][2] = v[2];
    this->mat[2][0] = v[0];
    this->mat[2][1] = v[1];
}

void DMatrix::operator=(DMatrix that) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->mat[i][j] = that.mat[i][j];
        }
    }
}

DMatrix DMatrix::operator*(const DMatrix& m) {
    DMatrix n;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            n.mat[i][j] = this->mat[i][0]*m.mat[0][j]
            + this->mat[i][1]*m.mat[1][j]
            + this->mat[i][2]*m.mat[2][j];
        }
    }
    return n;
}

void DMatrix::operator*=(const DMatrix& m) {
    *this = *this*m;
}

DMatrix DMatrix::inverse() {
    DMatrix inv;
    double m00 = this->mat[0][0];
    double m01 = this->mat[0][1];
    double m02 = this->mat[0][2];
    double m10 = this->mat[1][0];
    double m11 = this->mat[1][1];
    double m12 = this->mat[1][2];
    double m20 = this->mat[2][0];
    double m21 = this->mat[2][1];
    double m22 = this->mat[2][2];
    double det =    m00*(m11*m22-m21*m12)-
                    m01*(m10*m22-m12*m20)+
                    m02*(m10*m21-m11*m20);
    if (det == 0) {
        std::cout << "no inverse" << std::endl;
        return DMatrix();
    }
    double invdet = 1/det;
    inv[0][0] = (m11*m22-m21*m12)*invdet;
    inv[0][1] = (m02*m21-m01*m22)*invdet;
    inv[0][2] = (m01*m12-m02*m11)*invdet;
    inv[1][0] = (m12*m20-m10*m22)*invdet;
    inv[1][1] = (m00*m22-m02*m20)*invdet;
    inv[1][2] = (m10*m02-m00*m12)*invdet;
    inv[2][0] = (m10*m21-m20*m11)*invdet;
    inv[2][1] = (m20*m01-m00*m21)*invdet;
    inv[2][2] = (m00*m11-m10*m01)*invdet;
    return inv;
}

DMatrix DMatrix::rotate(double t) {
//    double x = mat[0][2];
//    double y = mat[1][2];
    double r00 = cos(t);
    double r10 = sin(t);
    double r01 = -r10;
    double r11 = r00;
    DMatrix rotMatrix(r00, r01, 0,//x-r00*x-r01*y,
                      r10, r11, 0,//y-r10*x-r11*y,
                      0,   0,   1);
    *this*=rotMatrix;
    return *this;
}

DMatrix DMatrix::scale(double s) {
    return DMatrix();
}

DMatrix DMatrix::toNormal() {
//    return this->getRotation()*DMatrix(this->getBotRow().toNormal());
    DMatrix normal = *this;
    DVector v = this->toVector().toNormal();
//    c^2-d^2a^2-d^2b^2 = -1;
//    c^2+1 = d^2(a^2+b^2);
//    (c^2+1)/(a^2-b^2)=d^2
    double s = sqrt((v[0]*v[0]+1)/(mat[0][0]*mat[0][0]+mat[0][1]*mat[0][1]));
    s=s!=s?1:s;
    normal[0][0] = mat[0][0]*s;
    normal[0][1] = mat[0][1]*s;
    normal[0][2] = v[0];
    s = sqrt((v[1]*v[1]+1)/(mat[1][0]*mat[1][0]+mat[1][1]*mat[1][1]));
    s=s!=s?1:s;
    normal[1][0] = mat[1][0]*s;
    normal[1][1] = mat[1][1]*s;
    normal[1][2] = v[1];
    s = sqrt((v[2]*v[2]-1)/(mat[2][0]*mat[2][0]+mat[2][1]*mat[2][1]));
    s=s!=s?1:s;
    normal[2][0] = mat[2][0]*s;
    normal[2][1] = mat[2][1]*s;
    normal[2][2] = v[2];
    return normal;
}

void DMatrix::normalize() {
    DMatrix prev = *this;
    *this = this->toNormal();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (mat[i][j] != mat[i][j]) {
                prev.print();
                this->print();
                std::cout << "in normalize" << std::endl;
                throw std::exception();
            }
        }
    }
}

double* DMatrix::operator[](const int index) {
    return this->mat[index];
}

DVector DMatrix::toVector() {
    return DVector(this->mat[0][2], this->mat[1][2], this->mat[2][2]);
}

DVector DMatrix::getBotRow() {
    return DVector(this->mat[2][0], this->mat[2][1], this->mat[2][2]);
}

DMatrix DMatrix::getRotation() {
    DMatrix retVal = *this*(DMatrix(this->getBotRow().toNormal()).inverse());
    double h = hypot(retVal[0][0], retVal[0][1]);
    retVal[0][0] = retVal[0][0]/h;
    retVal[0][1] = retVal[0][1]/h;
    retVal[1][0] = -retVal[0][1];
    retVal[1][1] = retVal[0][0];
    retVal[0][2] = 0;
    retVal[1][2] = 0;
    retVal[2][2] = 1;
    retVal[2][0] = 0;
    retVal[2][1] = 0;
    return retVal;
}

double DMatrix::distance(DMatrix m) {
    return this->toVector().distance(m.toVector());
}

std::string DMatrix::toString() {
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < 2; i++) {
        ss << '(';
        for (int j = 0; j < 2; j++) {
            ss << this->mat[i][j] << ", ";
        }
        ss << this->mat[i][2] << ')' << std::endl;
    }
    for (int j = 0; j < 2; j++) {
        ss << this->mat[2][j] << ", ";
    }
    ss << this->mat[2][2] << "))";
    return ss.str();
}

void DMatrix::print() {
    std::cout << this->toString() << std::endl;
}

DMatrix DMatrix::translateMatX(double d) {
    return DMatrix(cosh(d), 0, sinh(d),
                   0, 1, 0,
                   sinh(d), 0, cosh(d));
}

DMatrix DMatrix::translateMatY(double d) {
    return DMatrix(1, 0, 0,
                   0, cosh(d), sinh(d),
                   0, sinh(d), cosh(d));
}

DMatrix DMatrix::scalarMat(double s) {
    return DMatrix(s, 0, 0,
                   0, s, 0,
                   0, 0, s);
}

DMatrix DMatrix::rotateMat(double t, DMatrix p) {
    double r00 = cos(t);
    double r10 = sin(t);
    double r01 = -r10;
    double r11 = r00;
    DMatrix r(r00, r01, 0,
              r10, r11, 0,
              0,   0,   1);
    return p*r*p.inverse();
}

