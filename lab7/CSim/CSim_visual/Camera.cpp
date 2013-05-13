#include "Camera.h"

namespace CSim
{

    Camera::Camera()
    {
        c[0] = c[1] = c[2] = 0;
        yaw = pitch = 0;
        up = down = left = right = back = forth = 0;
        dyaw = dpitch = 0;
        flong = blong = rlong = llong = uplong = dlong = 1;
    }

    Camera::Camera(CT x, CT y, CT z, CT ya, CT p)
    {
        c[0] = x;
        c[1] = y;
        c[2] = z;
        yaw = ya;
        pitch = p;
        up = down = left = right = back = forth = 0;
        dyaw = dpitch = 0;
        flong = blong = rlong = llong = uplong = dlong = 1;
    }

    void Camera::translate()
    {
        if(up) c[1] += uplong++; else uplong = 1;
        if(down) c[1] -= dlong++; else dlong = 1;
        if(left) c[0] -= llong++; else llong = 1;
        if(right) c[0] += rlong++; else rlong = 1;
        if(back) c[2] += blong++; else blong = 1;
        if(forth) c[2] -= flong++; else flong = 1;

        if(dyaw > 45 || dyaw < -45) dyaw = 0;
        if(dpitch > 45 || dpitch < -45) dpitch = 0;

        yaw += dyaw;
        if(yaw >= 360.0 || yaw <= -360.0) yaw = 0.0;
        pitch += dpitch;
        if(pitch >= 360.0 || pitch <= -360.0) pitch = 0.0;

        up = down = left = right = back = forth = 0;
        dyaw = dpitch = 0;
    }

}
