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
        // printf("min objeto x, y, z: %f %f %f\n", min.x, min.y, min.z);
        // printf("max objeto x, y, z: %f %f %f\n", max.x, max.y, max.z);
        return (p.x >= min.x && p.x <= max.x &&
                p.y >= min.y && p.y <= max.y &&
                p.z >= min.z && p.z <= max.z);
    }
};

struct BoundingSphere {
    Vertex center;
    float radius;

    BoundingSphere() : center(0,0,0), radius(0) {}

    // constrói a esfera a partir de uma bounding box
    BoundingSphere(const BoundingBox& box, float radius_factor) {
        center = Vertex(
            (box.min.x + box.max.x) * 0.5f,
            (box.min.y + box.max.y) * 0.5f,
            (box.min.z + box.max.z) * 0.5f
        );

        // raio é metade da diagonal
        float dx = box.max.x - box.min.x;
        float dy = box.max.y - box.min.y;
        float dz = box.max.z - box.min.z;
        radius = radius_factor * sqrt(dx*dx + dy*dy + dz*dz);
    }

    // testa se um ponto está dentro da esfera
    bool contains(const Vertex& p) const {
        float dx = p.x - center.x;
        float dy = p.y - center.y;
        float dz = p.z - center.z;
        float dist2 = dx*dx + dy*dy + dz*dz;
        return dist2 <= radius*radius;
    }
};

enum class Collision {
    NOT = 100, // sem colisão
    SUN,
    THIMBER_HEARTH,
    BRITTLE_HOLLOW,
    GIANTS_DEEP,
    GIANTS_DEEP_TORNADO,
    DARK_BRAMBLE,
    DARK_BRAMBLE_PORTAL,
    DARK_HOLE,
    WHITE_HOLE,
    VOID_WRONG_PORTAL,
    VOID_RIGHT_PORTAL,
    VOID_CORE,
    VOID_SHELL
};

// ==================== OBJ LOADER ====================
void loadObj(const char *fname, GLuint *objects, int qt_objects, int object_indexes[], BoundingBox bboxes[], float scale);

#endif