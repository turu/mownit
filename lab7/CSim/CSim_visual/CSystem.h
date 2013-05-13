#ifndef CSYSTEM_H
#define CSYSTEM_H

#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include "CVector.h"
#include "ICSystem.h"
#include "CBody.h"

using namespace std;

namespace CSim
{

    /*
        CSystem is a class representing a dynamic system of particles (bodies).
        It keeps a set of bodies in a data structure and updates the state of
        a system with every iteration.
    */

    class CSystem : public ICSystem
    {
        protected:

            /* path to the simulation's folder */
            string _path;

            /* system's name (and a name of the folder) */
            string _name;

            /* final path of the simulation*/
            string _dir;

            ofstream _file;

            /* object counter */
            int _size;

            /* zero-mass object counter */
            int _zero_size;

            /* global step variable. Can vary from object to object if variable step method is used */
            CT _step;

            /* Gravitational constant G */
            CT _G;

            /* energy */
            CT _E;
            CT _Ek;
            CT _Ep;

            /* momentum */
            CVector _Am;

            bool _first_step;

            /* Collection structure */
            deque<CBody*> _collection;

            /* Adjusts individual step size of an object. NOTE that under no circumstance can an object's step exceed system's step */
            void adjust_step(CBody*);

            void update_properties();
            void write_properties();

        public:

            /* Constructors */
            CSystem(string, string, CT, CT);
            /* \Constructors */

            /* Initializes the system */
            void init();

            /* reads data from file; returns number of added objects */
            int read_from_file(string);

            /* adds new body to the system*/
            void add_object(const CVector&, const CVector&, CT, CT);

            /* deletes ith object */
            void del_object(int);

            /* destroys all data; clears the system */
            void clear();

            /* advances the system by one time-step */
            void advance();

            /* advances the system by a given number of iterations */
            void play(int);

            /* Accessors */
            int get_size();
            string get_name();
            /* \Accessors */

            /* returns pointer ith object from the collection*/
            ICBody * get_object(int);

            /* Deconstructor*/
             ~CSystem();
            /* \Deconstructor */
    };

}

#endif

