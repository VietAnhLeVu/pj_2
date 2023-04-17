#include "Object.h"

Object::Object()
{
    frame = 0;
    pos.x = DEFAULT_X;
    pos.y = DEFAULT_Y;
    pos.h = DEFAULT_H;
    pos.w = DEFAULT_W;

}

Object::~Object()
{

}

bool Object::loadFromFile(SDL_Renderer* gRenderer,std::string path,int clip)
{
    bool load_img = LTexture::loadFromFile(gRenderer,path);
    if(load_img)
    {
        for(int i = 0; i<clip; i++)
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
    return SpriteClips[frame];
}

void Object::HandleEvent(SDL_Event& e)
{
    if(e.type == SDL_KEYDOWN)
    {
        switch(e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            if(action_type == -1)
                action_type = MOVERIGHT;
            //status = MOVERIGHT;
            break;
        case SDLK_LEFT:
            if(action_type == -1)
                action_type = MOVELEFT;
            //status = MOVELEFT;
            break;
        case SDLK_UP:
            if(action_type == -1)
                action_type = MOVEUP;
            //status = MOVEUP;
            break;
        case SDLK_DOWN:
            if(action_type == -1)
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

        // Here to restric moving one block per time
        pressed++;

        // Prevent other keys from being taken in
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                continue;
            }
        }
    }
    else if(e.type == SDL_KEYUP)
    {
        if(action_type != -1)
        {
            action_type = -1;
            pressed = 0;
        }
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

