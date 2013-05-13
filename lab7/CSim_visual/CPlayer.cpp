#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "CVector.h"
#include "CPlayer.h"

using namespace std;

namespace CSim
{

    CPlayer::CPlayer(string path, string name)
    {
        _path = path;
        _name = name;
        _size = _zero_size = 0;

        _collection.clear();
    }

    void CPlayer::init()
    {
        CVector x(3), v(3);
        CT m, st;
        ifstream file((_path+string("/config.txt")).c_str(), fstream::in);
        while(file>>m>>st>>x[0]>>x[1]>>x[2]>>v[0]>>v[1]>>v[2])
        {
            add_object(x, v, m, st);
        }
        file.close();
    }

    void CPlayer::add_object(const CVector & x, const CVector & v, CT mass, CT step)
    {
        //if an object is a test particle add it to the separate container
        if(mass > 0.)
        {
            _collection.push_front(new CRepBody(_size, _path+"/traces/", step, mass, x, v));
        }
        else
        {
            _collection.push_back(new CRepBody(_size, _path+"/traces/", step, mass, x, v));
            _zero_size++;
        }
        _size++;
    }

    void CPlayer::del_object(int i)
    {
        if(_collection[i]->get_mass() == 0) _zero_size--;
        _collection.erase(_collection.begin()+i);
        _size--;
    }

    void CPlayer::clear()
    {
        _collection.clear();
        _size = _zero_size = 0;
    }

    int CPlayer::get_size()
    {
        return _size;
    }

    string CPlayer::get_name()
    {
        return _name;
    }

    ICBody * CPlayer::get_object(int i)
    {
        return (ICBody*) _collection[i];
    }

    void CPlayer::advance()
    {
        int i;

        //update every object
        #pragma omp parallel for default(shared) private(i) schedule(guided)
        for(i = 0; i < _size; i++)
        {
            _collection[i]->update();
        }
    }

    void CPlayer::play(int n)
    {
        while(n--)
            advance();
    }

    CPlayer::~CPlayer()
    {
        _collection.clear();
    }

}
