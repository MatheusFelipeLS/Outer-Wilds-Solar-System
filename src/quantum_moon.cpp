#include "quantum_moon.h"
#include <SOIL/SOIL.h>

void QuantumMoon::draw() {
    if (!is_visible) return;

    glPushMatrix();
    glTranslatef(current_x, current_y, current_z);
    glRotatef(orbital_angle * 2.0f, 0.0f, 1.0f, 0.0f);

    GLfloat emission[] = {0.2f, 0.2f, 0.8f, 1.0f};
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glColor4f(
        0.9f + 0.1f * sinf(color_shift),
        0.9f + 0.1f * cosf(color_shift),
        1.0f,
        0.9f
    );

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, radius, slices, stacks);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.5f, 0.5f, 1.0f, 0.3f * glow_intensity);
    gluSphere(quad, radius * 1.1f, slices, stacks);
    glDisable(GL_BLEND);

    GLfloat no_emission[] = {0,0,0,0};
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void QuantumMoon::update_position(GLfloat delta_time) {
    if (!is_visible) return;

    // Atualiza ângulo orbital (em graus)
    orbital_angle += 18.0f * delta_time; // suaviza para sincronizar melhor com órbitas
    if (orbital_angle >= 360.0f) orbital_angle -= 360.0f;

    // Calcula base do planeta atual
    if (planet_distances && planet_rotations && num_planets > 0) {
        float prot = planet_rotations[current_planet] * PI_F/180.0f;
        float planet_x = planet_distances[current_planet] * cosf(prot);
        float planet_z = -planet_distances[current_planet] * sinf(prot);

        // Distância orbital relativa ao planeta (não fixa)
        orbital_distance = std::max(30.0f, planet_distances[current_planet] * 0.15f);

        float angRad = orbital_angle * PI_F/180.0f;
        current_x = planet_x + orbital_distance * cosf(angRad);
        current_y = 0.0f;
        current_z = planet_z - orbital_distance * sinf(angRad);
    } else {
        // fallback
        float angRad = orbital_angle * PI_F/180.0f;
        current_x = orbital_distance * cosf(angRad);
        current_y = 0.0f;
        current_z = -orbital_distance * sinf(angRad);
    }

    // efeitos visuais baseados em tempo acumulado para ficarem suaves
    time_acc += delta_time;
    color_shift = time_acc * 0.8f;
    glow_intensity = 0.7f + 0.3f * sinf(time_acc * 2.0f);
}

bool QuantumMoon::is_being_observed(float camX, float camY, float camZ, float pitch, float yaw) {
    // vetor câmera -> lua
    float dx = current_x - camX;
    float dy = current_y - camY;
    float dz = current_z - camZ;
    float distance = sqrtf(dx*dx + dy*dy + dz*dz);
    if (distance > 2000.0f) return false;

    // ângulos da câmera para a lua (yaw: horizontal, pitch: vertical) - assume pitch,yaw em graus
    float view_angle_x = atan2f(dx, -dz) * 180.0f / PI_F;
    float view_angle_y = atan2f(dy, sqrtf(dx*dx + dz*dz)) * 180.0f / PI_F;

    while (view_angle_x < -180.0f) view_angle_x += 360.0f;
    while (view_angle_x > 180.0f) view_angle_x -= 360.0f;

    float angle_diff_x = fabsf(view_angle_x - yaw);
    float angle_diff_y = fabsf(view_angle_y - pitch);
    if (angle_diff_x > 180.0f) angle_diff_x = 360.0f - angle_diff_x;

    bool observed = (angle_diff_x < 30.0f && angle_diff_y < 30.0f);
    return observed;
}

void QuantumMoon::quantum_jump() {
    if (num_planets <= 1) return;

    // seleciona planeta diferente
    int new_planet;
    do {
        new_planet = rand() % num_planets;
    } while (new_planet == current_planet && num_planets > 1);

    current_planet = new_planet;
    // manter orbital_angle em GRAUS
    orbital_angle = static_cast<float>(rand() % 360);
    // ajusta distância logo após salto
    if (planet_distances) {
        orbital_distance = std::max(30.0f, planet_distances[current_planet] * 0.15f);
    } else {
        orbital_distance = 60.0f;
    }
}

void QuantumMoon::update_observation(float camX, float camY, float camZ, float pitch, float yaw, float current_time) {
    bool observed = is_being_observed(camX, camY, camZ, pitch, yaw);
    if (observed) {
        last_observation_time = current_time;
        // se observado, garante que a lua esteja visível (ela fica "colapsada" quando não observada)
        is_visible = true;
        return;
    }

    // regras de salto: cooldown por não observação OU tempo máximo em um planeta
    bool cooldown_passed = (current_time - last_observation_time > quantum_cooldown);
    bool dwell_exceeded = (current_time - last_jump_time > dwell_max_seconds);
    if (cooldown_passed || dwell_exceeded) {
        if (planet_distances && planet_rotations && num_planets > 0) {
            // posição do planeta atual
            float prot = planet_rotations[current_planet] * PI_F/180.0f;
            float planet_x = planet_distances[current_planet] * cosf(prot);
            float planet_z = planet_distances[current_planet] * sinf(prot);
            float planet_y = 0.0f;

            // calcula se jogador está olhando para o planeta (ângulo mais apertado)
            float pdx = planet_x - camX;
            float pdy = planet_y - camY;
            float pdz = planet_z - camZ;
            float pdist = sqrtf(pdx*pdx + pdy*pdy + pdz*pdz);
            if (pdist < 0.1f) return;

            float view_angle_x = atan2f(pdx, -pdz) * 180.0f / PI_F;
            float view_angle_y = atan2f(pdy, sqrtf(pdx*pdx + pdz*pdz)) * 180.0f / PI_F;
            while (view_angle_x < -180.0f) view_angle_x += 360.0f;
            while (view_angle_x > 180.0f) view_angle_x -= 360.0f;

            float angle_diff_x = fabsf(view_angle_x - yaw);
            float angle_diff_y = fabsf(view_angle_y - pitch);
            if (angle_diff_x > 180.0f) angle_diff_x = 360.0f - angle_diff_x;

            // se o jogador está olhando para o planeta com tolerância menor, faz jump
            if ((angle_diff_x < 15.0f && angle_diff_y < 15.0f) || dwell_exceeded) {
                quantum_jump();
                last_observation_time = current_time;
                last_jump_time = current_time;
                // opcional: podemos momentaneamente tornar invisível até a próxima observação
                is_visible = false;
            }
        } else {
            // se sem planetas, pode saltar aleatoriamente
            quantum_jump();
            last_observation_time = current_time;
            last_jump_time = current_time;
        }
    }
}

bool QuantumMoon::is_player_near(float camX, float camY, float camZ, float threshold) {
    float dx = current_x - camX;
    float dy = current_y - camY;
    float dz = current_z - camZ;
    float distance = sqrtf(dx*dx + dy*dy + dz*dz);
    return distance < threshold;
}

void QuantumMoon::loadTexture(const char* filename) {
    textureID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );

    if(textureID == 0) {
        std::cout << "Erro carregando textura: " << SOIL_last_result() << std::endl;
    } else {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
}
