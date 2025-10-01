#include "timber_hearth.h"
#include <SOIL/SOIL.h>


TimberHearth::TimberHearth(
    GLfloat radius, GLfloat distance, GLfloat t0, 
    GLint slices, GLint stacks
) : translation(t0), distance(distance), radius(radius), slices(slices), stacks(stacks) {}


void TimberHearth::update_position(GLfloat t, GLfloat r) {
    translation += t;
    rotation += r;
}

void TimberHearth::draw() {
    if(d) {
        std::cout << "tl = " << translation
                    << " rt = " << rotation
                    << " dist = " << distance
                    << " radius = " << radius
        << std::endl;
    }
    GLfloat diffuse_color[] = {TIMBER_HEARTH_COLOR};
    GLfloat specular_color[] = {TIMBER_HEARTH_COLOR};
    GLfloat ambient_color[] = {TIMBER_HEARTH_COLOR};
    GLfloat shininess[] = {10.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    
    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor3f(1.0f, 1.0f, 1.0f); 

        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluQuadricNormals(quad, GL_SMOOTH);
        gluSphere(quad, radius, slices, stacks);
        gluDeleteQuadric(quad);

        glDisable(GL_TEXTURE_2D);
        
    glPopMatrix();
}


Collision TimberHearth::check_collision(float camX, float camY, float camZ) {
    float x = distance * cos(translation*RAD);
    float z = -distance * sin(translation*RAD);

    x -= camX;
    z -= camZ;
    double dist = sqrt((x*x) + (camY*camY) + (z*z));
    if(dist < radius) {
        return Collision::TIMBER_HEARTH;
    }
    return Collision::NOT;
}

void TimberHearth::loadTexture(const char* filename) {
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


