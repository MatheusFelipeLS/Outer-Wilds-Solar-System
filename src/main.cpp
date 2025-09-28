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
#include <math.h>
#include <numeric>

#include "utils.h"
#include "params.cpp"

#include "player.h"

#include "sun.h"
#include "thimber_hearth.h"
#include "brittle_hollow.h"
#include "giants_deep.h"
#include "dark_bramble.h"
#include "interloper.h"
#include "white_hole.h"
#include "void.h"
#include "quantum_moon.h"

#define DEBUG false
#define SPACE 32
#define EPSILON 1e-6 // talvez inutil
#define FPS 60
// #define TO_RADIANS 3.14/180.0 //tecnicamente tem essa mesma constante em brittle hollow. Talvez usar extern para remover

const int width = 16*50;
const int height = 9*50;
bool animating = false; 
int forward = 1;   
float dt = 1.0f;  
bool reset = true;
static GLint fogMode = GL_LINEAR;
bool map = false;

// Menu system
bool menu_open = false;
bool camera_locked = false;

// Speed configuration
float player_speed = 1.0f;
float speed_increment = 0.1f;
float min_speed = 0.1f;
float max_speed = 5.0f;

// Starfield
#define NUM_STARS 1500
#define STARFIELD_RADIUS 2000.0f
static float stars[NUM_STARS][3];
void init_stars();
void draw_stars();

// x, y, z, pitch, yaw iniciais
#define PLAYER_PARAMS BRITTLE_HOLLOW_DISTANCE, 0, -200, 0.0f, 0.0f

static Player player(PLAYER_PARAMS);
static Sun sun(SUN_PARAMS);
static ThimberHearth thimber_hearth(THIMBER_HEARTH_PARAMS);
static BrittleHollow brittle_hollow(BRITTLE_HOLLOW_PARAMS);
static GiantsDeep giants_deep(GIANTS_DEEP_PARAMS);
static DarkBramble dark_bramble(DARK_BRAMBLE_PARAMS);
static Interloper interloper(INTERLOPER_PARAMS);
static WhiteHole white_hole(WHITE_HOLE_PARAMS);
static Void infinity_void;

// declaração única (em escopo global ou before main loop)
static float planet_distances[] = {
    THIMBER_HEARTH_DISTANCE,
    BRITTLE_HOLLOW_DISTANCE,
    GIANTS_DEEP_DISTANCE,
    DARK_BRAMBLE_DISTANCE
};

// declarar uma única vez e passar o ponteiro pra QuantumMoon
static float planet_rotations[] = {0.0f, 0.0f, 0.0f, 0.0f};

// inicialização
static QuantumMoon quantum_moon(QUANTUM_MOON_PARAMS);

void display();
void reshape(int w,int h);
void timer(int);
void passive_motion(int,int);
void keyboard(unsigned char key,int x,int y);
void keyboard_up(unsigned char key,int x,int y);
void draw_menu();
void toggle_menu();

void set_system() {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_LIGHT0); // Sun
    glEnable(GL_LIGHT1); // White hole (acho q vou tirar a luz dele)
    glEnable(GL_LIGHT2); // Player
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glDisable(GL_FOG);
}

void set_void() {
    glDisable(GL_LIGHT0); // Sun
    glDisable(GL_LIGHT1); // White hole (acho q vou tirar a luz dele)
    glDisable(GL_LIGHT2); // Player
    glEnable(GL_FOG);
    {
        GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
        glFogi (GL_FOG_MODE, fogMode);
        glFogfv (GL_FOG_COLOR, fogColor);
        glFogf (GL_FOG_DENSITY, 0.35);
        glHint (GL_FOG_HINT, GL_DONT_CARE);
        glFogf (GL_FOG_START, 100.0);
        glFogf (GL_FOG_END, 500.0);
    }
   glClearColor(0.5, 0.5, 0.5, 1.0);  /* fog color */

   player.camX = -99.064293;
   player.camY = 40.000000;
   player.camZ = 14.945419;
}

void init_stars() {
	for (int i = 0; i < NUM_STARS; ++i) {
		float x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
		float y = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
		float z = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
		float len = sqrtf(x * x + y * y + z * z);
		if (len < 1e-3f) {
			x = 1.0f; y = 0.0f; z = 0.0f; len = 1.0f;
		}
		stars[i][0] = (x / len) * STARFIELD_RADIUS;
		stars[i][1] = (y / len) * STARFIELD_RADIUS;
		stars[i][2] = (z / len) * STARFIELD_RADIUS;
	}
}

void draw_stars() {
	// Render simple white points as distant stars, locked to camera translation
	glPushAttrib(GL_ENABLE_BIT | GL_POINT_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glPointSize(1.5f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < NUM_STARS; ++i) {
		glVertex3f(player.camX + stars[i][0], player.camY + stars[i][1], player.camZ + stars[i][2]);
	}
	glEnd();
	glPopAttrib();
}

void toggle_menu() {
    menu_open = !menu_open;
    camera_locked = menu_open;
    
    if (menu_open) {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    } else {
        glutSetCursor(GLUT_CURSOR_NONE);
    }
}

void draw_menu() {
    if (!menu_open) return;
    
    // Salva o estado atual da matriz
    glPushMatrix();
    glLoadIdentity();
    
    // Desabilita iluminação para o menu
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Configura projeção ortogonal para o menu
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Desenha fundo semi-transparente
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(width, 0);
        glVertex2f(width, height);
        glVertex2f(0, height);
    glEnd();
    
    // Desenha caixa do menu
    glColor4f(0.2f, 0.2f, 0.2f, 0.9f);
    float menu_x = width * 0.25f;
    float menu_y = height * 0.25f;
    float menu_w = width * 0.5f;
    float menu_h = height * 0.5f;
    
    glBegin(GL_QUADS);
        glVertex2f(menu_x, menu_y);
        glVertex2f(menu_x + menu_w, menu_y);
        glVertex2f(menu_x + menu_w, menu_y + menu_h);
        glVertex2f(menu_x, menu_y + menu_h);
    glEnd();
    
    // Borda do menu
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(menu_x, menu_y);
        glVertex2f(menu_x + menu_w, menu_y);
        glVertex2f(menu_x + menu_w, menu_y + menu_h);
        glVertex2f(menu_x, menu_y + menu_h);
    glEnd();
    
    // Desenha texto do menu
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 30);
    
    // Título do menu
    std::string title = "MENU DE CONFIGURACAO";
    for (char c : title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    // Controles de velocidade
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 60);
    std::string speed_controls = "CONTROLES DE VELOCIDADE:";
    for (char c : speed_controls) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 80);
    std::string plus_control = "+ ou = : Aumentar velocidade";
    for (char c : plus_control) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 100);
    std::string minus_control = "- : Diminuir velocidade";
    for (char c : minus_control) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    // Velocidade atual
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 130);
    std::string current_speed = "Velocidade atual: " + std::to_string(player_speed).substr(0, 3);
    for (char c : current_speed) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    // Outros controles
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 160);
    std::string other_controls = "OUTROS CONTROLES:";
    for (char c : other_controls) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 180);
    std::string esc_control = "ESC : Fechar menu";
    for (char c : esc_control) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    glRasterPos2f(menu_x + 20, menu_y + menu_h - 200);
    std::string m_control = "M : Abrir/fechar menu";
    for (char c : m_control) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    
    // Restaura o estado
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    // Reabilita iluminação e depth test
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void init(void) {
    set_system();
    glShadeModel (GL_SMOOTH);
    glutSetCursor(GLUT_CURSOR_NONE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); 

    GLfloat global_ambient[] = {0.5f, 0.5f, 0.5f, 0.5f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glutWarpPointer(width/2,height/2);

    GLuint dark_bramble_objects[33]; 
    // talvez tivesse como fazer isso sem ser hard code, mas teria que usar strings e eu to com preguiça.
    // A ideia seria renomear cada objeto no blender, descobrir qual objeto é o que e fazer um mapeamento
    int objects_indexes[] = {
        0, 0, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        2, 1, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        2, 2, 1
    };
    BoundingBox dark_bramble_bboxes[33];
    loadObj("3d_models/abrolho/abrolho.obj", dark_bramble_objects, 33, objects_indexes, dark_bramble_bboxes, 10.0);
    dark_bramble.set_portal(dark_bramble_objects[0]);
    dark_bramble.set_shell(dark_bramble_objects[1]);
    dark_bramble.set_ice(dark_bramble_objects[2]);
    dark_bramble.set_bouding_boxes(dark_bramble_bboxes, 33);

    GLuint tornados_objects[1]; 
    int tor_objects_indexes[] = {1};
    BoundingBox giants_deep_bboxes[1];
    loadObj("3d_models/profundezas/tornado.obj", tornados_objects, 1, tor_objects_indexes, giants_deep_bboxes, 1.0);
    giants_deep.set_tornado(tornados_objects[0]);

    player.set_solar_system(&sun, &thimber_hearth, &brittle_hollow, &giants_deep, &dark_bramble, &interloper, &white_hole, &infinity_void);

    GLuint void_shell[1]; 
    int void_shell_objects_indexes[] = {0};
    BoundingBox void_shell_bboxes[1];
    loadObj("3d_models/void/void_shell.obj", void_shell, 1, void_shell_objects_indexes, void_shell_bboxes, 50.0);

    GLuint void_core[18]; 
    int void_core_objects_indexes[] = {
        0, 0, 1,
        0, 0, 0,
        0, 1, 0,
        0, 1, 1,
        1, 1, 1,
        1, 1, 1
    };
    BoundingBox void_core_bboxes[18];
    loadObj("3d_models/void/void_core.obj", void_core, 18, void_core_objects_indexes, void_core_bboxes, 10.0);

    GLuint void_portal[2]; 
    int void_portal_objects_indexes[] = {0, 1};
    BoundingBox void_portal_bboxes[2];
    loadObj("3d_models/void/void_portal.obj", void_portal, 2, void_portal_objects_indexes, void_portal_bboxes, 10.0);


    infinity_void.set_shell(void_shell[0]);
    infinity_void.set_core(void_core);
    infinity_void.set_portal(void_portal);
    infinity_void.set_core_bounding_boxes(void_core_bboxes, 18, void_core_objects_indexes);
    infinity_void.set_shell_bounding_boxes(void_shell_bboxes[0]);
    infinity_void.set_portal_bounding_boxes(void_portal_bboxes);
    
    std::vector<int> aux(285);
    std::iota(aux.begin(), aux.end(), 0);
    GLuint brittle_hollow_surface[285]; 
    int brittle_hollow_objects_indexes[285];
    BoundingBox brittle_hollow_bboxes[285];
    for(size_t i = 0; i < aux.size(); i++) {
        brittle_hollow_objects_indexes[i] = aux[i];
    }

    loadObj("3d_models/vale_da_incerteza/vale.obj", brittle_hollow_surface, 285, brittle_hollow_objects_indexes, brittle_hollow_bboxes, 10.0);

    
    brittle_hollow.set_surface(brittle_hollow_surface, 285);
    brittle_hollow.set_surface_bouding_boxes(brittle_hollow_bboxes, 285);
    

    // Configura a lua quântica para orbitar os planetas (arrays globais)
    quantum_moon.set_planets(planet_distances, planet_rotations, 4);

    // Debug desabilitado para produção
    // quantum_moon.debug();

	// Inicializa as estrelas de fundo
	init_stars();

}

bool queda = false;
void display(void) {
    glLoadIdentity();
    player.set_speed(player_speed);
    player.camera(map);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Estrelas de fundo
	draw_stars();
    if(queda) {
        brittle_hollow.queda();
    }

    if(map) {
        if(!reset) {
            set_void();
            reset = true;
            infinity_void.add_portal();
            auto [rotation, distance] = infinity_void.last_rotation();
            player.move_to_portal(rotation, distance-30);
        }

        Portal p = infinity_void.inside(player.camX, player.camY, player.camZ);
        if(p == Portal::NOTHING) {
            map = true;
        } else if (p == Portal::RIGHT) {
            infinity_void.remove_portal();
            if(infinity_void.qt_portals() == 0) {
                auto [rotation, distance] = dark_bramble.current_position();
                player.move_to_solar_system(rotation, distance);
                map = false;
            }
        } else if (p == Portal::WRONG) {
            infinity_void.add_portal();
            auto [rotation, distance] = infinity_void.last_rotation();
            player.move_to_portal(rotation, distance-30);
        }

        infinity_void.draw();
    } else {   
        map = dark_bramble.inside(player.camX, player.camY, player.camZ);
        if(reset) {
            set_system();
            reset = false;
        }
        glDisable(GL_FOG);
        glClearColor(0.0, 0.0, 0.0, 0.0);

        sun.draw();
        thimber_hearth.draw();
        brittle_hollow.draw();
        giants_deep.draw();
        dark_bramble.draw();
        interloper.draw();   
        white_hole.draw();
        quantum_moon.draw();

        player.hole_teleport();
    }

    // Desenha o menu se estiver aberto
    draw_menu();

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
    // Só reposiciona o cursor se o menu não estiver aberto
    if (!camera_locked) {
        glutWarpPointer(width/2,height/2);
    }
    glutTimerFunc(1000/FPS,timer,0);
}

void passive_motion(int x,int y) {
    // Não atualiza a câmera se o menu estiver aberto
    if (camera_locked) return;
    
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
            player.move_up();   
            break;
        case 'b': // mover para baixo 
        case 'B': // mover para baixo 
            player.move_down();
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
        case 'l': 
        case 'L':
            player.turn_on_off_light();
            break;
        case 'g':
            infinity_void.desloc();
            break;
        case 'G':
            infinity_void.cdesloc();
            break;
        case 'm':
        case 'M':
            toggle_menu();
            break;
        case 'q':
        case 'Q':
            queda = !queda;
            break;
        case '+':
        case '=':
            if (player_speed < max_speed) {
                player_speed += speed_increment;
            }
            break;
        case '-':
            if (player_speed > min_speed) {
                player_speed -= speed_increment;
            }
            break;
        case 27:
            if (menu_open) {
                toggle_menu();
            } else {
                exit(0);
            }
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
        case SPACE: // mover para cima (32 = space bar em ASCII)
            player.stop_up();   
            break;
        case 'b': // mover para baixo 
        case 'B': // mover para baixo 
            player.stop_down();
            break;
    }
}

void idle(void) {
    static int last_time_ms = 0;
    int now_ms = glutGet(GLUT_ELAPSED_TIME);
    if (last_time_ms == 0) last_time_ms = now_ms;
    float delta_time = (now_ms - last_time_ms) / 1000.0f;
    if (delta_time > 0.1f) delta_time = 0.1f; // clamp para evitar pulos gigantes
    last_time_ms = now_ms;

    // gira tornado
    giants_deep.rotate_tornado();

    // Atualiza rotações dos planetas AQUI (antes de atualizar a lua)
    if (animating) {
        planet_rotations[0] += 0.2f * delta_time * 60.0f; // mantém comportamento parecido com antes
        planet_rotations[1] += 0.16f * delta_time * 60.0f;
        planet_rotations[2] += 0.12f * delta_time * 60.0f;
        planet_rotations[3] += 0.3f * delta_time * 60.0f;
    }

    // tempo em segundos — corrija o typo (1000.0f)
    float current_time = now_ms / 1000.0f;

    // Observação / salto quântico — agora vê rotações atualizadas
    quantum_moon.update_observation(player.camX, player.camY, player.camZ, player.pitch, player.yaw, current_time);

    // Atualiza posição com delta real
    quantum_moon.update_position(delta_time);

    if (!animating) {
        glutPostRedisplay();
        return;
    }

    sun.update_position(0.3f * delta_time * 60.0f);
    thimber_hearth.update_position(0.2f * delta_time * 60.0f, 0.3f);
    brittle_hollow.update_position(0.16f * delta_time * 60.0f, 0.3f);
    giants_deep.update_position(0.12f * delta_time * 60.0f, 0.3f);
    dark_bramble.update_position(0.3f * delta_time * 60.0f, -0.3f);
    interloper.update_position(0.001f * delta_time * 60.0f, 0.3f);

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    srand(time(NULL));

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
