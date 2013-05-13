#include <iostream>
#include <string>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <omp.h>
#include "CEngine.h"
#include "CFlags.h"

using namespace std;

namespace CSim
{

    CEngine::CEngine(ICSystem * s, int flags)
    {
        _system = s;
        CFlags = flags;
        _width = _height = 0;
        _cam = new Camera(0, 0, 300, 0, 0);
        _axis = true;
        _paused = false;
        _ftime = 1000. / 60.;
    }

    void CEngine::start(int w, int h)
    {
        /* Do some SDL initialization work */
        _width = w;
        _height = h;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

        const SDL_VideoInfo * info = SDL_GetVideoInfo();
        int bpp = info->vfmt->BitsPerPixel;

        string capt = "CSim Visual 0.5 - "+_system->get_name();
        SDL_WM_SetCaption(capt.c_str(), "");

        SDL_ShowCursor(0);
        SDL_WM_GrabInput(SDL_GRAB_ON);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_SetVideoMode(_width, _height, bpp, SDL_OPENGL | SDL_FULLSCREEN);

        /* ... and some more init work, this time for opengl */
        double ratio = (double)(_width) / (double)(_height);
        glShadeModel(GL_SMOOTH);

        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);

        GLfloat mat_ambient[] = { 0.0, 1.0, 0.0, 1.0 };
        GLfloat mat_diffuse[] = { 0.3, 0.1, 0.2, 1.0 };
        GLfloat mat_specular[] = { 7.0, 3.0, 3.0, 1.0 };
        GLfloat high_shininess[] = { 100.0 };
        GLfloat mat_emission[] = {0.5, 0.1, 0.1, 0.0};
        GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };

        //set clear colour (black)
        glClearColor(0, 0, 0, 1);

        //set materials and light
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

        glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        //set viewport
        glViewport(0, 0, _width, _height);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //set nice perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60., ratio, 10.0, 500000.0);

        /* generate display lists for different sized spheres */
        _dls = glGenLists(11);

        glNewList(_dls, GL_COMPILE);
        gluSphere(gluNewQuadric(), 0.8, 10, 10);
        glEndList();

        glNewList(_dls+6, GL_COMPILE);
        glBegin(GL_POINTS);
        glVertex3d(0, 0, 0);
        glEnd();
        glEndList();

        glNewList(_dls+1, GL_COMPILE);
        gluSphere(gluNewQuadric(), 1.0, 10, 10);
        glEndList();

        glNewList(_dls+7, GL_COMPILE);
        glBegin(GL_POINTS);
        glPointSize(1.0);
        glVertex3d(0, 0, 0);
        glEnd();
        glEndList();

        glNewList(_dls+2, GL_COMPILE);
        gluSphere(gluNewQuadric(), 1.4, 30, 30);
        glEndList();

        glNewList(_dls+8, GL_COMPILE);
        glBegin(GL_POINTS);
        glPointSize(1.4);
        glVertex3d(0, 0, 0);
        glEnd();
        glEndList();

        glNewList(_dls+3, GL_COMPILE);
        gluSphere(gluNewQuadric(), 1.8, 70, 70);
        glEndList();

        glNewList(_dls+9, GL_COMPILE);
        glBegin(GL_POINTS);
        glPointSize(1.8);
        glVertex3d(0, 0, 0);
        glEnd();
        glEndList();

        glNewList(_dls+4, GL_COMPILE);
        gluSphere(gluNewQuadric(), 2.2, 100, 100);
        glEndList();

        glNewList(_dls+10, GL_COMPILE);
        glBegin(GL_POINTS);
        glPointSize(2.2);
        glVertex3d(0, 0, 0);
        glEnd();
        glEndList();

        /* and we are done... or at least that's what we hope */
    }

    void CEngine::destroy()
    {
        glDeleteLists(_dls, 6);
        delete _cam;
        delete _system;
        SDL_Quit();
        exit(0);
    }

    void CEngine::handle_key_states()
    {
        Uint8 * k = SDL_GetKeyState(NULL);
        if(k[SDLK_UP]) _cam->forth = 1;
        if(k[SDLK_DOWN]) _cam->back = 1;
        if(k[SDLK_LEFT] || k[SDLK_a]) _cam->left = 1;
        if(k[SDLK_RIGHT] || k[SDLK_d]) _cam->right = 1;
        if(k[SDLK_w]) _cam->up = 1;
        if(k[SDLK_s]) _cam->down = 1;
        if(k[SDLK_KP8]) _cam->dpitch -= 0.7;
        if(k[SDLK_KP2]) _cam->dpitch += 0.7;
        if(k[SDLK_KP4]) _cam->dyaw -= 0.7;
        if(k[SDLK_KP6]) _cam->dyaw += 0.7;
    }

    void CEngine::handle_key_down(SDL_keysym * keys)
    {
        switch(keys->sym)
        {
            case SDLK_ESCAPE:
                destroy();
            break;
            case SDLK_x:
                _axis = !_axis;
            break;
            case SDLK_p:
                _paused = !_paused;
            break;
            default:
            break;
        }
    }

    void CEngine::handle_mouse_motion(SDL_MouseMotionEvent * m)
    {
        /*
            what we do here is take relative movement of a mouse and translate it
            into change of yaw and pitch angles of the camera
        */
        CT yawsign, pitchsign, xy, xm, ym;
        if(m->xrel < 0)
            yawsign = 1;
        else
            yawsign = -1;
        if(m->yrel < 0)
            pitchsign = 1;
        else
            pitchsign = -1;
        if(m->yrel != 0)
        {
            xy = m->xrel / m->yrel;
            ym = 1.0 / (1.0 + xy);
            xm = 1.0 - ym;
        }
        else
        {
            xm = 1.0;
            ym = 0.0;
        }
        _cam->dpitch = ym * pitchsign;
        _cam->dyaw = xm * yawsign;
    }

    void CEngine::handle_events()
    {
        SDL_Event evt;

        handle_key_states();

        while(SDL_PollEvent(&evt))
        {
            switch(evt.type)
            {
                case SDL_KEYDOWN:
                    handle_key_down(&evt.key.keysym);
                break;
                case SDL_MOUSEMOTION:
                    handle_mouse_motion(&evt.motion);
                break;
                case SDL_QUIT:
                    destroy();
                break;
            }
        }
    }

    void CEngine::draw_axis()
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glVertex3d(-10000.0, 0.0, 0.0);
        glVertex3d( 10000.0, 0.0, 0.0);
        glVertex3d(0.0, -10000.0, 0.0);
        glVertex3d(0.0,  10000.0, 0.0);
        glVertex3d(0.0, 0.0, -10000.0);
        glVertex3d(0.0, 0.0,  10000.0);
        glEnd();
        glEnable(GL_LIGHTING);
    }

    void CEngine::draw_sphere(CVector & pos, CVector & vel, CT mass)
    {

        glPushMatrix();

        glTranslated(pos[0], pos[1], pos[2]);
        if(mass < 100)
            glCallList(_dls+6);
        else if(mass < 1000)
            glCallList(_dls+1);
        else if(mass < 10000)
            glCallList(_dls+2);
        else if(mass < 100000)
            glCallList(_dls+3);
        else
            glCallList(_dls+4);

        glPopMatrix();

    }

    void CEngine::draw_frame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //translate the camera
        _cam->translate();

        //move and rotate the world around the camera
        glTranslated(-_cam->c[0], -_cam->c[1], -_cam->c[2]);
        glRotated(-_cam->yaw, 0.0, 1.0, 0.0);
        glRotated(-_cam->pitch, 1.0, 0.0, 0.0);

        if(_axis) draw_axis();

        int ssize = _system->get_size();


        //draw spheres
        glNewList(_dls+5, GL_COMPILE);
        for(int i = 0; i < ssize; i++)
        {
            draw_sphere(_system->get_object(i)->get_position(), _system->get_object(i)->get_velocity(), _system->get_object(i)->get_mass());
        }
        glEndList();

        glCallList(_dls+5);

        SDL_GL_SwapBuffers();
    }

    void CEngine::play()
    {
        int btime, etime, dtime;


        /*
            program's main loop
            handles upcoming events, draws next frame,
            advances the system and caps frame rate
        */
        while(1)
        {
            btime = SDL_GetTicks();
            handle_events();
            draw_frame();
            if(!_paused) _system->advance();
            etime = SDL_GetTicks();
            dtime = etime - btime;
            if(dtime < _ftime) SDL_Delay(_ftime - dtime);
        }
    }

    CEngine::~CEngine()
    {
        //destroy();
    }

}
