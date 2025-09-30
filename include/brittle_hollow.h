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
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks)
        {
            dh_radius = radius / 3.0f;
            available_pieces = std::vector<size_t>(285);
            std::iota(available_pieces.begin(), available_pieces.end(), 0);
            change_current_piece();
        }

        Collision check_collision(float camX, float camY, float camZ);

        void draw();

        void change_current_piece() {
            current = rand() % available_pieces.size();
            tx = ty = tz = 0.0f;
            set_current();
        }

        void set_current() {
            current_x = bspheres[available_pieces[current]].center.x;
            current_y = bspheres[available_pieces[current]].center.y;
            current_z = bspheres[available_pieces[current]].center.z;
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            rotation += r;
        }

        bool inside_dark_hole(GLdouble camX, GLdouble camY, GLdouble camZ);

        std::pair<double, double> get_black_hole_position() {
            return std::make_pair(distance * cos(translation*RAD), distance * sin(translation*RAD));
        }

        void debug() { d = true; }

        void set_surface(GLuint brittle_hollow_surface[], int qt);
        void set_surface_bouding_boxes(BoundingBox brittle_hollow_bboxes[], int qt);

        void queda();

        double dh_radius;

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;


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