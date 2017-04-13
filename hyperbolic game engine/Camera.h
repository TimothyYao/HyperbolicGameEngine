//
//  Camera.h
//  hyperbolic game engine
//
//  Created by Timothy Yao on 12/6/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include "HPosition.h"

class Camera {
public:
    HPosition position;
    DVector direction;
    int projection = 0;
    double velocity = 0;
    Camera();
    Camera(HMatrix cell, DMatrix offset);
//    void translate(DMatrix translation);
    void snap();
    void moveUpCell();
    void moveUpCell(unsigned int num);
    void moveDownCell();
    void moveDownCell(unsigned int num);
    void moveRightCell();
    void moveRightCell(unsigned int num);
    void moveLeftCell();
    void moveLeftCell(unsigned int num);
};


#endif /* Camera_h */
