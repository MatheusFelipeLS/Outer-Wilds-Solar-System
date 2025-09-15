#ifndef INTERLOPER_H
#define INTERLOPER_H

#include <GL/glut.h>
#include <iostream>
#include <vector>

#define INTERLOPER_COLOR 26.0f/255.0f, 224.0f/255.0f, 200.0f/255.0f, 1.0f


class Interloper {
    public:
        Interloper(
            GLfloat radius,
            GLfloat sun_radius, GLfloat initial_point,
            GLfloat t0,
            GLint slices, GLint stacks
        ) :
        translation(t0), radius(radius),
        slices(slices), stacks(stacks)
        {

            // ponto inicial
            this->ctrl_points[0][0] = initial_point;
            this->ctrl_points[0][1] = 0.0f;
            this->ctrl_points[0][2] = 0.0f;

            // primeiro ponto de controle
            this->ctrl_points[1][0] = initial_point * 0.9f;
            this->ctrl_points[1][1] = 0.0f;
            this->ctrl_points[1][2] = sun_radius * 3.0f;

            // segundo ponto de controle
            this->ctrl_points[2][0] = -sun_radius;
            this->ctrl_points[2][1] = 0.0f;
            this->ctrl_points[2][2] = sun_radius * 1.7f;

            // ponto final
            this->ctrl_points[3][0] = -sun_radius * 1.17f;
            this->ctrl_points[3][1] = 0.0f;
            this->ctrl_points[3][2] = 0.0f;
        }

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
                << std::endl;
            }

            GLfloat material_color[] = {INTERLOPER_COLOR};
            GLfloat shine[] = {80.0};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glMaterialfv(GL_FRONT, GL_SHININESS, shine);

            if (animation_direction) {
                bezier(
                    t, 
                    ctrl_points[0], 
                    ctrl_points[1], 
                    ctrl_points[2], 
                    ctrl_points[3], 
                    pos
                );
            } else {
                bezier(
                    t, 
                    ctrl_points[3], 
                    ctrl_points[2], 
                    ctrl_points[1], 
                    ctrl_points[0], 
                    pos
                );
            }

            glPushMatrix(); 
                glTranslatef(pos[0], pos[1], pos[2]);
                glRotatef (rotation, 0.0, 1.0, 0.0);
                glutSolidSphere(radius, slices, stacks);    
            glPopMatrix();
        }

        void bezier(float t, GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *p3, float *out) {
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
                animation_direction = !animation_direction;
                this->t = 0.0f;
                this->ctrl_points[1][2] = -this->ctrl_points[1][2];
                this->ctrl_points[2][2] = -this->ctrl_points[2][2];
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
        GLfloat ctrl_points[4][3];
        GLint slices;
        GLint stacks;

        // animação eliptica em torno do sol
        bool animation_direction = true; // true => indo em direção ao sol, false => se afastando do sol
        GLfloat t = 0.0f;

        GLint numPoints = sizeof(ctrl_points) / sizeof(ctrl_points[0]);
        GLfloat pos[3];

        bool d;
};

#endif