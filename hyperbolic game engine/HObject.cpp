//
//  HObject.cpp
//  hyperbolic game engine
//
//  Created by Timothy Yao on 11/30/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//

#include "HObject.h"

const double tileLen = acosh((1+sqrt(5))/2);

HObject::HObject() {
    size = 0;
}

double* HObject::getScreenLocation(Camera camera, DMatrix m) {
    HMatrix ht = camera.position.cell.inverse()*position.cell;
    if (camera.position.cell[2][2][0] > 100000000) {
        std::cout << "big integers" << std::endl;
    }
    DVector result = (camera.position.offset.inverse()*ht.toDouble()*position.offset*m).toVector();
    // change projection here
    double* xy;
    switch (camera.projection) {
        case 0:
            xy = result.poincare();
            break;
        default:
            xy = result.klein();
            break;
    }
    return xy;
}

void HObject::drawPoint(Camera camera, DMatrix m) {
    double* xy = getScreenLocation(camera, m);
    glBegin(GL_POINTS);
    glVertex2d(scale*xy[0], scale*xy[1]);
    glEnd();
}

void HObject::drawPoint(Camera camera, DMatrix m, GLfloat size, GLdouble red, GLdouble green, GLdouble blue) {
    glPointSize(size);
    glColor3d(red, green, blue);
    drawPoint(camera, m);
}

void HObject::drawCircle(Camera camera, double r) {
    
}

void HObject::drawLine(Camera camera, DMatrix p, DMatrix q, int divisions) {
    DMatrix m = p;
    double* a = getScreenLocation(camera, m);
    for (int i = 0; i < divisions; i++) {
        glBegin(GL_LINES);
        glVertex2d(scale*a[0], scale*a[1]);
        DMatrix n = DMatrix(p.toVector().hlerp(q.toVector(), ((double)(i+1))/divisions));
        a = getScreenLocation(camera, n);
        glVertex2d(scale*a[0], scale*a[1]);
        glEnd();
    }
}

void HObject::drawLine(Camera camera, DMatrix p, DMatrix q, int divisions, GLfloat width, GLdouble red, GLdouble green, GLdouble blue) {
    glLineWidth(width);
    glColor3d(red, green, blue);
    drawLine(camera, p, q, divisions);
}

void HObject::drawBorder(Camera camera, int divisions, GLfloat width, GLdouble red, GLdouble green, GLdouble blue) {
    glLineWidth(width);
    glColor3d(red, green, blue);
    drawBorder(camera, divisions);
}

void HObject::drawBorder(Camera camera, int divisions) {
    double w = sqrt(1+2/sqrt(5));
    double z = sqrt((w*w-1)/2);
    
    std::vector<DVector> v(4);
    v[0] = DVector(-z,z,w);
    v[1] = DVector(z,z,w);
    v[2] = DVector(z,-z,w);
    v[3] = DVector(-z,-z,w);
    
    for (int i = 0; i < 4; i++) {
        drawLine(camera, v[i], v[(i+1)%4], divisions);
    }
}

void HObject::drawBorder(Camera camera) {
    drawBorder(camera, 5);
}

void HObject::draw(Camera camera) {
    for (int i = 0; i < points.size(); i++) {
        drawPoint(camera, *points[i]);
    }
}

void HObject::drawCenter(Camera camera) {
    drawPoint(camera, DMatrix());
}

HPosition HObject::nextPosition() {
    HPosition p;
//    p.offset*=velocityY;
    p.offset*=DMatrix::translateMatY(velocityY);
    p.offset*=DMatrix::translateMatX(velocityX);
    p.normalize();
    return p;
}

void HObject::update() {
    if (!frozen) {
        DMatrix prev = position.offset;
        position.offset*=DMatrix::translateMatY(velocityY);
        position.offset*=DMatrix::translateMatX(velocityX);
        DMatrix after = position.offset;
        position.offset.normalize();
        if (position.offset[0][0]!=position.offset[0][0]) {
            prev.print();
            after.print();
            std::cout << "in update" << std::endl;
//            throw std::exception();
        }
//        position.offset.print();
        position.normalize();
    }
}

