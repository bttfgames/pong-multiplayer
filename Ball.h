#ifndef __ball_H__
#define __ball_H__

#include "SDL/SDL.h"
#include "Vector2D.h"
#include "CSurface.h"

using namespace math;

class Ball {
    private:
        SDL_Surface* source;
        SDL_Surface* destination;

    public:
        Ball(SDL_Surface* ball, SDL_Surface* screen, float speed_ball);
        Vector2D pos;
        Vector2D dir;
        float speed;
        float radius;
        bool collided;
        void Update(float elapsed_time, bool colision_player );
        //, Vector2D dir_player);
        void SetPosition(Vector2D position);
        void Draw();
};
#endif
