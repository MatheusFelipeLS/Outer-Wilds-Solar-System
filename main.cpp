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

int animating = 0;    
int forward = 1;      
float t = 0.0f;
float dt = 0.0001f;

float ctrlPoints[][3] ={
    {0.0, 3.0, -6.}, // ponto inicial
    {6.0, 2.5, 0.}, // arrasta a curva para direita
    {0.0, 2.0, 5.}, // arrasta a curva para baixo
    {0.0, 0.5, 3.}  // ponto final
};

int numPoints = sizeof(ctrlPoints) / sizeof(ctrlPoints[0]);

static int rotation_sun = 0, day = 0, year = 0;
static GLfloat distance = 0.0f;

// A translação aqui definida refere-se ao movimento em torno do Sol, não ao deslocamento no espaço
static int 
        translation_timber_hearth = 0.0, 
        translation_brittle_hollow = 0.0, 
        translation_giants_deep = 0.0, 
        translation_dark_bramble = 0.0;

// A rotação aqui definida refere-se ao movimento do planeta em todo de "si mesmo"
static int 
        rotation_timber_hearth = 0.0, 
        rotation_brittle_hollow = 0.0, 
        rotation_giants_deep = 0.0, 
        rotation_dark_bramble = 0.0;

void init(void) {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    glPushMatrix(); // Sol
    glutWireSphere(2.0, 20, 16);   
    glRotatef ((GLfloat) rotation_sun, 0.0, 1.0, 0.0);
    glPopMatrix();

    glPushMatrix(); // recanto lenhoso
    glRotatef ((GLfloat) translation_timber_hearth, 0.0, 1.0, 0.0);
    glTranslatef (distance, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glutWireSphere(0.2, 10, 8);    
    glPopMatrix();

    glPushMatrix(); // vale incerto
    glRotatef ((GLfloat) translation_brittle_hollow, 0.0, 1.0, 0.0);
    distance += 2.0f;
    glTranslatef (distance, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glutWireSphere(0.2, 10, 8);    
    glPopMatrix();
    
    glPushMatrix(); // profundezas do gigante
    glRotatef ((GLfloat) translation_giants_deep, 0.0, 1.0, 0.0);
    distance += 4.0f;
    glTranslatef (distance, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 8);    
    glPopMatrix();
    
    glPushMatrix(); // abrolho sinistro 
    glRotatef ((GLfloat) translation_dark_bramble, 0.0, 1.0, 0.0);
    distance += 4.0f;
    glTranslatef (distance, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glutWireSphere(0.9, 10, 8);    
    glPopMatrix();

    glutSwapBuffers();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y) {
    switch (key){
        case 'd':
            day = (day + 10) % 360;
            glutPostRedisplay();
            break;
        case 'D':
            day = (day - 10) % 360;
            glutPostRedisplay();
            break;
        case 'y':
            year = (year + 5) % 360;
            glutPostRedisplay();
            break;
        case 'Y':
            year = (year - 5) % 360;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void idle(void){
    if (animating){
        if (forward){
            t += dt;
            if (t >= numPoints - 3){ // chegou no fim
                t = numPoints - 3;
                // forward = 0; // começa a voltar
        }
    } else{ // voltando
            t -= dt;
            if (t <= 0.0f){ // chegou no início
                t = 0.0f;
                // forward = 1; // muda direção de novo
                animating = 0; // opcional: para a animação no início
        }
    }
        glutPostRedisplay();
    }
}

    int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
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
