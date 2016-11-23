#include "HCell.h"

HCell::HCell() {
    this->center = HVector();
}

HCell::HCell(HVector v) {
    this->center = v;
}
