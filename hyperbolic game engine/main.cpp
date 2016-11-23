//
//  main.cpp
//  hyperbolic game engine
//
//  Created by Timothy Yao on 11/8/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//


#include "HCell.h"
#include "DMatrix.h"

// Use the following line on a Windows machine:
// #include <GL/glut.h>
//
// This line is for Max OSX
#include <GLUT/glut.h>

const float DEG2RAD = 3.14159/180;

void drawCircle(float radius)
{
    glBegin(GL_LINE_LOOP);
    
    for (int i=0; i < 360; i++)
    {
        float degInRad = i*DEG2RAD;
        glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
    }
    
    glEnd();
}

// draws point at coordinate (x,y) with size s
void drawPoint(double x, double y, double size) {
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
}

// currently only draws center of cell
void drawCell(HCell c) {
    DVector v = c.center.computeDVector();
    double* coord = v.poincare();
    drawPoint(coord[0]*200, coord[1]*200, 3);
}

// GLUT display callback function
void display(void);
// GLUT window reshape callback function
void reshape(int, int);


void test();



int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    test();
    
    glutInit(&argc, argv);
    
    /* set the window size to 512 x 512 */
    glutInitWindowSize(512, 512);
    
    /* set the display mode to Red, Green, Blue and Alpha
     allocate a depth buffer
     enable double buffering
     */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    /* create the window (and call it Lab 1) */
    glutCreateWindow("HGE");
    
    /* set the glut display callback function
     this is the function GLUT will call every time
     the window needs to be drawn
     */
    glutDisplayFunc(display);
    
    /* set the glut reshape callback function
     this is the function GLUT will call whenever
     the window is resized, including when it is
     first created
     */
    glutReshapeFunc(reshape);
    
    /* set the default background color to black */
    glClearColor(0,0,0,1);
    
    /* enter the main event loop so that GLUT can process
     all of the window event messages
     */
    glutMainLoop();
    
}

/*! glut display callback function.  Every time the window needs to be drawn,
 glut will call this function.  This includes when the window size
 changes, or when another window covering part of this window is
 moved so this window is uncovered.
 */
void display()
{
    /* clear the color buffer (resets everything to black) */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* set the current drawing color to red */
    glColor3d(1, 0, 0);
    
    /* start drawing triangles, each triangle takes 3 vertices */
//    glBegin(GL_TRIANGLES);
    
//    /* give the 3 triangle vertex coordinates 1 at a time */
//    glVertex2f(10, 10);
//    glVertex2f(250, 400);
//    glVertex2f(400, 10);
//    
//    /* tell OpenGL we're done drawing triangles */
//    glEnd();
    
//    drawPoint(0, 0, 4);
    HVector v;
    drawCell(HCell());
    drawCell(HCell(v.up()));
    drawCell(HCell(v.right()));
    drawCell(HCell(v.left()));
    drawCell(HCell(v.down()));
    drawCell(HCell(v.right().up()));
//    drawCell(HCell(v.right().right().up()));
    drawCell(HCell(v.up().up()));
//    drawCell(HCell(v.up().up().up()));
    drawCell(HCell(v.up().right()));
    drawCell(HCell(v.up().left()));
//    drawCell(HCell(v.right().up()));
//    drawCell(HCell(v.up().right().right()));
//    drawCell(HCell(v.right().up().left()));
    
    
    
    // draw circle
//    drawCircle(20);
    
    
    
    /* swap the back and front buffers so we can see what we just drew */
    glutSwapBuffers();
}

/*! glut reshape callback function.  GLUT calls this function whenever
 the window is resized, including the first time it is created.
 You can use variables to keep track the current window size.
 */
void reshape(int width, int height)
{
    /* tell OpenGL we want to display in a recangle that is the
     same size as the window
     */
    glViewport(0,0,width,height);
    
    /* switch to the projection matrix */
    glMatrixMode(GL_PROJECTION);
    
    /* clear the projection matrix */
    glLoadIdentity();
    
    /* set the camera view, orthographic projection in 2D */
    gluOrtho2D(-256,256,-256,256);
    
    /* switch back to the model view matrix */
    glMatrixMode(GL_MODELVIEW);
}


void test() {
    HPoint p1;
    p1.print();
    HPoint p2(0, 1, 2, 3);
    p2.printDouble();
    int a[4] = {4, 2, 2, 4};
    HPoint p3(a);
    p3.printDouble();
    HPoint p4 = p2 + p3;
    p4.print();
    p4 = p2 - p3;
    p4.print();
    p4 = p2*p3;
    std::cout << "-----" << std::endl;
    p4.printDouble();
    std::cout << "-----" << std::endl;
    p2*=p3;
    p2.print();
    HVector v1(p1, p2, p3);
    std::cout << "-----" << std::endl;
    p1.print();
    p2.print();
    p3.print();
    std::cout << "-----" << std::endl;
    v1.print();
    DMatrix d1, d2;
    d1.mat[0][0] = 1;
    d1.mat[0][1] = 2;
    d1.mat[0][2] = 1;
    d1.mat[1][0] = 2;
    d1.mat[1][1] = 1;
    d1.mat[1][2] = 2;
    d1.mat[2][0] = 1;
    d1.mat[2][1] = 1;
    d1.mat[2][2] = 2;
    
    d2.mat[0][0] = 2;
    d2.mat[0][1] = 3;
    d2.mat[0][2] = 3;
    d2.mat[1][0] = 1;
    d2.mat[1][1] = 2;
    d2.mat[1][2] = 1;
    d2.mat[2][0] = 3;
    d2.mat[2][1] = 2;
    d2.mat[2][2] = 1;
    
    DMatrix d3 = d1*d2;
    d3.print();
    std::cout << "-----" << std::endl;
    
    HMatrix m1, m2;
    m1.mat[0][0] = HPoint(1, 0, 2, 0);
    m1.mat[0][1] = HPoint(2, 3, 3, 0);
    m1.mat[0][2] = HPoint(0, 0, 1, 1);
    m1.mat[1][0] = HPoint(1, 4, 0, 0);
    m1.mat[1][1] = HPoint(2, 0, 2, 2);
    m1.mat[1][1] = HPoint(1, 2, 5, 0);
    m1.mat[2][0] = HPoint(4, 2, 2, 3);
    m1.mat[2][1] = HPoint(2, 6, 1, 0);
    m1.mat[2][2] = HPoint(1, 0, 5, 1);
    
    m2.mat[0][0] = HPoint(3, 4, 1, 5);
    m2.mat[0][1] = HPoint(1, 3, 1, 1);
    m2.mat[0][2] = HPoint(1, 1, 5, 2);
    m2.mat[1][0] = HPoint(2, 6, 1, 2);
    m2.mat[1][1] = HPoint(2, 1, 2, 2);
    m2.mat[1][1] = HPoint(3, 2, 3, 3);
    m2.mat[2][0] = HPoint(1, 2, 1, 3);
    m2.mat[2][1] = HPoint(4, 3, 6, 2);
    m2.mat[2][2] = HPoint(2, 6, 1, 1);
    
    d1 = m1.toDouble();
    d2 = m2.toDouble();
    
    (d1*d2).print();
    
    (m1*m2).toDouble().print();
    
    std::cout << "-----" << std::endl;
    
    HVector v;
    v.printDouble();
    v.up().right().printDouble();
}
