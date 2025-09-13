#ifndef PLANET_H
#define PLANET_H

#include <GL/glut.h>
#include <iostream>

class Planet {
    public:
        Planet(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLfloat r, GLfloat g, GLfloat b,
            GLint slices, GLint stacks
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks),
        r(r), g(g), b(b)
        {}

        void draw() {
            if(d) {
                std::cout << "tl = " << translation
                          << " rt = " << rotation
                          << " dist = " << distance
                          << " radius = " << radius
                          << " r = " << r
                          << " g = " << g
                          << " b = " << b
                << std::endl;
            }
            GLfloat material_color[] = {r, g, b};
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
        GLfloat r;          
        GLfloat g;
        GLfloat b;

        bool d;
};

#endif