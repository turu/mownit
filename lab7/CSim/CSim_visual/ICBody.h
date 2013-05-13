#ifndef ICBODY_H
#define ICBODY_H

#include <iostream>
#include <string>
#include "CVector.h"

using namespace std;

namespace CSim
{

    /* ICBody interface */
    class ICBody
    {
        public:
             virtual int get_id() = 0;
             virtual void set_id(int) = 0;

             virtual string get_dir() = 0;
             virtual void set_dir(string) = 0;

             virtual CT get_step() = 0;
             virtual void set_step(CT) = 0;

             virtual CT get_mass() = 0;
             virtual void set_mass(CT) = 0;

             virtual CVector & get_velocity() = 0;
             virtual void set_velocity(const CVector &) = 0;

             virtual CVector & get_position() = 0;
             virtual CVector & get_pposition() = 0;
             virtual void set_position(const CVector &) = 0;


             virtual void update() = 0;
    };

}

#endif
