#include "player.h"

Player::Player(
    float camX0, float camY0, float camZ0,
    float pitch0, float yaw0
) :
camX(camX0), camY(camY0), camZ(camZ0),
pitch(pitch0), yaw(yaw0)
{}


void Player::update_physics(float delta_time, bool map) {
    // sem gravidade no mapa do void
    if (map) return;
    if (!planet_distances || !planet_rotations || !planet_radii || num_planets <= 0) return;

    // acumula aceleração gravitacional das massas próximas
    float accX = 0.0f, accY = 0.0f, accZ = 0.0f;
    // fatoriza força dos planetas pelo quão perto do sol eles orbitam
    float maxPlanetDist = 0.0f;
    for (int i = 0; i < num_planets; ++i) {
        if (planet_distances[i] > maxPlanetDist) maxPlanetDist = planet_distances[i];
    }
    if (maxPlanetDist < 1e-3f) maxPlanetDist = 1.0f;

    for (int i = 0; i < num_planets; ++i) {
        float prot = planet_rotations[i] * TO_RADIANS; // graus -> rad
        float px = planet_distances[i] * cosf(prot);
        float pz = -planet_distances[i] * sinf(prot);
        float py = 0.0f; // plano XZ

        float dx = px - camX;
        float dy = py - camY;
        float dz = pz - camZ;
        float dist2 = dx*dx + dy*dy + dz*dz;
        if (dist2 < 1e-3f) continue;

        float dist = sqrtf(dist2);
        // ativa gravidade quando dentro de uma "esfera de influência" proporcional ao raio
        float influence = planet_radii[i] * 40.0f; // amplia alcance
        if (dist > influence) continue;

        // força newtoniana simplificada com clamp para evitar explosões
        float inv = 1.0f / dist;
        float inv2 = inv * inv;
        // Planetas interiores puxam mais: escala por (maxDist / dist_orbital)^exp
        float nearFactor = powf(maxPlanetDist / std::max(planet_distances[i], 1.0f), 2.5f); // planetas internos muito mais fortes
        float accel = gravity_strength * nearFactor * inv2; // G*M efetivo
        // amortecer quando muito perto da superfície
        float surface = std::max(planet_radii[i], 1.0f);
        if (dist < surface * 1.1f) {
            accel *= (dist / (surface * 1.1f));
        }

        accX += accel * dx * inv;
        accY += accel * dy * inv;
        accZ += accel * dz * inv;
    }

    // Sol no centro (0,0,0) com influência forte
    {
        float px = 0.0f, py = 0.0f, pz = 0.0f;
        float dx = px - camX;
        float dy = py - camY;
        float dz = pz - camZ;
        float dist2 = dx*dx + dy*dy + dz*dz;
        if (dist2 > 1e-3f) {
            float dist = sqrtf(dist2);
            float sun_radius = sun ? sun->get_radius() : 1.0f;
            float influence = sun_radius * 150.0f; // alcance muito maior do sol
            if (dist < influence) {
                float inv = 1.0f / dist;
                float inv2 = inv * inv;
                float accel = gravity_strength * 150.0f * inv2; // sol puxa MUITO MUITO forte
                if (dist < sun_radius * 1.2f) {
                    accel *= (dist / (sun_radius * 1.2f));
                }
                accX += accel * dx * inv;
                accY += accel * dy * inv;
                accZ += accel * dz * inv;
            }
        }
    }

    // integra velocidade
    velX += accX * delta_time;
    velY += accY * delta_time;
    velZ += accZ * delta_time;

    // arrasto linear simples para estabilidade
    float drag = std::max(0.0f, 1.0f - linear_drag * delta_time);
    velX *= drag; velY *= drag; velZ *= drag;

    // clamp de velocidade máxima
    float v2 = velX*velX + velY*velY + velZ*velZ;
    if (v2 > max_phys_speed*max_phys_speed) {
        float v = sqrtf(v2);
        float s = max_phys_speed / v;
        velX *= s; velY *= s; velZ *= s;
    }

    // tenta mover com colisão básica (reutiliza check_collision por eixo)
    float stepX = velX * delta_time;
    float stepY = velY * delta_time;
    float stepZ = velZ * delta_time;

    if (check_collision(map, stepX, 0.0f, 0.0f) == Collision::NOT) {
        camX += stepX;
    } else {
        velX = 0.0f;
    }
    if (check_collision(map, 0.0f, stepY, 0.0f) == Collision::NOT) {
        camY += stepY;
    } else {
        velY = 0.0f;
    }
    if (check_collision(map, 0.0f, 0.0f, stepZ) == Collision::NOT) {
        camZ += stepZ;
    } else {
        velZ = 0.0f;
    }
}


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
        teleport(x, 0.0f, z, brittle_hollow->dh_radius); // acho q o gap (white hole radius) tá muito grande
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