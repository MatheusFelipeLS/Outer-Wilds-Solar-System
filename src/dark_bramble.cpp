#ifndef DARK_BRAMBLE_H
#define DARK_BRAMBLE_H

#include <GL/glut.h>
#include <iostream>

#define DARK_BRAMBLE_COLOR 255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f, 1.0f

class DarkBramble {
    public:
        DarkBramble(
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
            GLfloat shininess[] = {10.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
                

            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glCallList(sphere);
            glPopMatrix();

            GLfloat sla[] = {0.26f, 0.125f, 0.0f, 1.0f};
            GLfloat sla_shininess[] = {10.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, sla);
            glMaterialfv(GL_FRONT, GL_SPECULAR, sla);
            glMaterialfv(GL_FRONT, GL_AMBIENT, sla);
            glMaterialfv(GL_FRONT, GL_SHININESS, sla_shininess);

            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glCallList(icosphere);
            glPopMatrix();
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        void debug() {
            d = true;
        }

        void set_sphere(GLuint s) {
            sphere = s;
        }

        void set_icosphere(GLuint i) {
            icosphere = i;
        }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint sphere;
        GLuint icosphere;

        bool d;
};

#endif