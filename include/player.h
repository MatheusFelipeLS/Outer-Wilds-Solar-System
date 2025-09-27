#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#include "sun.h"
#include "thimber_hearth.h"
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
        
        Collision check_collision(bool map, float deltaX, float deltaY, float deltaZ);
        void teleport(float x, float y, float z, float delta_min);

        void update_pitch_yall(int dev_x, int dev_y);
        
        void turn_on_off_light() { light_on = !light_on; }

        void hole_teleport();
        
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

        // void set_position(float x, float y, float z);
        void set_solar_system(
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

        void move_to_solar_system(float rotation, float distance) {
            float x = distance * cos(rotation*RAD);
            float z = -distance * sin(rotation*RAD);
            
            camX = x;
            camY = -150;
            camZ = z;
        }

        void move_to_portal(int rotation, float distance) {
            float x = distance * cos(rotation*RAD);
            float z = -distance * sin(rotation*RAD);

            camX = x;
            camY = 50.0f;
            camZ = z;
        }

        float camX = 50.0; 
        float camY = 0.0; 
        float camZ = -30.0;
        float pitch = 0.0; 
        float yaw = 0.0;

    private:
        Motion motion = {false,false,false,false,false,false};
        bool light_on = false;

        Sun *sun;
        ThimberHearth *thimber_hearth;
        BrittleHollow *brittle_hollow;
        GiantsDeep *giants_deep;
        DarkBramble *dark_bramble;
        Interloper *interloper;
        WhiteHole *white_hole;
        Void *void_map;
};

#endif