#include "white_hole.h"
#include <SOIL/SOIL.h>

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

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluQuadricNormals(quad, GLU_SMOOTH);

        gluSphere(quad, radius, slices, stacks);

        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);

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

void WhiteHole::loadTexture(const char* filename) {
    textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );

    if(textureID == 0) {
        std::cout << "Erro carregando textura: " << SOIL_last_result() << std::endl;
    } else {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}

