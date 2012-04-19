#include "Ball.h"

Ball::Ball(SDL_Surface* ball, SDL_Surface* screen, float speed_ball){
	 source = ball;
	 destination = screen;
	 radius = (source->w/2) - 2;
	 pos = Vector2D(destination->w / 2 ,destination->h / 2);
	 speed = speed_ball;
	 dir = Vector2D(1.0f, 0.0f);
	 dir.rotate(toRadians(30));
};

void Ball::Update(float elapsed_time, bool collision_player ){
//, Vector2D dir_player){

	if ((collision_player) && (!collided))	{
		//dir += change;
		dir.normalize();
		dir = Vector2D(-dir.x, dir.y);
	}

	pos += (dir * speed * elapsed_time);
	//pos += (dir * 0.01f);

	if ((pos.x > destination->w + radius)||(pos.x < - radius))
	{
		pos = Vector2D(destination->w / 2 ,destination->h / 2);
		dir.rotate(toRadians(30 - rand()%60));
	}
	if (pos.y + radius > destination->h )
		dir = Vector2D(dir.x, - abs(dir.y));
	if (pos.y < radius)
		dir = Vector2D(dir.x, abs(dir.y));

	collided = collision_player;

}

void Ball::SetPosition(Vector2D position){
	pos = position;
}

void Ball::Draw(){
    CSurface::OnDraw(destination,source, pos.x -radius , pos.y - radius);
}
