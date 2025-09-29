#ifndef GIANTS_DEEP_H
#define GIANTS_DEEP_H

#include <GL/glut.h>
#include <iostream>

#define GIANTS_DEEP_SURFACE_COLOR 0.0f, 253.0f/255.0f, 72.0f/255.0f, 1.0f
#define GIANTS_DEEP_INNER_GLOBE_COLOR 10.0f/255.0f, 105.0f/255.0f, 74.0f/255.0f, 1.0f

class GiantsDeep {
    public:
        GiantsDeep(
            GLfloat radius, GLfloat distance, GLfloat t0, 
            GLint slices, GLint stacks
        ) :
        translation(t0), distance(distance), radius(radius),
        slices(slices), stacks(stacks)
        {}

        void draw() {
            if(d) {
                std::cout << "tl = " << translation
                          << " rt = " << rotation
                          << " dist = " << distance
                          << " radius = " << radius
                << std::endl;
            }
            
            GLfloat material_color[] = {GIANTS_DEEP_SURFACE_COLOR};
            GLfloat shininess[] = {30.0f};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, surface_diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, surface_specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, surface_ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, surface_shininess);

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);

                //glutSolidSphere(radius, slices, stacks);  // não suporta textura !!1

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textureID);

                quad = gluNewQuadric();
                gluQuadricTexture(quad, GL_TRUE);   
                gluQuadricNormals(quad, GLU_SMOOTH);
                gluSphere(quad, radius, slices, stacks);

        GLfloat inner_globe_diffuse_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
        GLfloat inner_globe_specular_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
        GLfloat inner_globe_ambient_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
        GLfloat inner_globe_shininess[] = {30.0f};

        glMaterialfv(GL_FRONT, GL_DIFFUSE, inner_globe_diffuse_color);
        glMaterialfv(GL_FRONT, GL_SPECULAR, inner_globe_specular_color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, inner_globe_ambient_color);
        glMaterialfv(GL_FRONT, GL_SHININESS, inner_globe_shininess);
    
        glutSolidSphere(inner_globe_radius, slices, stacks);   
    glPopMatrix();

    // talvez devesse transformar os tornados em uma classe. TQV
    GLfloat tornado_diffuse_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
    GLfloat tornado_specular_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
    GLfloat tornado_ambient_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
    GLfloat tornado_shininess[] = {30.0f};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, tornado_diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tornado_specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, tornado_ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, tornado_shininess);

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance - inner_globe_radius, 0.0f, 0.0);
        glScalef(4.0, 1.0f, 1.0f);
        glRotatef (-90.0f, 0.0, 0.0, 1.0);
        glRotatef (tornado_rotation, 0.0, 1.0, 0.0);
        glCallList(tornados);
    glPopMatrix();

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, -inner_globe_radius, 0.0);
        glScalef(1.0, 4.0f, 1.0f);
        glRotatef (-90.0f, 0.0, 1.0, 0.0);
        glRotatef (tornado_rotation, 0.0, 1.0, 0.0);
        glCallList(tornados);
    glPopMatrix();
}

        void update_position(GLfloat t, GLfloat r) {
            translation += t;
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
        GLfloat rotation = 0.0;    // movimento de rotação => quanto girou em torno de si mesmo
        GLfloat distance;    // distancia do sol
        GLfloat radius;      // raio do planeta
        GLint slices;
        GLint stacks;
        GLuint textureID;
        GLUquadric* quad;

        bool d;
};

#endif