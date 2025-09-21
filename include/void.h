#ifndef VOID_H
#define VOID_H

#include <GL/glut.h>
#include <iostream>
#include "utils.h"

class Void {
    public:
        Void() {};

        void draw();
        bool check_colision(float camX, float camY, float camZ);

        void set_shell(GLuint s) { shell = s; }
        void set_core(GLuint c[]) { 
            for(int i = 0; i < 3; i++) {
                core[i] = c[i]; 
            }
        }
        
        void set_portal(GLuint p[]) { 
            for(int i = 0; i < 2; i++) {
                portal[i] = p[i]; 

            }
        }

        void set_bounding_boxes(BoundingBox bb[], int qt_bb, int void_core_objects_indexes[]);

        // foi usado só para debug
        void desloc() { t += 1.5; }
        void cdesloc() { t -= 1.5; }
        
    private:
        GLuint shell;
        GLuint core[3];
        GLuint portal[2];
        GLfloat t;
        int rot = 400;
        BoundingBox bboxes[18];
        BoundingSphere bspheres[8];
};

#endif