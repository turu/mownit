#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <omp.h>
#include "CVector.h"
#include "CSystem.h"
#include "CFlags.h"

using namespace std;

namespace CSim
{

    CSystem::CSystem(string path, string name, CT G, CT step)
    {
        _first_step = true;
        _path = path;
        _name = name;
        _G = G;
        _step = step;
        _size = _zero_size = 0;
        _E = _Ek = _Ep = 0;

        _collection.clear();
    }

    void CSystem::init()
    {
        int res;
        /* Creates simulation directories under provided path */
        _dir = _path+"/"+_name;
        if(CFlags&CE_WRITE)
        {
            res = mkdir(_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            res = mkdir((_dir+"/traces").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        _file.open((_dir+"/sysprop.dat").c_str(), fstream::out);
        _file.precision(20);
        _file.unsetf(ios::floatfield);
    }

    int CSystem::read_from_file(string name)
    {
        int ct = 0;
        CVector x(3), v(3);
        CT m, st;
        ofstream cfile;
        ifstream file(name.c_str(), fstream::in);
        file.precision(20);
        file.unsetf(ios::floatfield);
        if(CFlags&CE_WRITE)
        {
            cfile.open((_dir+string("/config.txt")).c_str(), fstream::out);
            cfile.precision(20);
            cfile.unsetf(ios::floatfield);
        }
        while(file>>m>>st>>x[0]>>x[1]>>x[2]>>v[0]>>v[1]>>v[2])
        {
            if(CFlags&CE_WRITE)
            {
                cfile<<m<<" "<<st<<" "<<x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<v[0]<<" "<<v[1]<<" "<<v[2]<<endl;
            }
            add_object(x, v, m, st);
            ct++;
        }
        file.close();
        if(CFlags&CE_WRITE) cfile.close();
        return ct;
    }

    void CSystem::add_object(const CVector & x, const CVector & v, CT mass, CT step)
    {
        if(mass > 0.)
        {
            _collection.push_front(new CBody(this, _size, _dir+"/traces/", step, mass, _G, x, v));
        }
        else
        {
            _collection.push_back(new CBody(this, _size, _dir+"/traces/", step, mass, _G, x, v));
            _zero_size++;
        }
        _size++;
    }

    void CSystem::del_object(int i)
    {
        if(_collection[i]->get_mass() == 0) _zero_size--;
        _collection.erase(_collection.begin()+i);
        _size--;
    }

    void CSystem::clear()
    {
        _collection.clear();
        _size = _zero_size = 0;
    }

    int CSystem::get_size()
    {
        return _size;
    }

    string CSystem::get_name()
    {
        return _name;
    }

    ICBody * CSystem::get_object(int i)
    {
        return (ICBody*) _collection[i];
    }

    void CSystem::adjust_step(CBody * obj)
    {
        CVector & old_vel = obj->get_pvelocity();
        CVector & cur_vel = obj->get_velocity();

        CT ols, curs, ratio;
        curs = fabs(cur_vel[0]) + fabs(cur_vel[1]) + fabs(cur_vel[2]);
        ols = fabs(old_vel[0]) + fabs(old_vel[1]) + fabs(old_vel[2]);
        ratio = ols / curs;

        if(ratio < 0.75 || ratio > 1.33)
        {
            obj->set_step(Min(Max(obj->get_step() * ratio, 0.00001), _step));
        }
    }

    void CSystem::advance()
    {
        CBody * obj;
        int i;
        CT stepsum, objstep;
        bool first_iter;

        if(_first_step)
        {
            _first_step = false;
            #pragma omp parallel for default(shared) private(i) schedule(guided)
            for(i = 0; i < _size; i++)
            {
                _collection[i]->update_acc(false);
            }
        }

        //update every object
        #pragma omp parallel for default(shared) private(first_iter, i, obj, stepsum, objstep) schedule(guided)
        for(i = 0; i < _size; i++)
        {
            stepsum = 0.00001;
            obj = _collection[i];
            objstep = obj->get_step();
            first_iter = true;

            /* if object's step is smaller than system's step, advance it by several iterations */
            obj->update();
            stepsum += objstep;
//            while(stepsum < _step)
//            {
//                obj->update_acc(true);
//                obj->update_vel();
//                if(stepsum + objstep < _step) obj->discard_old();
//                obj->update();
//                stepsum += objstep;
//                first_iter = false;
//            }
        }

        //discard old data and adjust step sizes
        #pragma omp parallel for default(shared) private(i, obj) schedule(guided)
        for(i = 0; i < _size; i++)
        {
            obj = _collection[i];
            obj->update_acc(false);
            obj->update_vel();
            obj->discard_old();
            //if(CFlags&CE_ADJUSTABLE)adjust_step(obj);
            if(CFlags&CE_WRITE)obj->write();
        }

        update_properties();
        if(CFlags&CE_WRITE) write_properties();

    }

    void CSystem::update_properties()
    {
        int i;
        CBody * obj;
        _E = _Ek = _Ep = 0.;
        _Am[0] = _Am[1] = _Am[2] = 0.;
        for(i = 0; i < _size - _zero_size; i++)
        {
            obj = _collection[i];
            _Ek += obj->get_mass()*obj->get_velocity().magnitude2();
            _Ep += obj->get_potential();
            _Am += obj->get_momentum();
        }
        _Ep *= 0.5;
        _Ek *= 0.5;
        _E = _Ek + _Ep;
    }

    void CSystem::write_properties()
    {
        _file<<_Ek<<" "<<_Ep<<" "<<_E<<" "<<_Am[0]<<" "<<_Am[1]<<" "<<_Am[2]<<endl;
    }

    void CSystem::play(int n)
    {
        while(n--)
            advance();
    }

    CSystem::~CSystem()
    {
        _file.close();
        _collection.clear();
    }

}
