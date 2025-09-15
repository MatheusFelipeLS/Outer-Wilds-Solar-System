#ifndef GIANTS_DEEP_H
#define GIANTS_DEEP_H

#include <GL/glut.h>
#include <iostream>

#define GIANTS_DEEP_SURFACE_COLOR 0.0f, 253.0f/255.0f, 72.0f/255.0f, 1.0f
#define GIANTS_DEEP_INNER_GLOBE_COLOR 10.0f/255.0f, 105.0f/255.0f, 74.0f/255.0f, 1.0f

class GiantsDeep {
    public:
        GiantsDeep(
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
            
            GLfloat material_color[] = {GIANTS_DEEP_SURFACE_COLOR};
            GLfloat shininess[] = {30.0f};

            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);

                glutSolidSphere(radius, slices, stacks);    

                GLfloat inner_globe_material_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};

                glMaterialfv(GL_FRONT, GL_DIFFUSE, inner_globe_material_color);
                glMaterialfv(GL_FRONT, GL_SPECULAR, inner_globe_material_color);
                glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
                glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
            
                glutSolidSphere(radius / 4, slices, stacks);    
            glPopMatrix();
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        void debug() {
            d = true;
        }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0.0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;

        bool d;
};

#endif