#ifndef HCELL_H
#define HCELL_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include "HVector.h"
#include "HMatrix.h"

class HCell {
public:
  HVector center;
  void* data;
  int id;
  HCell();
  HCell(HVector v);
private:

};

#endif
