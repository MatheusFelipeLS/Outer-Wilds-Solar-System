#include "dark_bramble.h"

DarkBramble::DarkBramble(
    GLfloat radius, GLfloat distance, GLfloat t0, 
    GLint slices, GLint stacks
) : translation(t0), distance(distance), radius(radius), slices(slices), stacks(stacks) {}

void DarkBramble::draw() {
    if(d) {
        std::cout << "tl = " << translation
                    << " rt = " << rotation
                    << " dist = " << distance
                    << " radius = " << radius
        << std::endl;
    }


    GLfloat portal_diffuse[] = {DARK_BRAMBLE_COLOR};
    GLfloat portal_specular[] = {DARK_BRAMBLE_COLOR};
    GLfloat portal_ambient[] = {DARK_BRAMBLE_COLOR};
    GLfloat portal_shininess[] = {0.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, portal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, portal_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, portal_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, portal_shininess);
        

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);
        glCallList(portal);
    glPopMatrix();

    GLfloat wooden_diffuse[] = {0.26f, 0.125f, 0.0f, 1.0f};
    GLfloat wooden_specular[] = {0.0, 0.0, 0.0f, 1.0f};
    GLfloat wooden_ambient[] = {0.26f, 0.125f, 0.0f, 1.0f};
    GLfloat wooden_shininess[] = {0.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wooden_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wooden_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, wooden_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, wooden_shininess);

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);
        glCallList(shell);
    glPopMatrix();

    GLfloat ice_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ice_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ice_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ice_shininess[] = {127.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ice_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ice_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ice_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, ice_shininess);

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);
        glCallList(ice);
    glPopMatrix();
}


// false se não teve colisão
Collision DarkBramble::check_colision(float camX, float camY, float camZ) {
    double x = distance * cos(translation*RAD);
    double z = -distance * sin(translation*RAD);
    
    float dist = sqrt(((camX-x) * (camX-x)) + ((camZ-z) * (camZ-z)));
    float alpha = acos((camX-x)/dist);
    if(camZ-z > 0) {
        alpha *= (-1);
    } 

    // começando de dois para ignorar as entradas do portal
    for (size_t i = 0; i < 2; i++) {
        float x_ = dist * cos(alpha - (translation+rotation)*RAD);
        float z_ = dist * sin(alpha - (translation+rotation)*RAD + 3.14159);
        if (bspheres[i].contains(Vertex(x_, camY, z_))) {
            // std::cout << "Colisão detectada com objeto " << i << std::endl;
            return Collision::DARK_BRAMBLE_PORTAL;
        }
    }

    for(size_t i = 2; i < sizeof(bboxes) / sizeof(bboxes[0]); i++) {
        float x_ = dist * cos(alpha - (translation+rotation)*RAD);
        float z_ = dist * sin(alpha - (translation+rotation)*RAD + 3.14159);
        // printf("a %f %f %f %f %f %f\n", dist, alpha * 180/3.14159, x_, z_, camX-x, camZ-z);
        if(bspheres[i].contains(Vertex(x_, camY, z_))) {
            std::cout << "Colisão detectada com objeto " << i << std::endl;
            return Collision::DARK_BRAMBLE;
        }
    }
    return Collision::NOT;
}

bool DarkBramble::inside(float camX, float camY, float camZ) {
    double x = distance * cos(translation*RAD);
    double z = distance * sin(translation*RAD);
    for (size_t i = 0; i < 2; i++) {
        // printf("i = %ld\n", i);
        if (bboxes[i].contains(Vertex((camX-x), camY, (camZ+z)))) {
            std::cout << "Colisão detectada com objeto " << i << std::endl;
            return true;
        }
    }
    return false;
}

void DarkBramble::set_bouding_boxes(BoundingBox bb[], int qt_bb) {
    float radius_factor = 0.5;
    for(int i = 0; i < qt_bb; i++) {
        bboxes[i] = bb[i];
        bspheres[i] = BoundingSphere(bboxes[i], radius_factor);
        if(i >= 2) radius_factor = 0.33;
    }
}

std::pair<float, float> DarkBramble::current_position() {
    return std::make_pair(translation, distance);
}

void DarkBramble::update_position(GLfloat t, GLfloat r) {
    translation += t;
    rotation += r;
}