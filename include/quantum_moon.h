#ifndef QUANTUM_MOON_H
#define QUANTUM_MOON_H

#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "utils.h"

#define QUANTUM_MOON_COLOR 0.8f, 0.8f, 1.0f, 0.9f
static constexpr float PI_F = 3.14159265f;

class QuantumMoon {
    public:
        QuantumMoon(GLfloat radius, GLint slices, GLint stacks) :
            radius(radius), slices(slices), stacks(stacks)
        {
            current_planet = 0;
            orbital_angle = 0.0f; // em GRAUS
            orbital_distance = 60.0f;
            is_visible = true;
            last_observation_time = 0.0f;
            quantum_cooldown = 0.8f; // mais saltos
            last_jump_time = 0.0f;
            dwell_max_seconds = 20.0f; 
            current_x = orbital_distance;
            current_y = 0.0f;
            current_z = 0.0f;
            glow_intensity = 1.0f;
            color_shift = 0.0f;
            planet_distances = nullptr;
            planet_rotations = nullptr;
            num_planets = 0;
            time_acc = 0.0f;
            srand((unsigned)time(nullptr));
            d = false;
        }

        void draw();
        void update_position(GLfloat delta_time);

        // Observação: chame isso todo frame com tempo atual (segundos)
        void update_observation(float camX, float camY, float camZ, float pitch, float yaw, float current_time);

        bool is_being_observed(float camX, float camY, float camZ, float pitch, float yaw);
        void quantum_jump();
        void set_planets(float* planet_distances, float* planet_rotations, int num_planets) {
            this->planet_distances = planet_distances;
            this->planet_rotations = planet_rotations;
            this->num_planets = num_planets;
        }
        void get_position(float& x, float& y, float& z) {
            x = current_x; y = current_y; z = current_z;
        }
        bool is_player_near(float camX, float camY, float camZ, float threshold = 50.0f);
        void debug() { d = true; }

    private:
        GLfloat radius;
        GLint slices, stacks;

        // posição atual
        float current_x, current_y, current_z;

        // órbita
        int current_planet;
        float orbital_angle; // **graus**
        float orbital_distance;

        // planetas
        float* planet_distances;
        float* planet_rotations;
        int num_planets;

        // observação quântica
        bool is_visible;
        float last_observation_time;
        float quantum_cooldown;
        float last_jump_time;
        float dwell_max_seconds;

        // efeitos visuais
        float glow_intensity;
        float color_shift;
        float time_acc;

        bool d;
};

#endif
