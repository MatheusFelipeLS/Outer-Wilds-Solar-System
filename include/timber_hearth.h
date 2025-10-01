#ifndef TIMBER_HEARTH_H
#define TIMBER_HEARTH_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

#define TIMBER_HEARTH_COLOR 1.0f, 1.0f, 1.0f

class TimberHearth {
    public:
        TimberHearth(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        );

        void draw();

        void update_position(GLfloat t, GLfloat r);

        Collision check_collision(float camX, float camY, float camZ);

        void debug() { d = true; }

        void loadTexture(const char* filename);

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint textureID;


        bool d;
};

#endif