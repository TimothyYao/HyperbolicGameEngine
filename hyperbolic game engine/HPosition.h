#ifndef HPOSITION_H
#define HPOSITION_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "DVector.h"
#include "HCell.h"
#include "HMatrix.h"
#include "DMatrix.h"

class HPosition {
public:
    HCell cell;
    DMatrix offset;
    HPosition();
    HPosition(HCell c, DMatrix v);
    bool needNormalize(HMatrix& result);
    void normalize();
private:
    
};

#endif
