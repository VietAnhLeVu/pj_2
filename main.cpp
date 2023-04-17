#include "game.h"

/*
const int FPS = 12;
const int DELAY_TIME = 1000 / FPS;
Uint32 frameStart;
Uint32 frameTime;
*/




Game gameplay;

int main(int argc,char* argv[])
{
    if(!gameplay.init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{       //gameplay.music();
			gameplay.running();
	}
    gameplay.close();
	return 0;
}
