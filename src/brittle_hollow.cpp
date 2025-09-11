#ifndef BRITTLE_HOLLOW
#define BRITTLE_HOLLOW

#include <GL/glut.h>
#include <iostream>

class BrittleHollow {
    public:
        BrittleHollow(
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
                std::cout << "tl =" << translation
                          << "rt =" << rotation
                          << "dist =" << distance
                          << "radius =" << radius
                          << "r = " << r
                          << "g = " << g
                          << "b = " << b
                << std::endl;
            }
            
            glPushMatrix(); 
                glRotatef (translation, 0.0, 1.0, 0.0);
                glTranslatef (distance, 0.0, 0.0);
                glRotatef (rotation, 0.0, 1.0, 0.0);

                GLfloat material_color[] = {r, g, b};
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
                glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
                glutSolidSphere(radius, slices, stacks);    

                material_color[0] = 10.0f/255.0f;
                material_color[1] = 105.0f/255.0f;
                material_color[2] = 174.0f/255.0f;
                glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
                glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
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
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLfloat r;          
        GLfloat g;
        GLfloat b;

        bool d;
};

#endif