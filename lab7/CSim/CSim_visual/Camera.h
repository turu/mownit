#ifndef CAMERA_H
#define CAMERA_H

#include "CVector.h"

namespace CSim
{

    /*
        Camera class
    */

    class Camera
    {
        public:

            //position
            CVector c;

            //angles
            CT yaw;
            CT pitch;

            //movement indicators
            bool up;
            bool down;
            bool left;
            bool right;
            bool back;
            bool forth;
            CT dyaw;
            CT dpitch;

            int flong;
            int blong;
            int rlong;
            int llong;
            int uplong;
            int dlong;

            Camera();
            Camera(CT , CT, CT, CT, CT);

            void translate();
    };

}

#endif
