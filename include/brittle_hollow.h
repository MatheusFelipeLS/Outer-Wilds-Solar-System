#ifndef BRITTLE_HOLLOW_H
#define BRITTLE_HOLLOW_H

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

#include "white_hole.h"
#include "utils.h"

// #define BRITTLE_HOLLOW_COLOR 74.0f/255.0f, 0.0f, 128.0f/255.0f, 1.0f
#define BRITTLE_HOLLOW_DIFFUSE_COLOR 0.290196, 0.000000, 0.403922,
#define BRITTLE_HOLLOW_SPECULAR_COLOR 0.290196, 0.000000, 0.568627,
#define BRITTLE_HOLLOW_AMBIENT_COLOR 0.035294, 0.000000, 0.505882,
#define BRITTLE_HOLLOW_SHININESS 0.000000

#define RAD 3.14159/180

class BrittleHollow {
    public:
        BrittleHollow(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        );

        Collision check_collision(float camX, float camY, float camZ);

        void draw();

        void change_current_piece();

        void set_current();

        void update_position(GLfloat t, GLfloat r);

        bool inside_dark_hole(GLdouble camX, GLdouble camY, GLdouble camZ);

        std::pair<double, double> get_black_hole_position();

        void debug() { d = true; }

        void set_surface(GLuint brittle_hollow_surface[], int qt);
        void set_surface_bouding_boxes(BoundingBox brittle_hollow_bboxes[], int qt);

        void move_piece();

        void loadTexture(const char* filename);

        void set_white_hole_position(Vertex p);
        void set_white_hole(WhiteHole *white_hole) { wh = white_hole; }

        double dh_radius;

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint textureID;

        GLuint surface[285];
        BoundingSphere bspheres[285];
        
        size_t current; // indice da peça que está sendo movida
        float tx = 0.0f; // quanto a peça foi deslocada (no eixo x)
        float ty = 0.0f; // quanto a peça foi deslocada (no eixo y)
        float tz = 0.0f; // quanto a peça foi deslocada (no eixo z)
        float current_x; // posição inicial da peça (no eixo x)
        float current_y; // posição inicial da peça (no eixo y)
        float current_z; // posição inicial da peça (no eixo z)
        std::vector<size_t> available_pieces;
        float delta = 1000.0; // tem relação direta com a velocidade que o objeto se move
        int qt_objects = 285; // quantidade de formas que moldam o planeta
        
        Vertex wh_position; // white hole position
        std::vector<size_t> teleported_pieces; // índice dos pedaços que foram teleportadas
        std::vector<Vertex> piece_position; // posição dos pedaços teleportados
        WhiteHole *wh;
        float max_x;
        float max_y;
        float max_z;

        bool d;
};

#endif