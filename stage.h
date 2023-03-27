#ifndef STAGE_H_
#define STAGE_H_


#include "CommonFunc.h"
#include "Texture.h"
#include "Object.h"

class Tile_type : public Object
{
public:
    Tile_type(){;}
    ~Tile_type(){;}
};

class Stage
{
public:
    Stage();
    ~Stage(){;}

    void LoadStage(std::string file_path);

    void LoadTiles(SDL_Renderer* gRenderer);
    void DrawStage(SDL_Renderer* gRenderer);
    void CheckStageCollision(Object& player);

private:
    Tile_type block[20];
    int stage_block[11][20];
    SDL_Rect stage_block_rect[11][20];
    SDL_Rect dest;
};

#endif // STAGE_H_
