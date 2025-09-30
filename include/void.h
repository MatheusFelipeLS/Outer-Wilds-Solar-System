#ifndef VOID_H
#define VOID_H

#include <GL/glut.h>
#include <iostream>
#include <vector>

#include "utils.h"

enum class Portal {RIGHT, WRONG, NOTHING};

class Void {
    public:
        Void() {};

        void draw();
        Collision check_colision(float camX, float camY, float camZ);
        Portal inside(float camX, float camY, float camZ);
        
        void add_portal();

        void remove_portal();

        int qt_portals();

        std::pair<int, float> last_rotation();

        void set_shell(GLuint s);
        void set_core(GLuint c[]);
        
        void set_portal(GLuint p[]);

        void set_shell_bounding_boxes(BoundingBox bb);
        void set_portal_bounding_boxes(BoundingBox bb[]);
        void set_core_bounding_boxes(BoundingBox bb[], int qt_bb, int void_core_objects_indexes[]);
        
    private:
        int void_core_objects_indexes[18];

        GLuint shell;
        GLuint core[2];
        GLuint portal[2];

        std::vector<int> rotations;

        BoundingBox core_bboxes[18];
        BoundingSphere core_bspheres[18];

        BoundingBox shell_bbox;
        BoundingSphere shell_bsphere;

        BoundingBox portal_bboxes[2];
        BoundingSphere portal_bspheres[2];
        std::vector<int> right_portal;
        int possible_indexes[8] = {0, 1, 3, 4, 5, 6, 8, 9};

        float portal_distance = 823.50;
};

#endif