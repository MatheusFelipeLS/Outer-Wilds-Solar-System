#include "white_hole.h"

void WhiteHole::draw() {
    GLfloat light_pos[4] = {x, y, z, 1.0f};
    GLfloat light_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat emission[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // branco
#ifndef WHITE_HOLE_H
#define WHITE_HOLE_H

#include <GL/glut.h>
#include <iostream>
#include <SOIL/SOIL.h>


class WhiteHole {
    public:
        WhiteHole(
            GLfloat radius, GLfloat slices, GLfloat stacks,
            GLfloat x, GLfloat y, GLfloat z
        ) : 
        radius(radius), slices(slices), stacks(stacks),
        x(x), y(y), z(z), textureID(0), quad(nullptr)
        {}

        void draw() {
            GLfloat light_pos[4] = {x, y, z, 1.0f};
            GLfloat light_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
            GLfloat emission[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // branco

    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_color);

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
            glPushMatrix();
                glTranslatef(x, y, z);
                quad = gluNewQuadric();
                gluQuadricTexture (quad, GL_TRUE);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textureID);

                quad = gluNewQuadric();
                gluQuadricTexture(quad, GL_TRUE);   
                gluQuadricNormals(quad, GLU_SMOOTH);
                gluSphere(quad, radius, slices, stacks);

                glDisable(GL_TEXTURE_2D);

                
            glPopMatrix();
            
            // Depois, sempre volte a emissão pro "zero" pros outros planetas:
            GLfloat black_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, black_emission);
        }

bool WhiteHole::inside(GLdouble camX, GLdouble camY, GLdouble camZ) {
    return (
        (camX <= x+radius && camX >= x-radius) && 
        (camY <= y+radius && camY >= y-radius) && 
        (camZ <= z+radius && camZ >= z-radius)
    );
}
        bool inside(GLdouble camX, GLdouble camY, GLdouble camZ) {
            return (
                (camX <= x+radius && camX >= x-radius) && 
                (camY <= y+radius && camY >= y-radius) && 
                (camZ <= z+radius && camZ >= z-radius)
            );
        }


        void texture(const char* filename) {
            textureID = SOIL_load_OGL_texture(
                filename,
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
            );

            if (textureID == 0) {
                std::cout << "Erro ao carregar textura: " << filename << std::endl;
                return;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }


    private:
        GLfloat radius;
        GLfloat slices;
        GLfloat stacks;
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLuint textureID;
        GLUquadric* quad;

};

#endif