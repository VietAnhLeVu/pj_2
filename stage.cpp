#include "stage.h"

Stage::Stage()
{
    dest.x = 0;
    dest.y = 0;
    dest.w = 64;
    dest.h = 64;
}
void Stage::LoadStage(std::string file_path)
{
    std::ifstream file(file_path);
    if(!file.is_open())
    {
        std::cout << "Cannot load stage file !" << std::endl;
    }
    for(int i = 0;i<MAX_VERTICAL_BLOCK;i++)
    {
        for(int j = 0;j<MAX_HORIZONTAL_BLOCK;j++)
        {
            file >> stage_block[i][j];
        }
    }
}

void Stage::LoadTiles(SDL_Renderer* gRenderer)
{
    char file_img[20];

    for(int i = 0;i<20;i++)
    {
        sprintf(file_img,"resource/%d.png",i);
        std::ifstream file(file_img);
        if(!file.is_open())
        {
            continue;
        }

        file.close();

        block[i].LTexture::loadFromFile(gRenderer,file_img);
    }
}

void Stage::DrawStage(SDL_Renderer* gRenderer)
{
    for(int i = 0;i<MAX_VERTICAL_BLOCK;i++)
    {
        for(int j = 0;j<MAX_HORIZONTAL_BLOCK;j++)
        {
           int type = stage_block[i][j];
           dest.x = j*64;
           dest.y = i*64;
           block[type].render(dest.x,dest.y,gRenderer,NULL);
         }
    }
}

void Stage::CheckStageCollision(Object& player)
{
    int now_pos_y = (player.GetX())/64;
    int now_pos_x = (player.GetY())/64;
    switch(player.GetStatus())
    {
  case Object::MOVELEFT:
    if(stage_block[now_pos_x][now_pos_y] != 0)
        player.Manual_MoveObject(player.GetX()+64,player.GetY());
    break;
  case Object::MOVERIGHT:
    if(stage_block[now_pos_x][now_pos_y] != 0)
        player.Manual_MoveObject(player.GetX()-64,player.GetY());
    break;
  case Object::MOVEUP:
    if(stage_block[now_pos_x][now_pos_y] != 0)
        player.Manual_MoveObject(player.GetX(),player.GetY()+64);
    break;
  case Object::MOVEDOWN:
    if(stage_block[now_pos_x][now_pos_y] != 0)
        player.Manual_MoveObject(player.GetX(),player.GetY()-64);
    break;
  default:
    break;
    };
}
