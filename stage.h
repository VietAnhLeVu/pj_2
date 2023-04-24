#ifndef STAGE_H_
#define STAGE_H_


#include "CommonFunc.h"
#include "Texture.h"
#include "Object.h"

class Tile_type : public Object
{
public:
    Tile_type() {;}
    ~Tile_type() {;}
};

class Stage
{
public:
    Stage();
    ~Stage() {;}
    enum StageMoveType
    {
        MOVELEFT,
        MOVERIGHT,
        MOVEDOWN,
        MOVEUP,
    };
// rule for baba
    bool baba_is_you = false;
    bool baba_is_win = false;
    bool baba_is_stop = false;
    bool baba_is_push = false;
    bool baba_is_flag = false;
    bool baba_is_wall = false;
    bool baba_is_rock = false;
    bool baba_is_sink = false;
    bool baba_is_kill = false;
    bool baba_is_water = false;
    bool baba_is_skull = false;
// rule for flag
    bool flag_is_you = false;
    bool flag_is_win = false;
    bool flag_is_stop = false;
    bool flag_is_push = false;
    bool flag_is_baba = false;
    bool flag_is_wall = false;
    bool flag_is_rock = false;
    bool flag_is_sink = false;
    bool flag_is_kill = false;
    bool flag_is_water = false;
    bool flag_is_skull = false;
// rule for wall
    bool wall_is_you = false;
    bool wall_is_win = false;
    bool wall_is_stop = false;
    bool wall_is_push = false;
    bool wall_is_flag = false;
    bool wall_is_baba = false;
    bool wall_is_rock = false;
    bool wall_is_sink = false;
    bool wall_is_kill = false;
    bool wall_is_water = false;
    bool wall_is_skull = false;
// rule for rock
    bool rock_is_you = false;
    bool rock_is_win = false;
    bool rock_is_stop = false;
    bool rock_is_push = false;
    bool rock_is_flag = false;
    bool rock_is_wall = false;
    bool rock_is_baba = false;
    bool rock_is_sink = false;
    bool rock_is_kill = false;
    bool rock_is_water = false;
    bool rock_is_skull = false;
// rule for water
    bool water_is_you = false;
    bool water_is_win = false;
    bool water_is_stop = false;
    bool water_is_push = false;
    bool water_is_flag = false;
    bool water_is_wall = false;
    bool water_is_baba = false;
    bool water_is_sink = false;
    bool water_is_kill = false;
    bool water_is_rock = false;
    bool water_is_skull = false;
// rule for skull
    bool skull_is_you = false;
    bool skull_is_win = false;
    bool skull_is_stop = false;
    bool skull_is_push = false;
    bool skull_is_flag = false;
    bool skull_is_wall = false;
    bool skull_is_baba = false;
    bool skull_is_sink = false;
    bool skull_is_kill = false;
    bool skull_is_water = false;
    bool skull_is_rock = false;

    void LoadStage(std::string file_path);

    void LoadTiles(SDL_Renderer* gRenderer);
    void DrawStage(SDL_Renderer* gRenderer);

    //RULE
    void CheckStageCollision(Object& player,int ObjectType);
    void CheckStagePush(Object& player,int ObjectType);

    void HandleEvent(SDL_Event& e);
    void MoveStageTile(int ObjectType);
    void RestartRule();
    void CheckRule();
    bool CheckWin(Object& player,int ObjectType);
    void ChangeBlock(int ObjectType_src,int ObjectType_dst);
    void ManipulateActionType(int src_action_type)
    {
        action_type = src_action_type;
    }
    void ChangePlayerTile(int y,int x,int type)
    {
        stage_block[x][y] = type;
    }
private:
    int action_type = -1;
    Tile_type block[MAX_BLOCK_TYPE];
    int stage_block[11][20];
    SDL_Rect stage_block_rect[11][20];
    SDL_Rect dest;
};

#endif // STAGE_H_
