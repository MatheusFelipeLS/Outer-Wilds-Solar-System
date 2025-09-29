#include "interloper.h"

<<<<<<< HEAD
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
=======
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <SOIL/SOIL.h>

>>>>>>> 236f45c ([fix]: texture)

    // primeiro ponto de controle
    this->ctrl_points[1][0] = initial_point * 0.9f;
    this->ctrl_points[1][1] = 0.0f;
    this->ctrl_points[1][2] = sun_radius * 3.0f;

    // segundo ponto de controle
    this->ctrl_points[2][0] = -sun_radius;
    this->ctrl_points[2][1] = 0.0f;
    this->ctrl_points[2][2] = sun_radius * 1.7f;

<<<<<<< HEAD
    // ponto final
    this->ctrl_points[3][0] = -sun_radius * 1.17f;
    this->ctrl_points[3][1] = 0.0f;
    this->ctrl_points[3][2] = 0.0f;
}
=======
class Interloper {
    public:
        Interloper(
            GLfloat radius,
            GLfloat sun_radius, GLfloat initial_point,
            GLfloat t0,
            GLint slices, GLint stacks
        ) :
        translation(t0), radius(radius),
        slices(slices), stacks(stacks), textureID(0), quad(nullptr), d(false)
        {
>>>>>>> 236f45c ([fix]: texture)

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

    GLfloat material_color[] = {INTERLOPER_COLOR};
    GLfloat shininess[] = {80.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
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
        glutSolidSphere(radius, slices, stacks);    
    glPopMatrix();
}

void Interloper::bezier(float t, GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *p3, float *out) {
    float poly = 1.0f - t;
    float b0 = poly * poly * poly;
    float b1 = 3 * t * poly * poly;
    float b2 = 3 * t * t * poly;
    float b3 = t * t * t;

<<<<<<< HEAD
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
=======
            GLfloat material_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat shininess[] = {80.0};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_color);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material_color);
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_color);
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
                //glutSolidSphere(radius, slices, stacks);    

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textureID);

                quad = gluNewQuadric();
                gluQuadricTexture(quad, GL_TRUE);   
                gluQuadricNormals(quad, GLU_SMOOTH);
                gluSphere(quad, radius, slices, stacks);

                glDisable(GL_TEXTURE_2D);

            glPopMatrix();
        }

        void bezier(float t, GLfloat *p0, GLfloat *p1, GLfloat *p2, GLfloat *p3, float *out) {
            float poly = 1.0f - t;
            float b0 = poly * poly * poly;
            float b1 = 3 * t * poly * poly;
            float b2 = 3 * t * t * poly;
            float b3 = t * t * t;

            for (int i = 0; i < 3; i++) {
                out[i] = p0[i] * b0 + p1[i] * b1 + p2[i] * b2 + p3[i] * b3;
            }
        }

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
            this->t += t;  
            if (this->t > 1-1e-5) {
                animation_direction = !animation_direction;
                this->t = 0.0f;
                this->ctrl_points[1][2] = -this->ctrl_points[1][2];
                this->ctrl_points[2][2] = -this->ctrl_points[2][2];
            } 
            rotation += r;
        }

        void debug() {
            d = true;
        }

        void texture(const char* filename) {
            textureID = SOIL_load_OGL_texture(
                filename,
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
            );

            if (textureID == 0) {
                std::cout << "Erro ao carregar textura: " << filename << std::endl;
                return;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

    private:
        GLfloat translation; // movimento de translação => quanto girou em torno do sol
        GLfloat rotation = 0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLfloat ctrl_points[4][3];
        GLint slices;
        GLint stacks;
        GLuint textureID;
        GLUquadric* quad;

        // animação eliptica em torno do sol
        bool animation_direction = true; // true => indo em direção ao sol, false => se afastando do sol
        GLfloat t = 0.0f;

        GLint numPoints = sizeof(ctrl_points) / sizeof(ctrl_points[0]);
        GLfloat pos[3];

        bool d;
};

#endif
>>>>>>> 236f45c ([fix]: texture)
