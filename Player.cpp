#include "Player.h"
#include "CSurface.h"


Player::Player(Vector2D pos_ini, SDL_Surface* ball, SDL_Surface* screen){
	 source = ball;
	 destination = screen;
	 width = source->w;
	 heigth = source->h;
	 pos = pos_ini;
	 player_speed = 200.0f;
};

void Player::Update(float elapsed_time){
	pos += ((dir * player_speed) * elapsed_time);
	dir += (dir * 2.0f * elapsed_time);
}

void Player::SetPosition(Vector2D position){
	pos = position;
}

bool Player::Collided(Vector2D ball_pos, float ball_radius){
	Vector2D p_origin(pos.x - (width/2), pos.y - (heigth/2));
	if ((p_origin.x - ball_radius< ball_pos.x) && (ball_pos.x < p_origin.x + width + ball_radius )
		&& (p_origin.y - ball_radius < ball_pos.y) && (ball_pos.y < p_origin.y + heigth + ball_radius))
		return true;

	return false;
}

void Player::Draw(){
    CSurface::OnDraw(destination, source, pos.x - (width/2),pos.y - (width/2) );
}
