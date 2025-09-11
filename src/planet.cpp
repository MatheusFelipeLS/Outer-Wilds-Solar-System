#ifndef PLANET_H
#define PLANET_H

#include <GL/glut.h>
#include <iostream>

class Planet {
    public:
        Planet(GLfloat radius, GLfloat distance, GLfloat r, GLfloat g, GLfloat b) { 
            translation = rotation = 0.0;
            this->distance = distance;
            this->radius = radius; 
            this->r = r;
            this->g = g;
            this->b = b;

        }

        void draw() {
            if(d) {
                std::cout << "tl =" << translation
                          << "rt =" << rotation
                          << "dist =" << distance
                          << "radius =" << radius
                          << "r = " << r
                          << "g = " << g
                          << "b = " << b
                << std::endl;
            }
            GLfloat material_color[] = {r, g, b};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glPushMatrix(); // profundezas do gigante
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glutSolidSphere(radius, 10, 8);    
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
        GLfloat translation;
        GLfloat rotation; 
        GLfloat distance;
        GLfloat radius;
        GLfloat r;
        GLfloat g;
        GLfloat b;

        bool d;
};

#endif