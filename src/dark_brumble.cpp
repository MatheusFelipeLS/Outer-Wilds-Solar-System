#ifndef DARK_BRUMBLE_H
#define DARK_BRUMBLE_H

#include <GL/glut.h>
#include <iostream>

#define DARK_BRAMBLE_COLOR 255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f

class DarkBrumble {
    public:
        DarkBrumble(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks)
        {}

        void draw() {
            if(d) {
                std::cout << "tl = " << translation
                          << " rt = " << rotation
                          << " dist = " << distance
                          << " radius = " << radius
                << std::endl;
            }
            GLfloat material_color[] = {DARK_BRAMBLE_COLOR};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glutSolidSphere(radius, slices, stacks);    
            glPopMatrix();
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        void debug() {
            d = true;
        }

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