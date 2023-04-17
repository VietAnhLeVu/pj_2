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
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            file >> stage_block[i][j];
        }
    }


}

void Stage::LoadTiles(SDL_Renderer* gRenderer)
{
    char file_img[20];

    for(int i = 0; i<MAX_BLOCK_TYPE; i++)
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
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            int type = stage_block[i][j];
            dest.x = stage_block_rect[i][j].x = j*64;
            dest.y = stage_block_rect[i][j].y =i*64;
            stage_block_rect[i][j].w = 64;
            stage_block_rect[i][j].h = 64;

            block[type].render(dest.x,dest.y,gRenderer,NULL);
        }
    }
}

void Stage::CheckStageCollision(Object& player,int ObjectType)
{
    /*
    int now_pos_y = (player.GetX())/64;
    int now_pos_x = (player.GetY())/64;
    int now_pos_x2 = (player.GetY()+63)/64;
    int now_pos_y2 = (player.GetX() + 63)/64;
    const SDL_Rect* s1 = &stage_block_rect[now_pos_x][now_pos_y];
    const SDL_Rect s = player.GetPosRect();
    const SDL_Rect* s2 = &s;
    switch(player.GetStatus())
    {
    case Object::MOVELEFT:
      if(stage_block[now_pos_x][now_pos_y])
      if(!SDL_IntersectRect(s1,s2,NULL))
        player.Manual_MoveObject(player.GetX()+64,player.GetY());
    break;
    case Object::MOVERIGHT:
        if(stage_block[now_pos_x][now_pos_y])
        if(!SDL_IntersectRect(s1,s2,NULL))
        player.Manual_MoveObject(player.GetX()-64,player.GetY());
    break;
    case Object::MOVEUP:
      if(stage_block[now_pos_x][now_pos_y])
      if(!SDL_IntersectRect(s1,s2,NULL))
        player.Manual_MoveObject(player.GetX(),player.GetY()+64);
    break;
    case Object::MOVEDOWN:
      if(stage_block[now_pos_x][now_pos_y])
      if(!SDL_IntersectRect(s1,s2,NULL))
        player.Manual_MoveObject(player.GetX(),player.GetY()-64);
    break;
    default:
    break;
    };
    */
    int now_pos_y = (player.GetX()+1)/64;
    int now_pos_x = (player.GetY()+1)/64;
    int now_pos_x2 = (player.GetY()+63)/64;
    int now_pos_y2 = (player.GetX() + 63)/64;

    switch(player.GetStatus())
    {
    case Object::MOVELEFT:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType )
            player.Manual_MoveObject(player.GetX()+64,player.GetY());
        break;
    case Object::MOVERIGHT:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType )
            player.Manual_MoveObject(player.GetX()-64,player.GetY());
        break;
    case Object::MOVEUP:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType )
            player.Manual_MoveObject(player.GetX(),player.GetY()+64);
        break;
    case Object::MOVEDOWN:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType )
            player.Manual_MoveObject(player.GetX(),player.GetY()-64);
        break;
    default:
        break;
    };
}


bool Stage::CheckStagePush(Object& player,int ObjectType)
{
    int now_pos_y = (player.GetX()+1)/64;
    int now_pos_x = (player.GetY()+1)/64;
    int now_pos_x2 = (player.GetY()+63)/64;
    int now_pos_y2 = (player.GetX() + 63)/64;

    switch(player.GetStatus())
    {
    case Object::MOVELEFT:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType)
        {
            if(now_pos_y -1 >= 0 && stage_block[now_pos_x][now_pos_y-1] == AIR)
            {
                std::swap(stage_block[now_pos_x][now_pos_y-1],stage_block[now_pos_x][now_pos_y]);
            }
            else if(stage_block[now_pos_x][now_pos_y-1] != AIR)
            {
                int block_check = 1;
                while(now_pos_y-block_check >=0 && stage_block[now_pos_x][now_pos_y-block_check] != AIR)
                {
                    switch(stage_block[now_pos_x][now_pos_y-block_check])
                    {
                    case FLAG:
                        if(flag_is_stop)
                            block_check = now_pos_y;
                        break;
                    case ROCK:
                        if(rock_is_stop)
                            block_check = now_pos_y;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                    {
                        if(wall_is_stop)
                            block_check = now_pos_y;
                    }
                    block_check++;

                }
                if(now_pos_y-block_check <= -1)
                {
                    CheckStageCollision(player,ObjectType);
                }
                else
                {
                    int current_air_y = now_pos_y-block_check;
                    while(block_check != -1)
                    {
                        if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <=WALL_TEXT)
                        {
                            std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                            current_air_y = now_pos_y-block_check;
                        }
                        switch(stage_block[now_pos_x][now_pos_y-block_check])
                        {
                        case FLAG:
                            if(flag_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y-block_check;
                            }
                            else if(!flag_is_push && !flag_is_stop)
                            {
                                if(stage_block[now_pos_x][now_pos_y-block_check+1] == FLAG)
                                {

                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check+1] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y-block_check+1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y-block_check+1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y-block_check;
                            }
                            else if(!rock_is_push && !rock_is_stop)
                            {
                                if(stage_block[now_pos_x][now_pos_y-block_check+1] == ROCK)
                                {

                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check+1] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y-block_check+1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y-block_check+1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                            break;
                        default:
                            break;
                        };
                        if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                        {
                            if(wall_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y-block_check;
                            }
                            else if(!wall_is_push && !wall_is_stop)
                            {
                                if(stage_block[now_pos_x][now_pos_y-block_check+1] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check+1] <= WALL_BLOCK)
                                {

                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check+1] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y-block_check+1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check+1] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y-block_check+1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                        }
                        block_check--;
                    }
                }
            }
            else
            {
                CheckStageCollision(player,ObjectType);
            }
        }
        else
        {
            CheckStageCollision(player,ObjectType);
        }
        break;
    case Object::MOVERIGHT:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType)
        {
            if(now_pos_y +1 < 20 && stage_block[now_pos_x][now_pos_y+1] == AIR)
            {
                std::swap(stage_block[now_pos_x][now_pos_y+1],stage_block[now_pos_x][now_pos_y]);
            }
            else if(stage_block[now_pos_x][now_pos_y+1] != AIR)
            {
                int block_check = 1;
                while(now_pos_y+block_check < MAX_HORIZONTAL_BLOCK && stage_block[now_pos_x][now_pos_y+block_check] != AIR)
                {
                    switch(stage_block[now_pos_x][now_pos_y+block_check])
                    {
                    case FLAG:
                        if(flag_is_stop)
                            block_check = now_pos_y;
                        break;
                    case ROCK:
                        if(rock_is_stop)
                            block_check = now_pos_y;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                    {
                        if(wall_is_stop)
                            block_check = now_pos_y;
                    }
                    block_check++;

                }
                if(now_pos_y+block_check >= MAX_HORIZONTAL_BLOCK)
                {
                    CheckStageCollision(player,ObjectType);
                }
                else
                {
                    int current_air_y = now_pos_y+block_check;
                    while(block_check != -1)
                    {
                        if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_TEXT)
                        {
                            std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                            current_air_y = now_pos_y+block_check;
                        }
                        switch(stage_block[now_pos_x][now_pos_y+block_check])
                        {
                        case FLAG:
                            if(flag_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y+block_check;
                            }
                            else if(!flag_is_push && !flag_is_stop)
                            {
                                if(stage_block[now_pos_x][now_pos_y+block_check-1] == FLAG)
                                {

                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check-1] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y+block_check-1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y+block_check-1;
                                    }

                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check-1] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check-1] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y+block_check-1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y+block_check-1;
                                    }

                                }
                            }
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y+block_check;
                            }
                            else if(!rock_is_push && !rock_is_stop)
                            {
                                if(stage_block[now_pos_x][now_pos_y+block_check-1] == ROCK)
                                {

                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check-1] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y+block_check-1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y+block_check-1;
                                    }

                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check-1] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check-1] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y+block_check-1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y+block_check-1;
                                    }

                                }
                            }
                            break;
                        default:
                            break;
                        };
                        if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                        {
                            if(wall_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y+block_check;
                            }
                            else if(!wall_is_push && !wall_is_stop)
                            {
                                if(stage_block[now_pos_x][now_pos_y+block_check-1] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check-1] <= WALL_BLOCK)
                                {

                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check-1] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y+block_check-1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y+block_check-1;
                                    }
                                    else
                                    {

                                    }
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check+1] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x][now_pos_y+block_check-1],stage_block[now_pos_x][current_air_y]);
                                        current_air_y = now_pos_y+block_check - 1;
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                        }
                        block_check--;
                    }
                }
            }
            else
            {
                CheckStageCollision(player,ObjectType);
            }
        }
        break;
    case Object::MOVEUP:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType)
        {
            if(now_pos_x -1 >= 0 && stage_block[now_pos_x-1][now_pos_y] == AIR)
            {
                std::swap(stage_block[now_pos_x-1][now_pos_y],stage_block[now_pos_x][now_pos_y]);
            }
            else if(stage_block[now_pos_x-1][now_pos_y] != AIR)
            {
                int block_check = 1;
                while(now_pos_x-block_check >= 0 && stage_block[now_pos_x-block_check][now_pos_y] != AIR)
                {
                    switch(stage_block[now_pos_x-block_check][now_pos_y])
                    {
                    case FLAG:
                        if(flag_is_stop)
                            block_check = now_pos_x;
                        break;
                    case ROCK:
                        if(rock_is_stop)
                            block_check = now_pos_x;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                    {
                        if(wall_is_stop)
                        {
                            block_check = now_pos_x;
                        }
                    }
                    block_check++;

                }
                if(now_pos_x-block_check < 0)
                {
                    CheckStageCollision(player,ObjectType);
                }
                else
                {
                    int current_air_x = now_pos_x-block_check;
                    while(block_check != -1)
                    {
                        if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_TEXT)
                        {
                            std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                            current_air_x = now_pos_x-block_check;
                        }
                        switch(stage_block[now_pos_x-block_check][now_pos_y])
                        {
                        case FLAG:
                            if(flag_is_push)
                            {
                                std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x-block_check;
                            }
                            else if(!flag_is_push && !flag_is_stop)
                            {
                                if(stage_block[now_pos_x-block_check+1][now_pos_y] == FLAG)
                                {

                                }
                                else if(stage_block[now_pos_x-block_check+1][now_pos_y] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x-block_check+1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x-block_check+1;
                                    }

                                }
                                else if(stage_block[now_pos_x-block_check+1][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check+1][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x-block_check+1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x-block_check+1;
                                    }

                                }
                            }
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x-block_check;
                            }
                            else if(!rock_is_push && !rock_is_stop)
                            {
                                if(stage_block[now_pos_x-block_check+1][now_pos_y] == ROCK)
                                {

                                }
                                else if(stage_block[now_pos_x-block_check+1][now_pos_y] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x-block_check+1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x-block_check+1;
                                    }

                                }
                                else if(stage_block[now_pos_x-block_check+1][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check+1][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x-block_check+1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x-block_check+1;
                                    }

                                }
                            }
                            break;
                        default:
                            break;
                        };
                        if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                        {
                            if(wall_is_push)
                            {
                                std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x-block_check;
                            }
                            else if(!wall_is_push && !wall_is_stop)
                            {
                                if(stage_block[now_pos_x-block_check+1][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check+1][now_pos_y] <= WALL_BLOCK)
                                {

                                }
                                else if(stage_block[now_pos_x-block_check+1][now_pos_y] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x-block_check+1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x-block_check+1;
                                    }
                                    else
                                    {

                                    }
                                }
                                else if(stage_block[now_pos_x-block_check+1][now_pos_y] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x-block_check+1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x-block_check + 1;
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                        }
                        block_check--;
                    }
                }
            }
            else
            {
                CheckStageCollision(player,ObjectType);
            }
        }
        break;
    case Object::MOVEDOWN:
        if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType)
        {
            if(now_pos_x+1 < MAX_VERTICAL_BLOCK && stage_block[now_pos_x+1][now_pos_y] == AIR)
            {
                std::swap(stage_block[now_pos_x+1][now_pos_y],stage_block[now_pos_x][now_pos_y]);
            }
            else if(stage_block[now_pos_x+1][now_pos_y] != AIR)
            {
                int block_check = 1;
                while(now_pos_x+block_check < MAX_VERTICAL_BLOCK && stage_block[now_pos_x+block_check][now_pos_y] != AIR)
                {
                    switch(stage_block[now_pos_x+block_check][now_pos_y])
                    {
                    case FLAG:
                        if(flag_is_stop)
                            block_check = now_pos_x;
                        break;
                    case ROCK:
                        if(rock_is_stop)
                            block_check = now_pos_x;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                    {
                        if(wall_is_stop)
                            block_check = now_pos_x;
                    }
                    block_check++;

                }
                if(now_pos_x+block_check >= MAX_VERTICAL_BLOCK)
                {
                    CheckStageCollision(player,ObjectType);
                }
                else
                {
                    int current_air_x = now_pos_x+block_check;
                    while(block_check != -1)
                    {
                        if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_TEXT)
                        {
                            std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                            current_air_x = now_pos_x+block_check;
                        }
                        switch(stage_block[now_pos_x+block_check][now_pos_y])
                        {
                        case FLAG:
                            if(flag_is_push)
                            {
                                std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x+block_check;
                            }
                            else if(!flag_is_push && !flag_is_stop)
                            {
                                if(stage_block[now_pos_x+block_check-1][now_pos_y] == FLAG)
                                {

                                }
                                else if(stage_block[now_pos_x+block_check-1][now_pos_y] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x+block_check-1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x+block_check-1;
                                    }

                                }
                                else if(stage_block[now_pos_x+block_check-1][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check-1][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x+block_check-1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x+block_check-1;
                                    }

                                }
                            }
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x+block_check;
                            }
                            else if(!rock_is_push && !rock_is_stop)
                            {
                                if(stage_block[now_pos_x+block_check-1][now_pos_y] == ROCK)
                                {

                                }
                                else if(stage_block[now_pos_x+block_check-1][now_pos_y] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x+block_check-1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x+block_check-1;
                                    }

                                }
                                else if(stage_block[now_pos_x+block_check-1][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check-1][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x+block_check-1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x+block_check-1;
                                    }

                                }
                            }
                            break;
                        default:
                            break;
                        };
                        if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                        {
                            if(wall_is_push)
                            {
                                std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x+block_check;
                            }
                            else if(!wall_is_push && !wall_is_stop)
                            {
                                if(stage_block[now_pos_x+block_check-1][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check-1][now_pos_y] <= WALL_BLOCK)
                                {

                                }
                                else if(stage_block[now_pos_x+block_check-1][now_pos_y] == ROCK)
                                {
                                    if(rock_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x+block_check-1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x+block_check-1;
                                    }
                                    else
                                    {

                                    }
                                }
                                else if(stage_block[now_pos_x+block_check-1][now_pos_y] == FLAG)
                                {
                                    if(flag_is_push)
                                    {
                                        std::swap(stage_block[now_pos_x+block_check-1][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                        current_air_x = now_pos_x+block_check - 1;
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                        }
                        block_check--;
                    }
                }
            }
            else
            {
                CheckStageCollision(player,ObjectType);
            }
        }
        break;
    default:
        break;
    };

}

void Stage::Copy_Stage_Block(int facsimile[][MAX_HORIZONTAL_BLOCK])
{
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            facsimile[i][j] = stage_block[i][j];
        }
    }
}

void Stage::HandleEvent(SDL_Event& e)
{
    if(e.type == SDL_KEYDOWN)
    {
        switch(e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            if(action_type == -1)
                action_type = MOVERIGHT;
            break;
        case SDLK_LEFT:
            if(action_type == -1)
                action_type = MOVELEFT;
            break;
        case SDLK_UP:
            if(action_type == -1)
                action_type = MOVEUP;
            break;
        case SDLK_DOWN:
            if(action_type == -1)
                action_type = MOVEDOWN;
            break;
        default:
            break;
        }
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
            action_type = -1;
    }
}

void Stage::MoveStageTile(int ObjectType)
{
    switch(action_type)
    {
    case MOVERIGHT:
        for(int i = MAX_VERTICAL_BLOCK-1; i>=0; i--)
        {
            for(int j = MAX_HORIZONTAL_BLOCK-1; j>=0; j--)
            {
                if(ObjectType == WALL_BLOCK)
                {
                    if(stage_block[i][j] >= HORIZONTAL_WALL && stage_block[i][j] <= WALL_BLOCK)
                    {
                        if(j != MAX_HORIZONTAL_BLOCK-1)
                            std::swap(stage_block[i][j],stage_block[i][j+1]);
                    }

                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(j != MAX_HORIZONTAL_BLOCK-1)
                            std::swap(stage_block[i][j],stage_block[i][j+1]);
                    }
                }
            }
        }
        break;
    case MOVELEFT:
        for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
        {
            for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
            {
                if(ObjectType == WALL_BLOCK)
                {
                    if(stage_block[i][j] >= HORIZONTAL_WALL && stage_block[i][j] <= WALL_BLOCK)
                    {
                        if(j != 0)
                            std::swap(stage_block[i][j],stage_block[i][j-1]);
                    }
                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(j != 0)
                            std::swap(stage_block[i][j],stage_block[i][j-1]);
                    }
                }
            }
        }
        break;
    case MOVEUP:
        for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
        {
            for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
            {
                if(ObjectType == WALL_BLOCK)
                {
                    if(stage_block[i][j] >= HORIZONTAL_WALL && stage_block[i][j] <= WALL_BLOCK)
                    {
                        if(i != 0)
                            std::swap(stage_block[i][j],stage_block[i-1][j]);
                    }
                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(i != 0)
                            std::swap(stage_block[i][j],stage_block[i-1][j]);
                    }
                }
            }

        }
        break;
    case MOVEDOWN:
        for(int i = MAX_VERTICAL_BLOCK-1; i>=0; i--)
        {
            for(int j = MAX_HORIZONTAL_BLOCK-1; j>=0; j--)
            {
                if(ObjectType == WALL_BLOCK)
                {
                    if(stage_block[i][j] >= HORIZONTAL_WALL && stage_block[i][j] <= WALL_BLOCK)
                    {
                        if(i != MAX_VERTICAL_BLOCK-1)
                            std::swap(stage_block[i][j],stage_block[i+1][j]);
                    }
                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(i != MAX_VERTICAL_BLOCK-1)
                            std::swap(stage_block[i][j],stage_block[i+1][j]);
                    }
                }
            }
        }
        break;
    default:
        break;
    };
}

bool Stage::CheckWin(Object& player,int ObjectType)
{
    int now_pos_y = (player.GetX()+1)/64;
    int now_pos_x = (player.GetY()+1)/64;
    int now_pos_x2 = (player.GetY()+63)/64;
    int now_pos_y2 = (player.GetX() + 63)/64;
    if(stage_block[now_pos_x][now_pos_y] == ObjectType || stage_block[now_pos_x2][now_pos_y] == ObjectType  || stage_block[now_pos_x][now_pos_y2] == ObjectType || stage_block[now_pos_x2][now_pos_y2] == ObjectType )
    {
        return true;
    }
    return false;
}

void Stage::ChangeBlock(int ObjectType_src,int ObjectType_dst)
{
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            if(stage_block[i][j] == ObjectType_src)
                stage_block[i][j] = ObjectType_dst;
        }
    }
}

void Stage::RestartRule()
{
    // rule for baba
    baba_is_you = false;
    baba_is_win = false;
    baba_is_stop = false;
    baba_is_push = false;
    baba_is_flag = false;
    baba_is_wall = false;
    baba_is_rock = false;
// rule for flag
    flag_is_you = false;
    flag_is_win = false;
    flag_is_stop = false;
    flag_is_push = false;
    flag_is_baba = false;
    flag_is_wall = false;
    flag_is_rock = false;
// rule for wall
    wall_is_you = false;
    wall_is_win = false;
    wall_is_stop = false;
    wall_is_push = false;
    wall_is_flag = false;
    wall_is_baba = false;
    wall_is_rock = false;
// rule for rock
    rock_is_you = false;
    rock_is_win = false;
    rock_is_stop = false;
    rock_is_push = false;
    rock_is_flag = false;
    rock_is_wall = false;
    rock_is_baba = false;
}
