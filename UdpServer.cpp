#include <stdlib.h>
#include <stdio.h>
#include <string.>
#include <iostream>
#include <sstream>

//#include "chat.h"

/* This is easy enough.  we setup all the network stuff, then create net_thread
   which waits for connections or other events and deals with them. In the meantime
   the main loop deals with drawing the screen and waiting for the user to press
   'esc' to quit the server. Also waits for udp messages in the main loop.

   When we quit, we kill our thread and quit the program*/

/*
using namespace std;

SDL_Surface *screen,*back;
bool finished = false;
*/

static struct {
    int active;
    TCPsocket sock;
    IPaddress peer;
    Uint8 name[256+1];
} players[GAME_MAXplayers];
/*
//************IMAGE STUFF****************
SDL_Surface * ImageLoad(char *file)
{
SDL_Surface *temp1, *temp2;
temp1 = SDL_LoadBMP(file);
temp2 = SDL_DisplayFormat(temp1);
SDL_FreeSurface(temp1);
return temp2;
}
// Blit an image on the screen surface
void DrawIMG(SDL_Surface *img, int x, int y)
{
SDL_Rect dest;
dest.x = x;
dest.y = y;
SDL_BlitSurface(img, NULL, screen, &dest);
}
//******************************************
 */


void UdpServer::FindInactivePersonSlot(int &which){
    /* Look for inactive person slot */
    for ( which=0; which<GAME_MAXplayers; ++which ) {
        if ( players[which].sock && ! players[which].active ) {
            /* Kick them out.. */
            unsigned char data = GAME_BYE;
            SDLNet_TCP_Send(players[which].sock, &data, 1);
            SDLNet_TCP_DelSocket(socketset, players[which].sock);
            SDLNet_TCP_Close(players[which].sock);
            fprintf(stderr, "Killed inactive socket %d\n", which);
            break;
        }
    }
}

void UdpServer::RoomFull(TCPsocket newsock){
    /* No more room... */
    char data = GAME_BYE;
    SDLNet_TCP_Send(newsock, &data, 1);
    SDLNet_TCP_Close(newsock);

    //TODO:Imprimir TTF_FONT
    fprintf(stderr, "Sala Cheia!\n");
}

void UdpServer::addInactiveSocket(int playerPos, TCPsocket newsock){
    /* Add socket as an inactive person */
    players[which].sock = newsock;
    players[which].peer = *SDLNet_TCP_GetPeerAddress(newsock);
    SDLNet_TCP_AddSocket(socketset, players[playerPos].sock);
    fprintf(stderr, "New inactive socket %d\n", which);
}

void UdpServer::HandleServer(void){
    int i;
    TCPsocket newsock;

    newsock = SDLNet_TCP_Accept(servsock);
    if ( newsock == NULL ) {
        return;
    }

    /* Look for unconnected person slot */
    for ( i=0; i<GAME_MAXplayers; ++i ) {
        if ( !players[i].sock ) {
            break;
        }
    }
    if ( i == GAME_MAXplayers ) {
        findInactivePersonSlot(i);
    }
    if ( i == GAME_MAXplayers ) {
        roomFull(newsock);
    } else {
        addInactiveSocket(i, newsock);
    }
}

/* Send a "new client" notification */
void UdpServer::SendNew(int about, int to)
{
    char data[512];
    int n;

    n = strlen((char *)players[about].name)+1;
    data[0] = GAME_ADD;
    data[GAME_ADD_SLOT] = about;
    memcpy(&data[GAME_ADD_HOST], &players[about].peer.host, 4);
    memcpy(&data[GAME_ADD_PORT], &players[about].peer.port, 2);
    data[GAME_ADD_NLEN] = n;
    memcpy(&data[GAME_ADD_NAME], players[about].name, n); //if more info, add it here on next line ie appearance/colour
    SDLNet_TCP_Send(players[to].sock, data, GAME_ADD_NAME+n);
}

void UdpServer::SendID(int which){
    char data[512];
    data[0] = GAME_ID;
    data[1] = which;
    SDLNet_TCP_Send(players[which].sock,data,GAME_ID_LEN+1);
}

void UdpServer::notifyAllConnectionClosed(char data[], int which){
    /* Notify all active clients */
#ifdef DEBUG
    fprintf(stderr, "Closing socket %d (was%s active)\n",
            which, players[which].active ? "" : " not");
#endif
    if ( players[which].active ) {
        players[which].active = 0;
        data[0] = GAME_DEL;
        data[GAME_DEL_SLOT] = which;
        for (int i=0; i<GAME_MAXplayers; ++i ) {
            if ( players[i].active ) {
                SDLNet_TCP_Send(players[i].sock,data,GAME_DEL_LEN);
            }
        }
    }
}

void UdpServer::deleteConnection(int which){
    SDLNet_TCP_DelSocket(socketset, players[which].sock);
    SDLNet_TCP_Close(players[which].sock);
    players[which].sock = NULL;
}



void UdpServer::HandleClient(int which)
{
    char data[512];

    /* Has the connection been closed? */
    if ( SDLNet_TCP_Recv(players[which].sock, data, 512) <= 0 ) {
        notifyAllConnectionClosed(data, which);
        deleteConnection(which);

    } else {
        switch (data[0]) {
            case GAME_HELLO: {
                 /* Yay!  An active connection */
                 memcpy(&players[which].peer.port,
                         &data[GAME_HELLO_PORT], 2);
                 memcpy(players[which].name,
                         &data[GAME_HELLO_NAME], 256);
                 players[which].name[256] = 0;

                 fprintf(stderr, "Activating socket %d (%s)\n",
                         which, players[which].name);

                 /* Notify all active clients */
                 for (int i=0; i<GAME_MAXplayers; ++i ) {
                     if ( players[i].active ) {
                         SendNew(which, i);
                     }
                 }

                 /* Notify about all active clients */
                 players[which].active = 1;
                 for (int i=0; i<GAME_MAXplayers; ++i ) {
                     if ( players[i].active ) {
                         SendNew(i, which);
                     }
                 }

                 /*Tell player which one in the slot they are */
                 SendID(which);
            }
            break;
            default: break;
        }

    }
}

void UdpServer::sendOutUDPs(char* msg, int channel){
    UDPpacket *p;

    istringstream iss(msg, istringstream::in);
    string id;
    iss >> id;
    for (int i=0; i<GAME_MAXplayers; ++i ) {
        string c = ""+i;
        if ( players[i].active && c != id.c_str()) {
            //udp related stuff
            /* Allocate memory for the packet */
            if (!(p = SDLNet_AllocPacket(512)))
            {
                fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
            }

            //copy data in
            memcpy(p->data, msg, p->maxlen);
            p->len = strlen((char *)p->data) + 1;

            p->address.host =  players[i].peer.host;    /* Set the destination host */
            p->address.port =  players[i].peer.port;    /* And destination port SDL_SwapBE16(7777);*/

            /* various readout, to see if packet is setup correctly

               cout<<"Port to connect to "<<p->address.port<<" "<<SDL_SwapBE16(p->address.port)<<endl;
               cout<<"Host "<<((p->address.host>>24)&0xFF)<<" "<<
               ((p->address.host>>16)&0xFF)<<" "<<((p->address.host>>8)&0xFF)<<
               " "<< (p->address.host&0xFF)<<endl;*/
            int sent = SDLNet_UDP_Send(udpSocket, channel, p); /* send udp message*/
        }
    }

    //free packet
    SDLNet_FreePacket(p);
}

void UdpServer::handleUDP(){
    //Check is a udp packet has come in,
    //	if so, propogate it out to other players
    if (SDLNet_UDP_Recv(udpSocket, udpPacket)) {
        /*	Readout, to see if your receiving messages (dont leave on, will make a
            huge stdout file if running for a long time)

            cout<<"Check it, got a msg"<<endl;
            printf("UDP Packet incoming\n");
            printf("\tChan:    %d\n", udpPacket->channel);
            printf("\tData:    %s\n", (char *)udpPacket->data);
            printf("\tLen:     %d\n", udpPacket->len);
            printf("\tMaxlen:  %d\n", udpPacket->maxlen);
            printf("\tStatus:  %d\n", udpPacket->status);
            printf("\tAddress: %x %x\n",udpPacket->address.host, udpPacket->address.port);*/

        sendOutUDPs((char *)udpPacket->data, udpPacket->channel);
    }
}

void UdpServer::checkIfFinished(){
    // Check if we have some interesting events...
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        // Has a key been pressed down?
        if(event.type == SDL_KEYDOWN)
        {
            // If it was escape then quit
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                //we are quiting the server now
                finished = 1;
            }
        }
    }
}

static void  UdpServer::cleanup(int exitcode){
    if ( servsock != NULL ) {
        SDLNet_TCP_Close(servsock);
        servsock = NULL;
    }
    if ( socketset != NULL ) {
        SDLNet_FreeSocketSet(socketset);
        socketset = NULL;
    }
    SDLNet_FreePacket(udpPacket);
    SDLNet_Quit();
    SDL_Quit();
    cout<<"Exitcode "<<exitcode<<endl;
    exit(exitcode);
}

void UdpServer::initSDL(){
    /* Initialize SDL */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
        exit(1);
    }

    /* Set a 640x480 video mode*/
    screen = SDL_SetVideoMode(640, 480, 0, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set video mode: %s\n",SDL_GetError());
        SDL_Quit();
        exit(1);
    }
}

void UdpServer::initSDLNet(){
    /* Initialize the network */
    if ( SDLNet_Init() < 0 ) {
        fprintf(stderr, "Couldn't initialize net: %s\n",
                SDLNet_GetError());
        SDL_Quit();
        exit(1);
    }

    /* Open a socket */
    if (!(udpSocket = SDLNet_UDP_Open(7777)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Make space for the packet */
    if (!(udpPacket = SDLNet_AllocPacket(512)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
}

void UdpServer::initChannels(){
    /* Initialize the channels */
    for (int i=0; i<GAME_MAXplayers; ++i ) {
        players[i].active = 0;
        players[i].sock = NULL;
    }
}

void UdpServer::allocateSockets(){
    /* Allocate the socket set */
    socketset = SDLNet_AllocSocketSet(GAME_MAXplayers+1);
    if ( socketset == NULL ) {
        fprintf(stderr, "Couldn't create socket set: %s\n",
                SDLNet_GetError());
        cleanup(2);
    }
}

void UdpServer::createServerSocket(){
    IPaddress serverIP;

    /* Create the server socket */
    SDLNet_ResolveHost(&serverIP, NULL, GAME_PORT);
    printf("Server IP: %x, %d\n", serverIP.host, serverIP.port);
    servsock = SDLNet_TCP_Open(&serverIP);
    if ( servsock == NULL ) {
        fprintf(stderr, "Couldn't create server socket: %s\n",
                SDLNet_GetError());
        cleanup(2);
    }
    SDLNet_TCP_AddSocket(socketset, servsock);
}

void UdpServer::initServer(){
    initSDL();
    initSDLNet();
    initChannels();
    allocateSockets();
    createServerSocket();
}

// the network input loop
int UdpServer::net_thread_main(void *data)
{
    while(!finished){

        /* Wait for events */
        /*This function here is why we need a thread if we are doing keyboard
          input and drawing to the screen. This function waits until something
          interesting happens. So it will sit at this line until a user connects
          or something */
        SDLNet_CheckSockets(socketset, ~0);

        /* Check for new connections */
        if ( SDLNet_SocketReady(servsock) ) {
            HandleServer();
        }

        /* Check for events on existing clients */
        for (int i=0; i<GAME_MAXplayers; ++i ) {
            if ( SDLNet_SocketReady(players[i].sock) ) {
                HandleClient(i);
            }
        }
    }
    return(0);
}


/*
   int main(int argc, char *argv[])
   {
   initServer();
   cout<<"INIT'd"<<endl;

//create net_thread to deal with network events
net_thread=SDL_CreateThread(net_thread_main,servsock);
if(!net_thread)
{
printf("SDL_CreateThread: %s\n",SDL_GetError());
cleanup(9);
}

// We also load in all the graphics...
back = ImageLoad("data/bg.bmp");

Loop, waiting for network events
while ( !finished ) {
//for clean exiting
checkIfFinished();
if(finished){
//kill net_thread as it may be stuck waiting for events
cout <<"Attempting to kill"<<endl;
SDL_KillThread(net_thread);
}
//draw background
DrawIMG(back, 0,0);
// Filp the screen - if you dont, you wont see anything!
SDL_Flip(screen);

handleUDP();


}
//make sure this thread is dead, before cleanup
cout<<"Waiting for thread to be dead"<<endl;
SDL_WaitThread(net_thread,NULL);
cleanup(0);

return 0;
}
 */
