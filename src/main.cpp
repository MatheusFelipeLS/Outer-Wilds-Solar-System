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

#include "player.h"
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
#define FPS 60
// #define TO_RADIANS 3.14/180.0 //tecnicamente tem essa mesma constante em brittle hollow. Talvez usar extern para remover

const int width = 16*50;
const int height = 9*50;
bool animating = false; // indica se a animação deve ocorrer ou não
int forward = 1;   // indica se a animação vai do início ao fim ou ao contrário   
float dt = 1.0f;  // velocidade da animação

// x, y, z, pitch, yaw iniciais
#define PLAYER_PARAMS 0.0f, SUN_RADIUS + 10, 0.0f, 0.0f, 0.0f

static Player player(PLAYER_PARAMS);
static Sun sun(SUN_PARAMS);
static ThimberHearth timber_hearth(THIMBER_HEARTH_PARAMS);
static BrittleHollow brittle_hollow(BRITTLE_HOLLOW_PARAMS);
static GiantsDeep giants_deep(GIANTS_DEEP_PARAMS);
static DarkBramble dark_bramble(DARK_BRAMBLE_PARAMS);
static Interloper interloper(INTERLOPER_PARAMS);
static WhiteHole white_hole(WHITE_HOLE_PARAMS);

void display();
void reshape(int w,int h);
void timer(int);
void passive_motion(int,int);
void keyboard(unsigned char key,int x,int y);
void keyboard_up(unsigned char key,int x,int y);

typedef struct {
    float x, y, z;
} Vertex;

#define MAX_VERTICES 100000
#define MAX_FACES    200000   // depois de triangulação pode dobrar

Vertex vertices[MAX_VERTICES];
int faces[MAX_FACES][3];  // sempre triângulos
int num_vertices = 0, num_faces = 0;

GLuint icosphere;
GLuint sphere;

// ==================== OBJ LOADER ====================
void loadObj(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("Can't open file %s\n", fname);
        exit(1);
    }

    int indexes = 0;
    char line[256];
    int current_idx = 0;
    while (fgets(line, sizeof(line), fp)) {
        // vértices
        if (line[0] == 'v' && line[1] == ' ') {
            Vertex v;
            printf("%d %d\n", current_idx, num_vertices);
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            vertices[num_vertices++] = v;
            current_idx++;
        }
        // faces
        else if (line[0] == 'f') {
            int v[4];
            int count = sscanf(line,
                "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                &v[0], &v[1], &v[2], &v[3]);

            if (count == 3) {
                // triângulo
                faces[num_faces][0] = v[0] - 1;
                faces[num_faces][1] = v[1] - 1;
                faces[num_faces][2] = v[2] - 1;
                num_faces++;
            } else if (count == 4) {
                // quad → triangula em 2 triângulos
                faces[num_faces][0] = v[0] - 1;
                faces[num_faces][1] = v[1] - 1;
                faces[num_faces][2] = v[2] - 1;
                num_faces++;

                faces[num_faces][0] = v[0] - 1;
                faces[num_faces][1] = v[2] - 1;
                faces[num_faces][2] = v[3] - 1;
                num_faces++;
            }
        } else if(line[0] == 'o') {
            indexes = current_idx; 
            printf("%d\n", current_idx);
        }
    }
    fclose(fp);

    printf("%d %d\n", indexes, num_faces);
    // Criar display list
    icosphere = glGenLists(1);
    glNewList(icosphere, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < indexes; i++) {
            Vertex v1 = vertices[faces[i][0]];
            Vertex v2 = vertices[faces[i][1]];
            Vertex v3 = vertices[faces[i][2]];
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
        glEnd();
    }
    glEndList();

    sphere = glGenLists(1);
    glNewList(sphere, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        for (int i = indexes; i < num_faces; i++) {
            Vertex v1 = vertices[faces[i][0]];
            Vertex v2 = vertices[faces[i][1]];
            Vertex v3 = vertices[faces[i][2]];
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
        glEnd();
    }
    glEndList();
}


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

    loadObj("3d_models/abrolho/abrolho.obj");
    dark_bramble.set_icosphere(icosphere);
    dark_bramble.set_sphere(sphere);

}

void hole_teleport() {
    if(brittle_hollow.inside_dark_hole(player.camX, player.camY, player.camZ)) {
        //teleportando para o buraco branco
        player.teleport(WHITE_HOLE_TX, WHITE_HOLE_TY, WHITE_HOLE_TZ, WHITE_HOLE_RADIUS);
    } else if(white_hole.inside(player.camX, player.camY, player.camZ)) {
        auto [x, z] = brittle_hollow.get_black_hole_position();
        player.teleport(x, 0.0f, z, WHITE_HOLE_RADIUS); // acho q o gap (white hole radius) tá muito grande
    }
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    player.camera();

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
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 5000.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutWarpPointer(width/2,height/2);
    glutTimerFunc(1000/FPS,timer,0);
}

void passive_motion(int x,int y) {
    /* two variables to store X and Y coordinates, as observed from the center
      of the window */
    int dev_x,dev_y;
    dev_x = (width/2)-x;
    dev_y = (height/2)-y;

    /* apply the changes to pitch and yaw*/
    player.update_pitch_yall(dev_x, dev_y);
}

void keyboard(unsigned char key,int x,int y) {
    if(DEBUG) {
        std::cout << key << " " << x << " " << y << std::endl;
    }

    switch(key) {
        case SPACE: // mover para cima (32 = space bar em ASCII)
            player.camY += 0.5;    
            break;
        case 'b': // mover para baixo 
        case 'B': // mover para baixo 
            player.camY -= 0.5;
            break;
        case 'W':
        case 'w':
            player.move_forward();
            break;
        case 'A':
        case 'a':
        player.move_left();
            break;
        case 'S':
        case 's':
            player.move_backward();
            break;
        case 'D':
        case 'd':
            player.move_right();
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
            player.stop_forward();
            break;
        case 'A':
        case 'a':
            player.stop_left();
            break;
        case 'S':
        case 's':
            player.stop_backward();
            break;
        case 'D':
        case 'd':
            player.stop_right();
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
