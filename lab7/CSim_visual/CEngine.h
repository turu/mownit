#ifndef CENGINE_H
#define CENGINE_H

#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ICSystem.h"
#include "CVector.h"
#include "CFlags.h"
#include "Camera.h"

using namespace std;

namespace CSim
{

    /* CEngine is the main class of a simulation */
    class CEngine
    {
        protected:

            bool _axis;
            bool _paused;

            /* pointer to a system object */
            ICSystem * _system;

            /* screen resolution */
            int _height;
            int _width;

            /* frame cap */
            int _ftime;

            /* camera object */
            Camera * _cam;

            /* display lists */
            GLuint _dls;

            void handle_events();
            void handle_key_states();
            void handle_mouse_motion(SDL_MouseMotionEvent*);
            void handle_key_down(SDL_keysym*);

            void draw_axis();
            void draw_sphere(CVector &, CVector &, CT);
            void draw_frame();

            void destroy();

        public:

            /* Constructors */
            CEngine(ICSystem*, int);
            /* \Constructors */

            /* Initializes graphic environment */
            void start(int, int);

            /* Starts simulation */
            void play();

            /* Destructors */
            ~CEngine();
            /* \Destructors */
    };

}

#endif
