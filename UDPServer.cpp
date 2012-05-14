#include "UDPServer.h"

UDPServer::UDPServer(){
	if (SDLNet_Init() < 0)	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		up = false;
	}

	/* Open a socket for server*/
	if (!(sd_server = SDLNet_UDP_Open(PORT)))	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		up = false;
	}

	/* Allocate memory for the packet */
	if (!(p_received = SDLNet_AllocPacket(512)))	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		up = false;
	}

	up = true;
}


bool UDPServer::isUp(){
	return up;
}

void UDPServer::ReceivePackage(){
	if (SDLNet_UDP_Recv(sd_server, p_received))	{
		//recebe struct
		memcpy(&serverPacket, (void *)p_received->data, 512);
	}
}