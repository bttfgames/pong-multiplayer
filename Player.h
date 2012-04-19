#ifndef __player_H__
#define __player_H__


#include <SDL/SDL.h>
#include "Vector2D.h"

using namespace math;

class Player
 {
 private:

	 SDL_Surface* source;
	 SDL_Surface* destination;

 public:
	 Player(Vector2D pos_ini, SDL_Surface* ball, SDL_Surface* screen);
	 float width;
	 float height;
	 Vector2D pos;
	 Vector2D dir;
	 float player_speed;
	 void Update(float elapsed_time);
	 void SetPosition(Vector2D position);
	 bool Collided(Vector2D ball_pos, float ball_radius);
	 void Draw();

};
#endif
