/* Convert four letters into a number */
#define MAKE_NUM(A, B, C, D)	(((A+B)<<8)|(C+D))

/* Defines for the chat client */
#define GAME_SCROLLBACK	512		/* Save 512 lines in scrollback */
#define GAME_PROMPT	"> "
#define GAME_PACKETSIZE	256		/* Maximum length of a message */

/* Defines shared between the server and client */
#define GAME_PORT	7777//MAKE_NUM('C','H','A','T')

/* The protocol between the chat client and server */
#define GAME_HELLO	0	/* 0+Port+len+name */
#define GAME_HELLO_PORT		1
#define GAME_HELLO_NLEN		GAME_HELLO_PORT+2
#define GAME_HELLO_NAME		GAME_HELLO_NLEN+1
#define GAME_ADD	1	/* 1+N+IP+Port+len+name */
#define GAME_ADD_SLOT		1
#define GAME_ADD_HOST		GAME_ADD_SLOT+1
#define GAME_ADD_PORT		GAME_ADD_HOST+4
#define GAME_ADD_NLEN		GAME_ADD_PORT+2
#define GAME_ADD_NAME		GAME_ADD_NLEN+1
#define GAME_DEL	2	/* 2+N */
#define GAME_DEL_SLOT		1
#define GAME_DEL_LEN		GAME_DEL_SLOT+1
#define GAME_BYE	255	/* 255 */
#define GAME_BYE_LEN		1
#define GAME_ID 5
#define GAME_ID_SLOT 1
#define GAME_ID_LEN 2

/* The maximum number of people who can talk at once */
#define GAME_MAXPEOPLE	4

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "SDL_net.h"

class UdpServer{
    private:
        SDL_Thread *netThread; //=NULL;
        static TCPsocket serverSocket; //= NULL;
        static SDLNet_SocketSet socketSet; //= NULL;

        UDPsocket *udpSocket;  //nao tinha ponteiro.     /* Socket descriptor */
        UDPpacket *udpPacket;       /* Pointer to packet memory */


    public:
        UdpServer();
        ~UdpServer();

        static void Cleanup(int exitcode);
        void FindInactivePersonSlot(int &which);
        void RoomFull(TCPsocket newsock);
        void AddInactiveSocket(int which, TCPsocket newsock);
        void HandleServer(void);
        void SendNew(int about, int to);
        void SendID(int which);
        void NotifyAllConnectionClosed(char data[], int which);
        void DeleteConnection(int which);
        void HandleClient(int which);
        void SendOutUDPs(char* msg, int channel);
        void HandleUDP();
        void CheckIfFinished();
        void InitSDLNet();
        void InitChannels();
        void AllocateSockets();
        void CreateServerSocket();
        void InitServer();
        int NetThreadMain(void *data);

}

