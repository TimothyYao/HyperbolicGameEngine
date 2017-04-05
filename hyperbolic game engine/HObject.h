//
//  HObject.h
//  hyperbolic game engine
//
//  Created by Timothy Yao on 11/30/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//

#ifndef HObject_h
#define HObject_h

#include "HPosition.h"
#include "Camera.h"
#include <vector>
#include <GLUT/glut.h>

class HObject {
public:
    HObject();
    
    double* getScreenLocation(Camera camera, DMatrix m);
    
    void drawPoint(Camera camera, DMatrix m);
    void drawPoint(Camera camera, DMatrix m, GLfloat size, GLdouble red, GLdouble green, GLdouble blue);
    void drawCircle(Camera camera, double r);
    void drawLine(Camera camera, DMatrix p, DMatrix q, int divisions);
    void drawLine(Camera camera, DMatrix p, DMatrix q, int divisions, GLfloat width, GLdouble red, GLdouble green, GLdouble blue);
    void drawBorder(Camera camera, int divisions, GLfloat width, GLdouble red, GLdouble green, GLdouble blue);
    void drawBorder(Camera camera, int divisions);
    void drawBorder(Camera camera);
    virtual void draw(Camera camera);
    void draw(Camera camera, GLfloat size, GLdouble red, GLdouble green, GLdouble blue);
    void drawCenter(Camera camera);
    HPosition nextPosition();
    void update();
    
    std::vector<DMatrix*> points;
    std::string name;
    double size = 1;
    bool frozen = false;
    DMatrix velocity = DMatrix();
    HPosition position = HPosition();
};


#endif /* HObject_h */
