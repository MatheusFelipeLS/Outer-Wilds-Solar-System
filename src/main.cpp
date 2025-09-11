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
#include "sun.cpp"
#include "planet.cpp"
#include "giants_deep.cpp"
#include "brittle_hollow.cpp"

#define DEBUG true
#define SPACE 32
#define EPSILON 1e6

bool animating = true; // indica se a animação deve ocorrer ou não
int forward = 1;   // indica se a animação vai do início ao fim ou ao contrário   
float dt = 1.0f;  // velocidade da animação
int side = -1;

static GLfloat distance = 0.0f;

static GLdouble lookfrom[] = {0.0f, 0.0f, 75.0f};
static GLdouble lookat[] = {0.0f, 0.0f, 34.0f};

/* raio, r, g, b */ 
static Sun sun(8.0f, 1.0f, 1.0f, 1.0f, 30, 20);

/* raio do planeta, distancia do sol, r, g, b */ 
static Planet 
    timber_hearth(
        1.0f, 
        distance + 14.0f, 
        0.0f, 
        0.0f, 0.0f, 200.0f/255.0f,
        30, 20
    );

static BrittleHollow
    brittle_hollow(
        1.0f, 
        distance + 18.0f, 
        0.0f, 
        74.0f/255.0f, 0.0f, 128.0f/255.0f,
        30, 20
    );

static GiantsDeep 
    giants_deep(
        4.0f, 
        distance + 28.0f, 
        0.0f, 
        0.0f, 253.0f/255.0f, 72.0f/255.0f,
        30, 20
    );

static Planet 
    dark_bramble(
        4.0f, 
        distance + 42.0f, 
        0.0f, 
        255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f,
        30, 20
    ), 
    interloper(
        0.3f, 
        distance + 50.0f, 
        0.0f, 
        26.0f/255.0f, 224.0f/255.0f, 200.0f/255.0f,
        30, 20
    );


void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt (lookfrom[0], lookfrom[1], lookfrom[2], lookat[0], lookat[1], lookat[2], 0.0, 1.0, 0.0);

    sun.draw();
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
}

void keyboard (unsigned char key, int x, int y) {
    if(DEBUG) {
        std::cout << key << " " << x << " " << y << std::endl;
        std::cout << lookfrom[0] << " " << lookfrom[1] << " " << lookfrom[2] << std::endl;
    }

    if(lookfrom[2] > lookat[2]) {
        side = 1;
    } else {
        side = -1;
    }

    // não funciona como eu achei que funcionaria
    switch (key){
        case SPACE: // mover para cima (32 = space bar em ASCII)
            lookfrom[1] += 0.5f;
            lookat[1] += 0.5f;
            break;
        case 'b': // mover para baixo 
            lookfrom[1] -= 0.5f;
            lookat[1] -= 0.5f;
            break;
        case 'a': // mover para esquerda
            lookfrom[0] -= 0.5f * side;
            lookat[0] -= 0.5f * side;
            break;
        case 'd': // mover para direita
            lookfrom[0] += 0.5f * side;
            lookat[0] += 0.5f * side;
            break;
        case 's': // mover para trás
            lookfrom[2] += 0.5f * side;
            lookat[2] += 0.5f * side;
            break;
        case 'w': // mover para frente
            lookfrom[2] -= 0.5f * side;
            lookat[2] -= 0.5f * side;
            break;
        case '1': // ativar/desativar animação
            animating = !animating;
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }

    glutPostRedisplay();
    
}

void idle(void) {
    if (!animating) return;

    timber_hearth.update_position(0.2f, 0.3f);
    brittle_hollow.update_position(0.16f, 0.3f);
    giants_deep.update_position(0.12f, 0.3f);
    dark_bramble.update_position(0.08f, 0.3f);
    interloper.update_position(0.02f, 0.3f);
    
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
