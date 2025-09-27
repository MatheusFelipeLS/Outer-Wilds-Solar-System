#ifndef DARK_BRAMBLE_H
#define DARK_BRAMBLE_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

#define DARK_BRAMBLE_COLOR 128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f, 0.3f

class DarkBramble {
    public:
        DarkBramble(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks)
        {}

        void draw();

        // false se não teve colisão
        Collision check_colision(float camX, float camY, float camZ);

        bool inside(float camX, float camY, float camZ);

        void set_bouding_boxes(BoundingBox bb[], int qt_bb) {
            float radius_factor = 0.5;
            for(int i = 0; i < qt_bb; i++) {
                bboxes[i] = bb[i];
                bspheres[i] = BoundingSphere(bboxes[i], radius_factor);
                if(i >= 2) radius_factor = 0.33;
            }

        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

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