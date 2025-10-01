#include "interloper.h"
#include <SOIL/SOIL.h>


Interloper::Interloper(
    GLfloat radius,
    GLfloat sun_radius, GLfloat initial_point,
    GLint slices, GLint stacks
) :
    radius(radius),
    slices(slices), stacks(stacks)
{
    // ponto inicial
    this->ctrl_points[0][0] = initial_point;
    this->ctrl_points[0][1] = 0.0f;
    this->ctrl_points[0][2] = 0.0f;

    // primeiro ponto de controle
    this->ctrl_points[1][0] = initial_point * 0.9f;
    this->ctrl_points[1][1] = 0.0f;
    this->ctrl_points[1][2] = sun_radius * 3.0f;

    // segundo ponto de controle
    this->ctrl_points[2][0] = -sun_radius;
    this->ctrl_points[2][1] = 0.0f;
    this->ctrl_points[2][2] = sun_radius * 1.9f;

    // ponto final
    this->ctrl_points[3][0] = -sun_radius * 1.2f;
    this->ctrl_points[3][1] = 0.0f;
    this->ctrl_points[3][2] = 0.0f;
}

void Interloper::draw() {
    if(d) {
        std::cout 
                //   << "tl = " << translation
                //   << " rt = " << rotation
                //   << " dist = " << distance
                    << " t = " << t
                    << " p0 = " << pos[0]
                    << " p1 = " << pos[1]
                    << " p2 = " << pos[2]
        << std::endl;
    }
    
    GLfloat diffuse_color[] = {INTERLOPER_DIFFUSE_COLOR};
    GLfloat specular_color[] = {INTERLOPER_SPECULAR_COLOR};
    GLfloat ambient_color[] = {INTERLOPER_AMBIENT_COLOR};
    GLfloat shininess[] = {INTERLOPER_SHININESS};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    if (animation_direction) {
        bezier(
            t, 
            ctrl_points[0], 
            ctrl_points[1], 
            ctrl_points[2], 
            ctrl_points[3], 
            pos
        );
    } else {
        bezier(
            t, 
            ctrl_points[3], 
            ctrl_points[2], 
            ctrl_points[1], 
            ctrl_points[0], 
            pos
        );
    }

    glPushMatrix(); 
        glTranslatef(pos[0], pos[1], pos[2]);
        glRotatef (rotation, 0.0, 1.0, 0.0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);

        gluQuadricNormals(quad, GL_SMOOTH);

        gluSphere(quad, radius, slices, stacks);
        
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Interloper::bezier(float t, GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *p3, float *out) {
    float poly = 1.0f - t;
    float b0 = poly * poly * poly;
    float b1 = 3 * t * poly * poly;
    float b2 = 3 * t * t * poly;
    float b3 = t * t * t;

    for (int i = 0; i < 3; i++) {
        out[i] = p0[i] * b0 + p1[i] * b1 + p2[i] * b2 + p3[i] * b3;
    }
}

void Interloper::update_position(GLfloat t, GLfloat r) {
    this->t += t;  
    if (this->t > 1-1e-5) {
        animation_direction = !animation_direction;
        this->t = 0.0f;
        this->ctrl_points[1][2] = -this->ctrl_points[1][2];
        this->ctrl_points[2][2] = -this->ctrl_points[2][2];
    } 
    rotation += r;
}

void Interloper::loadTexture(const char* filename) {
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

