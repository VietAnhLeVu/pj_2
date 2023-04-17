#ifndef COMMONFUNC_H_
#define COMMONFUNC_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <SDL_mixer.h>

#define SCREEN_WIDTH 1280 // 20 block
#define SCREEN_HEIGHT 704 // 11 block
#define MAX_HORIZONTAL_BLOCK 20
#define MAX_VERTICAL_BLOCK 11
#define MAX_BLOCK_TYPE 30
#define DEFAULT_X 384
#define DEFAULT_Y 448
#define DEFAULT_H 64
#define DEFAULT_W 64


enum ObjectType
{
    AIR = 0,
    HORIZONTAL_WALL,
    VERTICAL_WALL,
    RIGHT_DOWN_CORNER_WALL,
    LEFT_DOWN_CORNER_WALL,
    LEFT_UP_CORNER_WALL,
    RIGHT_UP_CORNER_WALL,
    WALL_BLOCK,
    FLAG,
    ROCK,
    IS_TEXT,
    FLAG_TEXT,
    WIN_TEXT,
    YOU_TEXT,
    BABA_TEXT,
    PUSH_TEXT,
    ROCK_TEXT,
    STOP_TEXT,
    WALL_TEXT,

};

namespace SDL_CommonFunc
{
bool CheckFocusWithRect(const int& x,const int& y,const SDL_Rect& rect);

}

#endif // COMMONFUNC_H_
