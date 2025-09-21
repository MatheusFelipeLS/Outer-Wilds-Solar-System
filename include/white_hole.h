#ifndef WHITE_HOLE_H
#define WHITE_HOLE_H

#include <GL/glut.h>
#include <iostream>

class WhiteHole {
    public:
        WhiteHole(
            GLfloat radius, GLfloat slices, GLfloat stacks,
            GLfloat x, GLfloat y, GLfloat z
        ) : 
        radius(radius), slices(slices), stacks(stacks),
        x(x), y(y), z(z)
        {}

        void draw();

        bool inside(GLdouble camX, GLdouble camY, GLdouble camZ);

        GLfloat radius;
        GLfloat slices;
        GLfloat stacks;
        GLfloat x;
        GLfloat y;
        GLfloat z;
    private:
        GLUquadric *quad;

};

#endif