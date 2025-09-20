#include "thimber_hearth.h"

void ThimberHearth::draw() {
    if(d) {
        std::cout << "tl = " << translation
                    << " rt = " << rotation
                    << " dist = " << distance
                    << " radius = " << radius
        << std::endl;
    }
    GLfloat diffuse_color[] = {THIMBER_HEARTH_COLOR};
    GLfloat specular_color[] = {THIMBER_HEARTH_COLOR};
    GLfloat ambient_color[] = {THIMBER_HEARTH_COLOR};
    GLfloat shininess[] = {10.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    
    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);
        glutSolidSphere(radius, slices, stacks);    
    glPopMatrix();
}