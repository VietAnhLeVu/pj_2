#ifndef OBJECT_H_
#define OBJECT_H_

#include "Texture.h"
#include "CommonFunc.h"

class Object:public LTexture
{
public:
    Object();
    ~Object();

    enum MoveType
    {
      MOVELEFT,
      MOVERIGHT,
      MOVEDOWN,
      MOVEUP,
    };


    bool loadFromFile(SDL_Renderer* gRenderer,std::string path,int clip);

    int GetFrame()
    {
        return frame;
    }
    void IncreaseFrame()
    {
        frame++;
    }
    void ReloadFrame()
    {
        frame = 0;
    }

    float GetX()
    {
        return pos.x;
    }

    float GetY()
    {
        return pos.y;
    }

    int GetActionType()
    {
        return action_type;
    }

    int GetStatus()
    {
        return status;
    }

    SDL_Rect* GetSpriteClips();

virtual void HandleEvent(const SDL_Event& e);

    void MoveObject();

    void Manual_MoveObject(int x,int y)
    {
        pos.x = x;
        pos.y = y;
    }
private:

    std::vector <SDL_Rect*> SpriteClips;
    int frame;
    SDL_Event O_event;
    int status;
    SDL_Rect pos;
    int action_type = -1;

};

#endif // OBJECT_H_
