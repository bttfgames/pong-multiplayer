
//The headers
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "ball.h"
#include "player.h"
#include "Vector2D.h"
#include "SDL/SDL_net.h"

using namespace math;
using namespace std;

//The screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 60;

//elapsed time
int prevTime;

//The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *ball = NULL;
SDL_Surface *background = NULL;
SDL_Surface *surf_player1 = NULL;
SDL_Surface *surf_player2 = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 255, 255, 255 };

//variaveis do jogo
float speed_ball = 400.0f;
float rest = 1.5f;
struct game {
	int id;
	//char* player;
	Vector2D ball_pos;
	Vector2D player1;
	Vector2D player2;
	float posx;
	float posy;
} p_game;

//variaveis de rede
float time_send = 0.05f;	//em sengundos
float time_last_send = 0.0f;
bool server = false;
char* ip_server = new char[15];
Uint16 port_server;
UDPsocket sd_server;
UDPsocket sd_client;
IPaddress srvadd;
UDPpacket *p_send;
UDPpacket *p_received;


void UDP_start()
{
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket on random port for client*/
	if (!(sd_client = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (server)
	{
		/* Open a socket for server*/
		if (!(sd_server = SDLNet_UDP_Open(2000)))
		{
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		/* Allocate memory for the packet */
		if (!(p_received = SDLNet_AllocPacket(512)))
		{
			fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

	} else

	{
		/* Open a socket for client*/
		if (!(sd_server = SDLNet_UDP_Open(2001)))
		{
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		/* Resolve server name for client */
		if (SDLNet_ResolveHost(&srvadd, ip_server , port_server) == -1)
		{
			fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", ip_server, port_server, SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		/* Allocate memory for the packet */
		if (!(p_send = SDLNet_AllocPacket(512)))
		{
			fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
	}

}


bool init()
{
    //Initialize all SDL subsystems
    if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    //If there was an error in setting up the screen
    if(screen == NULL)
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption("Basico SDL", NULL);

    //If everything initialized fine
    return true;
}


void clean_up()
{
    //Free the surfaces
    //SDL_FreeSurface(background);

    //Close the font
    //TTF_CloseFont(font);

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load(filename.c_str());

    //If the image loaded
    if(loadedImage != NULL)
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat(loadedImage);

        //Free the old surface
        SDL_FreeSurface(loadedImage);

        //If the surface was optimized
        if(optimizedImage != NULL)
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 255, 0, 255);

            //Set all pixels of color R 255, G 0, B 255 (magenta) to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

bool load_files()
{
    //Load the images
    ball = load_image("Tennis_Ball_32.png");
    background = load_image("tennis_court_grass.jpg");
	surf_player1 = load_image("player1.jpg");
	surf_player2 = load_image("player2.jpg");


    //Open the font
    font = TTF_OpenFont("arial.ttf", 40);

    //If there was a problem in loading the font
    if(font == NULL)
    {
        return false;
    }

    //If there was a problem loading the ball
    if(ball == NULL)
    {
        return false;
    }

    //If there was a problem in loading the background
    if(background == NULL)
    {
        return false;
    }


    //If everything loaded fine
    return true;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface(source, clip, destination, &offset);
}

float elapsed_time()
{
	int currTime = SDL_GetTicks();
	int timeElapsed = currTime - prevTime;
	prevTime = currTime;

	return float(timeElapsed) / 1000.0f;
}

int main( int argc, char* args[] )
{
	//pegar parametros
	int str = 0;
	printf ("(1)Cliente ou (2)ervidor:\t");
	scanf ("%d", &str);

	if (str==1)
	{
	printf ("Qual o IP do servidor:\t");
	scanf ("%s", ip_server);

	printf ("Qual a porta:\t");
	scanf("%d", &port_server);

	server = false;
	}

	if (str==2)
	{
	port_server = 2001;
	server = true;
	}

	//Start UDP
	UDP_start();

    //Start SDL
	init();

	//Load the files
    if(load_files() == false)
    {
        return 1;
    }

	//The surface for the winning message
    SDL_Surface *message = NULL;

	//Print a message saying left paddle wins
	if (server)
    message = TTF_RenderText_Solid(font, "Pong Tenis - Server", textColor);
	else
    message = TTF_RenderText_Solid(font, "Pong Tenis - Client", textColor);

    //Quit flag
    bool quit = false;

	//Cria objetos
	vector<Ball*> ball_tenis;
	ball_tenis.push_back(new Ball(ball, screen, speed_ball));

	vector<Player*> player;
	player.push_back(new Player(Vector2D(22,screen->h/2), surf_player1, screen));
	player.push_back(new Player(Vector2D(screen->w - 22,screen->h/2), surf_player2, screen));

    //While the user hasn't quit
    while( quit == false )
    {

		float frame_time = elapsed_time();

		//While there's events to handle
        while( SDL_PollEvent(&event) )
        {

            //If the user has Xed the window
            if (event.type == SDL_QUIT)
            {
                //Quit the program
                quit = true;
            }


			if (!server)
			{
				//If a key was pressed
				if( event.type == SDL_KEYDOWN )
				{
					switch( event.key.keysym.sym )
					{
						case SDLK_ESCAPE: quit = true; break;
						case SDLK_w: player[0]->dir = Vector2D(0,-1); break;
						case SDLK_s: player[0]->dir = Vector2D(0,1); break;
						case SDLK_UP: player[1]->dir = Vector2D(0,-1); break;
						case SDLK_DOWN: player[1]->dir = Vector2D(0,1); break;
					}
				}

				//If a key was pressed
				if( event.type == SDL_KEYUP )
				{
					switch( event.key.keysym.sym )
					{
						case SDLK_w: player[0]->dir = Vector2D(0,0); break;
						case SDLK_s: player[0]->dir = Vector2D(0,0); break;
						case SDLK_UP: player[1]->dir = Vector2D(0,0); break;
						case SDLK_DOWN: player[1]->dir = Vector2D(0,0); break;
					}
				}
			}

        }

		//Update


		for (unsigned i=0;i<player.size();i++)
		player[i]->update(frame_time);

		//Verifica colisao com a bola e atualiza
		bool ball_colision_player = false;
		Vector2D dir_player;

		if (player[0]->colision(ball_tenis[0]->pos, ball_tenis[0]->radius))
			{
				ball_colision_player = true;
				//dir_player =  player[0]->dir * rest;
			}
		else
		if (player[1]->colision(ball_tenis[0]->pos, ball_tenis[0]->radius))
			{
				ball_colision_player = true;
				//dir_player = player[0]->dir * rest;
			}

		//Atualiza posicao para server e client
		if(server)
		{
			if (SDLNet_UDP_Recv(sd_server, p_received))
			{
			//recebe struct
			memcpy(&p_game, (void *)p_received->data, 512);
			}

			//ball_tenis[0]->position(Vector2D(p_game.posx,p_game.posy));
			ball_tenis[0]->position(p_game.ball_pos);
			player[0]->position(p_game.player1);
			player[1]->position(p_game.player2);
		}
		else
		{
			ball_tenis[0]->update(frame_time, ball_colision_player, dir_player);
		}


		//Montar a tela

		//Show the background
		apply_surface(0, 0, background, screen);

		//Show the ball
		ball_tenis[0]->draw();

		//Show players
		player[0]->draw();
		player[1]->draw();

		//Show the Text
		apply_surface(200, 300, message, screen);

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
		if ((1.0f / FRAMES_PER_SECOND) > frame_time)
            SDL_Delay( ((1.0f / FRAMES_PER_SECOND) - frame_time) * 1000);

		if (!server)
		{
			//Envia dados do cliente para o servidor
			if (time_last_send > time_send)
			{
			//Empacota struct
			p_game.id ++;
			p_game.ball_pos = ball_tenis[0]->pos;
			p_game.player1 = player[0]->pos;
			p_game.player2 = player[1]->pos;
			p_game.posx = ball_tenis[0]->pos.x;
			p_game.posy = ball_tenis[0]->pos.y;
			memcpy(p_send->data, (void *)&p_game, sizeof(p_game));
			p_send->len = sizeof(p_game);

			p_send->address.host = srvadd.host;	/* Set the destination host */
			p_send->address.port = srvadd.port;	/* And destination port */

			SDLNet_UDP_Send(sd_client, -1, p_send); /* This sets the p->channel */

			time_last_send = 0.0f;
			}
		}

		time_last_send += frame_time;

    }

    //Clean up
    clean_up();

    return 0;
}
