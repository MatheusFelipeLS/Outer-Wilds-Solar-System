#include "sun.h"

<<<<<<< HEAD
void Sun::draw() {
    if(d) {
        std::cout << "rt = " << rotation
                    << " radius = " << radius
        << std::endl;
    }
        
=======
#include <GL/glut.h>
#include <iostream>
#include <SOIL/SOIL.h>
>>>>>>> 236f45c ([fix]: texture)

    glLightfv(GL_LIGHT0, GL_POSITION, this->light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, this->light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, this->light_color);

    // material emissivo (faz a esfera "brilhar")
    glMaterialfv(GL_FRONT, GL_EMISSION, this->emission);

<<<<<<< HEAD
    glPushMatrix();
        quad = gluNewQuadric ();
        gluQuadricTexture ( quad , GL_TRUE );
        gluSphere ( quad , radius , slices , stacks);
    glPopMatrix();
=======
        GLuint textureID;

        void draw() {
            if(d) {
                std::cout << "rt = " << rotation
                          << " radius = " << radius
                << std::endl;
            }
                
            // GLfloat ambient[4] = {0.12f, 0.12f, 0.08f, 1.0f};
>>>>>>> 236f45c ([fix]: texture)

    
    // reseta para não deixar os outros objetos brilharem também
    GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

}

<<<<<<< HEAD
void Sun::update_position(GLfloat r) {
    rotation += r;
    if (emission[1] >= 1.0f) {
        transition_color_signal = -1;
    } else if (emission[1] <= lower_bound_color) {
        transition_color_signal = 1;
    }
    emission[1] += transition_color_signal * 0.003;
    light_color[1] = emission[1];
}
=======
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textureID);

                quad = gluNewQuadric();
                gluQuadricTexture(quad, GL_TRUE);   // importante!
                gluQuadricNormals(quad, GLU_SMOOTH);
                gluSphere(quad, radius, slices, stacks);

                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            
            // reseta para não deixar os outros objetos brilharem também
            GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 0.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

        }

        void update_position(GLfloat r) {
            rotation += r;
            if (emission[1] >= 1.0f) {
                transition_color_signal = -1;
            } else if (emission[1] <= lower_bound_color) {
                transition_color_signal = 1;
            }
            emission[1] += transition_color_signal * 0.003;
            light_color[1] = emission[1];
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


        void pulse_brightness() {
            // Alterna o sentido do pulso quando atinge limites
            if (emission[1] >= 1.0f) transition_color_signal = -1;
            else if (emission[1] <= lower_bound_color) transition_color_signal = 1;

            // Atualiza a intensidade do brilho (com passo suave)
            emission[0] += transition_color_signal * 0.003f; // vermelho
            emission[1] += transition_color_signal * 0.003f; // verde
            emission[2] += transition_color_signal * 0.003f; // azul

            // Atualiza a cor da luz do sol para acompanhar o brilho
            light_color[0] = emission[0];
            light_color[1] = emission[1];
            light_color[2] = emission[2];
        }

        void debug() {
            d = true;
        }

    private:
        GLfloat rotation = 0.0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLint transition_color_signal = 1;
        GLUquadric *quad;
        GLfloat lower_bound_color = 0.5f;
        GLfloat light_pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat emission[4] = {1.0f, 1.0f, 0.0f, 1.0f}; // amarelo

        bool d; // debug
};

#endif
>>>>>>> 236f45c ([fix]: texture)
