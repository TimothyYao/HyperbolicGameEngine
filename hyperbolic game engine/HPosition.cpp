#include "HPosition.h"


HPosition::HPosition() {
    this->cell = HCell();
    this->offset = DMatrix();
}

HPosition::HPosition(HCell c, DMatrix v) {
    this->cell = c;
    this->offset = v;
}

// needs some revision
// look at distances from planes instead of the current solution
bool HPosition::needNormalize(HMatrix& nearest) {

    
    std::vector<HMatrix> directions = std::vector<HMatrix>(4);
    directions[0] = HMatrix::upMatrix();
    directions[1] = HMatrix::downMatrix();
    directions[2] = HMatrix::leftMatrix();
    directions[3] = HMatrix::rightMatrix();
    
    for (int i = 0; i < 4; i++) {
        
        if (offset[2][2] > 10) {
            std::cout << "problem" << std::endl;
            //        offset.print();
            offset.print();
            std::cout << offset.toVector().toNormal().distancePlane(DVector().planeBetween(directions[i].toDouble().toVector().toNormal())) << std::endl;
        }
        if (offset.toVector().toNormal().distancePlane(DVector().planeBetween(directions[i].toDouble().toVector().toNormal())) > 0) {
            nearest = directions[i];
//            std::cout << "changed" << std::endl;oqs
            return true;
        }
    }
    return false;
            
//    bool changed = false;
//    int direction = -1;
//    double minDist = offset.distance(DMatrix());
//    std::vector<HMatrix> neighbors = std::vector<HMatrix>(4);
//    for (int i = 0; i < 4; i++) { neighbors[i] = cell.center; }
//    neighbors[0].up();
//    neighbors[1].down();
//    neighbors[2].left();
//    neighbors[3].right();
//    for (int i = 0; i < neighbors.size(); i++) {
//        double dist = (cell.center.toDouble()*offset).distance(neighbors[i].toDouble());
//        if (dist < minDist) {
//            changed = true;
//            minDist = dist;
//            direction = i;
//        }
//    }
//    switch (direction) {
//        case 0:
//            nearest = HMatrix::upMatrix();
//            break;
//        case 1:
//            nearest = HMatrix::downMatrix();
//            break;
//        case 2:
//            nearest = HMatrix::leftMatrix();
//            break;
//        case 3:
//            nearest = HMatrix::rightMatrix();
//            break;
//        default:
//            break;
//    }
//    return changed;
}

void HPosition::normalize() {
    HMatrix nearest;
    if (needNormalize(nearest)) {
        cell.center*=nearest;
        offset=nearest.inverse().toDouble()*offset;
    }
}
