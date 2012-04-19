#include "CApp.h"
#include <stdio.h>

CApp::CApp() {
    Surf_Display = NULL;
    Surf_Field = NULL;
    Surf_Ball = NULL;
    Surf_P1 = NULL;
    Surf_P2 = NULL;
    Running = true;
    ball = NULL;
    prevTime = 0;
    for (int i = 0; i <= TOTAL_PLAYERS; i++){
        players[i] = NULL;
    }
}


int CApp::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }

    //comeco do jogo cria uma bola e 2 jogadores
    ball = new Ball(Surf_Ball,Surf_Display,400.0f);
    players[0] = new Player(Vector2D(Surf_P1->w ,Surf_Display->h/2 + 100), Surf_P1, Surf_Display);
    players[1] = new Player(Vector2D(Surf_Display->w - Surf_P2->w ,Surf_Display->h/2), Surf_P2, Surf_Display);

    SDL_Event Event;

    while(Running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        OnLoop();
        OnCollision();
        OnRender();
    }

    OnCleanup();

    return 0;
}



void CApp::OnLoop() {
    //atualiza posicao da bola e checa colisao
    float elapsed = GetElapsed();
    players[0]->Update(elapsed);
    players[1]->Update(elapsed);
    ball->Update(elapsed,ballCollided);
    ballCollided = false;
}


void CApp::OnCollision(){
    if (players[0]->Collided(ball->pos, ball->radius)) {
        ballCollided = true;
    }

    if (players[1]->Collided(ball->pos, ball->radius)) {
        ballCollided = true;
    }
}

//Inputs
void CApp::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}

void CApp::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
    switch( sym ) {
        //case SDLK_ESCAPE: Running = false; break;
        case SDLK_w: players[0]->dir = Vector2D(0,-1); break;
        case SDLK_s: players[0]->dir = Vector2D(0,1); break;
        case SDLK_UP: players[1]->dir = Vector2D(0,-1); break;
        case SDLK_DOWN: players[1]->dir = Vector2D(0,1); break;
    }

}
void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode){
    switch( sym ) {
        case SDLK_w: players[0]->dir = Vector2D(0,0); break;
        case SDLK_s: players[0]->dir = Vector2D(0,0); break;
        case SDLK_UP: players[1]->dir = Vector2D(0,0); break;
        case SDLK_DOWN: players[1]->dir = Vector2D(0,0); break;
    }
};

void CApp::OnCleanup() {
    SDL_FreeSurface(Surf_Field);
    SDL_FreeSurface(Surf_P1);
    SDL_FreeSurface(Surf_P2);
    SDL_FreeSurface(Surf_Display);
}

void CApp::OnExit(){
    SDL_Quit();
}


bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf( "Não iniciado!");
        return false;
    }

    if((Surf_Display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        printf( "Display error!");
        return false;
    }

    if((Surf_Field = CSurface::OnLoad((char*)"./images/field.bmp")) == NULL) {
        return false;
    }

    if((Surf_P1 = CSurface::OnLoad((char*)"./images/p1.bmp")) == NULL) {
        return false;
    }

    if((Surf_P2 = CSurface::OnLoad((char*)"./images/p2.bmp")) == NULL) {
        return false;
    }

    if((Surf_Ball = CSurface::OnLoad((char*)"./images/ball.bmp")) == NULL) {
        return false;
    }

    CSurface::Transparent(Surf_Ball, 255, 0, 255);

    return true;
}

void CApp::OnRender() {
    //Field
    CSurface::OnDraw(Surf_Display, Surf_Field, 0, 0);
    ball->Draw();
    players[0]->Draw();
    players[1]->Draw();
    SDL_Flip(Surf_Display);
}


float CApp::GetElapsed(){
    int currTime = SDL_GetTicks();
    int timeElapsed = currTime - prevTime;
    prevTime = currTime;
    return float(timeElapsed) / 1000.0f;
}

int main(int argc, char* argv[]) {
    CApp theApp;
    return theApp.OnExecute();
}
