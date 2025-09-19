#ifndef BRITTLE_HOLLOW_H
#define BRITTLE_HOLLOW_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define BRITTLE_HOLLOW_COLOR 74.0f/255.0f, 0.0f, 128.0f/255.0f, 1.0f
#define RAD 3.14159/180

class BrittleHollow {
    public:
        BrittleHollow(
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
            
            GLfloat material_color[] = {BRITTLE_HOLLOW_COLOR};
            GLfloat shininess[] = {10.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);


                glutSolidSphere(radius, slices, stacks);    

                GLfloat bh_ambient[] = {0.0f, 0.0f, 0.0f};
                GLfloat bh_diffuse[] = {1.0f, 1.0f, 1.0f};
                GLfloat bh_specular[] = {0.1f, 0.1f, 0.1f};
                GLfloat bh_shininess[] = { 120.0f };
                glMaterialfv(GL_FRONT, GL_DIFFUSE, bh_ambient);
                glMaterialfv(GL_FRONT, GL_SPECULAR, bh_diffuse);
                glMaterialfv(GL_FRONT, GL_AMBIENT, bh_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, bh_shininess);
                
                glutSolidSphere(radius / 4, slices, stacks);    
            glPopMatrix();
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        bool inside_dark_hole(GLdouble camX, GLdouble camY, GLdouble camZ) {
            // encontrando a posição do buraco negro
            auto [x, z] = get_black_hole_position();
            double dh_radius = radius / 4.0f;

            return (
                (camX < x+dh_radius && camX > x-dh_radius) && 
                (camY < dh_radius && camY > -dh_radius) && 
                ((-camZ) < z+dh_radius && (-camZ) > z-dh_radius)
            );
        }

        std::pair<double, double> get_black_hole_position() {
            return std::make_pair(distance * cos(translation*RAD), distance * sin(translation*RAD));
        }

        void debug() {
            d = true;
        }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;

        bool d;
};

#endif