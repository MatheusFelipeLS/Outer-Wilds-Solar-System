/*
* Copyright (c) 1993-1997, Silicon Graphics, Inc.
* ALL RIGHTS RESERVED 
* Permission to use, copy, modify, and distribute this software for 
* any purpose and without fee is hereby granted, provided that the above
* copyright notice appear in all copies and that both the copyright notice
* and this permission notice appear in supporting documentation, and that 
* the name of Silicon Graphics, Inc. not be used in advertising
* or publicity pertaining to distribution of the software without specific,
* written prior permission. 
*
* THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
* AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
* INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
* FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
* GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
* KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
* LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
* THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
* ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
* POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
* 
* US Government Users Restricted Rights 
* Use, duplication, or disclosure by the Government is subject to
* restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
* (c)(1)(ii) of the Rights in Technical Data and Computer Software
* clause at DFARS 252.227-7013 and/or in similar or successor
* clauses in the FAR or the DOD or NASA FAR Supplement.
* Unpublished-- rights reserved under the copyright laws of the
* United States.  Contractor/manufacturer is Silicon Graphics,
* Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
*
* OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
*/

/*
*  planet.c
*  This program shows how to composite modeling transformations
*  to draw translated and rotated models.
*  Interaction:  pressing the d and y keys (day and year)
*  alters the rotation of the planet around the sun.
*/
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include "planet.cpp"

int animating = 0; // indica se a animação deve ocorrer ou não
int forward = 1;   // indica se a animação vai do início ao fim ou ao contrário   
float dt = 1.0f;  // velocidade da animação

static int rotation_sun = 0, day = 0, year = 0;
static GLfloat distance = 0.0f;

// A translação aqui definida refere-se ao movimento em torno do Sol, não ao deslocamento no espaço
static GLfloat 
        translation_timber_hearth = 0.0, 
        translation_brittle_hollow = 0.0, 
        translation_giants_deep = 0.0, 
        translation_dark_bramble = 0.0,
        translation_interloper = 0.0;

// A rotação aqui definida refere-se ao movimento do planeta em todo de "si mesmo"
static GLfloat
        rotation_timber_hearth = 0.0, 
        rotation_brittle_hollow = 0.0, 
        rotation_giants_deep = 0.0, 
        rotation_dark_bramble = 0.0,
        rotation_interloper = 0.0;

static GLdouble x = 0.0, y = 0.0, z = 30.0;

static Planet 
    timber_hearth(0.2f, distance + 6.0f, 0.0f, 0.0f, 200.0f/255.0f), 
    brittle_hollow(0.2f, distance + 8.0f, 74.0f/255.0f, 0.0f, 128.0f/255.0f), 
    giants_deep(1.0f, distance + 12.0f, 0.0f, 100.0f/255.0f, 0.0f), 
    dark_bramble(1.0f, distance + 16.0f, 255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f), 
    interloper(0.1f, distance + 18.0f, 26.0f/255.0f, 224.0f/255.0f, 200.0f/255.0f);

void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    GLfloat light_pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_color[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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
    
    timber_hearth.draw();
    brittle_hollow.draw();
    giants_deep.draw();
    dark_bramble.draw();
    interloper.draw();

    glutSwapBuffers();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y) {
    printf("%c\n",key);
    switch (key){
        case 'z':
            z -= 0.5;
            printf("a\n");
            glutPostRedisplay();
            break;
        case 'Z':
            z += 0.5;
            printf("b\n");
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }

    
}

void idle(void) {
    timber_hearth.update_position(0.2f, 0.3f);
    brittle_hollow.update_position(0.16f, 0.3f);
    giants_deep.update_position(0.12f, 0.3f);
    dark_bramble.update_position(0.08f, 0.3f);
    interloper.update_position(0.1f, 0.3f);
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle); 
    glutMainLoop();
    return 0;
}
