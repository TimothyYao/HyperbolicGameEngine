#include "DMatrix.h"

DMatrix::DMatrix() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      this->mat[i][j] = 0;
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

DMatrix DMatrix::operator*(DMatrix m) {
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

void DMatrix::operator*=(DMatrix m) {
  *this = *this*m;
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
