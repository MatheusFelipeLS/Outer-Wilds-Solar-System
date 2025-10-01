#include "giants_deep.h"
#include <SOIL/SOIL.h>


GiantsDeep::GiantsDeep(
    GLfloat radius, GLfloat distance, GLfloat t0, 
    GLint slices, GLint stacks
) : translation(t0), distance(distance), radius(radius), slices(slices), stacks(stacks)
{
    inner_globe_radius = radius / 3.0f;
}

void GiantsDeep::update_position(GLfloat t, GLfloat r) {
    translation += t;
    rotation += r;
}

void GiantsDeep::draw() {
    if(d) {
        std::cout << "tl = " << translation
                    << " rt = " << rotation
                    << " dist = " << distance
                    << " radius = " << radius
        << std::endl;
    }
    
    GLfloat surface_diffuse_color[] = {GIANTS_DEEP_SURFACE_COLOR};
    GLfloat surface_specular_color[] = {GIANTS_DEEP_SURFACE_COLOR};
    GLfloat surface_ambient_color[] = {GIANTS_DEEP_SURFACE_COLOR};
    GLfloat surface_shininess[] = {30.0f};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, surface_diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, surface_specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, surface_ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, surface_shininess);

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);

        glEnable(GL_TEXTURE_2D); 
        glBindTexture(GL_TEXTURE_2D, textureID); 

        glColor3f(1.0f, 1.0f, 1.0f); 

        GLUquadric* quad = gluNewQuadric(); 

        gluQuadricTexture(quad, GL_TRUE); 
        gluQuadricOrientation(quad, GLU_OUTSIDE); 

        gluQuadricNormals(quad, GL_SMOOTH); 
        gluSphere(quad, radius, slices, stacks); 
        gluDeleteQuadric(quad); 

        glDisable(GL_TEXTURE_2D);  

        GLfloat inner_globe_diffuse_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
        GLfloat inner_globe_specular_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
        GLfloat inner_globe_ambient_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
        GLfloat inner_globe_shininess[] = {30.0f};

        glMaterialfv(GL_FRONT, GL_DIFFUSE, inner_globe_diffuse_color);
        glMaterialfv(GL_FRONT, GL_SPECULAR, inner_globe_specular_color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, inner_globe_ambient_color);
        glMaterialfv(GL_FRONT, GL_SHININESS, inner_globe_shininess);
    
        glutSolidSphere(inner_globe_radius, slices, stacks);
        
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
            glRotatef(-90, 0, 0, 1);
            glTranslatef(0.0, inner_globe_radius-33, 0.0);
            glScalef(3.5, 4.4, 3.5);
            glCallList(tornados);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef(0, inner_globe_radius-3, 0);
            glScalef(1, 3.8, 1);
            glCallList(tornados);
        glPopMatrix();

        glPushMatrix(); 
            glTranslatef(0, -inner_globe_radius*3+3, 0);
            glScalef(1, 3.8, 1);
            glCallList(tornados);
        glPopMatrix();

        glPushMatrix(); 
            glRotatef(90, 1, 0, 0);
            glTranslatef(0, inner_globe_radius-3, 0);
            glScalef(1, 3.8, 1);
            glCallList(tornados);
        glPopMatrix();

        glPushMatrix(); 
            glRotatef(-90, 1, 0, 0);
            glTranslatef(0, inner_globe_radius-3, 0);
            glScalef(1, 3.8, 1);
            glCallList(tornados);
        glPopMatrix();

        glPushMatrix(); 
            glRotatef(90, 0, 0, 1);
            glTranslatef(0, inner_globe_radius-3, 0);
            glScalef(1, 3.8, 1);
            glCallList(tornados);
        glPopMatrix();

    glPopMatrix();

    // talvez devesse transformar os tornados em uma classe. TQV
    // GLfloat tornado_diffuse_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
    // GLfloat tornado_specular_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
    // GLfloat tornado_ambient_color[] = {GIANTS_DEEP_INNER_GLOBE_COLOR};
    // GLfloat tornado_shininess[] = {30.0f};

    // glMaterialfv(GL_FRONT, GL_DIFFUSE, tornado_diffuse_color);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, tornado_specular_color);
    // glMaterialfv(GL_FRONT, GL_AMBIENT, tornado_ambient_color);
    // glMaterialfv(GL_FRONT, GL_SHININESS, tornado_shininess);

    // glPushMatrix(); 
    //     glRotatef (translation, 0.0, 1.0, 0.0);
    //     glTranslatef (distance - inner_globe_radius, 0.0f, 0.0);
    //     glScalef(4.0, 1.0f, 1.0f);
    //     glRotatef (-90.0f, 0.0, 0.0, 1.0);
    //     glRotatef (tornado_rotation, 0.0, 1.0, 0.0);
    //     glCallList(tornados);
    // glPopMatrix();

    // glPushMatrix(); 
    //     glRotatef (translation, 0.0, 1.0, 0.0);
    //     glTranslatef (distance, -inner_globe_radius, 0.0);
    //     glScalef(1.0, 4.0f, 1.0f);
    //     glRotatef (-90.0f, 0.0, 1.0, 0.0);
    //     glRotatef (tornado_rotation, 0.0, 1.0, 0.0);
    //     glCallList(tornados);
    // glPopMatrix();
}

void GiantsDeep::loadTexture(const char* filename) {
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
