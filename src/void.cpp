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
    glTranslatef(30, 30, 30);
        glCallList(core[0]);
        glCallList(core[2]);
        glTranslatef(-30, -30, -30);
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
        glTranslatef(30, 30, 30);
        glCallList(core[1]);
        glTranslatef(-30, -30, -30);
        glRotatef(rot, 0.0, 1.0, 0.0);
        glTranslatef(787.500000, 0.0, 0.0f);
        glCallList(portal[0]);
    glPopMatrix();
}

void Void::desloc() {
    t += 1.5;
}