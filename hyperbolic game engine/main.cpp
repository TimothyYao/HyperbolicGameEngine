//
//  main.cpp
//  hyperbolic game engine
//
//  Created by Timothy Yao on 11/8/16.
//  Copyright © 2016 Timothy Yao. All rights reserved.
//


#include "HObject.h"
#include <chrono>

// Use the following line on a Windows machine:
// #include <GL/glut.h>
//
// This line is for Max OSX
#include <GLUT/glut.h>

const double DEG2RAD = 3.14159/180;
const double tileLen = acosh((1+sqrt(5))/2);
long msLastAnimated;
bool showTile = false;
bool overlapping = false;
bool gravityOn = true;

// draws point at coordinate (x,y) with size s
void drawPoint(GLdouble x, GLdouble y, GLdouble size) {
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
}

// GLUT callback functions
long getTime();
void display(void);
void reshape(int, int);
void processKeyboard(unsigned char, int, int);
void processSpecialKeys(int, int, int);
void animate(int);
void test();

long getTime() {
    return std::chrono::duration_cast< std::chrono::milliseconds >(
                                                    std::chrono::system_clock::now().time_since_epoch()
                                                                                           ).count();
}

class Platform : public HObject {
public:
    DVector start = DMatrix::translateMatX(-tileLen/2).toVector();
    DVector end = DMatrix::translateMatX(tileLen/2).toVector();
    DVector midTop = DMatrix::translateMatY(tileLen/2).toVector();
    DVector midBot = DMatrix::translateMatY(-tileLen/2).toVector();
    DVector midpoint;
    DVector vertPlane;
    DVector horiPlane;
    DVector gravity;
    bool active = true;
    bool floor = false;
    double halfLen;
    GLdouble r = 1;
    GLdouble g = 0;
    GLdouble b = 0;
    GLfloat size = 3;
    
    Platform() {
        initBounds();
    }
    Platform(GLdouble r, GLdouble g, GLdouble b) {;
        this->r = r;
        this->g = g;
        this->b = b;
        initBounds();
    }
    Platform(DVector start, DVector end) {
        this->start = start;
        this->end = end;
        initBounds();
    }
    Platform(DVector start, DVector end, GLdouble r, GLdouble g, GLdouble b) {
        this->start = start;
        this->end = end;
        this->r = r;
        this->g = g;
        this->b = b;
        initBounds();
    }
    void initBounds() {
        DMatrix t = position.cell.toDouble()*position.offset; //try to remove toDouble()
        DVector p = (t*start).toVector().toNormal();
        DVector q = (t*end).toVector().toNormal();
//        vertPlane = p.planeBetween(q);
        vertPlane = start.toNormal().planeBetween(end.toNormal());
        
        midpoint = start.midpoint(end);
        DMatrix left = DMatrix::translateMatX(-tileLen/2);
        DMatrix right = DMatrix::translateMatX(tileLen/2);
        DVector plane = left.toVector().planeBetween(right.toVector());
        double angle = acos(plane.innerProd(vertPlane));
        if (end[0]>midpoint[0]&&end[1]>midpoint[1]) {
            angle = -angle;
        }
        
        DMatrix rot = DMatrix(0,1,0,-1,0,0,0,0,1);
        
        midTop = (DMatrix(midpoint)*rot*DMatrix(start)*DMatrix(midpoint).inverse()).toVector().toNormal();
        midBot = (DMatrix(midpoint)*rot*DMatrix(end)*DMatrix(midpoint).inverse()).toVector().toNormal();
        horiPlane = midBot.toNormal().planeBetween(midTop.toNormal());
        halfLen = p.distance(q)/2;
        gravity = DVector().hlerp((DMatrix(midTop).inverse()*DMatrix(midBot)).toVector().toNormal(), 1/120);
    }
    
    void draw(Camera camera) {
        HObject::drawLine(camera, start, end, 500, size, r, g, b);
    }
    double distanceW(DMatrix offset, int rotations) {
        DMatrix objPos = DMatrix().rotate(DEG2RAD*90*rotations)*offset;
        return objPos.toVector().toNormal().distancePlane(vertPlane);
    }
    double distanceH(DMatrix offset, int rotations) {
        DMatrix objPos = DMatrix().rotate(DEG2RAD*90*rotations)*offset;
        return objPos.toVector().toNormal().distancePlane(horiPlane);
    }
    int getRotations(HPosition pos) {
        int rotations = 0;
        HMatrix objCellMatrix = pos.cell;
        while (objCellMatrix != pos.cell) {
            rotations++;
            objCellMatrix.rotateCC();
        }
        return rotations;
    }
    
    bool isColliding(HPosition pos) {
        if (!active) return false;
        if (!pos.cell.sameCell(position.cell) && !pos.cell.isAdjacent(position.cell)) {
            return false;
        }
        if (pos.cell.isAdjacent(position.cell)) {
            pos.offset = (position.cell.inverse()*pos.cell).toDouble()*pos.offset;
        }
        int rotations = getRotations(pos);
        return (std::abs(distanceW(pos.offset, rotations)) < halfLen &&
                std::abs(distanceH(pos.offset, rotations)) < 0.02);
    }
    
    bool isColliding(HObject obj) {
        HPosition pos = obj.position;
        if (!active) return false;
        if (!pos.cell.sameCell(position.cell) && !pos.cell.isAdjacent(position.cell)) {
            return false;
        }
        if (pos.cell.isAdjacent(position.cell)) {
            pos.offset = (position.cell.inverse()*pos.cell).toDouble()*pos.offset;
        }
        int rotations = getRotations(pos);
        return (std::abs(distanceW(pos.offset, rotations)) < halfLen &&
                std::abs(distanceH(pos.offset, rotations)) < 0.02 + obj.size);
    }
    
    DMatrix getGravity(HPosition pos) {
        if (pos.cell.sameCell(position.cell)) {
            int rotations = getRotations(pos);
            return DMatrix::translateMatY(-tileLen/6000);
            double dist = tileLen/distanceH(pos.offset, rotations);
            return /*obj.position.offset.getRotation().inverse()**/DMatrix(DVector().hlerp((DMatrix(midTop).inverse()*DMatrix(midBot)).toVector().toNormal(), dist/50000));
        }
        return DMatrix();
    }
    
    DMatrix getCorrection(HPosition pos, DMatrix previous) {
        int rotations = getRotations(pos);
        
        double dx0 = distanceW(previous, rotations);
        double dy0 = distanceH(previous, rotations);
        double dx = distanceW(pos.offset, rotations);
        double dy = distanceH(pos.offset, rotations);
        
        
        if (std::abs(dx) < halfLen && (std::abs(dy) < 0.02 || dy0*dy <= 0)) {
            //return base*DMatrix(midpoint.hlerp(midTop, (0.02-dy)/halfLen));
            if (dy0 == 0) {
                return DMatrix(midpoint).inverse()*DMatrix(midpoint.hlerp(midTop, (0.02-dy)/halfLen).toNormal());
            }
            if (dy0 == dy) {
                return DMatrix();
            }
            double t = std::abs((0.02-dy)/(dy0-dy));
            if (t > 1) {
                t = 1;
            }
//            PV = X
//            P-1PV = P-1X
            return DMatrix(DVector().hlerp((previous.inverse()*pos.offset).inverse().toVector().toNormal(),t));
//            return DMatrix(DVector().hlerp(obj.velocity.inverse().toVector().toNormal(),t));
        }
        return DMatrix();
    }
};


std::vector<HObject*> objects;
std::vector<Platform*> platforms;
Camera c;
HObject dude;
int counter = 0;

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
//    test();
    
    glutInit(&argc, argv);
    glutInitWindowSize(750, 750);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("HGE");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processKeyboard);
    glutSpecialFunc(processSpecialKeys);
    glClearColor(0,0,0,1);
    
//    dude.velocity = DMatrix::translateMatX(tileLen/120);
//    dude.frozen = true;
    
    for (int i = 0; i < 13; i++) {
        objects.push_back(new HObject());
    }
    for (int i = 0; i < 3; i++) {
        objects[i+1]->position.cell.up();
        objects[i+4]->position.cell.down();
        objects[i+7]->position.cell.right();
        objects[i+10]->position.cell.left();
    }
    objects[2]->position.cell.right();
    objects[3]->position.cell.left();
    objects[5]->position.cell.right();
    objects[6]->position.cell.left();
    objects[8]->position.cell.up();
    objects[9]->position.cell.down();
    objects[11]->position.cell.up();
    objects[12]->position.cell.down();
    
    double w = sqrt(1+2/sqrt(5));
    double z = sqrt((w*w-1)/2);
    
    std::vector<DVector> v(4);
    v[0] = DVector(-z,z,w);
    v[1] = DVector(z,z,w);
    v[2] = DVector(z,-z,w);
    v[3] = DVector(-z,-z,w);
    
    for (int i = 0; i < 4; i++) {
        platforms.push_back(new Platform());
        platforms.back()->floor = true;
    }
    platforms[0]->position.cell.left();
    platforms[2]->position.cell.right();
    platforms[3]->position.cell.right();
    platforms[3]->position.cell.down();
    
    for (int i = 0; i < 4; i++) {
        platforms[i]->start = v[3];
        platforms[i]->end = v[2];
    }
    
    DVector v1 = DVector().hlerp(HMatrix::leftMatrix().toDouble().toVector(), 0.25);
    DVector v2 = DVector().hlerp(HMatrix::rightMatrix().toDouble().toVector(), 0.25);
    DVector v3 = v[3].hlerp(v[2], 0.25);
    DVector v4 = v[3].hlerp(v[2], 0.75);
    
    for (int i = 0; i < 3; i++) {
        platforms.push_back(new Platform(DVector(), DVector(), 0, 1, 0));
        platforms.back()->position.cell.right();
    }
    platforms[4]->start = v1;
    platforms[4]->end = v2;
    platforms[4]->floor = true;
    platforms[5]->start = v1;
    platforms[5]->end = v3;
    platforms[6]->start = v2;
    platforms[6]->end = v4;
    
//    platforms.push_back(new Platform(DVector(), (DMatrix::translateMatY(tileLen/2)*DMatrix::translateMatX(-tileLen/2)).toVector(), 0, 1, 0));
    
    for (int i = 0; i < platforms.size(); i++) {
        platforms[i]->initBounds();
    }
    
    
    glutTimerFunc(5, animate, 0);
//     try glutTimerFunc
//    
//    dude.points.push_back(DMatrix());
//    
//    
//    for (int i = 0; i < 13; i++) {
////        objects[i]
//    }
    msLastAnimated = getTime();
//    HObject::scale = 300;
    glutMainLoop();
    
}

void animate(int value) {
    int stepsize = 1000/60;
    static long time = 0;
    time += getTime()-msLastAnimated;
    msLastAnimated = getTime();
    while (time > stepsize) {
        time -= stepsize;
        
        // update every 1/60 s here
        //if (DMatrix().distance(dude.position.cell.center.toDouble()*dude.position.offset) > tileLen*3) {
          //  dude.position = HPosition();
        //}
        DMatrix previous = dude.position.offset;
        dude.update();
        overlapping = false;
        for (int i = 0; i < platforms.size(); i++) {
            if (platforms[i]->isColliding(dude.position)) {
                DMatrix correction = platforms[i]->getCorrection(dude.position, previous);
                dude.position.offset*=correction;
                dude.position.normalize();
                overlapping = true;
                if (platforms[i]->floor) {
                    dude.grounded = true;
                    dude.hasJumped = false;
                }
                dude.velocityY = 0;
                dude.velocityX = 0;
            }
        }
        if (gravityOn && !overlapping) {
            dude.velocityY-=tileLen/6000;
        }
//        for (int i = 0; i < platforms.size(); i++) {
//            dude.velocity*=platforms[i]->getGravity(dude.position);
//        }
        dude.position.normalize();
        c.position.cell = dude.position.cell;
        c.position.offset = dude.position.offset;
        glutPostRedisplay();
        
    }
    glutTimerFunc(5, animate, 0);
}

/*! glut display callback function.
 */
void display()
{
    /* clear the color buffer (resets everything to black) */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* set the current drawing color to red */
    glColor3d(1, 0, 0);
    glPointSize(1);
    if (showTile) {
        for (int i = 0; i < 13; i++) {
            objects[i]->drawCenter(c);
            objects[i]->drawBorder(c);
            objects[i]->drawLine(c, DMatrix(), DMatrix().toVector().midpoint(HMatrix().up().toDouble().toVector()), 50);
        }
    } else {
        for (int i = 0; i < platforms.size(); i++) {
            platforms[i]->draw(c);
//            platforms[i]->drawPoint(c, platforms[i]->midTop);
//            platforms[i]->drawPoint(c, platforms[i]->midBot);
//            DMatrix midpoint(platforms[i]->start+platforms[i]->end);
//            glPointSize(5);
//            platforms[i]->drawPoint(c, midpoint);
//            glPointSize(1);
        }
    }
    double golden = (1+sqrt(5))/2;
    double sgolden = sqrt(golden);
    DVector v(0, sgolden/4, golden);
    v.normalize();
    DMatrix up(1, 0, 0, 0, v[2], v[1], 0, v[1], v[2]);
    DMatrix right(v[2], 0, v[1], 0, 1, 0, v[1], 0, v[2]);
    glColor3d(0, 1, 1);
    glPointSize(7);
    dude.drawPoint(c, dude.position.offset.inverse());
    glPointSize(3);
    dude.drawCenter(c);
    dude.drawLine(c, DMatrix(), up, 1);
    dude.drawLine(c, DMatrix(), right, 1);
    dude.drawLine(c, up, right, 1);
//    dude.drawPoint(c, dude.position.offset.inverse()*DMatrix(dude.position.offset.toVector().toNormal())*dude.position.offset.getRotation(), 3, 0, 0, 1);
//    dude.drawLine(c, DMatrix(), dude.position.offset.inverse()*DMatrix(dude.position.offset.toVector().toNormal())*dude.position.offset.getRotation()*up, 1);
    glColor3d(0, 0, 1);
    if (gravityOn) {
        drawPoint(250, 250, 20);
    }
//    std::cout << counter << std::endl;
    counter++;
    
    /* swap the back and front buffers so we can see what we just drew */
    glutSwapBuffers();
}

void processKeyboard(unsigned char key, int xx, int yy) {
    double d = acosh((1+sqrt(5))/2);
    switch (key) {
        case 'r': // reset camera to origin
            c.position = HPosition();
            break;
        case 32: // snap camera to cell
            c.position.offset = DMatrix();
            break;
        case 'p': // snap object to cell
            dude.position.offset = DMatrix();
            break;
        case 'o': // reset object to origin
            dude.position = HPosition();
            break;
        case 'q': // rotate object counterclockwise 15 degrees
            dude.position.offset.rotate(15*DEG2RAD);
            break;
        case 'e': // rotate object clockwise 15 degrees
            dude.position.offset.rotate(-15*DEG2RAD);
//            dude.position.offset*=DMatrix::rotateMat(15*DEG2RAD, dude.position.offset);
            break;
        case 'z': // rotate camera counterclockwise 15 degrees
            c.position.offset.rotate(-15*DEG2RAD);
            break;
        case 'x': // rotate camera clockwise 15 degrees
            c.position.offset.rotate(15*DEG2RAD);
            break;
        case ',': // rotate camera counterclockwise 90 degrees
            c.position.cell.rotateCC();
            break;
        case '.': // rotate camera clockwise 90 degrees
            c.position.cell.rotateCW();
            break;
        case 'w': // move object up d/5 units
            if (dude.grounded) {
                dude.velocityY = tileLen/105;
                dude.grounded = false;
            } else if (!dude.hasJumped) {
                dude.velocityY = tileLen/105;
                dude.hasJumped = true;
            }
//            dude.frozen = false;
            break;
        case 's':
//            dude.frozen = true;
            dude.velocityY = 0;
            dude.velocityX = 0;
            break;
        case 'a':
            dude.velocityX-=tileLen/200;
            if (dude.velocityX < -tileLen/110) {
                dude.velocityX = -tileLen/110;
            }
            break;
        case 'd':
            dude.velocityX+=tileLen/200;
            if (dude.velocityX > tileLen/110) {
                dude.velocityX = tileLen/110;
            }
            break;
        case '`': // change projection
            c.projection = (c.projection+1)%2;
            break;
        case '=': // swap show tile
            showTile = !showTile;
            break;
        case 'g': //toggle gravity
            gravityOn = !gravityOn;
            break;
        case 'b': {
            std::cout << "cell:" << std::endl;
            dude.position.cell.print();
            std::cout << "offset:" << std::endl;
            dude.position.offset.print();
            std::cout << "offset.toVector():" << std::endl;
            dude.position.offset.toVector().print();
            std::cout << "offset.toVector().toNormal():" << std::endl;
            dude.position.offset.toVector().toNormal().print();
            std::cout << "offset.getRotation():" << std::endl;
            dude.position.offset.getRotation().print();
            std::cout << "offset.toVector().toNormal()*offset.getRotation():" << std::endl;
            (DMatrix(dude.position.offset.toVector().toNormal())*dude.position.offset.getRotation()).print();
            std::cout << "grounded: " << dude.grounded << std::endl;
            std::cout << "hasJumped: " << dude.hasJumped << std::endl;
            break;
        }
        default:
            break;
    }
    dude.position.normalize();
    c.position.cell = dude.position.cell;
    c.position.offset = dude.position.offset;
    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {
    
    double golden = (1+sqrt(5))/2;
    double sgolden = sqrt(golden);
    DVector v(0, sgolden/2, golden);
    v.normalize();
    DMatrix up(1, 0, 0, 0, v[2], v[1], 0, v[1], v[2]);
    DMatrix right(v[2], 0, v[1], 0, 1, 0, v[1], 0, v[2]);
    switch (key) {
        case GLUT_KEY_UP : {
            c.position.offset*=up;
            break;
        }
        case GLUT_KEY_DOWN : {
            c.position.offset*=up.inverse();
            break;
        }
        case GLUT_KEY_RIGHT : {
            c.position.offset*=right;
            break;
        }
        case GLUT_KEY_LEFT : {
            c.position.offset*=right.inverse();
            break;
        }
        default:
            break;
    }
    c.position.normalize();
    c.position.cell = dude.position.cell;
    c.position.offset = dude.position.offset;
    glutPostRedisplay();
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
    
//    HVector v;
//    v.printDouble();
//    v.up().right().printDouble();
}
