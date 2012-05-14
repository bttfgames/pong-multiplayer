all:
	g++ *.cpp -o pong `sdl-config --libs` -lSDL_net -lSDL_ttf -lSDL_image
