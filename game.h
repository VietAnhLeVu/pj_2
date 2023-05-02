#ifndef GAME_H_
#define GAME_H_

#include "Texture.h"
#include "CommonFunc.h"
#include "Object.h"
#include "stage.h"


class Game
{
public:
    Game(){;}
    ~Game(){;}
    bool init();
    void close();
    void running();
    void music();
    void menu();
    void Sink_Sound_Effect();
    void Kill_Sound_Effect();
    bool Get_GameOut_Request()
    {
        return out_game;
    }
private:
bool music_on =true;
bool out_game = false;
bool playing = false;
bool  win = false;
bool lost = false;
bool next_stage = false;

int playing_map;
int Baba_char =1;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

};

#endif // GAME_H_
