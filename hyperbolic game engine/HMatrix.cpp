#include "HMatrix.h"

HMatrix::HMatrix() {
    HPoint p;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->mat[i][j] = p;
            this->inv[i][j] = p;
        }
    }
    HPoint g(1, 0, 0, 0);
    for (int i = 0; i < 3; i++) {
        this->mat[i][i] = g;
        this->inv[i][i] = g;
    }
}

void HMatrix::operator=(HMatrix that) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->mat[i][j] = that.mat[i][j];
            this->inv[i][j] = that.inv[i][j];
        }
    }
}

bool HMatrix::operator==(HMatrix that) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (this->mat[i][j] != that[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool HMatrix::operator!=(HMatrix that) {
    return !this->operator==(that);
}

//HMatrix::HMatrix(const HVector a, const HVector& b, const HVector& c) {
//    for (int i = 0; i < 3; i++) {
//        this->mat[0][i] = a.v[i];
//        this->mat[1][i] = b.v[i];
//        this->mat[2][i] = c.v[i];
//    }
//}

bool HMatrix::sameCell(HMatrix cell) {
    return mat[0][2] == cell[0][2] &&
    mat[1][2] == cell[1][2];
}

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

HPoint* HMatrix::operator[](const int index) {
    return this->mat[index];
}

// bool HMatris::hasInverse() {
//
//   HMatrix m = this->mat*this->inv;
// }

void HMatrix::translateUp() {
    HMatrix inv = HMatrix::downMatrix()*this->inverse();
    *this = *this*HMatrix::upMatrix();
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->inv[i][j] = inv.mat[i][j];
        }
    }
}

void HMatrix::translateDown() {
    HMatrix inv = HMatrix::upMatrix()*this->inverse();
    *this = *this*HMatrix::downMatrix();
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->inv[i][j] = inv.mat[i][j];
        }
    }
}

void HMatrix::translateRight() {
    HMatrix inv = HMatrix::leftMatrix()*this->inverse();
    *this = *this*HMatrix::rightMatrix();
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->inv[i][j] = inv.mat[i][j];
        }
    }
}

void HMatrix::translateLeft() {
    HMatrix inv = HMatrix::rightMatrix()*this->inverse();
    *this = *this*HMatrix::leftMatrix();
    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            this->inv[i][j] = inv.mat[i][j];
        }
    }
}

HMatrix* HMatrix::up() {
    this->translateUp();
    return this;
}

HMatrix* HMatrix::down() {
    this->translateDown();
    return this;
}

HMatrix* HMatrix::right() {
    this->translateRight();
    return this;
}

HMatrix* HMatrix::left() {
    this->translateLeft();
    return this;
}

HMatrix HMatrix::rotateCC() {
    HPoint u(1, 0, 0, 0);
    HPoint o;
    HPoint nu(-1, 0, 0, 0);
    HMatrix rm;
    rm[0][0] = o;
    rm[0][1] = u;
    rm[0][2] = o;//x-u*y;
    rm[1][0] = nu;
    rm[1][1] = o;
    rm[1][2] = o;//y-nu*x;
    rm[2][0] = o;
    rm[2][1] = o;
    rm[2][2] = u;
    rm.inv[0][0] = o;
    rm.inv[0][1] = nu;
    rm.inv[0][2] = o;//x-nu*y;;
    rm.inv[1][0] = u;
    rm.inv[1][1] = o;
    rm.inv[1][2] = o;//y-u*x;
    rm.inv[2][0] = o;
    rm.inv[2][1] = o;
    rm.inv[2][2] = u;
    *this*=rm;
    return *this;
}

HMatrix HMatrix::rotateCW() {
    HPoint u(1, 0, 0, 0);
    HPoint o;
    HPoint nu(-1, 0, 0, 0);
    HPoint x = mat[0][2];
    HPoint y = mat[1][2];
    HMatrix rm;
    rm[0][0] = o;
    rm[0][1] = nu;
    rm[0][2] = o;//x-nu*y;;
    rm[1][0] = u;
    rm[1][1] = o;
    rm[1][2] = o;//y-u*x;
    rm[2][0] = o;
    rm[2][1] = o;
    rm[2][2] = u;
    rm.inv[0][0] = o;
    rm.inv[0][1] = u;
    rm.inv[0][2] = o;//x-u*y;
    rm.inv[1][0] = nu;
    rm.inv[1][1] = o;
    rm.inv[1][2] = o;//y-nu*x;
    rm.inv[2][0] = o;
    rm.inv[2][1] = o;
    rm.inv[2][2] = u;
    *this*=rm;
    return *this;
}

HMatrix HMatrix::inverse() {
    HMatrix m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m[i][j] = this->inv[i][j];
            m.inv[i][j] = this->mat[i][j];
        }
    }
    return m;
}

bool HMatrix::isAdjacent(HMatrix cell) {
    std::vector<HMatrix> adj(12);
    adj[0] = HMatrix::upMatrix();
    adj[1] = HMatrix::downMatrix();
    adj[2] = HMatrix::leftMatrix();
    adj[3] = HMatrix::rightMatrix();
    adj[4] = HMatrix::upMatrix()*HMatrix::rightMatrix();
    adj[5] = HMatrix::upMatrix()*HMatrix::leftMatrix();
    adj[6] = HMatrix::rightMatrix()*HMatrix::upMatrix();
    adj[7] = HMatrix::rightMatrix()*HMatrix::downMatrix();
    adj[8] = HMatrix::leftMatrix()*HMatrix::upMatrix();
    adj[9] = HMatrix::leftMatrix()*HMatrix::downMatrix();
    adj[10] = HMatrix::downMatrix()*HMatrix::rightMatrix();
    adj[11] = HMatrix::downMatrix()*HMatrix::leftMatrix();
    for (int i = 0; i < 12; i++) {
        if ((*this*adj[i]).sameCell(cell)) {
            return true;
        }
    }
    return false;
}

HMatrix HMatrix::upMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::o();
    m.mat[1][1] = HPoint::c();
    m.mat[1][2] = HPoint::s();
    m.mat[2][1] = HPoint::s();
    m.mat[2][2] = HPoint::c();
    
    m.inv[0][0] = HPoint::o();
    m.inv[1][1] = HPoint::c();
    m.inv[1][2] = !HPoint::s();
    m.inv[2][1] = !HPoint::s();
    m.inv[2][2] = HPoint::c();
    return m;
}

HMatrix HMatrix::downMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::o();
    m.mat[1][1] = HPoint::c();
    m.mat[1][2] = !HPoint::s();
    m.mat[2][1] = !HPoint::s();
    m.mat[2][2] = HPoint::c();
    
    m.inv[0][0] = HPoint::o();
    m.inv[1][1] = HPoint::c();
    m.inv[1][2] = HPoint::s();
    m.inv[2][1] = HPoint::s();
    m.inv[2][2] = HPoint::c();
    return m;
}

HMatrix HMatrix::rightMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::c();
    m.mat[0][2] = HPoint::s();
    m.mat[2][0] = HPoint::s();
    m.mat[1][1] = HPoint::o();
    m.mat[2][2] = HPoint::c();
    
    m.inv[0][0] = HPoint::c();
    m.inv[0][2] = !HPoint::s();
    m.inv[2][0] = !HPoint::s();
    m.inv[1][1] = HPoint::o();
    m.inv[2][2] = HPoint::c();
    return m;
}

HMatrix HMatrix::leftMatrix() {
    HMatrix m;
    m.mat[0][0] = HPoint::c();
    m.mat[0][2] = !HPoint::s();
    m.mat[2][0] = !HPoint::s();
    m.mat[1][1] = HPoint::o();
    m.mat[2][2] = HPoint::c();
    
    m.inv[0][0] = HPoint::c();
    m.inv[0][2] = HPoint::s();
    m.inv[2][0] = HPoint::s();
    m.inv[1][1] = HPoint::o();
    m.inv[2][2] = HPoint::c();
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

HVector HMatrix::toVector() {
    return HVector(this->mat[0][2], this->mat[1][2], this->mat[2][2]);
}

double HMatrix::distance(HMatrix m) {
    return this->toVector().computeDVector().distance(m.toVector().computeDVector());
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

std::string HMatrix::toStringInv() {
    std::stringstream ss;
    ss << '(';
    for (int i = 0; i < 2; i++) {
        ss << '(';
        for (int j = 0; j < 2; j++) {
            ss << this->inv[i][j].toString() << ", ";
        }
        ss << this->inv[i][2].toString() << ')' << std::endl;
    }
    for (int j = 0; j < 2; j++) {
        ss << this->inv[2][j].toString() << ", ";
    }
    ss << this->inv[2][2].toString() << "))";
    return ss.str();
}

void HMatrix::print() {
    std::cout << this->toString() << std::endl;
}

void HMatrix::printInv() {
    std::cout << this->toStringInv() << std::endl;
}


