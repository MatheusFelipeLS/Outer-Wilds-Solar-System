#ifndef SUN
#define SUN

#include <GL/glut.h>
#include <iostream>

class Sun {
    public:
        Sun(GLfloat radius, GLfloat r, GLfloat g, GLfloat b) { 
            this->rotation = 0.0;
            this->radius = radius; 
            this->r = r;
            this->g = g;
            this->b = b;

        }

        void draw() {
            if(d) {
                std::cout << "rt =" << rotation
                          << "radius =" << radius
                          << "r = " << r
                          << "g = " << g
                          << "b = " << b
                << std::endl;
            }
                    
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
    
            GLfloat light_pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
            GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 1.0f};

            glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
            glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

            // material emissivo (faz a esfera "brilhar")
            GLfloat emission[] = {1.0f, 1.0f, 0.0f, 1.0f}; // amarelo
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);

            glPushMatrix();
                glutSolidSphere(2.0, 20, 16);
            glPopMatrix();

            // reseta para não deixar os outros objetos brilharem também
            GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
            glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

        }

        void update_position(GLfloat r) {
            rotation += r;
        }

        void debug() {
            d = true;
        }

    private:
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat radius;      // raio do planeta
        GLfloat r;          
        GLfloat g;
        GLfloat b;

        bool d;
};

#endif