#ifndef HCELL_H
#define HCELL_H
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include "HVector.h"
#include "HMatrix.h"

// THIS CLASS IS DEPRECATED!

class HCell {
public:
//    HVector center;
    HMatrix center;
    void* data;
    int id;
    HCell();
//    HCell(HVector v);
    HCell(HMatrix m);
    bool sameCell(HCell cell);
private:
    
};

#endif
