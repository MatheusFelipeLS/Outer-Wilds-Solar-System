#ifndef UTILS_H
#define UTILS_H

#include <GL/glut.h>
#include <iostream>

#define MAX_VERTICES 100000
#define MAX_FACES    200000   // depois de triangulação pode dobrar

typedef struct {
    float x, y, z;
} Vertex;

// ==================== OBJ LOADER ====================

void loadObj(const char *fname, GLuint *objects, int qt_objects, int object_indexes[]) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("Can't open file %s\n", fname);
        exit(1);
    }

    Vertex vertices[MAX_VERTICES];
    int faces[MAX_FACES][3];  // sempre triângulos
    int num_vertices = 0, num_faces = 0;
    char line[256];
    int count = 0;
    int indexes[qt_objects];
    while (fgets(line, sizeof(line), fp)) {
        // vértices
        if (line[0] == 'v' && line[1] == ' ') {
            Vertex v;
            sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
            vertices[num_vertices++] = v;
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
        } else if(line[0] == 'o' && line[1] == ' ' && num_vertices > 0) {
            indexes[count] = num_faces;
            count++;
        }
    }
    fclose(fp);

    indexes[count] = num_faces;

    int new_indexes[qt_objects+1];
    for(int i = 1; i < qt_objects; i++) {
        new_indexes[i] = -1;
    }

    new_indexes[0] = 0;
    new_indexes[1] = indexes[0];
    int k = 1;
    for(int i = 0; i < qt_objects-1; i++) {
        if(object_indexes[i] == object_indexes[i+1]) {
            new_indexes[k] = indexes[i+1];
        } else {
            k++;
            new_indexes[k] = indexes[i+1];
        }
    }
    k++;

    for(int i = 0; i < k; i++) {
        printf("k: %d\n", new_indexes[i]);
    }

    for(int i = 0; i < k; i++) {
        objects[i] = glGenLists(1);
        glNewList(objects[i], GL_COMPILE);
        {
            glBegin(GL_TRIANGLES);
            for (int j = new_indexes[i]; j < new_indexes[i+1]; j++) {
                Vertex v1 = vertices[faces[j][0]];
                Vertex v2 = vertices[faces[j][1]];
                Vertex v3 = vertices[faces[j][2]];
                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
                glVertex3f(v3.x, v3.y, v3.z);
            }
            glEnd();
        }
        glEndList();
    }
}

#endif