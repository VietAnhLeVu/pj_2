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
private:
bool music_on =true;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

};

#endif // GAME_H_
