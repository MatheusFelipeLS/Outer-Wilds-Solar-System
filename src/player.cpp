#include "player.h"

Player::Player(
    float camX0, float camY0, float camZ0,
    float pitch0, float yaw0
) :
camX(camX0), camY(camY0), camZ(camZ0),
pitch(pitch0), yaw(yaw0)
{}


// depois adicionar o resto das colisões
Collision Player::check_collision(bool map, float deltaX, float deltaY, float deltaZ) {
    // std::cout << "map " << map << std::endl;
    if(map) {
        Collision type = void_map->check_colision(camX + deltaX, camY + deltaY, camZ + deltaZ);
        return type;  
    } else {
        Collision type = dark_bramble->check_colision(camX + deltaX, camY + deltaY, camZ + deltaZ);
        if(type != Collision::NOT) {
            return type;
        }

        type = brittle_hollow->check_collision(camX + deltaX, camY + deltaY, camZ + deltaZ);
        return type;
    }
}


void Player::camera(bool map) {
    float deltaX, deltaY, deltaZ;
    Collision collision_checker;

    if(motion.Forward) {
        deltaX = cos((yaw+90)*TO_RADIANS) * speed;
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90)*TO_RADIANS) * speed;
        collision_checker = check_collision(map, deltaX, deltaY, deltaZ);
        if(collision_checker == Collision::NOT || collision_checker == Collision::DARK_BRAMBLE_PORTAL) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Backward) {
        deltaX = cos((yaw+90+180)*TO_RADIANS) * speed;
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90+180)*TO_RADIANS) * speed;
        collision_checker = check_collision(map, deltaX, deltaY, deltaZ);
        if(collision_checker == Collision::NOT || collision_checker == Collision::DARK_BRAMBLE_PORTAL) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Left) {
        deltaX = cos((yaw+90+90)*TO_RADIANS) * speed;
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90+90)*TO_RADIANS) * speed;
        collision_checker = check_collision(map, deltaX, deltaY, deltaZ);
        if(collision_checker == Collision::NOT || collision_checker == Collision::DARK_BRAMBLE_PORTAL) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Right) {
        deltaX = cos((yaw+90-90)*TO_RADIANS) * speed;
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90-90)*TO_RADIANS) * speed;
        collision_checker = check_collision(map, deltaX, deltaY, deltaZ);
        if(collision_checker == Collision::NOT || collision_checker == Collision::DARK_BRAMBLE_PORTAL) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Up) {
        collision_checker = check_collision(map, 0.0f, speed, 0.0f);
        if(collision_checker == Collision::NOT || collision_checker == Collision::DARK_BRAMBLE_PORTAL) 
            camY += speed;
    }
    if(motion.Down) {
        collision_checker = check_collision(map, 0.0f, -speed, 0.0f);
        if(collision_checker == Collision::NOT || collision_checker == Collision::DARK_BRAMBLE_PORTAL)
            camY -= speed;
    }

    // printf("tipo de colisão: %d\n", collision_checker);
    // printf("x, y, z, pitch, yaw: %f %f %f %f %f\n", camX, camY, camZ, pitch, yaw);
    glRotatef(-pitch,1.0,0.0,0.0); // Along X axis
    glRotatef(-yaw,0.0,1.0,0.0);    //Along Y axis

    glTranslatef(-camX,-camY,-camZ);

    if (light_on) {
        GLfloat light_pos[] = {-camX,-camY,-camZ, 0.0};
        glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
        GLfloat light_color[] = {1.0f,1.0f,1.0f, 1.0};
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_color);
        glLightf (GL_LIGHT2 , GL_CONSTANT_ATTENUATION , 0.5);
        glLightf (GL_LIGHT2 , GL_LINEAR_ATTENUATION , 0.5);
        glLightf (GL_LIGHT2 , GL_QUADRATIC_ATTENUATION , 0.2);
    }
}

void Player::teleport(float x, float y, float z, float delta_min) {
    //teleportando para o buraco branco
    int delta = (rand() % ((int) (2 * delta_min)));
    camX = x + delta_min + delta;

    delta = (rand() % ((int) (2 * delta_min)));
    camY = y + delta_min + delta;

    delta = (rand() % ((int) (2 * delta_min)));
    camZ = -(z + delta_min + delta);
}

void Player::update_pitch_yall(int dev_x, int dev_y) {
    yaw += (float)dev_x/10.0;
    pitch += (float)dev_y/10.0;
}

void Player::hole_teleport() {
    if(brittle_hollow->inside_dark_hole(camX, camY, camZ)) {
        //teleportando para o buraco branco
        Vertex p = white_hole->get_position();
        teleport(p.x, p.y, p.z, white_hole->radius);
    } else if(white_hole->inside(camX, camY, camZ)) {
        auto [x, z] = brittle_hollow->get_black_hole_position();
        teleport(x, 0.0f, z, brittle_hollow->dh_radius/1.5); // acho q o gap (white hole radius) tá muito grande
    }
}

void Player::set_solar_system(
    Sun *sun,
    ThimberHearth *thimber_hearth,
    BrittleHollow *brittle_hollow,
    GiantsDeep *giants_deep,
    DarkBramble *dark_bramble,
    Interloper *interloper,
    WhiteHole *white_hole,
    Void *v
) {
    this->sun = sun;
    this->thimber_hearth = thimber_hearth;
    this->brittle_hollow = brittle_hollow;
    this->giants_deep = giants_deep;
    this->dark_bramble = dark_bramble;
    this->interloper = interloper;
    this->white_hole = white_hole;
    this->void_map = v;
}

void Player::move_to_solar_system(float rotation, float distance) {
    float x = distance * cos(rotation*RAD);
    float z = -distance * sin(rotation*RAD);
    
    camX = x;
    camY = -150;
    camZ = z;
}

void Player::move_to_portal(int rotation, float distance) {
    float x = distance * cos(rotation*RAD);
    float z = -distance * sin(rotation*RAD);

    camX = x;
    camY = 50.0f;
    camZ = z;
}