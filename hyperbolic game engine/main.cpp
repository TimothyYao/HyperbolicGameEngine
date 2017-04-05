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
GLdouble trsize = 10;

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
        DMatrix t = position.cell.center.toDouble()*position.offset; //try to remove toDouble()
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
//        midpoint.print();
//        (DMatrix()*rot).print();
//        midBot = midpoint*DMatrix().rotate(angle)*midBot;
        
        
//        horiPlane = (t*midBot).toVector().toNormal().planeBetween((t*midTop).toVector().toNormal());
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
    int getRotations(HObject obj) {
        int rotations = 0;
        HMatrix objCellMatrix = obj.position.cell.center;
        while (objCellMatrix != position.cell.center) {
            rotations++;
            objCellMatrix.rotateCC();
        }
        return rotations;
    }
    
    bool isColliding(HObject obj) {
        if (!active) return false;
        if (!obj.position.cell.sameCell(position.cell)) return false;
        int rotations = getRotations(obj);
        return (std::abs(distanceW(obj.position.offset, rotations)) < halfLen &&
                std::abs(distanceH(obj.position.offset, rotations)) < 0.02);
    }
    
    DMatrix getGravity(HObject obj) {
        if (obj.position.cell.sameCell(position.cell)) {
            int rotations = getRotations(obj);
            double scale = tileLen/distanceH(obj.position.offset, rotations);
            return DMatrix(DVector().hlerp((DMatrix(midTop).inverse()*DMatrix(midBot)).toVector().toNormal(), scale/50000));
        }
        return DMatrix();
    }
    
    DMatrix getCorrection(HObject obj, DMatrix previous) {
        int rotations = getRotations(obj);
        
        double dx0 = distanceW(previous, rotations);
        double dy0 = distanceH(previous, rotations);
        double dx = distanceW(obj.position.offset, rotations);
        double dy = distanceH(obj.position.offset, rotations);
        
        double t = std::abs((0.02-dy)/(dy0-dy));
        if (t > 1) {
            t = 1;
        }
        
        if (std::abs(dx) < halfLen && (std::abs(dy) < 0.02 || dy0*dy <= 0)) {
            //return base*DMatrix(midpoint.hlerp(midTop, (0.02-dy)/halfLen));
            if (dy0 == 0) {
                return DMatrix(midpoint).inverse()*DMatrix(midpoint.hlerp(midTop, (0.02-dy)/halfLen).toNormal());
            }
            if (dy0 == dy) {
                return DMatrix();
            }
            return DMatrix(DVector().hlerp(obj.velocity.inverse().toVector().toNormal(),t));
//            return obj.position.offset.inverse()*DMatrix(obj.position.offset.toVector().toNormal().hlerp(previous.toVector(), t).toNormal());
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
        objects[i+1]->position.cell.center.up();
        objects[i+4]->position.cell.center.down();
        objects[i+7]->position.cell.center.right();
        objects[i+10]->position.cell.center.left();
    }
    objects[2]->position.cell.center.right();
    objects[3]->position.cell.center.left();
    objects[5]->position.cell.center.right();
    objects[6]->position.cell.center.left();
    objects[8]->position.cell.center.up();
    objects[9]->position.cell.center.down();
    objects[11]->position.cell.center.up();
    objects[12]->position.cell.center.down();
    
    for (int i = 0; i < 3; i++) {
        platforms.push_back(new Platform());
    }
//    platforms[1]->active = false;
    platforms[0]->position.cell.center.left();
    platforms[2]->position.cell.center.right();
    platforms.push_back(new Platform(DVector(), (DMatrix::translateMatY(tileLen/2)*DMatrix::translateMatX(-tileLen/2)).toVector(), 0, 1, 1));
//    platforms.push_back(new Platform(DMatrix(), DMatrix::translateMatY(tileLen/2), 0, 1, 1));
    platforms[3]->position.cell.center.right();
//    platforms[3]->position.offset = DMatrix::translateMatX(-tileLen/2);
    
    
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
            if (platforms[i]->isColliding(dude)) {
                DMatrix correction = platforms[i]->getCorrection(dude, previous);
                dude.position.offset*=correction;
                dude.position.normalize();
                overlapping = true;
                dude.velocity = DMatrix();
//                dude.frozen = true;
                trsize = 20;
            }
        }
        if (!overlapping) {
            for (int i = 0; i < platforms.size(); i++) {
                dude.velocity*=platforms[i]->getGravity(dude);
            }
        }
        dude.position.normalize();
        c.position.cell.center = dude.position.cell.center;
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
            platforms[i]->drawPoint(c, platforms[i]->midTop);
            platforms[i]->drawPoint(c, platforms[i]->midBot);
            DMatrix midpoint(platforms[i]->start+platforms[i]->end);
            glPointSize(5);
            platforms[i]->drawPoint(c, midpoint);
            glPointSize(1);
        }
    }
    double golden = (1+sqrt(5))/2;
    double sgolden = sqrt(golden);
    DVector v(0, sgolden/4, golden);
    v.normalize();
    DMatrix up(1, 0, 0, 0, v[2], v[1], 0, v[1], v[2]);
    DMatrix right(v[2], 0, v[1], 0, 1, 0, v[1], 0, v[2]);
    glColor3d(0, 1, 0);
    glPointSize(7);
    dude.drawPoint(c, dude.position.offset.inverse());
    glPointSize(3);
    dude.drawCenter(c);
    dude.drawLine(c, DMatrix(), up, 1);
    dude.drawLine(c, DMatrix(), right, 1);
    dude.drawLine(c, up, right, 1);
    glColor3d(0, 0, 1);
    if (overlapping) {
        drawPoint(250, 250, trsize);
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
            c.position.cell.center.rotateCC();
            break;
        case '.': // rotate camera clockwise 90 degrees
            c.position.cell.center.rotateCW();
            break;
        case 'w': // move object up d/5 units
            dude.velocity = DMatrix::translateMatY(tileLen/120)*dude.velocity;
//            dude.frozen = false;
            break;
        case 's':
//            dude.frozen = true;
            dude.velocity = DMatrix();
            break;
        case 'a':
            dude.velocity = DMatrix::translateMatX(-tileLen/120)*dude.velocity;
            break;
        case 'd':
//            dude.frozen = false;
            dude.velocity = DMatrix::translateMatX(tileLen/120)*dude.velocity;
            break;
        case '`': // change projection
            c.projection = (c.projection+1)%2;
            break;
        case '=': // swap show tile
            showTile = !showTile;
            break;
        case 'b': {
            std::cout << platforms[1]->distanceW(dude.position.offset, 0) << std::endl;
            std::cout << platforms[1]->distanceH(dude.position.offset, 0) << std::endl;
            break;
        }
        default:
            break;
    }
    dude.position.normalize();
    c.position.cell.center = dude.position.cell.center;
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
    c.position.cell.center = dude.position.cell.center;
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
