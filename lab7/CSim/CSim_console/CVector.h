#ifndef VECTOR_H
#define VECTOR_H

#include <iterator>

namespace CSim
{

    typedef long double CT;

    class CVector
    {
        public:

            /* vals table */
            CT * _v;

            /* dimension */
            int _dim;

            /* Constructors */
            CVector();
            CVector(int);
            CVector(int, CT);
            CVector(const CVector & cv);
            template <class InputIterator>
                CVector(InputIterator, InputIterator, int);
            /* \Constructors */

            CVector & operator=(const CVector&);
            CT& operator[](int);

            /* arithmetic operators */
            bool operator==(const CVector&);
            CVector operator+(const CVector&);
            void operator+=(const CVector&);
            CVector operator-(const CVector&);
            void operator-=(const CVector&);
            CT operator*(const CVector&);
            CVector operator*(const CT&);
            void operator*=(const CT&);
            CVector cross(const CVector&);
            /* \arithmetic operators */

            CT magnitude();

            /* Destructor */
            ~CVector();
            /* \Destructor */
    };

}

#endif

