#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "CVector.h"
#include "CBody.h"
#include "CFlags.h"

using namespace std;

namespace CSim
{


    CBody::CBody(ICSystem * p, int id, string dir, CT step, CT mass, CT G, const CVector & x, const CVector & v) :
    _system(p), _id(id), _sysdir(dir), _step(step), _mass(mass), _G(G)
    {
        _pos = new CVector(x);
        _vel = new CVector(v);
        _ppos = new CVector(x);
        _pvel = new CVector(v);
        _acc = new CVector(3, 0.);

        if(CFlags&CE_WRITE)
        {
            char csid[100];
            sprintf(csid, "%d", id);
            _file.open((dir+string(csid)+".dat").c_str(), fstream::ate | fstream::trunc);
            _file.precision(20);
            _file.unsetf(ios::floatfield);
            write();
        }
    }

    string CBody::get_dir()
    {
        return _sysdir;
    }

    void CBody::set_dir(string d)
    {
        _sysdir = d;
    }

    int CBody::get_id()
    {
        return _id;
    }

    void CBody::set_id(int n)
    {
        _id = n;
    }

    CT CBody::get_mass()
    {
        return _mass;
    }

    void CBody::set_mass(CT m)
    {
        _mass = m;
    }

    CVector & CBody::get_position()
    {
        return *_pos;
    }

    CVector & CBody::get_pposition()
    {
        return *_ppos;
    }

    void CBody::set_position(const CVector & x)
    {
        *_pos = x;
    }

    CVector & CBody::get_velocity()
    {
        return *_vel;
    }

    CVector & CBody::get_pvelocity()
    {
        return *_pvel;
    }

    void CBody::set_velocity(const CVector & v)
    {
        *_vel = v;
    }

    CT CBody::get_step()
    {
        return _step;
    }

    void CBody::set_step(CT s)
    {
        _step = s;
    }

    CVector & CBody::get_acc()
    {
        return *_acc;
    }

    CT CBody::get_potential()
    {
        return _U;
    }

    CVector CBody::get_momentum()
    {
        return _pos->cross((*_vel) * _mass);
    }

    void CBody::write()
    {
        _file << (*_pos)[0] << " " << (*_pos)[1] << " " << (*_pos)[2] << " " << (*_vel)[0] << " " << (*_vel)[1] << " " << (*_vel)[2] << endl;
    }

    void CBody::discard_old()
    {
        /* discards old position and velocity */
        *_ppos = *_pos;
        *_pvel = *_vel;
    }

    void CBody::update_acc(bool old)
    {
        CT bkm, Um, rl;
        ICBody * obj;
        CVector r;
        _U = 0.;

        (*_acc)[0] = (*_acc)[1] = (*_acc)[2] = 0.;

        int s = _system->get_size();

        for(int i = 0; i < s; i++)
        {
            obj = _system->get_object(i);

            //don't do anything if the body has no mass
            //NOTE: all zero-mass particles are kept at the end of _collection container so we can safely skip
            //further computations when we encounter first such object
            if((bkm = obj->get_mass()) == 0.) break;

            if(obj->get_id() == _id) continue;

            //save position and mass of the body
            CVector & bkp = (old ? obj->get_pposition() : obj->get_position());

            r = bkp - *_pos;
            rl = r.magnitude();
            Um = bkm * _G / rl;
            _U -= Um * _mass;

            (*_acc) += r * (Um / (rl*rl));
        }
    }

    void CBody::update()
    {
        CT hsp, hsp2;

        hsp = 0.5 * _step;
        hsp2 = hsp * _step;

        /* Step 1: Calculate new position and partial velocity */
        (*_pos) += (*_vel) * _step + (*_acc) * hsp2;
        (*_vel) += (*_acc) * hsp;
    }

    void CBody::update_vel()
    {
        (*_vel) += (*_acc) * (_step * 0.5);
    }

    CBody::~CBody()
    {
        delete _pos;
        delete _vel;
        delete _ppos;
        delete _pvel;
        delete _acc;
        if(CFlags&CE_WRITE) _file.close();
    }

}
