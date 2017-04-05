#include "HCell.h"

HCell::HCell() {
    this->center = HMatrix();
}

HCell::HCell(HMatrix m) {
    this->center = m;
}

bool HCell::sameCell(HCell cell) {
    return center[0][2] == cell.center[0][2] &&
           center[1][2] == cell.center[1][2];
}
