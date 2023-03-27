#ifndef MENU_H_
#define MENU_H_

#include "Texture.h"
#include "Object.h"
#include "CommonFunc.h"

class Button: public Object
{
public:
Button();
~Button(){;}

bool HandleEvent();
private:
SDL_Event m_event;
};

#endif // MENU_H_
