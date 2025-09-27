#include "utils.h"

// ==================== OBJ LOADER ====================
void loadObj(const char *fname, GLuint *objects, int qt_objects, int object_indexes[], BoundingBox bboxes[], float scale) {
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

    int captured = 0;
    int idx = 0;
    while(captured < qt_objects) {
        objects[idx] = glGenLists(1);
        glNewList(objects[idx], GL_COMPILE);
        int it = 0;
        for(int i = 0; i < qt_objects; i++) {
            if(object_indexes[i] != idx) {
                it = indexes[i];
                continue;
            }

            captured++;
            {
            glBegin(GL_TRIANGLES);
            for (int j = it; j < indexes[i]; j++) {
                Vertex v1 = vertices[faces[j][0]] * scale;
                Vertex v2 = vertices[faces[j][1]] * scale;
                Vertex v3 = vertices[faces[j][2]] * scale;
                bboxes[i].expand(v1);
                bboxes[i].expand(v2);
                bboxes[i].expand(v3);
                glVertex3f(v1.x, v1.y, v1.z);
                glVertex3f(v2.x, v2.y, v2.z);
                glVertex3f(v3.x, v3.y, v3.z);
            }
            glEnd();
            }
            it = indexes[i];
        }
        glEndList();
        idx++;
    }

}