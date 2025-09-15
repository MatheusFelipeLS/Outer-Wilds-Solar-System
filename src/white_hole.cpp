#ifndef WHITE_HOLE_H
#define WHITE_HOLE_H

#include <GL/glut.h>
#include <iostream>

class WhiteHole {
    public:
        WhiteHole(
            GLfloat radius, GLfloat slices, GLfloat stacks,
            GLfloat x, GLfloat y, GLfloat z
        ) : 
        radius(radius), slices(slices), stacks(stacks),
        x(x), y(y), z(z)
        {}

        void draw() {
            GLfloat light_pos[4] = {x, y, z, 1.0f};
            GLfloat light_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            GLfloat emission[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // branco

            glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
            glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);

            // material emissivo (faz a esfera "brilhar")
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);

            glPushMatrix();
                glTranslatef(x, y, z);
                quad = gluNewQuadric();
                gluQuadricTexture (quad, GL_TRUE);
                gluSphere (quad, radius, slices, stacks);
            glPopMatrix();
            
            // Depois, sempre volte a emissão pro "zero" pros outros planetas:
            GLfloat black_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, black_emission);
        }

        bool inside(GLdouble lookfrom[]) {
            printf("wh radius: %f\n", radius);
            printf("%f %f %f\n", lookfrom[0], lookfrom[1], lookfrom[2]);
            return (
                (lookfrom[0] <= x+radius && lookfrom[0] >= x-radius) && 
                (lookfrom[1] <= y+radius && lookfrom[1] >= y-radius) && 
                (lookfrom[2] <= z+radius && lookfrom[2] >= z-radius)
            );
        }

    private:
        GLfloat radius;
        GLfloat slices;
        GLfloat stacks;
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLUquadric *quad;

};

#endif