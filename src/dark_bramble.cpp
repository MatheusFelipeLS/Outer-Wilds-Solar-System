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
                glScalef(10.0f, 10.0f, 10.0f);
                glCallList(portal);
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
                glScalef(10.0f, 10.0f, 10.0f);
                glCallList(shell);
            glPopMatrix();

            GLfloat ice_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat ice_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat ice_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat ice_shininess[] = {127.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ice_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, ice_specular);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ice_ambient);
            glMaterialfv(GL_FRONT, GL_SHININESS, ice_shininess);

            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glScalef(10.0f, 10.0f, 10.0f);
                glCallList(ice);
            glPopMatrix();
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        void debug() {
            d = true;
        }

        void set_portal(GLuint s) { portal = s; }

        void set_shell(GLuint i) { shell = i; }

        void set_ice(GLuint i) { ice = i; }

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

        bool d;
};

#endif