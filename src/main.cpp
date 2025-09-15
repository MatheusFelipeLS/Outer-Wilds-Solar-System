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
#define STB_IMAGE_IMPLEMENTATION 
#include "../include/stb_image.h"

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

#define DEBUG true
#define SPACE 32
#define EPSILON 1e-6 // talvez inutil

#define WHITE_HOLE_RADIUS SUN_RADIUS/50.0f

void loadTexture ( const char * filename, GLuint &texture) {
    int width , height , nrChannels ;
    std::cout << filename << std::endl; 
    unsigned char * data = stbi_load ( filename , & width , & height ,
    & nrChannels , 0);
    if ( data ) {
        glGenTextures (1 , & texture );
        glBindTexture ( GL_TEXTURE_2D , texture );
        // Set texture wrapping and filtering parameters
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S ,
        GL_REPEAT );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T ,
        GL_REPEAT );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER ,
        GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER ,
        GL_LINEAR );
        // Load the texture data ( check if it 's RGB or RGBA )
        if ( nrChannels == 3) {
            gluBuild2DMipmaps ( GL_TEXTURE_2D , GL_RGB , width ,
            height , GL_RGB , GL_UNSIGNED_BYTE , data );
        } else if ( nrChannels == 4) {
            gluBuild2DMipmaps ( GL_TEXTURE_2D , GL_RGBA , width ,
            height , GL_RGBA , GL_UNSIGNED_BYTE , data );
        }
        stbi_image_free ( data );
    } else {
        std::cerr << "Failed to load texture : " << filename << std::endl ;
    }
}

bool animating = true; // indica se a animação deve ocorrer ou não
int forward = 1;   // indica se a animação vai do início ao fim ou ao contrário   
float dt = 1.0f;  // velocidade da animação
int side = -1;

// visão do sistema inteiro
static GLdouble lookfrom[] = {0, 250, 0};
static GLdouble lookat[] = {0, -99, 0};

// zoom no profundezas do gigante
// static GLdouble lookfrom[] = {141, 56, -1};
// static GLdouble lookat[] = {141, -124, -1};

static Sun sun(SUN_PARAMS);
static ThimberHearth timber_hearth(THIMBER_HEARTH_PARAMS);
static BrittleHollow brittle_hollow(BRITTLE_HOLLOW_PARAMS);
static GiantsDeep giants_deep(GIANTS_DEEP_PARAMS);
static DarkBramble dark_bramble(DARK_BRAMBLE_PARAMS);
static Interloper interloper(INTERLOPER_PARAMS);


void draw_white_hole() {
    GLfloat light_pos[4] = {-250.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat emission[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // branco

    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);

    // material emissivo (faz a esfera "brilhar")
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glPushMatrix();
        glTranslatef(-250.0f, 0.0f, 0.0f);
        GLUquadric *quad = gluNewQuadric ();
        gluQuadricTexture ( quad , GL_TRUE );
        gluSphere ( quad , WHITE_HOLE_RADIUS , SLICES , STACKS);
    glPopMatrix();
    
    // Depois, sempre volte a emissão pro "zero" pros outros planetas:
    GLfloat black_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, black_emission);
}

void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_LIGHTING); 

    GLfloat global_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt (lookfrom[0], lookfrom[1], lookfrom[2], lookat[0], lookat[1], lookat[2], 0.0, 0.0, 1.0);

    sun.draw();
    timber_hearth.draw();
    brittle_hollow.draw();
    giants_deep.draw();
    dark_bramble.draw();
    interloper.draw();   

    draw_white_hole();

    if(brittle_hollow.inside_dark_hole(lookfrom)) {
        int delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        lookfrom[0] = -250.0f + WHITE_HOLE_RADIUS + delta;
        lookat[0] = -250.0f + WHITE_HOLE_RADIUS + delta;

        delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        lookfrom[1] = 0.0f + WHITE_HOLE_RADIUS + delta;
        lookat[1] = 0.0f + WHITE_HOLE_RADIUS + delta - 1;

        delta = (rand() % ((int) (2 * WHITE_HOLE_RADIUS)));
        lookfrom[2] = 0.0f + WHITE_HOLE_RADIUS + delta;
        lookat[2] = 0.0f + WHITE_HOLE_RADIUS + delta;
    }
    
    
    glutSwapBuffers();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(100.0, (GLfloat) w/(GLfloat) h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y) {
    if(DEBUG) {
        std::cout << key << " " << x << " " << y << std::endl;
        std::cout << "lf: " <<  lookfrom[0] << ", " << lookfrom[1] << ", " << lookfrom[2] << std::endl;
        std::cout << "la:" << lookat[0] << ", " << lookat[1] << ", " << lookat[2] << std::endl;
    }

    if(lookfrom[2] > lookat[2]) {
        side = 1;
    } else {
        side = -1;
    }

    double velocity = 1.0f;
    // não funciona como eu achei que funcionaria
    switch (key){
        case SPACE: // mover para cima (32 = space bar em ASCII)
            lookfrom[1] += velocity;
            lookat[1] += velocity;
            break;
        case 'b': // mover para baixo 
            lookfrom[1] -= velocity;
            lookat[1] -= velocity;
            break;
        case 'a': // mover para esquerda
            lookfrom[0] -= velocity * side;
            lookat[0] -= velocity * side;
            break;
        case 'd': // mover para direita
            lookfrom[0] += velocity * side;
            lookat[0] += velocity * side;
            break;
        case 's': // mover para trás
            lookfrom[2] += velocity * side;
            lookat[2] += velocity * side;
            break;
        case 'w': // mover para frente
            lookfrom[2] -= velocity * side;
            lookat[2] -= velocity * side;
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
