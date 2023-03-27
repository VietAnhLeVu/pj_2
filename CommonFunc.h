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

#define SCREEN_WIDTH 1280 // 20 block
#define SCREEN_HEIGHT 720 // 11 block
#define MAX_HORIZONTAL_BLOCK 20
#define MAX_VERTICAL_BLOCK 11

namespace SDL_CommonFunc
{
    bool CheckFocusWithRect(const int& x,const int& y,const SDL_Rect& rect);

}

#endif // COMMONFUNC_H_
