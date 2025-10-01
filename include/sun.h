#ifndef SUN_H
#define SUN_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "utils.h"

#define SUN_LIGHT_COLOR 1.0f, 1.0f, 1.0f, 1.0f

class Sun {
    public:
        Sun(
            GLfloat radius, 
            GLint slices, GLint stacks
        );

        void draw();

        void update_position(GLfloat r);

        Collision check_collision(float camX, float camY, float camZ);

        void debug() { d = true; }

        void loadTexture(const char* filename);

        float get_radius() const { return radius; }

    private:
        GLfloat rotation = 0.0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLint transition_color_signal = 1;
        GLUquadric *quad;
        GLuint textureID;
        GLfloat lower_bound_color = 0.5f;
        GLfloat light_pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat emission[4] = {1.0f, 1.0f, 0.0f, 1.0f}; // amarelo

        bool d; // debug
};

#endif