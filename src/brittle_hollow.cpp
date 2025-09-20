#include "brittle_hollow.h"

void BrittleHollow::draw() {
    if(d) {
        std::cout << "tl = " << translation
                    << " rt = " << rotation
                    << " dist = " << distance
                    << " radius = " << radius
        << std::endl;
    }
    
    GLfloat surface_diffuse_color[] = {BRITTLE_HOLLOW_COLOR};
    GLfloat surface_specular_color[] = {BRITTLE_HOLLOW_COLOR};
    GLfloat surface_ambient_color[] = {BRITTLE_HOLLOW_COLOR};
    GLfloat surface_shininess[] = {10.0f};
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, surface_diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, surface_specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, surface_ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, surface_shininess);

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);


        glutSolidSphere(radius, slices, stacks);    

        GLfloat bh_ambient[] = {0.0f, 0.0f, 0.0f};
        GLfloat bh_diffuse[] = {1.0f, 1.0f, 1.0f};
        GLfloat bh_specular[] = {0.1f, 0.1f, 0.1f};
        GLfloat bh_shininess[] = { 120.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, bh_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, bh_diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, bh_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, bh_shininess);
        
        glutSolidSphere(radius / 4, slices, stacks);    
    glPopMatrix();
}


bool BrittleHollow::inside_dark_hole(GLdouble camX, GLdouble camY, GLdouble camZ) {
    // encontrando a posição do buraco negro
    auto [x, z] = get_black_hole_position();
    double dh_radius = radius / 4.0f;

    return (
        (camX < x+dh_radius && camX > x-dh_radius) && 
        (camY < dh_radius && camY > -dh_radius) && 
        ((-camZ) < z+dh_radius && (-camZ) > z-dh_radius)
    );
}
