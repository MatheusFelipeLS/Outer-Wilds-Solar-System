#include "sun.h"
#include <SOIL/SOIL.h>

Sun::Sun(
    GLfloat radius, 
    GLint slices, GLint stacks
) : radius(radius), slices(slices), stacks(stacks) {}

void Sun::draw() {
    if(d) {
        std::cout << "rt = " << rotation
                    << " radius = " << radius
        << std::endl;
    }
        

    glLightfv(GL_LIGHT0, GL_POSITION, this->light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, this->light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, this->light_color);

    // material emissivo (faz a esfera "brilhar")
    glMaterialfv(GL_FRONT, GL_EMISSION, this->emission);

    glPushMatrix();
        glRotatef(rotation * 0.1f, 0.0f, 1.0f, 0.0f); 
        quad = gluNewQuadric ();
        gluQuadricTexture ( quad , GL_TRUE );

        // textura 
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluQuadricNormals(quad, GL_SMOOTH);

        gluSphere(quad, radius, slices, stacks);

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    
    // reseta para não deixar os outros objetos brilharem também
    GLfloat no_emission[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

}

void Sun::update_position(GLfloat r) {
    rotation += r;
    if (emission[1] >= 1.0f) {
        transition_color_signal = -1;
    } else if (emission[1] <= lower_bound_color) {
        transition_color_signal = 1;
    }
    emission[1] += transition_color_signal * 0.003;
    light_color[1] = emission[1];
}

Collision Sun::check_collision(float camX, float camY, float camZ) {
    double dist = sqrt((camX * camX) + (camY * camY) + (camZ * camZ));
    if(dist < radius) {
        return Collision::SUN;
    }
    return Collision::NOT;
}

void Sun::loadTexture(const char* filename) {
    textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );

    if(textureID == 0) {
        std::cout << "Erro carregando textura: " << SOIL_last_result() << std::endl;
    }
}
