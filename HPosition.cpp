#include "HPosition.h"

HPosition::HPosition() {
    this->cell = HCell();
    this->offset = DVector();
}

HPosition::HPosition(HCell c, DVector v) {
    this->cell = c;
    this->offset = v;
}
