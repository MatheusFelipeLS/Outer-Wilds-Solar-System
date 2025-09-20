#ifndef INTERLOPER_H
#define INTERLOPER_H

#include <GL/glut.h>
#include <iostream>
#include <vector>

#define INTERLOPER_COLOR 26.0f/255.0f, 224.0f/255.0f, 200.0f/255.0f, 1.0f


class Interloper {
    public:
        Interloper(GLfloat radius, GLfloat sun_radius, GLfloat initial_point, GLint slices, GLint stacks);

        void draw();

        void bezier(float t, GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *p3, float *out);

        void update_position(GLfloat t, GLfloat r);

        void debug() { d = true; }

    private:
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLfloat ctrl_points[4][3];
        GLint slices;
        GLint stacks;

        // animação eliptica em torno do sol
        bool animation_direction = true; // true => indo em direção ao sol, false => se afastando do sol
        GLfloat t = 0.0f;

        GLint numPoints = sizeof(ctrl_points) / sizeof(ctrl_points[0]);
        GLfloat pos[3];

        bool d;
};

#endif