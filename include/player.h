#ifndef PLAYER_H
#define PLAYER_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define TO_RADIANS 3.14159/180

struct Motion {
    bool Forward, Backward, Left, Right;
};

class Player {
    public:
        Player(float camX0, float camY0, float camZ0, float pitch0, float yaw0);
        
        void camera();

        void teleport(float x, float y, float z, float delta_min);

        void update_pitch_yall(int dev_x, int dev_y);

        void move_forward() { motion.Forward = true; }
        void move_backward() { motion.Backward = true; }
        void move_right() { motion.Right = true; }
        void move_left() { motion.Left = true; }

        void stop_forward() { motion.Forward = false; }
        void stop_backward() { motion.Backward = false; }
        void stop_right() { motion.Right = false; }
        void stop_left() { motion.Left = false; }

        float camX = 50.0; 
        float camY = 0.0; 
        float camZ = -30.0;

    private:
        float pitch = 0.0; 
        float yaw = 0.0;
        Motion motion = {false,false,false,false};
};

#endif