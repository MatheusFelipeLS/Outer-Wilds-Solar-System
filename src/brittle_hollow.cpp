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
        current = available_pieces.size();
        int r = (int) wh->radius;
        tx = ty = tz = 0.0f;
        // max = 3.5 * diametro do wh
        max_x = r + (float) (rand() % (6 * r)); 
        if(rand() % 2 == 1) {
            max_x *= (-1);
        }
        max_y = r + (float) (rand() % (6 * r));
        if(rand() % 2 == 1) {
            max_y *= (-1);
        }
        max_z = r + (float) (rand() % (6 * r));
        if(rand() % 2 == 1) {
            max_z *= (-1);
        }
        piece_position.push_back(Vertex(wh->p.x + max_x, wh->p.y + max_y, wh->p.z + max_z));
    }

    glPushMatrix(); 
        glRotatef (translation, 0.0, 1.0, 0.0);
        glTranslatef (distance, 0.0, 0.0);
        glRotatef (rotation, 0.0, 1.0, 0.0);

        GLfloat a[] = {1.0, 1.0, 1.0, 1.0};
        for(size_t i = 0; i < available_pieces.size(); i++) {
            glPushMatrix();
            if(current < available_pieces.size() && i == available_pieces[current]) {
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

    glMaterialfv(GL_FRONT, GL_DIFFUSE, surface_diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, surface_specular_color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, surface_ambient_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, surface_shininess);
    for(int i = 0; i < (int) teleported_pieces.size()-1; i++) {
        glPushMatrix();
            glTranslatef(piece_position[i].x, piece_position[i].y, piece_position[i].z);
            glCallList(surface[teleported_pieces[i]]);
        glPopMatrix();
    }
    // printf("%f %f %f %f %f %f\n", tx, max_x, ty, max_y, tz, max_z);
    if(teleported_pieces.size() > 0 && current == available_pieces.size()) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, a);
        glMaterialfv(GL_FRONT, GL_SPECULAR, a);
        glMaterialfv(GL_FRONT, GL_AMBIENT, a);
        glMaterialfv(GL_FRONT, GL_SHININESS, a);
        glPushMatrix();
            glTranslatef(wh->p.x + tx, wh->p.y + ty, wh->p.z + tz);
            glCallList(surface[teleported_pieces.back()]);
        glPopMatrix();
    }
}

void BrittleHollow::set_white_hole_position(Vertex p) {
    wh_position = p;
}

Collision BrittleHollow::check_collision(float camX, float camY, float camZ) {
    float x = distance * cos(translation*RAD);
    float z = -distance * sin(translation*RAD);
    
    float dist = sqrt(((camX-x) * (camX-x)) + ((camZ-z) * (camZ-z)));
    float alpha = acos((camX-x)/dist);
    if(camZ-z > 0) {
        alpha *= (-1);
    } 

    float x_ = dist * cos(alpha - (translation+rotation)*RAD);
    float z_ = dist * sin(alpha - (translation+rotation)*RAD + 3.14159);
    for(size_t i = 0; i < available_pieces.size(); i++) {
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
    }
    set_current();
}

void BrittleHollow::queda() {
    if(available_pieces.size() < qt_objects * 0.3) {
        return;
    }

    if(
        piece_position.size() > 0 && 
        (std::abs(tx) <= std::abs(max_x) - 0.5 || std::abs(tx) >= std::abs(max_x) + 0.5) 
        && current == available_pieces.size()
    ) {
        // printf("ainda está caindo %f\n", tx);
        tx += (max_x - wh->radius) / delta;
        ty += (max_y - wh->radius) / delta;
        tz += (max_z - wh->radius) / delta;
        return;
        // getchar();
    } else if(current == available_pieces.size() && piece_position.size() > 0) {
        change_current_piece();
        return;
    }

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
    teleported_pieces.push_back(available_pieces[current]);
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