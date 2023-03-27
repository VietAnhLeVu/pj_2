#include "menu.h"

bool Button::Button()
{
    pos.w = mWidth;
    pos.h = mHeight;
}

bool Button::HandleEvent(){
    bool play = false;
    while(!play)
    {
        while(SDL_PollEvent(&m_event))
        {
            switch(m_event.type)
            {
            case SDL_QUIT:
                return 0;
                break;
            case SDL_MOUSEMOTION:
                {
                xm = m_event.motion.x;
                ym = m_event.motion.y;
                if(SDL_CommonFunc::CheckFocusWithRect(xm,ym,pos))
                {
                    pos.y += 16;
                }

                }
                break;
            default:
                break;
            }
        }
    }
}
