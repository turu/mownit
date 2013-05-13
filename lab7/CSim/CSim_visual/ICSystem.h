#ifndef ICSYSTEM_H
#define ICSYSTEM_H

#include "CVector.h"
#include "ICBody.h"

namespace CSim
{

    /* ICSystem interface */
    class ICSystem
    {
        public:
            virtual void init() = 0;
            virtual int get_size() = 0;
            virtual string get_name() = 0;
            virtual ICBody * get_object(int) = 0;
            virtual void del_object(int) = 0;
            virtual void add_object(const CVector&, const CVector&, CT, CT) = 0;
            virtual void clear() = 0;
            virtual void advance() = 0;
            virtual void play(int) = 0;
    };

}

#endif
