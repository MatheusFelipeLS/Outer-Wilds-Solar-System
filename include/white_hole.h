#ifndef WHITE_HOLE_H
#define WHITE_HOLE_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

class WhiteHole {
    public:
        WhiteHole(
            GLfloat radius, GLfloat slices, GLfloat stacks,
            GLfloat x, GLfloat y, GLfloat z
        ) : 
        radius(radius), slices(slices), stacks(stacks)
        {
            p.x = x;
            p.y = y;
            p.z = z;
        }

        void draw();

        bool inside(GLdouble camX, GLdouble camY, GLdouble camZ);

        Vertex get_position();

        void loadTexture(const char* filename);


        GLfloat radius;
        GLfloat slices;
        GLfloat stacks;
        Vertex p;
        GLuint textureID;


    private:
        GLUquadric *quad;

};

#endif