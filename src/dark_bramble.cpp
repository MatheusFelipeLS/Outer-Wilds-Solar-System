#ifndef DARK_BRAMBLE_H
#define DARK_BRAMBLE_H

#include <GL/glut.h>
#include <iostream>

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

        void draw() {
            if(d) {
                std::cout << "tl = " << translation
                          << " rt = " << rotation
                          << " dist = " << distance
                          << " radius = " << radius
                << std::endl;
            }

            GLfloat portal_diffuse[] = {DARK_BRAMBLE_COLOR};
            GLfloat portal_specular[] = {DARK_BRAMBLE_COLOR};
            GLfloat portal_ambient[] = {DARK_BRAMBLE_COLOR};
            GLfloat portal_shininess[] = {0.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, portal_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, portal_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, portal_ambient);
            glMaterialfv(GL_FRONT, GL_SHININESS, portal_shininess);
                

            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glCallList(sphere);
            glPopMatrix();

            GLfloat wooden_diffuse[] = {0.26f, 0.125f, 0.0f, 1.0f};
            GLfloat wooden_specular[] = {0.0, 0.0, 0.0f, 1.0f};
            GLfloat wooden_ambient[] = {0.26f, 0.125f, 0.0f, 1.0f};
            GLfloat wooden_shininess[] = {0.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, wooden_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, wooden_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, wooden_ambient);
            glMaterialfv(GL_FRONT, GL_SHININESS, wooden_shininess);

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