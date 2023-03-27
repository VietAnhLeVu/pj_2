#include "Texture.h"
#include "CommonFunc.h"
#include "Object.h"
#include "Timer.h"
#include "stage.h"


const int FPS = 12;
const int DELAY_TIME = 1000 / FPS;
Uint32 frameStart;
Uint32 frameTime;



//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer,0, 0, 0,0 );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

void close()
{

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Object baba;

int main(int argc,char* argv[])
{
if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
			//Main loop flag
			bool quit = false;
			//Event handler
			SDL_Event e;
            LTimer fpsTimer;
            fpsTimer.start();


            // init stage
            Stage lvl_1;
            lvl_1.LoadStage("resource/map_1.dat");
            lvl_1.LoadTiles(gRenderer);


			//While application is running
			while( !quit )
			{   // Get frame start
			    frameStart = fpsTimer.getTicks();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					baba.HandleEvent(e);
					baba.MoveObject();
				}

                // Load baba
                switch(baba.GetStatus())
                {
                case baba.MOVELEFT:
                    baba.loadFromFile(gRenderer,"resource/baba_left.png",12);
                    break;
                case baba.MOVERIGHT:
                    baba.loadFromFile(gRenderer,"resource/baba_right.png",12);
                    break;
                case baba.MOVEDOWN:
                    baba.loadFromFile(gRenderer,"resource/baba_down.png",12);
                    break;
                case baba.MOVEUP:
                    baba.loadFromFile(gRenderer,"resource/baba_up.png",12);
                    break;
                default:
                    baba.loadFromFile(gRenderer,"resource/baba_right.png",12);
                    break;
                };


				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 105, 105, 105, 0xFF );
				SDL_RenderClear( gRenderer );


                // render stage
                lvl_1.DrawStage(gRenderer);
                // Check stage collision
                lvl_1.CheckStageCollision(baba);
				// render baba
				SDL_Rect* baba_clip = baba.GetSpriteClips();
				baba.render(baba.GetX(),baba.GetY(),gRenderer,baba_clip);


                // Manipulate baba frame
                if(baba.GetActionType() != -1)
                baba.IncreaseFrame();
                if(baba.GetFrame() > 144)
                {
                    baba.ReloadFrame();
                }

				//Update screen
				SDL_RenderPresent( gRenderer );


                //Limit frame
                frameTime = SDL_GetTicks() - frameStart;
                if (frameTime < DELAY_TIME)
                {
                SDL_Delay(DELAY_TIME - frameTime);
                }
            }

	}

	close();
	return 0;

}
