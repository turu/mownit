#include <iostream>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "CVector.h"

using namespace std;

namespace CSim
{

    CVector::CVector()
    {
        _v = (CT*) malloc(3*sizeof(CT));
        //_v = new CT[3];
        _dim = 3;
    }

    CVector::CVector(int n)
    {
        //_v = new CT[n];
        //cout<<n<<endl;
        _v = (CT*) malloc(n*sizeof(CT));
        _dim = n;
    }

    CVector::CVector(int n, CT val)
    {
        //_v = new CT[n];
        _dim = n;
        if(val != 0)
        {
            _v = (CT*) malloc(n*sizeof(CT));
            for(int i = 0; i < n; i++) _v[i] = val;
        }
        else
        {
            _v = (CT*) calloc(n, sizeof(CT));
        }
    }

    template<class InputIterator>
    CVector::CVector(InputIterator first, InputIterator last, int n)
    {
        //_v = new CT[n];
        _v = (CT*) malloc(n*sizeof(CT));
        _dim = n;
        int i = 0;
        while(first != last)
        {
            _v[i++] = *first;
            first++;
        }
    }

    CVector::CVector(const CVector & x)
    {
        //_v = new CT[x._dim];
        _v = (CT*) malloc(x._dim*sizeof(CT));
        _dim = x._dim;
        //for(int i = 0; i < _dim; i++) _v[i] = x._v[i];
        memcpy(_v, x._v, _dim*sizeof(CT));
    }

    CVector & CVector::operator=(const CVector & x)
    {
        if(_dim != x._dim) _v = (CT*) realloc(_v, x._dim * sizeof(CT));
        _dim = x._dim;

        //for(int i = 0; i < _dim; i++) _v[i] = x._v[i];
        memcpy(_v, x._v, _dim*sizeof(CT));

        return *this;
    }

    CT & CVector::operator[](int i)
    {
        return _v[i];
    }

    bool CVector::operator==(const CVector & rhs)
    {
        if(_dim != rhs._dim) return false;

        for(int i = 0; i < _dim; i++)
            if(_v[i] != rhs._v[i]) return false;

        return true;
    }

    CVector CVector::operator+(const CVector & rhs)
    {
        CVector res(_dim);

        for(int i = 0; i < _dim; i++)
        {
            res._v[i] = _v[i] + rhs._v[i];
        }

        return res;
    }

    void CVector::operator+=(const CVector & rhs)
    {
        for(int i = 0; i < _dim; i++)
        {
            _v[i] += rhs._v[i];
        }
    }

    CVector CVector::operator-(const CVector & rhs)
    {
        CVector res(_dim);

        for(int i = 0; i < _dim; i++)
        {
            res._v[i] = _v[i] - rhs._v[i];
        }

        return res;
    }

    void CVector::operator-=(const CVector & rhs)
    {
        for(int i = 0; i < _dim; i++)
        {
            _v[i] -= rhs._v[i];
        }
    }

    CVector CVector::operator*(const CT & s)
    {
        CVector res(_dim);

        for(int i = 0; i < _dim; i++)
        {
            res._v[i] = _v[i] * s;
        }

        return res;
    }

    void CVector::operator*=(const CT & s)
    {
        for(int i = 0; i < _dim; i++)
        {
            _v[i] *= s;
        }
    }

    CT CVector::operator*(const CVector & rhs)
    {
        CT res = 0;

        for(int i = 0; i < _dim; i++)
        {
            res = res + _v[i] * rhs._v[i];
        }

        return res;
    }

    CVector CVector::cross(const CVector & rhs)
    {
        CVector res(_dim);

        res._v[0] = _v[1] * rhs._v[2] - _v[2] * rhs._v[1];
        res._v[1] = _v[2] * rhs._v[0] - _v[0] * rhs._v[2];
        res._v[2] = _v[0] * rhs._v[1] - _v[1] * rhs._v[0];

        return res;
    }

    CT CVector::magnitude2()
    {
        CT res = 0;

        for(int i = 0; i < _dim; i++)
        {
            res += _v[i] * _v[i];
        }

        return res;
    }

    CT CVector::magnitude()
    {
       return sqrt(magnitude2());
    }

    CVector::~CVector()
    {
        delete[] _v;
    }

}
