#ifndef CBODY_H
#define CBODY_H

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include "ICSystem.h"
#include "CVector.h"

using namespace std;

namespace CSim
{

    /*
        CBody is a class representing a particle in a dynamic system.
        It has properties such as mass, position and velocity.
        It traces it's own movement and saves it to a file.
        CBody updates its position and velocity by solving Newtonian's second law
        ordinary difference equation of motion using Velocity Verlet method.
        Velocity Verlet is used because it's a sympletic integrator, which means that
        it conserves (slightly changed) energy of a system (which is vital in orbital dynamics)
        Local position error is O(step^4), local velocity error is O(step^2),
        while global error is O(step^2) for both variables.

        NOTE: current implementation has time complexity of O(n^2) where 'n' is a number of particles that have some mass.
        It is more efficient for computing test (zero-mass) particles.
    */

    class CBody : public ICBody
    {
        protected:

            /* pointer to a CSystem mother-object */
            ICSystem * _system;

            /* id of the body (provided upon construction by CSystem object) */
            int _id;

            /* system directory (provided upon construction by CSystem object) */
            string _sysdir;

            /* file object; contains traces of the body's movement (and perhaps other data too)*/
            ofstream _file;

            /* size of a single step in time (delta t) */
            CT _step;

            /* body's mass */
            CT _mass;

            /* Gravitational constant (provided upon construction by CSystem object) */
            CT _G;

            /* potential */
            CT _U;

            /* body's current position and velocity */
            CVector * _vel;
            CVector * _pos;

            /* body's previous position and velocity; used by other bodies while updating themselves */
            CVector * _pvel;
            CVector * _ppos;

            /* acceleration after last step */
            CVector * _acc;

        public:

            /* Constructors */
            CBody(ICSystem *, int, string, CT, CT, CT, const CVector &, const CVector &);
            /* \Constructors */

            /* Accessors */
             int get_id();
             void set_id(int);

             string get_dir();
             void set_dir(string);

             CT get_step();
             void set_step(CT);

             CT get_mass();
             void set_mass(CT);

             CVector & get_acc();
             CT get_potential();
             CVector get_momentum();

             CVector & get_velocity();
             CVector & get_pvelocity();
             void set_velocity(const CVector &);

             CVector & get_position();
             CVector & get_pposition();
             void set_position(const CVector &);
            /* \Accessors */

            void update_acc(bool);
            void update_vel();

            /*
                Updates position and velocity of a body
            */
             void update();

            /*
                Writes current position and velocity to the file
            */
             void write();

            /*
                Discards old velocity and position
            */
             void discard_old();

            /* Deconstructor*/
             ~CBody();
            /* \Deconstructor */

    };

}

#endif

