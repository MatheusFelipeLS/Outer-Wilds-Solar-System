#ifndef BRITTLE_HOLLOW_H
#define BRITTLE_HOLLOW_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "utils.h"

#define BRITTLE_HOLLOW_COLOR 74.0f/255.0f, 0.0f, 128.0f/255.0f, 1.0f
#define RAD 3.14159/180

class BrittleHollow {
    public:
        BrittleHollow(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks)
        {}

        void draw();

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        bool inside_dark_hole(GLdouble camX, GLdouble camY, GLdouble camZ);

        std::pair<double, double> get_black_hole_position() {
            return std::make_pair(distance * cos(translation*RAD), distance * sin(translation*RAD));
        }

        void debug() { d = true; }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;

        bool d;
};

#endif