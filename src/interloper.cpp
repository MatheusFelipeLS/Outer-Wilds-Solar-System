#ifndef INTERLOPER_H
#define INTERLOPER_H

#include <GL/glut.h>
#include <iostream>

class Interloper {
    public:
        Interloper(
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
                std::cout 
                        //   << "tl = " << translation
                        //   << " rt = " << rotation
                        //   << " dist = " << distance
                          << " t = " << t
                          << " p0 = " << pos[0]
                          << " p1 = " << pos[1]
                          << " p2 = " << pos[2]
                        //   << " r = " << r
                        //   << " g = " << g
                        //   << " b = " << b
                << std::endl;
            }

            GLfloat material_color[] = {r, g, b};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);

            bezier(
                t, 
                ctrlPoints[0+animation_idx], 
                ctrlPoints[1+animation_idx], 
                ctrlPoints[2+animation_idx], 
                ctrlPoints[3+animation_idx], 
                pos
            );

            glPushMatrix(); 
                glTranslatef(pos[0], pos[1], pos[2]);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glutSolidSphere(radius, slices, stacks);    
            glPopMatrix();

            // glPushMatrix(); 
            //     glRotatef (translation, 0.0, 1.0, 0.0);
            //     glTranslatef (distance, 0.0, 0.0);
            //     glRotatef (rotation, 0.0, 1.0, 0.0);
            //     glutSolidSphere(radius, slices, stacks);    
            // glPopMatrix();
        }

        void bezier(float t, float *p0, float *p1, float *p2, float *p3, float *out) {
            float poly = 1.0f - t;
            float b0 = poly * poly * poly;
            float b1 = 3 * t * poly * poly;
            float b2 = 3 * t * t * poly;
            float b3 = t * t * t;

            for (int i = 0; i < 3; i++) {
                out[i] = p0[i] * b0 + p1[i] * b1 + p2[i] * b2 + p3[i] * b3;
            }
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            this->t += t;  
            if (this->t > 1-1e-5) {
                animation_idx = 4 - animation_idx;
                this->t = 0.0f;
            } 
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

        // animação eliptica em torno do sol
        GLint animation_idx = 0;
        GLfloat t = 0.0f;
        GLfloat ctrlPoints[8][3] = {
            // animação 1
            {50.0f, 0.0f, 0.0f}, // ponto inicial
            {45.0f, 0.0f, 30.0f}, // arrasta a curva para direita
            {-10.0f, 0.0f, 15.0f}, // arrasta a curva para baixo
            {-12.0f, 0.0f, 0.0f},  // ponto final  

            // animação 2
            {-12.0f, 0.0f, 0.0f},  // ponto final
            {-10.0f, 0.0f, -15.0f}, // arrasta a curva para baixo
            {45.0f, 0.0f, -30.0f}, // arrasta a curva para direita
            {50.0f, 0.0f, 0.0f}, // ponto inicial

        };

        GLint numPoints = sizeof(ctrlPoints) / sizeof(ctrlPoints[0]);
        GLfloat pos[3];

        bool d;
};

#endif