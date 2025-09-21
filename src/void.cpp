#include "void.h"

void Void::draw() {

    GLfloat portal_diffuse[] = {128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f, 0.3f};
    GLfloat portal_specular[] = {128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f, 0.3f};
    GLfloat portal_ambient[] = {128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f, 0.3f};
    GLfloat portal_shininess[] = {0.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, portal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, portal_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, portal_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, portal_shininess);

    printf("t %f\n", t);
    if(rot == 400) {
        rot = rand() % 360;
    }
    printf("rot %d", rot);

    glPushMatrix();
        glCallList(core[0]);
        glCallList(core[2]);
        // colocando o portal numa posição random
        glRotatef(rot, 0.0, 1.0, 0.0); 
        glTranslatef(787.500000, 0.0, 0.0f); /// parametrizar melhor isso depois
        glCallList(portal[1]);
    glPopMatrix();


    GLfloat wooden_diffuse[] = {0.26f, 0.125f, 0.0f, 1.0f};
    GLfloat wooden_specular[] = {0.0, 0.0, 0.0f, 1.0f};
    GLfloat wooden_ambient[] = {0.26f, 0.125f, 0.0f, 1.0f};
    GLfloat wooden_shininess[] = {0.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wooden_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wooden_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, wooden_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, wooden_shininess);

    glPushMatrix();
        glCallList(shell);
        glCallList(core[1]);
        glRotatef(rot, 0.0, 1.0, 0.0);
        glTranslatef(787.500000, 0.0, 0.0f);
        glCallList(portal[0]);
    glPopMatrix();
}

void Void::set_bounding_boxes(BoundingBox bb[], int qt_bb, int void_core_objects_indexes[]) {
    int k = 0;
    for(int i = 0; i < qt_bb; i++) {
        bboxes[i] = bb[i];
        if(void_core_objects_indexes[i] == 0) {
            bspheres[k] = BoundingSphere(bb[i]);
            k++;
        }
    }  
}

bool Void::check_colision(float camX, float camY, float camZ) {
    // começando de dois para ignorar as entradas do portal
    double x = 0;
    double z = 0;
    printf("x e z do planeta: %f %f\n", x, z);
    printf("meu x, y, z: %f %f %f\n", camX-x, camY, camZ+z);
    for (size_t i = 0; i < 2; i++) {
        printf("i = %ld\n", i);
        if (bspheres[i].contains(Vertex((camX-x), camY, (camZ+z)))) {
            std::cout << "Colisão detectada com objeto " << i << std::endl;
            return false;
        }
    }
    
    for (size_t i = 2; i < sizeof(bboxes) / sizeof(bboxes[0]); i++) {
        printf("i = %ld\n", i); 
        if (bboxes[i].contains(Vertex(camX-x, camY, camZ+z))) {
            std::cout << "Colisão detectada com objeto " << i << std::endl;
            return true;
        }
    }
    return false;
}
