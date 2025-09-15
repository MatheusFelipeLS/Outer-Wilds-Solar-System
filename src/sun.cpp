#ifndef SUN
#define SUN

#include <GL/glut.h>
#include <iostream>

#define SUN_LIGHT_COLOR 1.0f, 1.0f, 1.0f, 1.0f

class Sun {
    public:
        Sun(
            GLfloat radius, 
            GLint slices, GLint stacks
        ) :
        radius(radius),
        slices(slices), stacks(stacks)
        {}

        void draw() {
            if(d) {
                std::cout << "rt = " << rotation
                          << " radius = " << radius
                << std::endl;
            }
                
            // GLfloat ambient[4] = {0.12f, 0.12f, 0.08f, 1.0f};

            glLightfv(GL_LIGHT0, GL_POSITION, this->light_pos);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, this->light_color);
            glLightfv(GL_LIGHT0, GL_SPECULAR, this->light_color);
            // glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

            // material emissivo (faz a esfera "brilhar")
            glMaterialfv(GL_FRONT, GL_EMISSION, this->emission);

            glPushMatrix();
                quad = gluNewQuadric ();
                gluQuadricTexture ( quad , GL_TRUE );
                gluSphere ( quad , radius , slices , stacks);
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