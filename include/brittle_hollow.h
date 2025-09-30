#ifndef BRITTLE_HOLLOW_H
#define BRITTLE_HOLLOW_H

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include "utils.h"

#define BRITTLE_HOLLOW_COLOR 74.0f/255.0f, 0.0f, 128.0f/255.0f, 1.0f
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

        void queda();

        void loadTexture(const char* filename);


        double dh_radius;

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint textureID;



        bool d;

        GLuint surface[285];
        BoundingSphere bspheres[285];
        float piece_distance[285];
        float piece_rotation[285];
        int current;

        float tx = 0.0f;
        float ty = 0.0f;
        float tz = 0.0f;
        float current_x;
        float current_y;
        float current_z;
        std::vector<size_t> available_pieces;
        float delta = 1000.0;
        int qt_objects = 285;
};

#endif