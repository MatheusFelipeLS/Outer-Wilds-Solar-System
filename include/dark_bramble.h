#ifndef DARK_BRAMBLE_H
#define DARK_BRAMBLE_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

#define DARK_BRAMBLE_PORTAL_DIFFUSE_COLOR 1.000000, 1.000000, 1.000000, 1.0f
#define DARK_BRAMBLE_PORTAL_SPECULAR_COLOR 1.000000, 1.000000, 1.000000, 1.0f
#define DARK_BRAMBLE_PORTAL_AMBIENT_COLOR 1.000000, 1.000000, 1.000000, 1.0f
#define DARK_BRAMBLE_PORTAL_SHININESS 10.0f

#define DARK_BRAMBLE_WOODEN_DIFFUSE_COLOR 0.125490, 0.000000, 0.074510, 1.0f
#define DARK_BRAMBLE_WOODEN_SPECULAR_COLOR 0.000000, 0.000000, 0.000000, 1.0f
#define DARK_BRAMBLE_WOODEN_AMBIENT_COLOR 0.270588, 0.156863, 0.000000, 1.0f
#define DARK_BRAMBLE_WOODEN_SHININESS 0.549020f

#define DARK_BRAMBLE_ICE_DIFFUSE_COLOR 1.0f, 1.0f, 1.0f, 1.0f
#define DARK_BRAMBLE_ICE_SPECULAR_COLOR 1.0f, 1.0f, 1.0f, 1.0f
#define DARK_BRAMBLE_ICE_AMBIENT_COLOR 1.0f, 1.0f, 1.0f, 1.0f
#define DARK_BRAMBLE_ICE_SHININESS 127

class DarkBramble {
    public:
        DarkBramble(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        );

        void draw();

        // false se não teve colisão
        Collision check_colision(float camX, float camY, float camZ);

        bool inside(float camX, float camY, float camZ);

        void set_bouding_boxes(BoundingBox bb[], int qt_bb);

        std::pair<float, float> current_position();
        
        void update_position(GLfloat t, GLfloat r);

        void set_portal(GLuint s) { portal = s; }
        
        void set_shell(GLuint i) { shell = i; }
        
        void set_ice(GLuint i) { ice = i; }

        void debug() { d = true; }


    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint portal;
        GLuint shell;
        GLuint ice;

        BoundingBox bboxes[33];
        BoundingSphere bspheres[33];

        bool d;
};

#endif