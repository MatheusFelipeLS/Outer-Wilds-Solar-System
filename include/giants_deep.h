#ifndef GIANTS_DEEP_H
#define GIANTS_DEEP_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

// #define GIANTS_DEEP_SURFACE_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define GIANTS_DEEP_SURFACE_DIFFUSE_COLOR 0.160784, 0.647059, 0.419608, 1.0f
#define GIANTS_DEEP_SURFACE_SPECULAR_COLOR 0.000000, 0.172549, -0.043137, 1.0f
#define GIANTS_DEEP_SURFACE_AMBIENT_COLOR 0.000000, 0.533333, 0.117647, 1.0f
#define GIANTS_DEEP_SURFACE_SHININESS 2.207855
#define GIANTS_DEEP_INNER_GLOBE_COLOR 10.0f/255.0f, 105.0f/255.0f, 74.0f/255.0f, 1.0f

class GiantsDeep {
    public:
        GiantsDeep(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        );

        GLuint textureID;
        GLuint textureID2;

        void draw();

        void update_position(GLfloat t, GLfloat r);

        void set_tornado(GLuint t) { tornados = t; }

        void rotate_tornado() { tornado_rotation += 0.01; }

        void loadTexture(const char* filename, GLuint &texID);

        void debug() { d = true; }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0.0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLfloat inner_globe_radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint tornados;
        GLfloat tornado_rotation = 0.0;

        bool d;
};

#endif