#include "void.h"

void Void::draw() {
    GLfloat wooden_diffuse[] = {0.26f, 0.125f, 0.0f, 1.0f};
    GLfloat wooden_specular[] = {0.0, 0.0, 0.0f, 1.0f};
    GLfloat wooden_ambient[] = {0.26f, 0.125f, 0.0f, 1.0f};
    GLfloat wooden_shininess[] = {0.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wooden_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wooden_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, wooden_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, wooden_shininess);

    glCallList(shell);

    
}