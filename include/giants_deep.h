#ifndef GIANTS_DEEP_H
#define GIANTS_DEEP_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

#define GIANTS_DEEP_SURFACE_COLOR 1.0f, 1.0f, 1.0f, 1.0f
#define GIANTS_DEEP_INNER_GLOBE_COLOR 10.0f/255.0f, 105.0f/255.0f, 74.0f/255.0f, 1.0f

class GiantsDeep {
    public:
        GiantsDeep(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        );

        void draw();

        void update_position(GLfloat t, GLfloat r);

        void set_tornado(GLuint t) { tornados = t; }

        void rotate_tornado() { tornado_rotation += 0.01; }

        void loadTexture(const char* filename);


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
        GLuint textureID;


        bool d;
};

#endif