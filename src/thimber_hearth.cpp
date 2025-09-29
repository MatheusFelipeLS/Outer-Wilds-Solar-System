#include "thimber_hearth.h"

<<<<<<< HEAD
void ThimberHearth::draw() {
    if(d) {
        std::cout << "tl = " << translation
                    << " rt = " << rotation
                    << " dist = " << distance
                    << " radius = " << radius
        << std::endl;
    }
    GLfloat diffuse_color[] = {THIMBER_HEARTH_COLOR};
    GLfloat specular_color[] = {THIMBER_HEARTH_COLOR};
    GLfloat ambient_color[] = {THIMBER_HEARTH_COLOR};
    GLfloat shininess[] = {10.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    
    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);
        glutSolidSphere(radius, slices, stacks);    
    glPopMatrix();
}
=======
#include <GL/glut.h>
#include <iostream>
#include <SOIL/SOIL.h>


#define THIMBER_HEARTH_COLOR 0.0f, 0.0f, 200.0f/255.0f

class ThimberHearth {
    public:
        ThimberHearth(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks), textureID(0), quad(nullptr), d(false)
        {}

        void draw() {
            if(d) {
                std::cout << "tl = " << translation
                          << " rt = " << rotation
                          << " dist = " << distance
                          << " radius = " << radius
                << std::endl;
            }
            
            GLfloat material_color[] = {1.0f, 1.0f, 1.0f, 1.0f};;
            GLfloat shininess[] = {10.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
            
            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                //glutSolidSphere(radius, slices, stacks); 
                
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textureID);

                quad = gluNewQuadric();
                gluQuadricTexture(quad, GL_TRUE);   // importante!
                gluQuadricNormals(quad, GLU_SMOOTH);
                gluSphere(quad, radius, slices, stacks);

                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        void debug() {
            d = true;
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
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint textureID;    // ID da textura
        GLUquadric* quad;    // quadric usado para gluSphere


        bool d;
};

#endif
>>>>>>> 236f45c ([fix]: texture)
