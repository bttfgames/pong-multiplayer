#ifndef _CAPP_H_
#define _CAPP_H_

#include "SDL/SDL.h"
#include "CSurface.h"
#include "CEvent.h"
#include "Player.h"
#include "Ball.h"
#include "MathUtil.h"
#include "Vector2D.h"


#define TOTAL_PLAYERS 4

class CApp: public CEvent {
    private:
        SDL_Surface* Surf_Display;
        bool Running;
        bool ballCollided;
        int prevTime;

        //pong surfaces
        SDL_Surface*    Surf_Field;
        SDL_Surface*    Surf_Ball;
        SDL_Surface*    Surf_P1;
        SDL_Surface*    Surf_P2;

        Ball* ball;
        Player* players[TOTAL_PLAYERS];

    public:
        CApp();
        bool OnInit();
        int OnExecute();
        void OnEvent(SDL_Event* Event);
        void OnLoop();
        void OnRender();
        void OnCleanup();
        void OnExit();
        void OnCollision();
        void Reset();
        float GetElapsed();

        //Inputs
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);


};

#endif
