#ifndef VOID_H
#define VOID_H

#include <GL/glut.h>
#include <iostream>

class Void {
    public:
        Void() {};

        void draw();

        void set_shell(GLuint s) { shell = s; }
        void set_core(GLuint c[]) { 
            for(int i = 0; i < 3; i++) {
                core[i] = c[i]; 
            }
        }

        void set_conector(GLuint c) { conector = c; }
        
        void set_portal(GLuint p[]) { 
            for(int i = 0; i < 2; i++) {
                portal[i] = p[i]; 

            }
        }

        void desloc();

        void cdesloc() { t -= 1.5; }
        
    private:
        GLuint shell;
        GLuint core[3];
        GLuint conector;
        GLuint portal[2];
        GLfloat t;
        int rot = 400;
};

#endif