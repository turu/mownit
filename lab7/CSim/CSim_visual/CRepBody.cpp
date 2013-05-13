#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "CVector.h"
#include "CRepBody.h"

using namespace std;

namespace CSim
{

    CRepBody::CRepBody(int id, string dir, CT step, CT mass, const CVector & x, const CVector & v) :
    _id(id), _sysdir(dir), _step(step), _mass(mass)
    {
        _pos = new CVector(x);
        _vel = new CVector(v);

        char sid[100];
        sprintf(sid, "%d", id);

        _file.open((dir+string(sid)+".dat").c_str(), fstream::in);
    }

    string CRepBody::get_dir()
    {
        return _sysdir;
    }

    void CRepBody::set_dir(string d)
    {
        _sysdir = d;
    }

    int CRepBody::get_id()
    {
        return _id;
    }

    void CRepBody::set_id(int n)
    {
        _id = n;
    }

    CT CRepBody::get_mass()
    {
        return _mass;
    }

    void CRepBody::set_mass(CT m)
    {
        _mass = m;
    }

    CVector & CRepBody::get_position()
    {
        return *_pos;
    }

    CVector & CRepBody::get_pposition()
    {
        return get_position();
    }

    void CRepBody::set_position(const CVector & x)
    {
        *_pos = x;
    }

    CVector & CRepBody::get_velocity()
    {
        return *_vel;
    }

    void CRepBody::set_velocity(const CVector & v)
    {
        *_vel = v;
    }

    CT CRepBody::get_step()
    {
        return _step;
    }

    void CRepBody::set_step(CT s)
    {
        _step = s;
    }

    void CRepBody::update()
    {
        //if haven't reached the end of file yet...
        if(_file.is_open() && !(_file>>(*_pos)[0]>>(*_pos)[1]>>(*_pos)[2]>>(*_vel)[0]>>(*_vel)[1]>>(*_vel)[2]).good())
        {
            _file.close();
        }
    }

    CRepBody::~CRepBody()
    {
        delete _pos;
        delete _vel;
        _file.close();
    }

}
