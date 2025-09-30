#include "brittle_hollow.h"
#include <SOIL/SOIL.h>

BrittleHollow::BrittleHollow(
    GLfloat radius, GLfloat distance, GLfloat t0, 
    GLint slices, GLint stacks
) : translation(t0), distance(distance), radius(radius), slices(slices), stacks(stacks)
{
    dh_radius = radius / 3.0f;
    available_pieces = std::vector<size_t>(285);
    std::iota(available_pieces.begin(), available_pieces.end(), 0);
    change_current_piece();
}

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

    
    if(
        (bspheres[available_pieces[current]].center.x < dh_radius && bspheres[available_pieces[current]].center.x > -dh_radius) &&
        (bspheres[available_pieces[current]].center.y < dh_radius && bspheres[available_pieces[current]].center.y > -dh_radius) &&
        (bspheres[available_pieces[current]].center.z < dh_radius && bspheres[available_pieces[current]].center.z > -dh_radius)
    ) {
        available_pieces.erase(available_pieces.begin()+current);
        change_current_piece();
    }

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);

        GLfloat a[] = {1.0, 1.0, 1.0, 1.0};
        for(size_t i = 0; i < available_pieces.size(); i++) {
            glPushMatrix();
            if(i == available_pieces[current] && available_pieces.size() > qt_objects * 0.3) {
                glPushMatrix();
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, a);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, a);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, a);
                    glMaterialfv(GL_FRONT, GL_SHININESS, a);
                    glTranslatef(-tx, -ty, -tz);
                    glCallList(surface[available_pieces[current]]);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, surface_diffuse_color);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, surface_specular_color);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, surface_ambient_color);
                    glMaterialfv(GL_FRONT, GL_SHININESS, surface_shininess);
                glPopMatrix();
            } else {
                glPushMatrix();
                glCallList(surface[available_pieces[i]]);
                glPopMatrix();
            }

            glPopMatrix();
        }

        GLfloat bh_ambient[] = {0.0f, 0.0f, 0.0f};
        GLfloat bh_diffuse[] = {1.0f, 1.0f, 1.0f};
        GLfloat bh_specular[] = {0.1f, 0.1f, 0.1f};
        GLfloat bh_shininess[] = { 120.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, bh_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, bh_diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, bh_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, bh_shininess);
        
        glutSolidSphere(dh_radius, slices, stacks);    
    glPopMatrix();
}


Collision BrittleHollow::check_collision(float camX, float camY, float camZ) {
    float x = distance * cos(translation*RAD);
    float z = -distance * sin(translation*RAD);
    
    float dist = sqrt(((camX-x) * (camX-x)) + ((camZ-z) * (camZ-z)));
    float alpha = acos((camX-x)/dist);
    if(camZ-z > 0) {
        alpha *= (-1);
    } 

    for(size_t i = 0; i < available_pieces.size(); i++) {
        float x_ = dist * cos(alpha - (translation+rotation)*RAD);
        float z_ = dist * sin(alpha - (translation+rotation)*RAD + 3.14159);
        // printf("a %f %f %f %f %f %f\n", dist, alpha * 180/3.14159, x_, z_, camX-x, camZ-z);
        if(bspheres[available_pieces[i]].contains(Vertex(x_, camY, z_))) {
            printf("Dentro do objeto %ld\n", i);
            return Collision::BRITTLE_HOLLOW;
        }
    }

    return Collision::NOT;
}


bool BrittleHollow::inside_dark_hole(GLdouble camX, GLdouble camY, GLdouble camZ) {
    // encontrando a posição do buraco negro
    auto [x, z] = get_black_hole_position();

    return (
        (camX < x+dh_radius && camX > x-dh_radius) && 
        (camY < dh_radius && camY > -dh_radius) && 
        ((-camZ) < z+dh_radius && (-camZ) > z-dh_radius)
    );
}

void BrittleHollow::set_surface(GLuint brittle_hollow_surface[], int qt) {
    for(int i = 0; i < qt; i++) {
        this->surface[i] = brittle_hollow_surface[i];
    }
}

void BrittleHollow::set_surface_bouding_boxes(BoundingBox brittle_hollow_bboxes[], int qt) {
    for(int i = 0; i < qt; i++) {
        this->bspheres[i] = BoundingSphere(brittle_hollow_bboxes[i], 0.4);
        this->piece_distance[i] = sqrt(
            (bspheres[available_pieces[i]].center.x * bspheres[available_pieces[i]].center.x) + 
            (bspheres[available_pieces[i]].center.y * bspheres[available_pieces[i]].center.y) + 
            (bspheres[available_pieces[i]].center.z * bspheres[available_pieces[i]].center.z)
        );
        this->piece_rotation[i] = acos(bspheres[available_pieces[i]].center.x / piece_distance[i]); // já tá em rad
        printf("r %f\n", piece_rotation[i]);
    }
    set_current();
}

void BrittleHollow::queda() {
    tx += current_x / delta;
    ty += current_y / delta;
    tz += current_z / delta;

    bspheres[available_pieces[current]].center.x -= current_x / delta;
    bspheres[available_pieces[current]].center.y -= current_y / delta;
    bspheres[available_pieces[current]].center.z -= current_z / delta;

}

void BrittleHollow::loadTexture(const char* filename) {
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

void BrittleHollow::change_current_piece() {
    current = rand() % available_pieces.size();
    tx = ty = tz = 0.0f;
    set_current();
}

void BrittleHollow::set_current() {
    current_x = bspheres[available_pieces[current]].center.x;
    current_y = bspheres[available_pieces[current]].center.y;
    current_z = bspheres[available_pieces[current]].center.z;
}

void BrittleHollow::update_position(GLfloat t, GLfloat r) {
    translation += t;
    rotation += r;
}

std::pair<double, double> BrittleHollow::get_black_hole_position() {
    return std::make_pair(distance * cos(translation*RAD), distance * sin(translation*RAD));
}