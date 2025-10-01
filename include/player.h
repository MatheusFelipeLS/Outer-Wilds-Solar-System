#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#include "sun.h"
#include "timber_hearth.h"
#include "brittle_hollow.h"
#include "giants_deep.h"
#include "dark_bramble.h"
#include "interloper.h"
#include "white_hole.h"
#include "void.h"

#define TO_RADIANS 3.14159/180

struct Motion {
    bool Forward, Backward, Left, Right, Up, Down;
};

class Player {
    public:
        Player(float camX0, float camY0, float camZ0, float pitch0, float yaw0);
        
        void camera(bool map);
        void update_physics(float delta_time, bool map);
        
        Collision check_collision(bool map, float deltaX, float deltaY, float deltaZ);
        void teleport(float x, float y, float z, float delta_min);

        void update_pitch_yall(int dev_x, int dev_y);
        
        void turn_on_off_light() { light_on = !light_on; }
        
        void set_speed(float speed) { this->speed = speed; }
        float get_speed() { return speed; }

        void hole_teleport();
        void set_planets(float* planet_distances, float* planet_rotations, float* planet_radii, int num_planets) {
            this->planet_distances = planet_distances;
            this->planet_rotations = planet_rotations;
            this->planet_radii = planet_radii;
            this->num_planets = num_planets;
        }
        
        void move_forward() { motion.Forward = true; }
        void move_backward() { motion.Backward = true; }
        void move_right() { motion.Right = true; }
        void move_left() { motion.Left = true; }
        void move_up() { motion.Up = true; }
        void move_down() { motion.Down = true; }

        void stop_forward() { motion.Forward = false; }
        void stop_backward() { motion.Backward = false; }
        void stop_right() { motion.Right = false; }
        void stop_left() { motion.Left = false; }
        void stop_up() { motion.Up = false; }
        void stop_down() { motion.Down = false; }

        void set_solar_system(Sun *sun, TimberHearth *timber_hearth, BrittleHollow *brittle_hollow, GiantsDeep *giants_deep, DarkBramble *dark_bramble, Interloper *interloper, WhiteHole *white_hole, Void *v);

        void move_to_solar_system(float rotation, float distance);
        void move_to_portal(int rotation, float distance);

        float camX = 50.0; 
        float camY = 0.0; 
        float camZ = -30.0;
        float pitch = 0.0; 
        float yaw = 0.0;

    private:
        Motion motion = {false,false,false,false,false,false};
        bool light_on = false;
        float speed = 1.0f;
        
        // Física simples
        float velX = 0.0f, velY = 0.0f, velZ = 0.0f;
        float gravity_strength = 2000.0f; // constante base de gravidade (bem mais forte)   
        float linear_drag = 0.10f; // menos arrasto para sentir a atração
        float max_phys_speed = 300.0f; // limite de velocidade mais alto

        // Dados dos planetas (compartilhados com a lua quântica)
        float* planet_distances = nullptr;
        float* planet_rotations = nullptr; // em graus
        float* planet_radii = nullptr;
        int num_planets = 0;

        Sun *sun;
        TimberHearth *timber_hearth;
        BrittleHollow *brittle_hollow;
        GiantsDeep *giants_deep;
        DarkBramble *dark_bramble;
        Interloper *interloper;
        WhiteHole *white_hole;
        Void *void_map;
};

#endif