#include "player.h"

Player::Player(
    float camX0, float camY0, float camZ0,
    float pitch0, float yaw0
) :
camX(camX0), camY(camY0), camZ(camZ0),
pitch(pitch0), yaw(yaw0)
{}


// depois adicionar o resto das colisões
bool Player::check_collision(float deltaX, float deltaY, float deltaZ) {
    return dark_bramble->check_colision(camX + deltaX, camY + deltaY, camZ + deltaZ);
}


void Player::camera() {
    float deltaX, deltaY, deltaZ;
    if(motion.Forward) {
        deltaX = cos((yaw+90)*TO_RADIANS);
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90)*TO_RADIANS);
        if(!check_collision(deltaX, deltaY, deltaZ)) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Backward) {
        deltaX = cos((yaw+90+180)*TO_RADIANS);
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90+180)*TO_RADIANS);
        if(!check_collision(deltaX, deltaY, deltaZ)) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Left) {
        deltaX = cos((yaw+90+90)*TO_RADIANS);
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90+90)*TO_RADIANS);
        if(!check_collision(deltaX, deltaY, deltaZ)) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Right) {
        deltaX = cos((yaw+90-90)*TO_RADIANS);
        deltaY = 0.0f;
        deltaZ = -sin((yaw+90-90)*TO_RADIANS);
        if(!check_collision(deltaX, deltaY, deltaZ)) {
            camX += deltaX;
            camZ += deltaZ;
        }
    }
    if(motion.Up) {
        if(!check_collision(0.0f, 1.0f, 0.0f)) 
            camY += 1.0;
    }
    if(motion.Down) {
        if(!check_collision(0.0f, -1.0f, 0.0f))
            camY -= 1.0;
    }

    glRotatef(-pitch,1.0,0.0,0.0); // Along X axis
    glRotatef(-yaw,0.0,1.0,0.0);    //Along Y axis

    glTranslatef(-camX,-camY,-camZ);

    if (light_on) {
        GLfloat light_pos[] = {-camX,-camY,-camZ, 0.0};
        glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
        GLfloat light_color[] = {1.0f,1.0f,1.0f, 1.0};
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_color);
        glLightf (GL_LIGHT1 , GL_CONSTANT_ATTENUATION , 0.5);
        glLightf (GL_LIGHT1 , GL_LINEAR_ATTENUATION , 0.5);
        glLightf (GL_LIGHT1 , GL_QUADRATIC_ATTENUATION , 0.2);
    }
}

void Player::teleport(float x, float y, float z, float delta_min) {
    //teleportando para o buraco branco
    int delta = (rand() % ((int) (2 * delta_min)));
    camX = x + delta_min + delta;

    delta = (rand() % ((int) (2 * delta_min)));
    camY = y + delta_min + delta;

    delta = (rand() % ((int) (2 * delta_min)));
    camZ = z + delta_min + delta;
}

void Player::update_pitch_yall(int dev_x, int dev_y) {
    yaw += (float)dev_x/10.0;
    pitch += (float)dev_y/10.0;
}