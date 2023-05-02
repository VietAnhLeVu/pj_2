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
    file >> DEFAULT_X;
    file >> DEFAULT_Y;


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
            ;
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


void Stage::CheckStagePush(Object& player,int ObjectType)
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
                        if(flag_is_sink && !flag_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y-block_check] = BROKEN_FLAG;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y-block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y-block_check] = AIR;
                            block_check--;
                        }
                        else if(flag_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
                        break;
                    case ROCK:
                        if(rock_is_sink && !rock_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y-block_check] = BROKEN_ROCK;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y-block_check])
                                {
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y-block_check] = AIR;
                            block_check--;
                        }
                        else if(rock_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
                        break;
                    case WATER:
                        if(water_is_sink && !water_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y-block_check] = BROKEN_WATER;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y-block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y-block_check] = AIR;
                            block_check--;
                        }
                        else if(water_is_stop)
                        {
                            block_check = MAX_HORIZONTAL_BLOCK;
                        }
                        break;
                    case SKULL:
                        if(skull_is_sink && !skull_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y-block_check] = BROKEN_SKULL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y-block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y-block_check] = AIR;
                            block_check--;
                        }
                        else if(skull_is_stop)
                        {
                            block_check = MAX_HORIZONTAL_BLOCK;
                        }
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x][now_pos_y-block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y-block_check] <= WALL_BLOCK)
                    {
                        if(wall_is_sink && !wall_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y-block_check] = BROKEN_WALL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y-block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y-block_check] = AIR;
                            block_check--;
                        }
                        else if(wall_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
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
                        if(stage_block[now_pos_x][now_pos_y-block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y-block_check] <=SKULL_TEXT)
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
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y-block_check;
                            }
                            break;
                        case WATER:
                            if(water_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y-block_check;
                            }
                            break;
                        case SKULL:
                            if(skull_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y-block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y-block_check;
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
                        if(flag_is_sink && !flag_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y+block_check] = BROKEN_FLAG;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y+block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y+block_check] = AIR;
                            block_check--;
                        }
                        else if(flag_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
                        break;
                    case ROCK:
                        if(rock_is_sink && !rock_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y+block_check] = BROKEN_ROCK;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y+block_check])
                                {
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y+block_check] = AIR;
                            block_check--;
                        }
                        else if(rock_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
                        break;
                    case WATER:
                        if(water_is_sink && !water_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y+block_check] = BROKEN_WATER;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y+block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y+block_check] = AIR;
                            block_check--;
                        }
                        else if(water_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
                        break;
                    case SKULL:
                        if(skull_is_sink && !skull_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y+block_check] = BROKEN_SKULL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y+block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y+block_check] = AIR;
                            block_check--;
                        }
                        else if(skull_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x][now_pos_y+block_check] >= HORIZONTAL_WALL && stage_block[now_pos_x][now_pos_y+block_check] <= WALL_BLOCK)
                    {
                        if(wall_is_sink && !wall_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x][now_pos_y+block_check] = BROKEN_WALL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x][now_pos_y+block_check])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x][now_pos_y+block_check] = AIR;
                            block_check--;
                        }
                        else if(wall_is_stop)
                            block_check = MAX_HORIZONTAL_BLOCK;
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
                        if(stage_block[now_pos_x][now_pos_y+block_check] >= IS_TEXT && stage_block[now_pos_x][now_pos_y+block_check] <= SKULL_TEXT)
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
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y+block_check;
                            }
                            break;
                        case WATER:
                            if(water_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y+block_check;
                            }
                            break;
                        case SKULL:
                            if(skull_is_push)
                            {
                                std::swap(stage_block[now_pos_x][now_pos_y+block_check],stage_block[now_pos_x][current_air_y]);
                                current_air_y = now_pos_y+block_check;
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
                        if(flag_is_sink && !flag_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x-block_check][now_pos_y] = BROKEN_FLAG;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x-block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x-block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(flag_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    case ROCK:
                        if(rock_is_sink && !rock_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x-block_check][now_pos_y] = BROKEN_ROCK;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x-block_check][now_pos_y])
                                {
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x-block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(rock_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    case WATER:
                        if(water_is_sink)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x-block_check][now_pos_y] = BROKEN_WATER;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x-block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x-block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(water_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    case SKULL:
                        if(skull_is_sink && !skull_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x-block_check][now_pos_y] = BROKEN_SKULL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x-block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x-block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(skull_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x-block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x-block_check][now_pos_y] <= WALL_BLOCK)
                    {
                        if(wall_is_sink && !wall_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x-block_check][now_pos_y] = BROKEN_WALL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x-block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x-block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(wall_is_stop)
                        {
                            block_check = MAX_VERTICAL_BLOCK;
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
                        if(stage_block[now_pos_x-block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x-block_check][now_pos_y] <= SKULL_TEXT)
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
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x-block_check;
                            }
                            break;
                        case WATER:
                            if(water_is_push)
                            {
                                std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x-block_check;
                            }
                            break;
                        case SKULL:
                            if(skull_is_push)
                            {
                                std::swap(stage_block[now_pos_x-block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x-block_check;
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
                        if(flag_is_sink && !flag_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x+block_check][now_pos_y] = BROKEN_FLAG;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x+block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x+block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(flag_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    case ROCK:
                        if(rock_is_sink && !rock_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x+block_check][now_pos_y] = BROKEN_ROCK;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x+block_check][now_pos_y])
                                {
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x+block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(rock_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    case WATER:
                        if(water_is_sink && !water_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x+block_check][now_pos_y] = BROKEN_WATER;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x+block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x+block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(water_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    case SKULL:
                        if(skull_is_sink && !skull_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x+block_check][now_pos_y] = BROKEN_SKULL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x+block_check][now_pos_y])
                                {
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                                {
                                    if(wall_is_push)
                                        quit_loop = true;
                                }
                                else if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x+block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(skull_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
                        break;
                    default:
                        break;
                    };
                    if(stage_block[now_pos_x+block_check][now_pos_y] >= HORIZONTAL_WALL && stage_block[now_pos_x+block_check][now_pos_y] <= WALL_BLOCK)
                    {
                        if(wall_is_sink && !wall_is_push)
                        {
                            sth_is_sunk = true;
                            stage_block[now_pos_x+block_check][now_pos_y] = BROKEN_WALL;
                            while(true)
                            {
                                bool quit_loop = false;
                                switch(stage_block[now_pos_x+block_check][now_pos_y])
                                {
                                case FLAG:
                                    if(flag_is_push)
                                        quit_loop = true;
                                    break;
                                case ROCK:
                                    if(rock_is_push)
                                        quit_loop = true;
                                    break;
                                case SKULL:
                                    if(skull_is_push)
                                        quit_loop = true;
                                    break;
                                case WATER:
                                    if(water_is_push)
                                        quit_loop = true;
                                    break;
                                default:
                                    break;
                                }
                                if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= SKULL_TEXT)
                                {
                                    quit_loop = true;
                                }
                                if(quit_loop)
                                    break;
                                block_check--;
                            }
                            stage_block[now_pos_x+block_check][now_pos_y] = AIR;
                            block_check--;
                        }
                        else if(wall_is_stop)
                            block_check = MAX_VERTICAL_BLOCK;
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
                        if(stage_block[now_pos_x+block_check][now_pos_y] >= IS_TEXT && stage_block[now_pos_x+block_check][now_pos_y] <= SKULL_TEXT)
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
                            break;
                        case ROCK:
                            if(rock_is_push)
                            {
                                std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x+block_check;
                            }
                            break;
                        case WATER:
                            if(water_is_push)
                            {
                                std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x+block_check;
                            }
                            break;
                        case SKULL:
                            if(skull_is_push)
                            {
                                std::swap(stage_block[now_pos_x+block_check][now_pos_y],stage_block[current_air_x][now_pos_y]);
                                current_air_x = now_pos_x+block_check;
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


void Stage::HandleEvent(SDL_Event& e)
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

void Stage::MoveStageTile(int ObjectType)
{
    bool cant_kill = false;
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
                        if(j != MAX_HORIZONTAL_BLOCK-1 && (stage_block[i][j+1] < HORIZONTAL_WALL || stage_block[i][j+1] > WALL_BLOCK))
                        {
                            if(stage_block[i][j+1] == AIR)
                            {
                                std::swap(stage_block[i][j],stage_block[i][j+1]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i][j+block_check] != AIR && j+block_check < MAX_HORIZONTAL_BLOCK)
                                {
                                    if(stage_block[i][j] == AIR)
                                        break;

                                    switch(stage_block[i][j+block_check])
                                    {
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    /*
                                    if(stage_block[i][j+block_check] >=HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                    {
                                        else if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                    }*/
                                    block_check++;
                                }
                                if(j+block_check < MAX_HORIZONTAL_BLOCK)
                                {
                                    int current_air = j+block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i][j+block_check])
                                        {
                                        case ROCK:
                                            if(rock_is_push)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case BABA_TILE:
                                            if(baba_is_push)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push || wall_is_you || (baba_is_wall && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                        }
                                        else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                            current_air = j+block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
                    }

                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(j != MAX_HORIZONTAL_BLOCK-1)
                        {
                            if(stage_block[i][j+1] == AIR)
                            {
                                std::swap(stage_block[i][j],stage_block[i][j+1]);
                            }
                            else
                            {

                                int block_check = 1;
                                while(stage_block[i][j+block_check] != AIR && j+block_check < MAX_HORIZONTAL_BLOCK)
                                {
                                    if(stage_block[i][j] == AIR)
                                        break;

                                    switch(stage_block[i][j+block_check])
                                    {
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push )
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j+kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    if(stage_block[i][j+block_check] >=HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                    {
                                        if(wall_is_kill && !wall_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+kill_block_check] >= IS_TEXT && stage_block[i][j+kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(wall_is_sink && !wall_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j+block_check] = BROKEN_WALL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j+block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j+block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(wall_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                    }
                                    block_check++;
                                }
                                if(j+block_check < MAX_HORIZONTAL_BLOCK)
                                {
                                    int current_air = j+block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i][j+block_check])
                                        {
                                        case ROCK:
                                            if(rock_is_push || rock_is_you || (baba_is_rock && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push || flag_is_you || (baba_is_flag && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push || skull_is_you || (baba_is_skull && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push || water_is_you || (baba_is_water && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        case BABA_TILE:
                                            if(baba_is_push || baba_is_you)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i][j+block_check] >= HORIZONTAL_WALL && stage_block[i][j+block_check] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push)
                                            {
                                                std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                                current_air = j+block_check;
                                            }
                                        }
                                        else if(stage_block[i][j+block_check] >= IS_TEXT && stage_block[i][j+block_check] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i][j+block_check],stage_block[i][current_air]);
                                            current_air = j+block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
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
                        if(j != 0 && (stage_block[i][j-1] < HORIZONTAL_WALL || stage_block[i][j-1] > WALL_BLOCK))
                        {
                            if(stage_block[i][j-1] == AIR)
                            {
                                std::swap(stage_block[i][j],stage_block[i][j-1]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i][j-block_check] != AIR && j-block_check >= 0)
                                {
                                    if(stage_block[i][j] == AIR)
                                    {
                                        break;
                                    }

                                    switch(stage_block[i][j-block_check])
                                    {
                                    case  BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    /*if(stage_block[i][j-block_check] >=HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                    {
                                        if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                    }*/
                                    block_check++;
                                }
                                if(j-block_check >= 0)
                                {
                                    int current_air = j-block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i][j-block_check])
                                        {
                                        case BABA_TILE:
                                            if(baba_is_push)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case ROCK:
                                            if(rock_is_push)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push || wall_is_you || (baba_is_wall && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                        }
                                        else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                            current_air = j-block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(j != 0)
                        {
                            if(stage_block[i][j-1] == AIR)
                            {
                                std::swap(stage_block[i][j],stage_block[i][j-1]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i][j-block_check] != AIR && j-block_check >= 0)
                                {
                                    if(stage_block[i][j] == AIR)
                                    {
                                        break;
                                    }

                                    switch(stage_block[i][j-block_check])
                                    {
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                    {
                                                        quit_loop = true;
                                                    }
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                    {
                                                        quit_loop = true;
                                                    }
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                    {
                                                        quit_loop = true;
                                                    }
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push )
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                    {
                                                        quit_loop = true;
                                                    }
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= HORIZONTAL_WALL && stage_block[i][j-kill_block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                    {
                                                        quit_loop = true;
                                                    }
                                                }
                                                else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    if(stage_block[i][j-block_check] >=HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                    {
                                        if(wall_is_kill && !wall_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-kill_block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_kill || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-kill_block_check] >= IS_TEXT && stage_block[i][j-kill_block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(wall_is_sink && !wall_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i][j-block_check] = BROKEN_WALL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i][j-block_check])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i][j-block_check] = AIR;
                                            block_check--;

                                        }
                                        else if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(wall_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                    }
                                    block_check++;
                                }
                                if(j-block_check >= 0)
                                {
                                    int current_air = j-block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i][j-block_check])
                                        {
                                        case BABA_TILE:
                                            if(baba_is_push || baba_is_you)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case ROCK:
                                            if(rock_is_push || rock_is_you || (baba_is_rock && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push || flag_is_you || (baba_is_flag && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push || skull_is_you || (baba_is_skull && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push || water_is_you || (baba_is_water && baba_is_you))
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i][j-block_check] >= HORIZONTAL_WALL && stage_block[i][j-block_check] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push)
                                            {
                                                std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                                current_air = j-block_check;
                                            }
                                        }
                                        else if(stage_block[i][j-block_check] >= IS_TEXT && stage_block[i][j-block_check] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i][j-block_check],stage_block[i][current_air]);
                                            current_air = j-block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
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
                        if(i != 0 && (stage_block[i-1][j] < HORIZONTAL_WALL || stage_block[i-1][j] > WALL_BLOCK))
                        {
                            if(stage_block[i-1][j] == AIR)
                            {
                                std::swap(stage_block[i][j],stage_block[i-1][j]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i-block_check][j] != AIR && i-block_check >= 0)
                                {
                                    if(stage_block[i][j] == AIR)
                                    {
                                        break;
                                    }

                                    switch(stage_block[i-block_check][j])
                                    {
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_sink)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    /*if(stage_block[i-block_check][j] >=HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                    {
                                        if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                    }*/
                                    block_check++;
                                }
                                if(i-block_check >= 0)
                                {
                                    int current_air = i-block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i-block_check][j])
                                        {
                                        case BABA_TILE:
                                            if(baba_is_push)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case ROCK:
                                            if(rock_is_push)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push || wall_is_you || (baba_is_wall && baba_is_you))
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                        }
                                        else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                            current_air = i-block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(i != 0)
                        {
                            if(stage_block[i-1][j] == AIR)
                            {
                                std::swap(stage_block[i-1][j],stage_block[i][j]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i-block_check][j] != AIR && i-block_check >= 0)
                                {
                                    if(stage_block[i][j] == AIR)
                                    {
                                        break;
                                    }

                                    switch(stage_block[i-block_check][j])
                                    {
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;
                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push|| ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i-kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push|| ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    if(stage_block[i-block_check][j] >=HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                    {
                                        if(wall_is_kill && !wall_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-kill_block_check][j] >= IS_TEXT && stage_block[i-kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(wall_is_sink && !wall_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i-block_check][j] = BROKEN_WALL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i-block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push|| ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i-block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(wall_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                    }
                                    block_check++;
                                }
                                if(i-block_check >= 0)
                                {
                                    int current_air = i-block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i-block_check][j])
                                        {
                                        case BABA_TILE:
                                            if(baba_is_push || baba_is_you)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case ROCK:
                                            if(rock_is_push || rock_is_you || (baba_is_rock && baba_is_you))
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push || flag_is_you || (baba_is_flag && baba_is_you))
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push || skull_is_you || (baba_is_skull && baba_is_you))
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push || water_is_you || (baba_is_water && baba_is_you))
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i-block_check][j] >= HORIZONTAL_WALL && stage_block[i-block_check][j] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push)
                                            {
                                                std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                                current_air = i-block_check;
                                            }
                                        }
                                        else if(stage_block[i-block_check][j] >= IS_TEXT && stage_block[i-block_check][j] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i-block_check][j],stage_block[current_air][j]);
                                            current_air = i-block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
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
                        if(i < MAX_VERTICAL_BLOCK-1 && (stage_block[i+1][j] < HORIZONTAL_WALL || stage_block[i+1][j] > WALL_BLOCK))
                        {
                            if(stage_block[i+1][j] == AIR)
                            {
                                std::swap(stage_block[i][j],stage_block[i+1][j]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i+block_check][j] != AIR && i+block_check < MAX_VERTICAL_BLOCK)
                                {
                                    if(stage_block[i][j] == AIR)
                                    {
                                        break;
                                    }

                                    switch(stage_block[i+block_check][j])
                                    {
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    /*if(stage_block[i+block_check][j] >=HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                    {
                                        if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                    }*/
                                    block_check++;
                                }
                                if(i+block_check < MAX_VERTICAL_BLOCK)
                                {
                                    int current_air = i+block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i+block_check][j])
                                        {
                                        case BABA_TILE:
                                            if(baba_is_push)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case ROCK:
                                            if(rock_is_push)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push || wall_is_you || (baba_is_wall && baba_is_you))
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                        }
                                        else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                            current_air = i+block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    if(stage_block[i][j] == ObjectType)
                    {
                        if(i < MAX_VERTICAL_BLOCK-1)
                        {
                            if(stage_block[i+1][j] == AIR)
                            {
                                std::swap(stage_block[i+1][j],stage_block[i][j]);
                            }
                            else
                            {
                                int block_check = 1;
                                while(stage_block[i+block_check][j] != AIR && i+block_check < MAX_VERTICAL_BLOCK)
                                {
                                    if(stage_block[i][j] == AIR)
                                    {
                                        break;
                                    }

                                    switch(stage_block[i+block_check][j])
                                    {
                                    case BABA_TILE:
                                        if(baba_is_kill && !baba_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(baba_is_sink && !baba_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_BABA_TILE;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(baba_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(baba_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case ROCK:
                                        if(rock_is_kill && !rock_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(rock_is_sink && !rock_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_ROCK;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(rock_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(rock_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case FLAG:
                                        if(flag_is_kill && !flag_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(flag_is_sink && !flag_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_FLAG;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(flag_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(flag_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case SKULL:
                                        if(skull_is_kill && !skull_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(skull_is_sink && !skull_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_SKULL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(skull_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(skull_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    case WATER:
                                        if(water_is_kill && !water_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= HORIZONTAL_WALL && stage_block[i+kill_block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(water_is_sink && !water_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_WATER;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                                {
                                                    if(wall_is_push)
                                                        quit_loop = true;
                                                }
                                                else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(water_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(water_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                        break;
                                    default:
                                        break;
                                    }
                                    if(stage_block[i+block_check][j] >=HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                    {
                                        if(wall_is_kill && !wall_is_push && !cant_kill)
                                        {
                                            int kill_block_check = block_check;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+kill_block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+kill_block_check][j] >= IS_TEXT && stage_block[i+kill_block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                kill_block_check--;
                                            }
                                            if(kill_block_check == 0)
                                            {
                                                sth_is_killed = true;
                                                stage_block[i][j] = AIR;
                                            }
                                            else
                                            {
                                                cant_kill = true;
                                            }
                                        }
                                        else if(wall_is_sink && !wall_is_push)
                                        {
                                            sth_is_sunk = true;
                                            stage_block[i+block_check][j] = BROKEN_WALL;
                                            while(true)
                                            {
                                                bool quit_loop = false;
                                                switch(stage_block[i+block_check][j])
                                                {
                                                case FLAG:
                                                    if(flag_is_push || ObjectType == FLAG)
                                                        quit_loop = true;
                                                    break;
                                                case SKULL:
                                                    if(skull_is_push || ObjectType == SKULL)
                                                        quit_loop = true;
                                                    break;
                                                case WATER:
                                                    if(water_is_push || ObjectType == WATER)
                                                        quit_loop = true;
                                                    break;
                                                case ROCK:
                                                    if(rock_is_push || ObjectType == ROCK)
                                                        quit_loop = true;
                                                    break;
                                                case BABA_TILE:
                                                    if(baba_is_push || ObjectType == BABA_TILE)
                                                        quit_loop = true;
                                                    break;
                                                default:
                                                    break;
                                                }
                                                if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                                {
                                                    quit_loop = true;
                                                }
                                                if(quit_loop)
                                                    break;
                                                block_check--;
                                            }
                                            stage_block[i+block_check][j] = AIR;
                                            block_check--;

                                        }
                                        else if(wall_is_stop)
                                        {
                                            block_check = MAX_HORIZONTAL_BLOCK;
                                        }
                                        else if(wall_is_win)
                                        {
                                            CheckStageMovingWin(i,j,block_check);
                                        }
                                    }
                                    block_check++;
                                }
                                if(i+block_check < MAX_VERTICAL_BLOCK)
                                {
                                    int current_air = i+block_check;
                                    block_check--;
                                    while(block_check > -1)
                                    {
                                        switch(stage_block[i+block_check][j])
                                        {
                                        case BABA_TILE:
                                            if(baba_is_push || baba_is_you)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case ROCK:
                                            if(rock_is_push || rock_is_you || (baba_is_rock && baba_is_you))
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case FLAG:
                                            if(flag_is_push || flag_is_you || (baba_is_flag && baba_is_you))
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case SKULL:
                                            if(skull_is_push || skull_is_you || (baba_is_skull && baba_is_you))
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        case WATER:
                                            if(water_is_push || water_is_you || (baba_is_water && baba_is_you))
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                            break;
                                        default:
                                            break;
                                        }
                                        if(stage_block[i+block_check][j] >= HORIZONTAL_WALL && stage_block[i+block_check][j] <= WALL_BLOCK)
                                        {
                                            if(wall_is_push)
                                            {
                                                std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                                current_air = i+block_check;
                                            }
                                        }
                                        else if(stage_block[i+block_check][j] >= IS_TEXT && stage_block[i+block_check][j] <= SKULL_TEXT)
                                        {
                                            std::swap(stage_block[i+block_check][j],stage_block[current_air][j]);
                                            current_air = i+block_check;
                                        }
                                        block_check--;
                                    }
                                }
                            }
                        }
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

bool Stage::CheckAlive(int ObjectType)
{
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            if(stage_block[i][j] == ObjectType)
            {
                return true;
            }
        }
    }
    return false;
}

void Stage::ChangeBlockWithPos(Object& player,int ObjectType)
{
    int now_pos_y = (player.GetX()+1)/64;
    int now_pos_x = (player.GetY()+1)/64;
    stage_block[now_pos_x][now_pos_y] = ObjectType;
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

void Stage::EraseBrokenTile()
{
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            if(stage_block[i][j] >= BROKEN_BABA_TILE && stage_block[i][j] <= BROKEN_SKULL)
                stage_block[i][j] = AIR;
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
    baba_is_sink = false;
    baba_is_kill = false;
    baba_is_water = false;
    baba_is_skull = false;
// rule for flag
    flag_is_you = false;
    flag_is_win = false;
    flag_is_stop = false;
    flag_is_push = false;
    flag_is_baba = false;
    flag_is_wall = false;
    flag_is_rock = false;
    flag_is_sink = false;
    flag_is_kill = false;
    flag_is_water = false;
    flag_is_skull = false;
// rule for wall
    wall_is_you = false;
    wall_is_win = false;
    wall_is_stop = false;
    wall_is_push = false;
    wall_is_flag = false;
    wall_is_baba = false;
    wall_is_rock = false;
    wall_is_sink = false;
    wall_is_kill = false;
    wall_is_water = false;
    wall_is_skull = false;
// rule for rock
    rock_is_you = false;
    rock_is_win = false;
    rock_is_stop = false;
    rock_is_push = false;
    rock_is_flag = false;
    rock_is_wall = false;
    rock_is_baba = false;
    rock_is_sink = false;
    rock_is_kill = false;
    rock_is_water = false;
    rock_is_skull = false;
// rule for water
    water_is_you = false;
    water_is_win = false;
    water_is_rock = false;
    water_is_stop = false;
    water_is_push = false;
    water_is_flag = false;
    water_is_wall = false;
    water_is_baba = false;
    water_is_sink = false;
    water_is_kill = false;
    water_is_skull = false;
// rule for skull
    skull_is_you = false;
    skull_is_win = false;
    skull_is_stop = false;
    skull_is_rock = false;
    skull_is_push = false;
    skull_is_flag = false;
    skull_is_wall = false;
    skull_is_baba = false;
    skull_is_sink = false;
    skull_is_kill = false;
    skull_is_water = false;
}


void Stage::CheckRule()
{
    for(int i = 0; i<MAX_VERTICAL_BLOCK; i++)
    {
        for(int j = 0; j<MAX_HORIZONTAL_BLOCK; j++)
        {
            // Check the rule for each object
            if(stage_block[i][j] == BABA_TEXT)
            {
                if(stage_block[i+1][j] == IS_TEXT || stage_block[i][j+1] == IS_TEXT)
                {
                    if(stage_block[i][j+1] == IS_TEXT)
                    {
                        switch(stage_block[i][j+2])
                        {
                        case STOP_TEXT:
                            baba_is_stop =true;
                            break;
                        case WIN_TEXT:
                            baba_is_win = true;
                            break;
                        case YOU_TEXT:
                            baba_is_you = true;
                            break;
                        case PUSH_TEXT:
                            baba_is_push = true;
                            break;
                        case FLAG_TEXT:
                            baba_is_flag = true;
                            break;
                        case WALL_TEXT:
                            baba_is_wall = true;
                            break;
                        case ROCK_TEXT:
                            baba_is_rock = true;
                            break;
                        case KILL_TEXT:
                            baba_is_kill = true;
                            break;
                        case SINK_TEXT:
                            baba_is_sink = true;
                            break;
                        case SKULL_TEXT:
                            baba_is_skull = true;
                            break;
                        case WATER_TEXT:
                            baba_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                    if(stage_block[i+1][j] == IS_TEXT)
                    {

                        switch(stage_block[i+2][j])
                        {
                        case STOP_TEXT:
                            baba_is_stop =true;
                            break;
                        case WIN_TEXT:
                            baba_is_win = true;
                            break;
                        case YOU_TEXT:
                            baba_is_you = true;
                            break;
                        case PUSH_TEXT:
                            baba_is_push = true;
                            break;
                        case FLAG_TEXT:
                            baba_is_flag = true;
                            break;
                        case WALL_TEXT:
                            baba_is_wall = true;
                            break;
                        case ROCK_TEXT:
                            baba_is_rock = true;
                            break;
                        case KILL_TEXT:
                            baba_is_kill = true;
                            break;
                        case SINK_TEXT:
                            baba_is_sink = true;
                            break;
                        case SKULL_TEXT:
                            baba_is_skull = true;
                            break;
                        case WATER_TEXT:
                            baba_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                }
            }
            else if(stage_block[i][j] == FLAG_TEXT)
            {
                if(stage_block[i+1][j] == IS_TEXT || stage_block[i][j+1] == IS_TEXT)
                {
                    if(stage_block[i][j+1] == IS_TEXT)
                    {
                        switch(stage_block[i][j+2])
                        {
                        case STOP_TEXT:
                            flag_is_stop =true;
                            break;
                        case WIN_TEXT:
                            flag_is_win = true;
                            break;
                        case YOU_TEXT:
                            flag_is_you = true;
                            break;
                        case PUSH_TEXT:
                            flag_is_push = true;
                            break;
                        case BABA_TEXT:
                            flag_is_baba = true;
                            break;
                        case WALL_TEXT:
                            flag_is_wall = true;
                            break;
                        case ROCK_TEXT:
                            flag_is_rock = true;
                            break;
                        case SINK_TEXT:
                            flag_is_sink = true;
                            break;
                        case KILL_TEXT:
                            flag_is_kill = true;
                            break;
                        case SKULL_TEXT:
                            flag_is_skull = true;
                            break;
                        case WATER_TEXT:
                            flag_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                    if(stage_block[i+1][j] == IS_TEXT)
                    {

                        switch(stage_block[i+2][j])
                        {
                        case STOP_TEXT:
                            flag_is_stop =true;
                            break;
                        case WIN_TEXT:
                            flag_is_win = true;
                            break;
                        case YOU_TEXT:
                            flag_is_you = true;
                            break;
                        case PUSH_TEXT:
                            flag_is_push = true;
                            break;
                        case BABA_TEXT:
                            flag_is_baba = true;
                            break;
                        case WALL_TEXT:
                            flag_is_wall = true;
                            break;
                        case ROCK_TEXT:
                            flag_is_rock = true;
                            break;
                        case KILL_TEXT:
                            flag_is_kill = true;
                            break;
                        case SINK_TEXT:
                            flag_is_sink = true;
                            break;
                        case SKULL_TEXT:
                            flag_is_skull = true;
                            break;
                        case WATER_TEXT:
                            flag_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                }
            }
            else if(stage_block[i][j] == WALL_TEXT)
            {
                if(stage_block[i+1][j] == IS_TEXT || stage_block[i][j+1] == IS_TEXT)
                {
                    if(stage_block[i][j+1] == IS_TEXT)
                    {
                        switch(stage_block[i][j+2])
                        {
                        case STOP_TEXT:
                            wall_is_stop =true;
                            break;
                        case WIN_TEXT:
                            wall_is_win = true;
                            break;
                        case YOU_TEXT:
                            wall_is_you = true;
                            break;
                        case PUSH_TEXT:
                            wall_is_push = true;
                            break;
                        case FLAG_TEXT:
                            wall_is_flag = true;
                            break;
                        case BABA_TEXT:
                            wall_is_baba = true;
                            break;
                        case ROCK_TEXT:
                            wall_is_rock = true;
                            break;
                        case KILL_TEXT:
                            wall_is_kill = true;
                            break;
                        case SINK_TEXT:
                            wall_is_sink = true;
                            break;
                        case SKULL_TEXT:
                            wall_is_skull = true;
                            break;
                        case WATER_TEXT:
                            wall_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                    if(stage_block[i+1][j] == IS_TEXT)
                    {

                        switch(stage_block[i+2][j])
                        {
                        case STOP_TEXT:
                            wall_is_stop =true;
                            break;
                        case WIN_TEXT:
                            wall_is_win = true;
                            break;
                        case YOU_TEXT:
                            wall_is_you = true;
                            break;
                        case PUSH_TEXT:
                            wall_is_push = true;
                            break;
                        case FLAG_TEXT:
                            wall_is_flag = true;
                            break;
                        case BABA_TEXT:
                            wall_is_baba = true;
                            break;
                        case ROCK_TEXT:
                            wall_is_rock = true;
                            break;
                        case KILL_TEXT:
                            wall_is_kill = true;
                            break;
                        case SINK_TEXT:
                            wall_is_sink = true;
                            break;
                        case WATER_TEXT:
                            wall_is_water = true;
                            break;
                        case SKULL_TEXT:
                            wall_is_skull = true;
                            break;
                        default:
                            break;
                        };
                    }
                }
            }
            else if(stage_block[i][j] == ROCK_TEXT)
            {
                if(stage_block[i+1][j] == IS_TEXT || stage_block[i][j+1] == IS_TEXT)
                {
                    if(stage_block[i][j+1] == IS_TEXT)
                    {
                        switch(stage_block[i][j+2])
                        {
                        case STOP_TEXT:
                            rock_is_stop =true;
                            break;
                        case WIN_TEXT:
                            rock_is_win = true;
                            break;
                        case YOU_TEXT:
                            rock_is_you = true;
                            break;
                        case PUSH_TEXT:
                            rock_is_push = true;
                            break;
                        case FLAG_TEXT:
                            rock_is_flag = true;
                            break;
                        case WALL_TEXT:
                            rock_is_wall = true;
                            break;
                        case BABA_TEXT:
                            rock_is_baba = true;
                            break;
                        case KILL_TEXT:
                            rock_is_kill = true;
                            break;
                        case SINK_TEXT:
                            rock_is_sink = true;
                            break;
                        case SKULL_TEXT:
                            rock_is_skull = true;
                            break;
                        case WATER_TEXT:
                            rock_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                    if(stage_block[i+1][j] == IS_TEXT)
                    {

                        switch(stage_block[i+2][j])
                        {
                        case STOP_TEXT:
                            rock_is_stop =true;
                            break;
                        case WIN_TEXT:
                            rock_is_win = true;
                            break;
                        case YOU_TEXT:
                            rock_is_you = true;
                            break;
                        case PUSH_TEXT:
                            rock_is_push = true;
                            break;
                        case FLAG_TEXT:
                            rock_is_flag = true;
                            break;
                        case WALL_TEXT:
                            rock_is_wall = true;
                            break;
                        case BABA_TEXT:
                            rock_is_baba = true;
                            break;
                        case KILL_TEXT:
                            rock_is_kill = true;
                            break;
                        case SINK_TEXT:
                            rock_is_sink = true;
                            break;
                        case SKULL_TEXT:
                            rock_is_skull = true;
                            break;
                        case WATER_TEXT:
                            rock_is_skull = true;
                            break;
                        default:
                            break;
                        };
                    }
                }
            }
            else if(stage_block[i][j] == SKULL_TEXT)
            {
                if(stage_block[i+1][j] == IS_TEXT || stage_block[i][j+1] == IS_TEXT)
                {
                    if(stage_block[i][j+1] == IS_TEXT)
                    {
                        switch(stage_block[i][j+2])
                        {
                        case STOP_TEXT:
                            skull_is_stop =true;
                            break;
                        case WIN_TEXT:
                            skull_is_win = true;
                            break;
                        case YOU_TEXT:
                            skull_is_you = true;
                            break;
                        case PUSH_TEXT:
                            skull_is_push = true;
                            break;
                        case FLAG_TEXT:
                            skull_is_flag = true;
                            break;
                        case WALL_TEXT:
                            skull_is_wall = true;
                            break;
                        case BABA_TEXT:
                            skull_is_baba = true;
                            break;
                        case KILL_TEXT:
                            skull_is_kill = true;
                            break;
                        case SINK_TEXT:
                            skull_is_sink = true;
                            break;
                        case ROCK_TEXT:
                            skull_is_rock = true;
                            break;
                        case WATER_TEXT:
                            skull_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                    if(stage_block[i+1][j] == IS_TEXT)
                    {

                        switch(stage_block[i+2][j])
                        {
                        case STOP_TEXT:
                            skull_is_stop =true;
                            break;
                        case WIN_TEXT:
                            skull_is_win = true;
                            break;
                        case YOU_TEXT:
                            skull_is_you = true;
                            break;
                        case PUSH_TEXT:
                            skull_is_push = true;
                            break;
                        case FLAG_TEXT:
                            skull_is_flag = true;
                            break;
                        case WALL_TEXT:
                            skull_is_wall = true;
                            break;
                        case BABA_TEXT:
                            skull_is_baba = true;
                            break;
                        case KILL_TEXT:
                            skull_is_kill = true;
                            break;
                        case SINK_TEXT:
                            skull_is_sink = true;
                            break;
                        case ROCK_TEXT:
                            skull_is_rock = true;
                            break;
                        case WATER_TEXT:
                            skull_is_water = true;
                            break;
                        default:
                            break;
                        };
                    }
                }
            }
            else if(stage_block[i][j] == WATER_TEXT)
            {
                if(stage_block[i+1][j] == IS_TEXT || stage_block[i][j+1] == IS_TEXT)
                {
                    if(stage_block[i][j+1] == IS_TEXT)
                    {
                        switch(stage_block[i][j+2])
                        {
                        case STOP_TEXT:
                            water_is_stop =true;
                            break;
                        case WIN_TEXT:
                            water_is_win = true;
                            break;
                        case YOU_TEXT:
                            water_is_you = true;
                            break;
                        case PUSH_TEXT:
                            water_is_push = true;
                            break;
                        case FLAG_TEXT:
                            water_is_flag = true;
                            break;
                        case WALL_TEXT:
                            water_is_wall = true;
                            break;
                        case BABA_TEXT:
                            water_is_baba = true;
                            break;
                        case KILL_TEXT:
                            water_is_kill = true;
                            break;
                        case SINK_TEXT:
                            water_is_sink = true;
                            break;
                        case ROCK_TEXT:
                            water_is_rock = true;
                            break;
                        case SKULL_TEXT:
                            water_is_skull = true;
                            break;
                        default:
                            break;
                        };
                    }
                    if(stage_block[i+1][j] == IS_TEXT)
                    {

                        switch(stage_block[i+2][j])
                        {
                        case STOP_TEXT:
                            water_is_stop =true;
                            break;
                        case WIN_TEXT:
                            water_is_win = true;
                            break;
                        case YOU_TEXT:
                            water_is_you = true;
                            break;
                        case PUSH_TEXT:
                            water_is_push = true;
                            break;
                        case FLAG_TEXT:
                            water_is_flag = true;
                            break;
                        case WALL_TEXT:
                            water_is_wall = true;
                            break;
                        case BABA_TEXT:
                            water_is_baba = true;
                            break;
                        case KILL_TEXT:
                            water_is_kill = true;
                            break;
                        case SINK_TEXT:
                            water_is_sink = true;
                            break;
                        case ROCK_TEXT:
                            water_is_rock = true;
                            break;
                        case SKULL_TEXT:
                            water_is_skull = true;
                            break;
                        default:
                            break;
                        };
                    }
                }
            }
            // Done for a block
        }

    }
}

void Stage::CheckStageMovingWin(int now_x,int now_y,int block_check)
{
    switch(action_type)
    {
  case MOVERIGHT:
      while(block_check > 0)
      {   bool quit_loop = false;
          switch(stage_block[now_x][now_y+block_check])
          {
          case ROCK:
            if(rock_is_push)
            {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case FLAG:
              if(flag_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case SKULL:
              if(skull_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case WATER:
              if(water_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case BABA_TILE:
              if(baba_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
        default:
            break;
          };
          if(stage_block[now_x][now_y+block_check] >= HORIZONTAL_WALL && stage_block[now_x][now_y+block_check] <= WALL_BLOCK)
          {
              if(wall_is_push)
                {
                win = false;
                quit_loop = true;
            }
              else
                win = true;
          }
          if(quit_loop)
            break;
          block_check--;
      }
    break;
  case MOVELEFT:
      while(block_check > 0)
      {   bool quit_loop = false;
          switch(stage_block[now_x][now_y-block_check])
          {
          case ROCK:
            if(rock_is_push)
            {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case FLAG:
              if(flag_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case SKULL:
              if(skull_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case WATER:
              if(water_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case BABA_TILE:
              if(baba_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
        default:
            break;
          };
          if(stage_block[now_x][now_y-block_check] >= HORIZONTAL_WALL && stage_block[now_x][now_y-block_check] <= WALL_BLOCK)
          {
              if(wall_is_push)
                {
                win = false;
                quit_loop = true;
            }
              else
                win = true;
          }
          if(quit_loop)
            break;
          block_check--;
      }
    break;
  case MOVEDOWN:
      while(block_check > 0)
      {   bool quit_loop = false;
          switch(stage_block[now_x+block_check][now_y])
          {
          case ROCK:
            if(rock_is_push)
            {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case FLAG:
              if(flag_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case SKULL:
              if(skull_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case WATER:
              if(water_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case BABA_TILE:
              if(baba_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
        default:
            break;
          };
          if(stage_block[now_x+block_check][now_y] >= HORIZONTAL_WALL && stage_block[now_x+block_check][now_y] <= WALL_BLOCK)
          {
              if(wall_is_push)
                {
                win = false;
                quit_loop = true;
            }
              else
                win = true;
          }
          if(quit_loop)
            break;
          block_check--;
      }
    break;
  case MOVEUP:
      while(block_check > 0)
      {   bool quit_loop = false;
          switch(stage_block[now_x-block_check][now_y])
          {
          case ROCK:
            if(rock_is_push)
            {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case FLAG:
              if(flag_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case SKULL:
              if(skull_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case WATER:
              if(water_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
          case BABA_TILE:
              if(baba_is_push)
                {
                win = false;
                quit_loop = true;
            }
            else
                win = true;
            break;
        default:
            break;
          };
          if(stage_block[now_x-block_check][now_y] >= HORIZONTAL_WALL && stage_block[now_x-block_check][now_y] <= WALL_BLOCK)
          {
              if(wall_is_push)
                {
                win = false;
                quit_loop = true;
            }
              else
                win = true;
          }
          if(quit_loop)
            break;
          block_check--;
      }
    break;
    };
}
