#include "Object.h"

Object::Object()
{
frame = 0;
pos.x = 640;
pos.y = 320;

}

Object::~Object()
{

}

bool Object::loadFromFile(SDL_Renderer* gRenderer,std::string path,int clip)
{
     bool load_img = LTexture::loadFromFile(gRenderer,path);
        if(load_img){
        for(int i = 0;i<clip;i++)
        {
            SDL_Rect* temp = new SDL_Rect;
            temp->w = LTexture::getWidth()/clip;
            temp->h = LTexture::getHeight();
            temp->x = i*(LTexture::getWidth()/clip);
            temp->y = 0;
            SpriteClips.push_back(temp);
        }
        }
    return load_img;
}

SDL_Rect* Object::GetSpriteClips()
{
    return SpriteClips[frame%12];
}

void Object::HandleEvent(const SDL_Event& e)
{
    if(e.type == SDL_KEYDOWN)
    {
        switch(e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            action_type = MOVERIGHT;
            //status = MOVERIGHT;
            break;
        case SDLK_LEFT:
            action_type = MOVELEFT;
            //status = MOVELEFT;
            break;
        case SDLK_UP:
            action_type = MOVEUP;
            //status = MOVEUP;
            break;
        case SDLK_DOWN:
            action_type = MOVEDOWN;
            //status = MOVEDOWN;
            break;
        default:
            break;
        }

        switch(action_type)
        {
        case MOVERIGHT:
            status = MOVERIGHT;
            break;
        case MOVELEFT:
            status = MOVELEFT;
            break;
        case MOVEUP:
            status = MOVEUP;
            break;
        case MOVEDOWN:
            status = MOVEDOWN;
            break;
        default:
            break;
        }

    }else if(e.type == SDL_KEYUP)
    {
        action_type = -1;
    }
}

void Object::MoveObject()
{
    switch(action_type)
    {
    case MOVERIGHT:
        pos.x+=64;
        break;
    case MOVELEFT:
        pos.x-=64;
        break;
    case MOVEUP:
        pos.y-=64;
        break;
    case MOVEDOWN:
        pos.y+=64;
        break;
    default:
        break;
    }
}

