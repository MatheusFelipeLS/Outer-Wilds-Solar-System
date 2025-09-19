#ifndef PLAYER
#define PLAYER

#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define TO_RADIANS 3.14159/180

struct Motion {
    bool Forward, Backward, Left, Right;
};

class Player {
    public:
        Player(
            float camX0, float camY0, float camZ0,
            float pitch0, float yaw0
        ) :
        camX(camX0), camY(camY0), camZ(camZ0),
        pitch(pitch0), yaw(yaw0)
        {}
        
        void camera() {
            if(motion.Forward) {
                camX += cos((yaw+90)*TO_RADIANS); // /5.0;
                camZ -= sin((yaw+90)*TO_RADIANS); // /5.0;
            }
            if(motion.Backward) {
                camX += cos((yaw+90+180)*TO_RADIANS); // /5.0;
                camZ -= sin((yaw+90+180)*TO_RADIANS); // /5.0;
            }
            if(motion.Left) {
                camX += cos((yaw+90+90)*TO_RADIANS); // /5.0;
                camZ -= sin((yaw+90+90)*TO_RADIANS); // /5.0;
            }
            if(motion.Right) {
                camX += cos((yaw+90-90)*TO_RADIANS); // /5.0;
                camZ -= sin((yaw+90-90)*TO_RADIANS); // /5.0;
            }

            /*limit the values of pitch
            between -60 and 70
            */
            // if(pitch>=70)
            //     pitch = 70;
            // if(pitch<=-60)
            //     pitch=-60;

            glRotatef(-pitch,1.0,0.0,0.0); // Along X axis
            glRotatef(-yaw,0.0,1.0,0.0);    //Along Y axis

            glTranslatef(-camX,-camY,-camZ);
        }

        void teleport(float x, float y, float z, float delta_min) {
            //teleportando para o buraco branco
            int delta = (rand() % ((int) (2 * delta_min)));
            camX = x + delta_min + delta;

            delta = (rand() % ((int) (2 * delta_min)));
            camY = y + delta_min + delta;

            delta = (rand() % ((int) (2 * delta_min)));
            camZ = z + delta_min + delta;
        }

        void update_pitch_yall(int dev_x, int dev_y) {
            yaw += (float)dev_x/10.0;
            pitch += (float)dev_y/10.0;
        }

        void move_forward() {
            motion.Forward = true;
        }

        void move_backward() {
            motion.Backward = true;
        }

        void move_right() {
            motion.Right = true;
        }

        void move_left() {
            motion.Left = true;
        }

        void stop_forward() {
            motion.Forward = false;
        }

        void stop_backward() {
            motion.Backward = false;
        }

        void stop_right() {
            motion.Right = false;
        }

        void stop_left() {
            motion.Left = false;
        }

        float camX = 50.0; 
        float camY = 0.0; 
        float camZ = -30.0;

    private:
        float pitch = 0.0; 
        float yaw = 0.0;
        Motion motion = {false,false,false,false};
};

#endif