#include <cstdlib>
#include <cstring>
#include <SDL.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <omp.h>
#include "CVector.h"
#include "CFlags.h"
#include "ICSystem.h"
#include "CSystem.h"
#include "CPlayer.h"
#include "CEngine.h"

using namespace std;
using namespace CSim;

int main(int argc, char ** argv)
{
    ios_base::sync_with_stdio(0);
    ICSystem * sys = NULL;

    extern char *optarg;
    char c;

    char mode = 0;
    int width = 0, height = 0, iters = 0;
    int flags = 0;
    string path, name, traces, config;
    CT step = 0, G = 0;

    /* we have to process all incoming arguments first, so that we know what mode we're gonna work in */
    while((c = getopt(argc, argv, "m:w:p:n:d:s:g:x:y:i:a:")) != -1)
    {
        switch(c){
            case 'm': //mode: s - simulation; r - rerun; t - text
                mode = optarg[0];
            break;
            case 'w': //should we write to file during sim
                if(optarg[0]=='y')
                    flags |= CE_WRITE;
                else
                    flags |= CE_NOT_WRITE;
            break;
            case 'a':
                if(optarg[0]=='y')
                    flags |= CE_ADJUSTABLE;
                else
                    flags |= CE_NOT_ADJUSTABLE;
            break;
            case 'p': //path to the simulation folder
                path.assign(optarg);
            break;
            case 'n': //simulation's name
                name.assign(optarg);
            break;
            case 'd': //path to the config file
                config.assign(optarg);
            break;
            case 's': //master step
                step = atof(optarg);
            break;
            case 'g': //G constant
                G = atof(optarg);
            break;
            case 'x': //screen's width
                width = atoi(optarg);
            break;
            case 'y': //screen's height
                height = atoi(optarg);
            break;
            case 'i': //how many iterations (used only in 't' mode)
                iters = atoi(optarg);
            break;
        }
    }

    CFlags = flags;

    if(mode == 's') //real-time sim mode
    {
        sys = new CSystem((CFlags&CE_WRITE ? path : ""), name, G, step);
        sys->init();
        ((CSystem * )sys)->read_from_file(config);
    }
    else if(mode == 'r') //re-run mode
    {
        sys = new CPlayer(path, name);
        sys->init();
    }
    else if(mode == 't') //text mode - real time sim, without graphics
    {
        sys = new CSystem(path, name, G, step);
        sys->init();
        ((CSystem *)sys)->read_from_file(config);
        sys->play(iters);
        return 0;
    }

    CEngine engine(sys, flags);
    engine.start(width, height);
    engine.play();

    return 0;
}
