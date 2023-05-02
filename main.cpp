#include "game.h"




Game gameplay;

int main(int argc,char* argv[])
{
    if(!gameplay.init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{       gameplay.music();
	    while(!gameplay.Get_GameOut_Request())
            {
            gameplay.menu();
			gameplay.running();
            }
            gameplay.close();
	}
	return 0;
}
