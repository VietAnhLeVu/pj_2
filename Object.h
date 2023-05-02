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


    //bool loadFromFile(SDL_Renderer* gRenderer,std::string path,int clip);
    void MakeSpriteClip(int clip);

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

    double GetX()
    {
        return pos.x;
    }

    double GetY()
    {
        return pos.y;
    }

    void SetX(float x)
    {
        pos.x = x;
    }
    void SetY(float y)
    {
        pos.y = y;
    }
    void SetWidth(float w)
    {
        pos.w = w;
    }
    void SetHeight(float h)
    {
        pos.h = h;
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

    void HandleEvent(SDL_Event& e);

    void MoveObject();

    void Manual_MoveObject(int x,int y)
    {
        pos.x = x;
        pos.y = y;
    }

    SDL_Rect GetPosRect()
    {
        return pos;
    }
    int Get_Pressed()
    {
        return pressed;
    }

    bool Get_Out_Of_Map()
    {
        return Out_of_map;
    }
    void Change_Out_Of_Map_Ch(bool cmd)
    {
        Out_of_map = cmd;
    }
private:
    int pressed = 0;
    std::vector <SDL_Rect*> SpriteClips;
    int frame;
    SDL_Event O_event;
    int status;
    SDL_Rect pos;
    int action_type = -1;
    bool Out_of_map;
};

#endif // OBJECT_H_
