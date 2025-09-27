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

    // printf("rotations.back() %f\n", rotations.back());

    for(size_t i = 0; i < rotations.size(); i++) {
        std::cout << rotations[i] << " ";
    }
    std::cout << std::endl;
    
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

    // portal
    glPushMatrix();
        // leva o sistema de coordenadas até o centro
        glTranslatef(portal_bspheres[1].center.x + (portal_distance + 21) * cos(rotations.back()*RAD), portal_bspheres[1].center.y, portal_bspheres[1].center.z - (portal_distance + 21) * sin(rotations.back()*RAD));
        glColor3f(0.0f, 1.0f, 0.0f);

        // desenha esfera em modo wireframe
        glutWireSphere(portal_bspheres[1].radius, 16, 16);
    glPopMatrix();


    glPushMatrix();
        glCallList(core[0]);
        // colocando o portal numa posição random
        glRotatef(rotations.back(), 0.0, 1.0, 0.0); 
        glTranslatef(portal_distance, 0.0f, 0.0f); // parametrizar melhor isso depois
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
        glRotatef(rotations.back(), 0.0f, 1.0f, 0.0f);
        glTranslatef(portal_distance, 0.0f, 0.0f);
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
    shell_bsphere = BoundingSphere(bb, 0.28);
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
    
    if(portal_bspheres[1].contains(Vertex(camX, camY, camZ))) {
        return Collision::NOT;
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
            return Portal::RIGHT;
        } else if(void_core_objects_indexes[i] == 0 && core_bspheres[i].contains(Vertex(camX, camY, camZ)) && i != right_portal.back()) {
            return Portal::WRONG;
        }
    }

    float x = (portal_distance + 21) * cos(rotations.back()*RAD); 
    float z = -(portal_distance + 21) * sin(rotations.back()*RAD);
    if(portal_bspheres[1].contains(Vertex(camX-x, camY, camZ-z))) {
        return Portal::RIGHT;
    }
  
    return Portal::NOTHING;
}