//
//  Camera.cpp
//  hyperbolic game engine
//
//  Created by Timothy Yao on 12/6/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//

#include "Camera.h"

Camera::Camera() {
}

Camera::Camera(HMatrix cell, DMatrix offset) {
    position = HPosition();
    position.cell = cell;
    position.offset = offset;
    direction = DVector(1, 0, 1);
    direction.normalize();
}

void Camera::snap() {
    this->position.offset = DMatrix();
}
void Camera::moveUpCell() {
    this->position.cell.translateUp();
}
void Camera::moveUpCell(unsigned int num) {
    for (unsigned int i = 0; i < num;i ++) {
        this->position.cell.translateUp();
    }
}
void Camera::moveDownCell() {
    this->position.cell.translateDown();
}
void Camera::moveDownCell(unsigned int num){
    for (unsigned int i = 0; i < num;i ++) {
        this->position.cell.translateDown();
    }
}
void Camera::moveRightCell() {
    this->position.cell.translateRight();
}
void Camera::moveRightCell(unsigned int num) {
    for (unsigned int i = 0; i < num;i ++) {
        this->position.cell.translateRight();
    }
}
void Camera::moveLeftCell() {
    this->position.cell.translateLeft();
}
void Camera::moveLeftCell(unsigned int num) {
    for (unsigned int i = 0; i < num;i ++) {
        this->position.cell.translateLeft();
    }
}


