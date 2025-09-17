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
#include <cstdlib>

#include "params.cpp"

#include "sun.cpp"
#include "thimber_hearth.cpp"
#include "brittle_hollow.cpp"
#include "giants_deep.cpp"
#include "dark_bramble.cpp"
#include "interloper.cpp"
#include "white_hole.cpp"

#define DEBUG false
#define SPACE 32
#define EPSILON 1e-6 // talvez inutil

bool animating = false; // indica se a animação deve ocorrer ou não
int forward = 1;   // indica se a animação vai do início ao fim ou ao contrário   
float dt = 1.0f;  // velocidade da animação
int side = -1;

// visão do sistema inteiro
// static GLdouble lookfrom[] = {0, 250, 0};
// static GLdouble lookat[] = {0, -99, 0};

// zoom no buraco negro
static GLdouble lookfrom[] = {98, 2, 0};
static GLdouble lookat[] = {98, -124, 0};

static Sun sun(SUN_PARAMS);
static ThimberHearth timber_hearth(THIMBER_HEARTH_PARAMS);
static BrittleHollow brittle_hollow(BRITTLE_HOLLOW_PARAMS);
static GiantsDeep giants_deep(GIANTS_DEEP_PARAMS);
static DarkBramble dark_bramble(DARK_BRAMBLE_PARAMS);
static Interloper interloper(INTERLOPER_PARAMS);
static WhiteHole white_hole(WHITE_HOLE_PARAMS);

struct Motion
{
    bool Forward,Backward,Left,Right;
};

Motion motion = {false,false,false,false};

#define FPS 60
#define TO_RADIANS 3.14/180.0

//width and height of the window ( Aspect ratio 16:9 )
const int width = 16*50;
const int height = 9*50;

float pitch = 0.0, yaw= 0.0;
float camX = 50.0, camY = 0.0, camZ = -30.0;

void display();
void reshape(int w,int h);
void timer(int);
void passive_motion(int,int);
void camera();
void keyboard(unsigned char key,int x,int y);
void keyboard_up(unsigned char key,int x,int y);

void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glutSetCursor(GLUT_CURSOR_NONE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    GLfloat global_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glutWarpPointer(width/2,height/2);

}

void hole_teleport() {
    if(brittle_hollow.inside_dark_hole(camX, camY, camZ)) {
        //teleportando para o buraco branco
        int delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        camX = -250.0f + WHITE_HOLE_RADIUS + delta;

        delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        camY = 0.0f + WHITE_HOLE_RADIUS + delta;

        delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        camZ = 0.0f + WHITE_HOLE_RADIUS + delta;
    } else if(white_hole.inside(camX, camY, camZ)) {
        auto [x, y] = brittle_hollow.get_black_hole_position();

        int delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        camX = x + WHITE_HOLE_RADIUS + delta;

        delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        camY = y + WHITE_HOLE_RADIUS + delta;

        delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        camZ = 0.0f + WHITE_HOLE_RADIUS + delta;
    }
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    camera();

    sun.draw();
    timber_hearth.draw();
    brittle_hollow.draw();
    giants_deep.draw();
    dark_bramble.draw();
    interloper.draw();   
    white_hole.draw();

    hole_teleport();

    glutSwapBuffers();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutWarpPointer(width/2,height/2);
    glutTimerFunc(1000/FPS,timer,0);
}

void passive_motion(int x,int y) {
    /* two variables to store X and Y coordinates, as observed from the center
      of the window
    */
    int dev_x,dev_y;
    dev_x = (width/2)-x;
    dev_y = (height/2)-y;

    /* apply the changes to pitch and yaw*/
    yaw += (float)dev_x/10.0;
    pitch += (float)dev_y/10.0;
}

void camera() {
    if(motion.Forward) {
        camX += cos((yaw+90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90)*TO_RADIANS)/5.0;
    }
    if(motion.Backward) {
        camX += cos((yaw+90+180)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90+180)*TO_RADIANS)/5.0;
    }
    if(motion.Left) {
        camX += cos((yaw+90+90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90+90)*TO_RADIANS)/5.0;
    }
    if(motion.Right) {
        camX += cos((yaw+90-90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90-90)*TO_RADIANS)/5.0;
    }

    /*limit the values of pitch
      between -60 and 70
    */
    // if(pitch>=70)
    //     pitch = 70;
    // if(pitch<=-60)
    //     pitch=-60;

    glRotatef(-pitch,1.0,0.0,0.0); // Along X axis
    glRotatef(-yaw,0.0,1.0,0.0);    //Along Y axis

    glTranslatef(-camX,-camY,-camZ);
}

void keyboard(unsigned char key,int x,int y) {
    if(DEBUG) {
        std::cout << key << " " << x << " " << y << std::endl;
    }

    switch(key) {
        case SPACE: // mover para cima (32 = space bar em ASCII)
            camY += 0.5;    
            break;
        case 'b': // mover para baixo 
        case 'B': // mover para baixo 
            camY -= 0.5;
            break;
        case 'W':
        case 'w':
            motion.Forward = true;
            break;
        case 'A':
        case 'a':
            motion.Left = true;
            break;
        case 'S':
        case 's':
            motion.Backward = true;
            break;
        case 'D':
        case 'd':
            motion.Right = true;
            break;
        case '1': // ativar/desativar animação
            animating = !animating;
            break;
        case 27:
            exit(0);
            break;
    }
}

void keyboard_up(unsigned char key,int x,int y) {
    switch(key) {
        case 'W':
        case 'w':
            motion.Forward = false;
            break;
        case 'A':
        case 'a':
            motion.Left = false;
            break;
        case 'S':
        case 's':
            motion.Backward = false;
            break;
        case 'D':
        case 'd':
            motion.Right = false;
            break;
    }
}

void idle(void) {
    if (!animating) return;

    sun.update_position(0.3f);
    timber_hearth.update_position(0.2f, 0.3f);
    brittle_hollow.update_position(0.16f, 0.3f);
    giants_deep.update_position(0.12f, 0.3f);
    dark_bramble.update_position(0.08f, 0.3f);
    interloper.update_position(0.001f, 0.3f);
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(25);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);

    init ();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passive_motion);
    glutTimerFunc(0,timer,0);    
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutIdleFunc(idle); 
    glutMainLoop();

    return 0;
}
