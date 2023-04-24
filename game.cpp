#include "game.h"

bool Game::init()
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
        gWindow = SDL_CreateWindow( "MakingTheRule", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
                {
                    printf("%s", Mix_GetError());
                }
            }
        }
    }

    return success;
}

void Game::music()
{
    Mix_Music* music = NULL;
    //Load Music
    music = Mix_LoadMUS("resource/backgroundm.mp3");
    if (music == NULL)
    {
        printf("%s", Mix_GetError());
    }
    Mix_PlayMusic(music,-1);
}

void Game::close()
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

void Game::running()
{
    //Main loop flag
    bool quit = false;
    //Event handler
    SDL_Event e;
    //LTimer fpsTimer;
    //fpsTimer.start();


    // init stage
    Stage lvl_1;
    lvl_1.LoadStage("resource/map_4.dat");
    lvl_1.LoadTiles(gRenderer);
    Object baba;
    baba.MakeSpriteClip(12);



    //While application is running
    while( !quit )

    {
        // Get frame start
        //frameStart = fpsTimer.getTicks();


        // RULE CHECK
        lvl_1.CheckRule();

        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            // Manipulate music
            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {

                case SDLK_m:
                    if(music_on)
                    {
                        Mix_PauseMusic();
                        music_on =false;
                    }
                    else
                    {
                        Mix_ResumeMusic();
                        music_on = true;
                    }
                    break;
                }
            }


            if(!lvl_1.baba_is_you)
            {
                lvl_1.HandleEvent(e);
            }
            if(lvl_1.baba_is_you &&
                    !lvl_1.baba_is_flag &&
                    !lvl_1.baba_is_rock &&
                    !lvl_1.baba_is_skull &&
                    !lvl_1.baba_is_wall &&
                    !lvl_1.baba_is_water&&
                    !lvl_1.rock_is_you &&
                    !lvl_1.flag_is_you &&
                    !lvl_1.wall_is_you &&
                    !lvl_1.water_is_you &&
                    !lvl_1.skull_is_you)
            {
                if(baba.Get_Out_Of_Map())
                {
                    baba.SetX(DEFAULT_X);
                    baba.SetY(DEFAULT_Y);
                    baba.Change_Out_Of_Map_Ch(false);
                }
                baba.HandleEvent(e);
                lvl_1.ManipulateActionType(baba.GetActionType());
                // Restrict moving one block per time
                if(baba.Get_Pressed() == 1 && !lvl_1.baba_is_stop)
                {
                    baba.MoveObject();
                    lvl_1.MoveStageTile(BABA_TILE);
                }




                // Manipulate baba frame
                if(baba.GetActionType() != -1)
                    baba.IncreaseFrame();
                if(baba.GetFrame() > 11)
                {
                    baba.ReloadFrame();
                }
                // Load baba
                if(lvl_1.baba_is_you)
                {
                    switch(baba.GetStatus())
                    {
                    case baba.MOVELEFT:
                        baba.loadFromFile(gRenderer,"resource/baba2_left.png");
                        break;
                    case baba.MOVERIGHT:
                        baba.loadFromFile(gRenderer,"resource/baba2_right.png");
                        break;
                    case baba.MOVEDOWN:
                        baba.loadFromFile(gRenderer,"resource/baba2_down.png");
                        break;
                    case baba.MOVEUP:
                        baba.loadFromFile(gRenderer,"resource/baba2_up.png");
                        break;
                    default:
                        baba.loadFromFile(gRenderer,"resource/baba2_right.png");
                        break;
                    };
                }

            }

            else if(lvl_1.rock_is_you && !lvl_1.rock_is_stop)
            {
                lvl_1.MoveStageTile(ROCK);
                if(!baba.Get_Out_Of_Map())
                {
                    lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                    baba.SetX(-128);
                    baba.SetY(-128);
                    baba.Change_Out_Of_Map_Ch(true);
                }


            }
            else if(lvl_1.flag_is_you && !lvl_1.flag_is_stop)
            {
                lvl_1.MoveStageTile(FLAG);
                if(!baba.Get_Out_Of_Map())
                {
                    lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                    baba.SetX(-128);
                    baba.SetY(-128);
                    baba.Change_Out_Of_Map_Ch(true);
                }
            }
            else if(lvl_1.wall_is_you && !lvl_1.wall_is_stop)
            {
                lvl_1.MoveStageTile(WALL_BLOCK);
                if(!baba.Get_Out_Of_Map())
                {
                    lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                    baba.SetX(-128);
                    baba.SetY(-128);
                    baba.Change_Out_Of_Map_Ch(true);
                }
            }
            else if(lvl_1.water_is_you && !lvl_1.water_is_stop)
            {
                lvl_1.MoveStageTile(WATER);
                if(!baba.Get_Out_Of_Map())
                {
                    lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                    baba.SetX(-128);
                    baba.SetY(-128);
                    baba.Change_Out_Of_Map_Ch(true);
                }
            }
            else if(lvl_1.skull_is_you && !lvl_1.skull_is_stop)
            {
                lvl_1.MoveStageTile(SKULL);
                if(!baba.Get_Out_Of_Map())
                {
                    lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                    baba.SetX(-128);
                    baba.SetY(-128);
                    baba.Change_Out_Of_Map_Ch(true);
                }
            }

            // Combination of rule
            if(lvl_1.baba_is_flag)
            {
                if(lvl_1.baba_is_you)
                {
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,FLAG);
                        baba.SetX(-128);
                        baba.SetY(-128);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    lvl_1.HandleEvent(e);
                    lvl_1.MoveStageTile(FLAG);
                }
                else
                {
                    lvl_1.ChangeBlock(BABA_TILE,FLAG);
                }
            }
            else if(lvl_1.baba_is_wall)
            {
                if(lvl_1.baba_is_you)
                {
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,WALL_BLOCK);
                        baba.SetX(-128);
                        baba.SetY(-128);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    lvl_1.HandleEvent(e);
                    lvl_1.MoveStageTile(WALL_BLOCK);
                }
                else
                {
                    lvl_1.ChangeBlock(BABA_TILE,WALL_BLOCK);
                }
            }
            else if(lvl_1.baba_is_rock)
            {
                if(lvl_1.baba_is_you)
                {
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,ROCK);
                        baba.SetX(-128);
                        baba.SetY(-128);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    lvl_1.HandleEvent(e);
                    lvl_1.MoveStageTile(ROCK);
                }
                else
                {
                    lvl_1.ChangeBlock(BABA_TILE,ROCK);
                }
            }else if(lvl_1.baba_is_water)
            {
                if(lvl_1.baba_is_you)
                {
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,WATER);
                        baba.SetX(-128);
                        baba.SetY(-128);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    lvl_1.HandleEvent(e);
                    lvl_1.MoveStageTile(WATER);
                }
                else
                {
                    lvl_1.ChangeBlock(BABA_TILE,WATER);
                }
            }else if(lvl_1.baba_is_skull)
            {
                if(lvl_1.baba_is_you)
                {
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,SKULL);
                        baba.SetX(-128);
                        baba.SetY(-128);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    lvl_1.HandleEvent(e);
                    lvl_1.MoveStageTile(SKULL);
                }
                else
                {
                    lvl_1.ChangeBlock(BABA_TILE,SKULL);
                }
            }

        }


// Manipulate rule for baba
        if(lvl_1.baba_is_win)
        {
            quit = true;
        }
        if(lvl_1.baba_is_push)
        {
            lvl_1.CheckStagePush(baba,BABA_TILE);
        }



// Manipulate rule for flag
        if(lvl_1.flag_is_win)
        {
            if(lvl_1.CheckWin(baba,FLAG))
                quit = true;
        }
        if(lvl_1.flag_is_stop && lvl_1.baba_is_you)
        {
            lvl_1.CheckStageCollision(baba,FLAG);
        }
        else if(lvl_1.flag_is_push && lvl_1.baba_is_you)
        {
            lvl_1.CheckStagePush(baba,FLAG);
        }

        if(lvl_1.flag_is_rock)
        {
            lvl_1.ChangeBlock(FLAG,ROCK);
        }
        else if(lvl_1.flag_is_wall)
        {
            lvl_1.ChangeBlock(FLAG,WALL_BLOCK);
        }
        else if(lvl_1.flag_is_baba)
        {
            lvl_1.ChangeBlock(FLAG,BABA_TILE);
        }
        else if(lvl_1.flag_is_water)
        {
            lvl_1.ChangeBlock(FLAG,WATER);
        }
        else if(lvl_1.flag_is_skull)
        {
            lvl_1.ChangeBlock(FLAG,SKULL);
        }

        // Manipulate the rule for the wall
        if(lvl_1.wall_is_win)
        {
            for(int wall_object = HORIZONTAL_WALL; wall_object<=WALL_BLOCK; wall_object++)
                if(lvl_1.CheckWin(baba,wall_object))
                    quit = true;
        }
        if(lvl_1.wall_is_stop && lvl_1.baba_is_you)
        {
            for(int wall_object = HORIZONTAL_WALL; wall_object<=WALL_BLOCK; wall_object++)
                lvl_1.CheckStageCollision(baba,wall_object);
        }
        else if(lvl_1.wall_is_push && lvl_1.baba_is_you)
        {
            for(int wall_object = HORIZONTAL_WALL; wall_object<=WALL_BLOCK; wall_object++)
                lvl_1.CheckStagePush(baba,wall_object);
        }
        if(lvl_1.wall_is_rock)
        {
            for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                lvl_1.ChangeBlock(i,ROCK);
        }
        else if(lvl_1.wall_is_flag)
        {
            for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                lvl_1.ChangeBlock(i,FLAG);
        }
        else if(lvl_1.wall_is_baba)
        {
            for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                lvl_1.ChangeBlock(i,BABA_TILE);
        }
        else if(lvl_1.wall_is_water)
        {
            for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                lvl_1.ChangeBlock(i,WATER);
        }
        else if(lvl_1.wall_is_skull)
        {
            for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                lvl_1.ChangeBlock(i,SKULL);
        }

        // Manipulate the rule for the rock
        if(lvl_1.rock_is_win)
        {
            if(lvl_1.CheckWin(baba,ROCK))
                quit = true;
        }
        if(lvl_1.rock_is_stop && lvl_1.baba_is_you)
        {
            lvl_1.CheckStageCollision(baba,ROCK);
        }
        else if(lvl_1.rock_is_push && lvl_1.baba_is_you)
        {
            lvl_1.CheckStagePush(baba,ROCK);
        }
        if(lvl_1.rock_is_flag)
        {
            lvl_1.ChangeBlock(ROCK,FLAG);
        }
        else if(lvl_1.rock_is_wall)
        {
            lvl_1.ChangeBlock(ROCK,WALL_BLOCK);
        }
        else if(lvl_1.rock_is_baba)
        {
            lvl_1.ChangeBlock(ROCK,BABA_TILE);
        }
        else if(lvl_1.rock_is_water)
        {
            lvl_1.ChangeBlock(ROCK,WATER);
        }
        else if(lvl_1.rock_is_baba)
        {
            lvl_1.ChangeBlock(ROCK,SKULL);
        }


        // Move the text
        for(int p = IS_TEXT;p<=SKULL_TEXT;p++)
        {
            lvl_1.CheckStagePush(baba,p);
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 75, 92, 28, 0xFF );
        SDL_RenderClear( gRenderer );


        // render stage
        lvl_1.DrawStage(gRenderer);


        // render baba
        if(lvl_1.baba_is_you && (!lvl_1.baba_is_flag && !lvl_1.baba_is_rock && !lvl_1.baba_is_wall))
        {
            SDL_Rect* baba_clip = baba.GetSpriteClips();
            baba.render(baba.GetX(),baba.GetY(),gRenderer,baba_clip);
        }
        else
        {
            SDL_Rect one_frame;
            one_frame.x= 0;
            one_frame.y = 0;
            one_frame.h = 64;
            one_frame.w = 64;
            baba.render(baba.GetX(),baba.GetY(),gRenderer,&one_frame);
        }

        //Update screen
        SDL_RenderPresent( gRenderer );
        lvl_1.RestartRule();
        /*
        //Limit frame
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < DELAY_TIME)
        {
        SDL_Delay(DELAY_TIME - frameTime);
        }*/
    }
}
