#ifndef VOID_H
#define VOID_H

#include <GL/glut.h>

class Void {
    public:
        Void();

        void draw();

        void set_shell(GLuint s) { shell = s; }
        void set_core(GLuint c) { core = c; }
        void set_conector(GLuint c) { conector = c; }
        
    private:
        GLuint shell;
        GLuint core;
        GLuint conector;
};

#endif