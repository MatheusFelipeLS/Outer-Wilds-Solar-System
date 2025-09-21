#ifndef THIMBER_HEARTH_H
#define THIMBER_HEARTH_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

#define THIMBER_HEARTH_COLOR 0.0f, 0.0f, 200.0f/255.0f

class ThimberHearth {
    public:
        ThimberHearth(
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

        void debug() { d = true; }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;

        bool d;
};

#endif