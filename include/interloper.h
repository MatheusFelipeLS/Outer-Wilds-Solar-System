#ifndef INTERLOPER_H
#define INTERLOPER_H

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include "utils.h"

#define INTERLOPER_DIFFUSE_COLOR 0.000000, 1.000000, 1.000000, 1.0
#define INTERLOPER_SPECULAR_COLOR 0.000000, 1.000000, 0.941176, 1.0
#define INTERLOPER_AMBIENT_COLOR 0.000000, 0.576471, 0.509804, 1.0
#define INTERLOPER_SHININESS 3.015686


class Interloper {
    public:
        Interloper(GLfloat radius, GLfloat sun_radius, GLfloat initial_point, GLint slices, GLint stacks);

        void draw();

        void bezier(float t, GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *p3, float *out);
        
        void update_position(GLfloat t, GLfloat r);

        void debug() { d = true; }

        void loadTexture(const char* filename);

    private:
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLfloat ctrl_points[4][3];
        GLint slices;
        GLint stacks;
        GLuint textureID;


        // animação eliptica em torno do sol
        bool animation_direction = true; // true => indo em direção ao sol, false => se afastando do sol
        GLfloat t = 0.0f;

        GLint numPoints = sizeof(ctrl_points) / sizeof(ctrl_points[0]);
        GLfloat pos[3];

        bool d;
};

#endif