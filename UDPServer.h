#include <SDL/SDL_net.h>
#include "Vector2D.h"
#define PORT 2000

using namespace math;

class UDPServer {
	private:
		//se server foi iniciado
		bool up;

	public:
		struct packet {
			int id;
			Vector2D ballPos;
			Vector2D player1;
			Vector2D player2;
			float posx;
			float posy;
		} serverPacket;

		//net
		UDPsocket sd_server;
		UDPpacket *p_send;
		UDPpacket *p_received;

		UDPServer();
		bool isUp();

		void ReceivePackage();

};
