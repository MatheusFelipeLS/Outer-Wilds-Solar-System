#ifndef UTILS_H
#define UTILS_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define RAD 3.14159/180
#define MAX_VERTICES 100000
#define MAX_FACES    200000   // depois de triangulação pode dobrar

struct Vertex {
    float x, y, z;
    Vertex() : x(0), y(0), z(0) {}
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

    Vertex operator+(const Vertex& o) const { return Vertex(x + o.x, y + o.y, z + o.z); }
    Vertex operator-(const Vertex& o) const { return Vertex(x - o.x, y - o.y, z - o.z); }
    Vertex operator*(float s) const { return Vertex(x * s, y * s, z * s); }
};

struct BoundingBox {
    Vertex min;
    Vertex max;

    BoundingBox() {
        min = Vertex(1e9, 1e9, 1e9);
        max = Vertex(-1e9, -1e9, -1e9);
    }

    void expand(const Vertex& v) {
        if (v.x < min.x) min.x = v.x;
        if (v.y < min.y) min.y = v.y;
        if (v.z < min.z) min.z = v.z;
        if (v.x > max.x) max.x = v.x;
        if (v.y > max.y) max.y = v.y;
        if (v.z > max.z) max.z = v.z;
    }

    // Testa se um ponto (ex: posição da câmera) está dentro da box
    bool contains(const Vertex& p) const {
        printf("min objeto x, y, z: %f %f %f\n", min.x, min.y, min.z);
        printf("max objeto x, y, z: %f %f %f\n", max.x, max.y, max.z);
        return (p.x >= min.x && p.x <= max.x &&
                p.y >= min.y && p.y <= max.y &&
                p.z >= min.z && p.z <= max.z);
    }
};

// ==================== OBJ LOADER ====================
void loadObj(const char *fname, GLuint *objects, int qt_objects, int object_indexes[], BoundingBox bboxes[]);

#endif