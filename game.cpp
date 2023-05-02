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
    music = Mix_LoadMUS("resource/sfx/backgroundm.mp3");
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

void Game::menu()
{
    bool quit = false;
    bool map_choose = false;
    bool tutorial_seeing = false;
    SDL_Event e;
    int mouse_x;
    int mouse_y;

    LTexture background;
    background.loadFromFile(gRenderer,"resource/menu_background.png");

    Object play_button;
    play_button.SetX(520);
    play_button.SetY(250);
    play_button.SetWidth(200);
    play_button.SetHeight(32);
    play_button.loadFromFile(gRenderer,"resource/play_button.png");

    Object exit_button;
    exit_button.SetX(520);
    exit_button.SetY(350);
    exit_button.SetWidth(200);
    exit_button.SetHeight(32);
    exit_button.loadFromFile(gRenderer,"resource/exit_button.png");

    Object tutorial_button;
    tutorial_button.SetX(520);
    tutorial_button.SetY(300);
    tutorial_button.SetHeight(32);
    tutorial_button.SetWidth(200);
    tutorial_button.loadFromFile(gRenderer,"resource/tutorial_button.png");

    Object tutorial;
    tutorial.SetX(0);
    tutorial.SetY(0);
    tutorial.SetWidth(1024);
    tutorial.SetHeight(704);
    tutorial.loadFromFile(gRenderer,"resource/tutorial.png");

    Object Baba1;
    Baba1.SetX(32);
    Baba1.SetY(500);
    Baba1.SetWidth(128);
    Baba1.SetHeight(128);
    Baba1.loadFromFile(gRenderer,"resource/choose_baba1.png");

    Object Baba2;
    Baba2.SetX(160);
    Baba2.SetY(500);
    Baba2.SetWidth(128);
    Baba2.SetHeight(128);
    Baba2.loadFromFile(gRenderer,"resource/choose_baba2.png");

    Object Map_board;
    Map_board.SetX(128);
    Map_board.SetY(128);
    Map_board.SetWidth(1024);
    Map_board.SetHeight(448);
    Map_board.loadFromFile(gRenderer,"resource/map_choose.png");

    Object return_button;
    return_button.SetX(2);
    return_button.SetY(2);
    return_button.SetWidth(99);
    return_button.SetHeight(99);
    return_button.loadFromFile(gRenderer,"resource/return_button_small.png");

    Object esc_button;
    esc_button.SetX(1181);
    esc_button.SetY(0);
    esc_button.SetHeight(99);
    esc_button.SetWidth(99);
    esc_button.loadFromFile(gRenderer,"resource/esc_button.png");


    Object upper_map[5];
    for(int i = 0; i<5; i++)
    {
        upper_map[i].SetX(32+(i+1)*192);
        upper_map[i].SetY(224);
        upper_map[i].SetWidth(64);
        upper_map[i].SetHeight(64);
        char map_img[30];
        sprintf(map_img,"resource/map_%d_choose.png",i+1);
        upper_map[i].loadFromFile(gRenderer,map_img);
    }


    SDL_Rect upper_map_choose[5];
    for(int i = 0; i<5; i++)
    {
        upper_map_choose[i].x = 192*(i+1);
        upper_map_choose[i].y = 192;
        upper_map_choose[i].w = 128;
        upper_map_choose[i].h = 128;
    }

    Object lower_map[5];
    for(int i = 0; i<5; i++)
    {
        lower_map[i].SetX(32+(i+1)*192);
        lower_map[i].SetY(416);
        lower_map[i].SetWidth(64);
        lower_map[i].SetHeight(64);
        char map_img[30];
        sprintf(map_img,"resource/map_%d_choose.png",i+6);
        lower_map[i].loadFromFile(gRenderer,map_img);
    }

    SDL_Rect lower_map_choose[5];
    for(int i = 0; i<5; i++)
    {
        lower_map_choose[i].x = 192*(i+1);
        lower_map_choose[i].y = 384;
        lower_map_choose[i].w = 128;
        lower_map_choose[i].h = 128;
    }

    while(!quit)
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            // User request to quit menu or get in the game
            if( e.type == SDL_QUIT )
            {
                quit = true;
                out_game =true;
            }
            if(playing)
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

            if(e.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&mouse_x,&mouse_y);
                if(!map_choose)
                {
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,play_button.GetPosRect()))
                        play_button.loadFromFile(gRenderer,"resource/chosen_play_button.png");
                    else
                        play_button.loadFromFile(gRenderer,"resource/play_button.png");

                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,exit_button.GetPosRect()))
                        exit_button.loadFromFile(gRenderer,"resource/chosen_exit_button.png");
                    else
                        exit_button.loadFromFile(gRenderer,"resource/exit_button.png");

                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,tutorial_button.GetPosRect()))
                        tutorial_button.loadFromFile(gRenderer,"resource/chosen_tutorial_button.png");
                    else
                        tutorial_button.loadFromFile(gRenderer,"resource/tutorial_button.png");

                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,Baba1.GetPosRect()) || Baba_char == 1)
                        Baba1.loadFromFile(gRenderer,"resource/chosen_baba1.png");
                    else
                        Baba1.loadFromFile(gRenderer,"resource/choose_baba1.png");

                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,Baba2.GetPosRect()) || Baba_char == 2)
                        Baba2.loadFromFile(gRenderer,"resource/chosen_baba2.png");
                    else
                        Baba2.loadFromFile(gRenderer,"resource/choose_baba2.png");
                }
                else
                {
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,return_button.GetPosRect()))
                        return_button.loadFromFile(gRenderer,"resource/return_button_big.png");
                    else
                        return_button.loadFromFile(gRenderer,"resource/return_button_small.png");

                    for(int i = 0; i<5; i++)
                    {
                        // Animation for upper map
                        if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,upper_map_choose[i]))
                        {
                            char map_img[30];
                            sprintf(map_img,"resource/map_%d_chosen.png",i+1);
                            upper_map[i].loadFromFile(gRenderer,map_img);
                        }
                        else
                        {
                            char map_img[30];
                            sprintf(map_img,"resource/map_%d_choose.png",i+1);
                            upper_map[i].loadFromFile(gRenderer,map_img);
                        }
                        // Animation for lower map
                        if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,lower_map_choose[i]))
                        {
                            char map_img[30];
                            sprintf(map_img,"resource/map_%d_chosen.png",i+6);
                            lower_map[i].loadFromFile(gRenderer,map_img);
                        }
                        else
                        {
                            char map_img[30];
                            sprintf(map_img,"resource/map_%d_choose.png",i+6);
                            lower_map[i].loadFromFile(gRenderer,map_img);
                        }
                    }
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(!map_choose && !tutorial_seeing)
                {
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,play_button.GetPosRect()))
                        map_choose = true;
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,exit_button.GetPosRect()))
                    {
                        quit = true;
                        out_game = true;
                    }
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,tutorial_button.GetPosRect()))
                        tutorial_seeing = true;
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,Baba1.GetPosRect()))
                        Baba_char =1;

                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,Baba2.GetPosRect()))
                        Baba_char =2;
                }
                else if(map_choose)
                {
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,return_button.GetPosRect()))
                        map_choose = false;
                    for(int i = 0; i<5; i++)
                    {
                        // Choose an upper map
                        if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,upper_map_choose[i]))
                        {
                            playing = true;
                            playing_map = i+1;
                        }

                        // Choose an lower map
                        if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,lower_map_choose[i]))
                        {
                            playing = true;
                            playing_map = i+6;
                        }

                    }
                }else if(tutorial_seeing)
                {
                    if(SDL_CommonFunc::CheckFocusWithRect(mouse_x,mouse_y,esc_button.GetPosRect()))
                    {
                        tutorial_seeing = false;
                    }
                }
            }


        }
        // render menu

        SDL_RenderClear(gRenderer);

        if(map_choose)
        {
            SDL_SetRenderDrawColor(gRenderer,92,131,57,0);
            SDL_RenderClear(gRenderer);
            Map_board.render(Map_board.GetX(),Map_board.GetY(),gRenderer);
            return_button.render(return_button.GetX(),return_button.GetY(),gRenderer);
            for(int i = 0; i<5; i++)
            {
                upper_map[i].render(upper_map[i].GetX(),upper_map[i].GetY(),gRenderer);
            }
            for(int i = 0; i<5; i++)
            {
                lower_map[i].render(lower_map[i].GetX(),lower_map[i].GetY(),gRenderer);
            }
        }
        else if(tutorial_seeing)
        {
            tutorial.render(tutorial.GetX(),tutorial.GetY(),gRenderer);
            esc_button.render(esc_button.GetX(),esc_button.GetY(),gRenderer);
        }
        else
        {
            background.render(0,0,gRenderer);
            Baba1.render(Baba1.GetX(),Baba1.GetY(),gRenderer);
            Baba2.render(Baba2.GetX(),Baba2.GetY(),gRenderer);
            play_button.render(play_button.GetX(),play_button.GetY(),gRenderer);
            exit_button.render(exit_button.GetX(),exit_button.GetY(),gRenderer);
            tutorial_button.render(tutorial_button.GetX(),tutorial_button.GetY(),gRenderer);
        }

        SDL_RenderPresent(gRenderer);


    }
}

void Game::running()
{
    //Main loop flag
    bool quit = false;
    bool restart = false;
    win = false;
    lost = false;
    //Event handler
    SDL_Event e;

    // init stage
    Stage lvl_1;
    lvl_1.LoadTiles(gRenderer);
    Object baba;
    if(Baba_char == 1)
    {
        baba.loadFromFile(gRenderer,"resource/baba1_right.png");
    }
    else
    {
        baba.loadFromFile(gRenderer,"resource/baba2_right.png");
    }
    baba.MakeSpriteClip(12);
    int Start_time = SDL_GetTicks();

    Object return_menu_button;
    return_menu_button.loadFromFile(gRenderer,"resource/return_menu_button.png");
    return_menu_button.SetX(1248);
    return_menu_button.SetY(0);
    return_menu_button.SetWidth(32);
    return_menu_button.SetHeight(32);

    Object win_stage;
    win_stage.loadFromFile(gRenderer,"resource/passed_stage.png");
    win_stage.SetX(544);
    win_stage.SetY(256);
    win_stage.SetWidth(192);
    win_stage.SetHeight(192);

    Object lost_stage;
    lost_stage.loadFromFile(gRenderer,"resource/lost_stage.png");
    lost_stage.SetX(544);
    lost_stage.SetY(256);
    lost_stage.SetWidth(192);
    lost_stage.SetHeight(192);

    while(playing)
    {
        // Check to move to the next map
        if(win && next_stage)
        {
            next_stage = false;
            playing_map++;
            win = false;
            quit = false;
        }
        char map_file[40];
        sprintf(map_file,"resource/map_resource/map_%d.dat",playing_map);
        lvl_1.LoadStage(map_file);
        lvl_1.ResetStageWin();
        int DEFAULT_X = lvl_1.GetDefaultX();
        int DEFAULT_Y = lvl_1.GetDefaultY();
        baba.SetX(DEFAULT_X);
        baba.SetY(DEFAULT_Y);
        if(restart)
        {
            quit = false;
            restart = false;
            win = false;
            lost = false;
        }
        //While application is running
        while( !quit )
        {
            // RULE CHECK
            lvl_1.CheckRule();
            //Erase the broken tiles every 1 second
            if(SDL_GetTicks() - Start_time >= 1000)
            {
                Start_time = SDL_GetTicks();
                lvl_1.EraseBrokenTile();
            }
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                    playing = false;
                    out_game = true;
                }

                // Manipulate key pressed
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
                    case SDLK_r:
                        quit = true;
                        restart = true;
                        break;
                    default:
                        break;
                    }
                }

                if(e.type == SDL_MOUSEBUTTONDOWN)
                {
                    if(SDL_CommonFunc::CheckFocusWithRect(e.motion.x,e.motion.y,return_menu_button.GetPosRect()))
                    {
                        quit = true;
                        playing = false;
                        lost = false;
                        win = false;
                    }
                    if(win && !lost)
                    {
                        if(playing_map < 10)
                        {
                        SDL_Rect return_button = {544,384,64,64};
                        if(SDL_CommonFunc::CheckFocusWithRect(e.motion.x,e.motion.y,return_button))
                        {
                            win = false;
                            quit = true;
                            playing = false;
                        }
                        SDL_Rect next_stage_button = {672,384,64,64};
                        if(SDL_CommonFunc::CheckFocusWithRect(e.motion.x,e.motion.y,next_stage_button))
                        {
                            quit = true;
                            next_stage = true;
                        }
                        }else if(playing_map == 10)
                        {
                            SDL_Rect return_button = {608,384,64,64};
                            if(SDL_CommonFunc::CheckFocusWithRect(e.motion.x,e.motion.y,return_button))
                            {
                            win = false;
                            quit = true;
                            playing = false;
                            }
                        }
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
                    // Losing condition
                    if(baba.GetX() < 0 && baba.GetY() < 0 && !lvl_1.CheckAlive(BABA_TILE))
                    {
                        lost = true;
                    }
                    // Definitely win
                    if(lvl_1.baba_is_win)
                    {
                    win = true;
                    }
                    if(lvl_1.baba_is_kill || lvl_1.baba_is_sink)
                    {
                        lvl_1.ChangeBlock(BABA_TILE,BROKEN_BABA_TILE);
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                        if(lvl_1.baba_is_sink && !lost)
                        {
                            Sink_Sound_Effect();
                        }else if(lvl_1.baba_is_kill && !lost)
                        {
                            Kill_Sound_Effect();
                        }
                        lost = true;
                    }
                    if(baba.Get_Out_Of_Map())
                    {
                        baba.SetX(DEFAULT_X);
                        baba.SetY(DEFAULT_Y);
                        baba.Change_Out_Of_Map_Ch(false);
                    }
                    baba.HandleEvent(e);
                    if(win || lost)
                    {
                    continue;
                    }
                    lvl_1.ManipulateActionType(baba.GetActionType());
                    // Restrict moving one block per time
                    if(baba.Get_Pressed() == 1 && !lvl_1.baba_is_stop)
                    {
                        baba.MoveObject();
                        lvl_1.MoveStageTile(BABA_TILE);
                    }

                    // Manipulate baba frame
                    if(baba.GetActionType() != -1 && baba.Get_Pressed() == 1)
                        baba.IncreaseFrame();
                    if(baba.GetFrame() > 11)
                    {
                        baba.ReloadFrame();
                    }
                    // Load baba
                    if(lvl_1.baba_is_you)
                    {
                        char baba_file[30];
                        switch(baba.GetStatus())
                        {
                        case baba.MOVELEFT:
                            sprintf(baba_file,"resource/baba%d_left.png",Baba_char);
                            baba.loadFromFile(gRenderer,baba_file);
                            break;
                        case baba.MOVERIGHT:
                            sprintf(baba_file,"resource/baba%d_right.png",Baba_char);
                            baba.loadFromFile(gRenderer,baba_file);
                            break;
                        case baba.MOVEDOWN:
                            sprintf(baba_file,"resource/baba%d_down.png",Baba_char);
                            baba.loadFromFile(gRenderer,baba_file);
                            break;
                        case baba.MOVEUP:
                            sprintf(baba_file,"resource/baba%d_up.png",Baba_char);
                            baba.loadFromFile(gRenderer,baba_file);
                            break;
                        default:
                            sprintf(baba_file,"resource/baba%d_right.png",Baba_char);
                            baba.loadFromFile(gRenderer,baba_file);
                            break;
                        };
                    }

                }
                else if(lvl_1.rock_is_you && !lvl_1.rock_is_stop)
                {
                    if(lvl_1.Get_Pressed() == 1)
                        lvl_1.MoveStageTile(ROCK);

                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    if(lvl_1.rock_is_sink || lvl_1.rock_is_kill || !lvl_1.CheckAlive(ROCK))
                    {
                        lvl_1.ChangeBlock(ROCK,BROKEN_ROCK);
                        if(lvl_1.rock_is_sink && !lost)
                            Sink_Sound_Effect();
                        else if(lvl_1.rock_is_kill && !lost)
                            Kill_Sound_Effect();
                        lost = true;
                    }
                    if(lvl_1.rock_is_win)
                    {
                        win = true;
                    }
                }
                else if(lvl_1.flag_is_you && !lvl_1.flag_is_stop)
                {
                    if(lvl_1.Get_Pressed() == 1)
                        lvl_1.MoveStageTile(FLAG);
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    if(lvl_1.flag_is_kill || lvl_1.flag_is_sink || !lvl_1.CheckAlive(FLAG))
                    {
                        lvl_1.ChangeBlock(FLAG,BROKEN_FLAG);
                        if(lvl_1.flag_is_sink && !lost)
                            Sink_Sound_Effect();
                        else if(lvl_1.flag_is_kill && !lost)
                            Kill_Sound_Effect();
                        lost = true;
                    }
                    if(lvl_1.flag_is_win)
                    {
                        win = true;
                    }
                }
                else if(lvl_1.wall_is_you && !lvl_1.wall_is_stop)
                {
                    if(lvl_1.Get_Pressed() == 1)
                        lvl_1.MoveStageTile(WALL_BLOCK);
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    if(lvl_1.wall_is_kill || lvl_1.wall_is_sink)
                    {
                        for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                        {
                            lvl_1.ChangeBlock(i,BROKEN_WALL);
                        }
                        if(lvl_1.wall_is_sink && !lost)
                            Sink_Sound_Effect();
                        else if(lvl_1.wall_is_kill && !lost)
                            Kill_Sound_Effect();
                        lost = true;
                    }
                    int check = 0;
                    for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                    {
                        check += lvl_1.CheckAlive(i);
                    }
                    if(!check)
                    {
                        lost = true;
                    }
                    if(lvl_1.wall_is_win)
                    {
                        win = true;
                    }
                }
                else if(lvl_1.water_is_you && !lvl_1.water_is_stop)
                {
                    if(lvl_1.Get_Pressed() == 1)
                        lvl_1.MoveStageTile(WATER);
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    if(lvl_1.water_is_kill || lvl_1.water_is_sink || !lvl_1.CheckAlive(WATER))
                    {
                        lvl_1.ChangeBlock(WATER,BROKEN_WATER);
                        if(lvl_1.water_is_sink || !lost)
                            Sink_Sound_Effect();
                        else if(lvl_1.water_is_kill && !lost)
                            Kill_Sound_Effect();
                        lost = true;
                    }
                    if(lvl_1.water_is_win)
                    {
                        win = true;
                    }
                }
                else if(lvl_1.skull_is_you && !lvl_1.skull_is_stop)
                {
                    if(lvl_1.Get_Pressed() == 1)
                        lvl_1.MoveStageTile(SKULL);
                    if(!baba.Get_Out_Of_Map())
                    {
                        lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,BABA_TILE);
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                        baba.Change_Out_Of_Map_Ch(true);
                    }
                    if(lvl_1.skull_is_kill || lvl_1.skull_is_sink || !lvl_1.CheckAlive(SKULL))
                    {
                        lvl_1.ChangeBlock(SKULL,BROKEN_SKULL);
                        if(lvl_1.skull_is_sink && !lost)
                            Sink_Sound_Effect();
                        else if(lvl_1.skull_is_kill && !lost)
                            Kill_Sound_Effect();
                        lost = true;
                    }
                    if(lvl_1.skull_is_win)
                    {
                        win = true;
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
                            baba.SetX(-9999);
                            baba.SetY(-9999);
                            baba.Change_Out_Of_Map_Ch(true);
                        }
                        lvl_1.HandleEvent(e);
                        if(lvl_1.Get_Pressed() == 1)
                            lvl_1.MoveStageTile(FLAG);
                        if(lvl_1.flag_is_kill || lvl_1.flag_is_sink || lvl_1.baba_is_kill || lvl_1.baba_is_sink || !lvl_1.CheckAlive(FLAG))
                        {
                            lvl_1.ChangeBlock(FLAG,BROKEN_FLAG);
                            if(lvl_1.flag_is_sink && !lost)
                                Sink_Sound_Effect();
                            else if(lvl_1.flag_is_kill && !lost)
                                Kill_Sound_Effect();
                            lost = true;
                        }
                        if(lvl_1.baba_is_win)
                        {
                        win = true;
                        }
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
                            baba.SetX(-9999);
                            baba.SetY(-9999);
                            baba.Change_Out_Of_Map_Ch(true);
                        }
                        lvl_1.HandleEvent(e);
                        if(lvl_1.Get_Pressed() == 1)
                            lvl_1.MoveStageTile(WALL_BLOCK);
                        if(lvl_1.wall_is_kill || lvl_1.wall_is_sink || lvl_1.baba_is_kill || lvl_1.baba_is_sink)
                        {
                            for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                            {
                                lvl_1.ChangeBlock(i,BROKEN_WALL);
                            }
                            if(lvl_1.wall_is_sink && !lost)
                                Sink_Sound_Effect();
                            else if(lvl_1.wall_is_kill && !lost)
                                Kill_Sound_Effect();
                            lost = true;
                        }
                        int check = 0;
                        for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                        {
                            check += lvl_1.CheckAlive(i);
                        }
                        if(!check)
                        {
                            lost = true;
                        }
                        if(lvl_1.baba_is_win)
                        {
                        win = true;
                        }
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
                        lvl_1.HandleEvent(e);
                        if(lvl_1.Get_Pressed() == 1)
                            lvl_1.MoveStageTile(ROCK);
                        if(!baba.Get_Out_Of_Map())
                        {
                            lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,ROCK);
                            baba.SetX(-9999);
                            baba.SetY(-9999);
                            baba.Change_Out_Of_Map_Ch(true);
                        }
                        if(lvl_1.rock_is_sink || lvl_1.rock_is_kill || lvl_1.baba_is_kill || lvl_1.baba_is_sink || !lvl_1.CheckAlive(ROCK))
                        {
                            lvl_1.ChangeBlock(ROCK,BROKEN_ROCK);
                            if(lvl_1.rock_is_sink && !lost)
                                Sink_Sound_Effect();
                            else if(lvl_1.rock_is_kill && !lost)
                                Kill_Sound_Effect();
                            lost = true;
                        }
                        if(lvl_1.baba_is_win)
                        {
                        win = true;
                        }

                    }
                    else
                    {
                        lvl_1.ChangeBlock(BABA_TILE,ROCK);
                    }
                }
                else if(lvl_1.baba_is_water)
                {
                    if(lvl_1.baba_is_you)
                    {
                        if(!baba.Get_Out_Of_Map())
                        {
                            lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,WATER);
                            baba.SetX(-9999);
                            baba.SetY(-9999);
                            baba.Change_Out_Of_Map_Ch(true);
                        }
                        if(lvl_1.water_is_sink || lvl_1.water_is_kill || lvl_1.baba_is_kill || lvl_1.baba_is_sink || !lvl_1.CheckAlive(WATER))
                        {
                            lvl_1.ChangeBlock(WATER,BROKEN_WATER);
                            if(lvl_1.water_is_sink && !lost)
                                Sink_Sound_Effect();
                            else if(lvl_1.water_is_kill && !lost)
                                Kill_Sound_Effect();
                            lost = true;
                        }
                        lvl_1.HandleEvent(e);
                        if(lvl_1.Get_Pressed() == 1)
                            lvl_1.MoveStageTile(WATER);
                        if(lvl_1.baba_is_win)
                        {
                        win = true;
                        }
                    }
                    else
                    {
                        lvl_1.ChangeBlock(BABA_TILE,WATER);
                    }
                }
                else if(lvl_1.baba_is_skull)
                {
                    if(lvl_1.baba_is_you)
                    {
                        if(!baba.Get_Out_Of_Map())
                        {
                            lvl_1.ChangePlayerTile((baba.GetX() +1)/64,(baba.GetY()+1)/64,SKULL);
                            baba.SetX(-9999);
                            baba.SetY(-9999);
                            baba.Change_Out_Of_Map_Ch(true);
                        }
                        if(lvl_1.skull_is_sink || lvl_1.skull_is_kill || lvl_1.baba_is_kill || lvl_1.baba_is_sink || !lvl_1.CheckAlive(SKULL))
                        {
                            lvl_1.ChangeBlock(SKULL,BROKEN_SKULL);
                            if(lvl_1.skull_is_sink && !lost)
                                Sink_Sound_Effect();
                            else if(lvl_1.rock_is_kill && !lost)
                                Kill_Sound_Effect();
                            lost = true;
                        }
                        lvl_1.HandleEvent(e);
                        if(lvl_1.Get_Pressed() == 1)
                            lvl_1.MoveStageTile(SKULL);
                        if(lvl_1.baba_is_win)
                        {
                        win = true;
                        }
                    }
                    else
                    {
                        lvl_1.ChangeBlock(BABA_TILE,SKULL);
                    }
                }

            }

    if(lvl_1.CheckSunk())
    {
        Sink_Sound_Effect();
    }
    if(lvl_1.CheckKill())
    {
        Kill_Sound_Effect();
    }
    // Check win for stage move
            if(lvl_1.CheckStageWin())
            {
                win = true;
            }
    //render win stage panel
            if(win && !lost)
            {   if(playing_map == 10)
                {
                    win_stage.loadFromFile(gRenderer,"resource/last_map_win.png");
                }
                win_stage.render(win_stage.GetX(),win_stage.GetY(),gRenderer);
                SDL_RenderPresent(gRenderer);
            }
            // render lost stage panel
            if(lost)
            {
                lost_stage.render(lost_stage.GetX(),lost_stage.GetY(),gRenderer);
                SDL_RenderPresent(gRenderer);
            }

    if(quit || win || lost)
            {
                break;
            }
// Manipulate rule for baba
            if(lvl_1.baba_is_push)
            {
                lvl_1.CheckStagePush(baba,BABA_TILE);
            }

// Manipulate rule for flag
            if(lvl_1.flag_is_stop && lvl_1.baba_is_you)
            {
                lvl_1.CheckStageCollision(baba,FLAG);
            }
            else if(lvl_1.flag_is_push && lvl_1.baba_is_you)
            {
                lvl_1.CheckStagePush(baba,FLAG);
            }
            else if(lvl_1.flag_is_sink)
            {
                if(lvl_1.CheckWin(baba,FLAG))
                {
                    lvl_1.ChangeBlockWithPos(baba,BROKEN_FLAG);
                    if(!baba.Get_Out_Of_Map())
                    {
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                    }
                    Sink_Sound_Effect();
                }
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
            else if(lvl_1.flag_is_kill)
            {
                if(lvl_1.CheckWin(baba,FLAG))
                {
                     baba.SetX(-9999);
                    baba.SetY(-9999);
                    Kill_Sound_Effect();
                }
            }else if(lvl_1.flag_is_win)
            {
                if(lvl_1.CheckWin(baba,FLAG))
                {
                    win = true;
                }
            }

            // Manipulate the rule for the wall
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
            else if(lvl_1.wall_is_sink)
            {
                for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                {
                    if(lvl_1.CheckWin(baba,i))
                    {
                        lvl_1.ChangeBlockWithPos(baba,BROKEN_WALL);
                        if(!baba.Get_Out_Of_Map())
                        {
                            baba.SetX(-9999);
                            baba.SetY(-9999);
                        }
                        Sink_Sound_Effect();
                    }
                }
            }
            else if(lvl_1.wall_is_kill)
            {
                for(int i = HORIZONTAL_WALL; i<=WALL_BLOCK; i++)
                {
                    if(lvl_1.CheckWin(baba,i))
                    {
                         baba.SetX(-9999);
                    baba.SetY(-9999);
                    Kill_Sound_Effect();
                    }
                }
            }else if(lvl_1.wall_is_win)
            {
                for(int wall_object = HORIZONTAL_WALL; wall_object<=WALL_BLOCK; wall_object++)
                    if(lvl_1.CheckWin(baba,wall_object))
                    {
                        win = true;
                    }
            }

// Manipulate the rule for the rock
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
            else if(lvl_1.rock_is_skull)
            {
                lvl_1.ChangeBlock(ROCK,SKULL);
            }
            else if(lvl_1.rock_is_sink)
            {
                if(lvl_1.CheckWin(baba,ROCK))
                {
                    lvl_1.ChangeBlockWithPos(baba,BROKEN_ROCK);
                    if(!baba.Get_Out_Of_Map())
                    {
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                    }
                    Sink_Sound_Effect();
                }
            }
            else if(lvl_1.rock_is_kill)
            {
                if(lvl_1.CheckWin(baba,ROCK))
                {
                     baba.SetX(-9999);
                    baba.SetY(-9999);
                    Kill_Sound_Effect();
                }
            }else if(lvl_1.rock_is_win)
            {
                if(lvl_1.CheckWin(baba,ROCK))
                {
                    win = true;
                }
            }
// Manipulate the rule for the water
            if(lvl_1.water_is_stop && lvl_1.baba_is_you)
            {
                lvl_1.CheckStageCollision(baba,WATER);
            }
            else if(lvl_1.water_is_push && lvl_1.baba_is_you)
            {
                lvl_1.CheckStagePush(baba,WATER);
            }
            if(lvl_1.water_is_flag)
            {
                lvl_1.ChangeBlock(WATER,FLAG);
            }
            else if(lvl_1.water_is_wall)
            {
                lvl_1.ChangeBlock(WATER,WALL_BLOCK);
            }
            else if(lvl_1.water_is_baba)
            {
                lvl_1.ChangeBlock(WATER,BABA_TILE);
            }
            else if(lvl_1.water_is_rock)
            {
                lvl_1.ChangeBlock(WATER,ROCK);
            }
            else if(lvl_1.water_is_skull)
            {
                lvl_1.ChangeBlock(WATER,SKULL);
            }
            else if(lvl_1.water_is_sink)
            {
                if(lvl_1.CheckWin(baba,WATER))
                {
                    lvl_1.ChangeBlockWithPos(baba,BROKEN_WATER);
                    if(!baba.Get_Out_Of_Map())
                    {
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                    }
                    Sink_Sound_Effect();
                }
            }
            else if(lvl_1.water_is_kill)
            {
                if(lvl_1.CheckWin(baba,WATER))
                {
                     baba.SetX(-9999);
                    baba.SetY(-9999);
                    Kill_Sound_Effect();
                }
            }else if(lvl_1.water_is_win)
            {
                if(lvl_1.CheckWin(baba,WATER))
                {
                    win = true;
                }
            }
// Manipulate the rule for the skull
            if(lvl_1.skull_is_stop && lvl_1.baba_is_you)
            {
                lvl_1.CheckStageCollision(baba,SKULL);
            }
            else if(lvl_1.skull_is_push && lvl_1.baba_is_you)
            {
                lvl_1.CheckStagePush(baba,SKULL);
            }
            if(lvl_1.skull_is_flag)
            {
                lvl_1.ChangeBlock(SKULL,FLAG);
            }
            else if(lvl_1.skull_is_wall)
            {
                lvl_1.ChangeBlock(SKULL,WALL_BLOCK);
            }
            else if(lvl_1.skull_is_baba)
            {
                lvl_1.ChangeBlock(SKULL,BABA_TILE);
            }
            else if(lvl_1.skull_is_water)
            {
                lvl_1.ChangeBlock(SKULL,WATER);
            }
            else if(lvl_1.skull_is_rock)
            {
                lvl_1.ChangeBlock(SKULL,ROCK);
            }
            else if(lvl_1.skull_is_sink)
            {
                if(lvl_1.CheckWin(baba,SKULL))
                {
                    lvl_1.ChangeBlockWithPos(baba,BROKEN_SKULL);
                    if(!baba.Get_Out_Of_Map())
                    {
                        baba.SetX(-9999);
                        baba.SetY(-9999);
                    }
                    Sink_Sound_Effect();
                }
            }
            else if(lvl_1.skull_is_kill)
            {
                if(lvl_1.CheckWin(baba,SKULL))
                {
                    baba.SetX(-9999);
                    baba.SetY(-9999);
                    Kill_Sound_Effect();
                }
            }else if(lvl_1.skull_is_win)
            {
                if(lvl_1.CheckWin(baba,SKULL))
                {
                    win = true;
                }
            }

            // Move the text
            for(int p = IS_TEXT; p<=SKULL_TEXT; p++)
            {
                lvl_1.CheckStagePush(baba,p);
            }
            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 75, 92, 28, 0xFF );
            SDL_RenderClear( gRenderer );


            // render stage
            lvl_1.DrawStage(gRenderer);


            // render baba
            if(lvl_1.baba_is_you && (!lvl_1.baba_is_flag && !lvl_1.baba_is_rock && !lvl_1.baba_is_wall && !lvl_1.baba_is_water && !lvl_1.baba_is_skull))
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


            //render return button
            return_menu_button.render(return_menu_button.GetX(),return_menu_button.GetY(),gRenderer);
            //Update screen
            SDL_RenderPresent( gRenderer );
            lvl_1.RestartRule();
        }
    }
}

void Game::Sink_Sound_Effect()
{
    Mix_Chunk* music = NULL;
    //Load Music
    music = Mix_LoadWAV("resource/sfx/sink_sound.wav");
    if (music == NULL)
    {
        printf("%s", Mix_GetError());
    }
    Mix_PlayChannel(-1,music,0);
     Mix_Volume(-1, 32);
}

void Game::Kill_Sound_Effect()
{
    Mix_Chunk* music = NULL;
    //Load Music
    music = Mix_LoadWAV("resource/sfx/kill_sound.wav");
    if (music == NULL)
    {
        printf("%s", Mix_GetError());
    }
    Mix_PlayChannel(-1,music,0);
     Mix_Volume(-1, 32);
}
