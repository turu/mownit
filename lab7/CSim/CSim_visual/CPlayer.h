#ifndef CPLAYER_H
#define CPLAYER_H

#include <iostream>
#include <deque>
#include "CVector.h"
#include "ICSystem.h"
#include "CRepBody.h"

using namespace std;

namespace CSim
{

    /*
        CPlayer is a class used for 'playing' previously completed simulation.
        It's behaviour is very similiar to CSystem, the difference being that
        instead of computing subsequent positions and velocities of objects,
        it reads them from files. CPlayer inherits from ICSystem interface.
    */

    class CPlayer : public ICSystem
    {

        protected:

            /* path to the simulation's folder */
            string _path;

            /* simulation's name */
            string _name;

            /* object counter */
            int _size;
            int _zero_size;

            /* Collection structure */
            deque<CRepBody*> _collection;

        public:

            /* Constructors */
            CPlayer(string, string);
            /* \Constructors */

            /* reads config file */
            void init();

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
             ~CPlayer();
            /* \Deconstructor */
    };

}

#endif
