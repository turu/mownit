#ifndef CREPBODY_H
#define CREPBODY_H

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

    class CRepBody : public ICBody
    {
        protected:

            /* id of the body (provided upon construction by CSystem object) */
            int _id;

            /* system directory (provided upon construction by CSystem object) */
            string _sysdir;

            /* file object; contains traces of the body's movement (and perhaps other data too)*/
            ifstream _file;

            /* size of a single step in time (delta t) */
            CT _step;

            /* body's mass */
            CT _mass;

            /* body's current position and velocity */
            CVector * _vel;
            CVector * _pos;

        public:

            /* Constructors */
            CRepBody(int, string, CT, CT, const CVector &, const CVector &);
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

             CVector & get_velocity();
             void set_velocity(const CVector &);

             CVector & get_position();
             CVector & get_pposition();
             void set_position(const CVector &);
            /* \Accessors */

            /*
                Reads new position and velocity from the file
            */
             void update();

            /* Deconstructor*/
             ~CRepBody();
            /* \Deconstructor */

    };

}

#endif
