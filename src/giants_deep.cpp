#include "giants_deep.h"

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

        glutSolidSphere(radius, slices, stacks);    

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
