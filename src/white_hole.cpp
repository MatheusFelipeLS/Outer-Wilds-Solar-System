#include "white_hole.h"

void WhiteHole::draw() {
    GLfloat light_pos[4] = {p.x, p.y, p.z, 1.0f};
    GLfloat light_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat emission[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // branco

    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_color);

    // material emissivo (faz a esfera "brilhar")
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glPushMatrix();
        glTranslatef(p.x, p.y, p.z);
        quad = gluNewQuadric();
        gluQuadricTexture (quad, GL_TRUE);
        gluSphere (quad, radius, slices, stacks);
    glPopMatrix();
    
    // Depois, sempre volte a emissão pro "zero" pros outros planetas:
    GLfloat black_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, black_emission);
}

bool WhiteHole::inside(GLdouble camX, GLdouble camY, GLdouble camZ) {
    return (
        (camX <= p.x+radius && camX >= p.x-radius) && 
        (camY <= p.y+radius && camY >= p.y-radius) && 
        (camZ <= p.z+radius && camZ >= p.z-radius)
    );
}

Vertex WhiteHole::get_position() {
    return p;
}