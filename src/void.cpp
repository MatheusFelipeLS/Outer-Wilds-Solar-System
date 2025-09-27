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

    for(int i = 0; i < 18; i++) {
        if(i != right_portal.back()) {
            continue;
        }
        glPushMatrix();
            // leva o sistema de coordenadas até o centro
            glTranslatef(core_bspheres[i].center.x, core_bspheres[i].center.y, core_bspheres[i].center.z);

            // define cor (vermelho, por exemplo)
            glColor3f(1.0f, 0.0f, 0.0f);

            // desenha esfera em modo wireframe
            glutWireSphere(core_bspheres[i].radius, 16, 16);
        glPopMatrix();
    }

    glPushMatrix();
        glCallList(core[0]);
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

void Void::set_core_bounding_boxes(BoundingBox bb[], int qt_bb, int void_core_objects_indexes[]) {
    for(int i = 0; i < qt_bb; i++) {
        core_bboxes[i] = bb[i];
        this->void_core_objects_indexes[i] = void_core_objects_indexes[i];
        if(void_core_objects_indexes[i] == 0) {
            core_bspheres[i] = BoundingSphere(bb[i], 0.5);
        } else {
            core_bspheres[i] = BoundingSphere(bb[i], 0.35);
        }
    }  
}

void Void::set_shell_bounding_boxes(BoundingBox bb) {
    shell_bbox = bb; 
    shell_bsphere = BoundingSphere(bb, 0.5);
}

void Void::set_portal_bounding_boxes(BoundingBox bb[]) {
    portal_bboxes[0] = bb[0]; 
    portal_bspheres[0] = BoundingSphere(bb[0], 0.5);
    portal_bboxes[1] = bb[1]; 
    portal_bspheres[1] = BoundingSphere(bb[1], 0.5);    
}

Collision Void::check_colision(float camX, float camY, float camZ) {
    if(!shell_bsphere.contains(Vertex(camX, camY, camZ))) {
        return Collision::VOID_SHELL;
    }

    for(int i = 0; i < 18; i++) {
        if(void_core_objects_indexes[i] == 1 && core_bspheres[i].contains(Vertex(camX, camY, camZ))) {
            return Collision::VOID_CORE;
        } else if(void_core_objects_indexes[i] == 0 && core_bspheres[i].contains(Vertex(camX, camY, camZ))) {
            return Collision::NOT;
        }
    }   

    return Collision::NOT;
}


Portal Void::inside(float camX, float camY, float camZ) {
    for (int i = 0; i < 18; i++) {
        if(void_core_objects_indexes[i] == 0 && core_bspheres[i].contains(Vertex(camX, camY, camZ)) && i == right_portal.back()) {
            std::cout << "RIGHT" << i << std::endl;
            return Portal::RIGHT;
        } else if(void_core_objects_indexes[i] == 0 && core_bspheres[i].contains(Vertex(camX, camY, camZ)) && i != right_portal.back()) {
            std::cout << "WRONG" << i << std::endl;
            return Portal::WRONG;
        }
    }
  
    return Portal::NOTHING;
    // 108.300247 24.000000 23.705524 esquerda 
    // 61.474453 62.000000 226.668900 direita
    // 175.063477 213.000000 157.644516 bola alta
    // 345.037750 40.000000 125.442276 bola de trás
}