#include "game_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <time.h>

/*Load all the textures use in game*/
Assets*         assets_init(void)
{
    Assets* assets = (Assets*)malloc(sizeof(Assets));

    assets->player = LoadTexture("assets/assets/player.png"); 
    assets->enemyOne =  LoadTexture("assets/assets/alien1.png");
    assets->enemyTwo = LoadTexture("assets/assets/alien2.png");
    assets->enemyThree = LoadTexture("assets/assets/alien3.png");
    assets->projectile = LoadTexture("assets/assets/missile.png");
    assets->titleScreen = LoadTexture("assets/assets/title.png");
    assets->background = LoadTexture("assets/assets/background.png");
    assets->Shield = LoadTexture("assets/assets/shield_map.png");
    assets->playerDeath = LoadTexture("assets/assets/playerdeath.png");
    assets->UFO = LoadTexture("assets/assets/ufo.png");
    assets->enemyDeath = LoadTexture("assets/assets/aliendeath.png");
    assets->Player_shoot = LoadSound("assets/assets/player_shoot.wav");
    assets->Enemy_shoot = LoadSound("assets/assets/enemy_shoot.wav");
    assets->Enemy_down = LoadSound("assets/assets/enemy_down.wav");
    assets->Shield_Damage = LoadSound("assets/assets/shield_damage.wav");
    assets->Player_hit = LoadSound("assets/assets/player_hit.wav");
    assets->Enemy_death = LoadSound("assets/assets/enemy_death.wav");
    assets->PowerUp = LoadSound("assets/assets/powerup.wav");
    assets->Music_SpIn = LoadMusicStream("assets/assets/Music_SpIn_1.0.wav");

    return assets;
}

/*Initialize the values of the GameStatus structure*/
GameStatus*     game_status_init(int SCREENWITDH)
{
    GameStatus* currentStatus = (GameStatus*)malloc(sizeof(GameStatus));

    currentStatus->exitGame = false;
    currentStatus->goingRight = true;
    currentStatus->isGamePaused = false;
    currentStatus->curentScreen = E_TITLESCREEN;
    currentStatus->selectedText = E_Retry;
    currentStatus->tilt = 0;        /*'float' number used to move the background vertically*/
    currentStatus->pan = 0;        /*'float' number used to move the background horizontally*/
    currentStatus->canPlayerShoot = 1;
    currentStatus->deltaTime = 0; /*Delta Time*/
    currentStatus->coolDown = 0;  /*Cooldown used for animation*/
    currentStatus->screenWIDTH = SCREENWITDH;
    high_score_init(currentStatus);

    return currentStatus;
}

/*Initialize the values of the ObjectList struct with the GameObjects we send in parameters*/
ObjectList*     object_list_init_gameobjects(GameObject* player, GameObject* projectile)
{
    ObjectList* objectList = (ObjectList*)malloc(sizeof(ObjectList));

    objectList->player = player;
    objectList->projectile = projectile;

    return objectList;
}

/*Initialize the values of the HighScore array with the values of the INI.txt file*/
void            high_score_init(GameStatus* currentStatus)
{
    FILE* high_score_file = fopen("INI.txt", "r");

    for (int i = 0; i < MAX_LINE; i++)
    {
        fgets(currentStatus->HighScore[i], MAX_DIGIT, high_score_file);
        fseek(high_score_file, 1L, SEEK_CUR);
    }

    fclose(high_score_file);
}

/*Initialize the not GameObjects Struct in the ObjectList struct*/
void            object_list_init_other(ObjectList* list, EnemyArmy enemies, ShieldsLineUp shield, UFO_PowerUp bonus)
{
    list->enemies = enemies;
    list->shield = shield;
    list->bonus = bonus;
    list->level = 1;
}

/*Initialize the entire game (GameObjects, ObjectList, ect)*/
ObjectList*     game_init(int speedPlayer, int speedProjectile, int speedEnemy, int screenWidth)
{
    GameObject*     player;
    player = gameobject_init(
             (Rectangle){screenWidth/2, 600, 50, 25}, WHITE, speedPlayer, 3);
    GameObject*     projectile;
    projectile = gameobject_init(
                 (Rectangle){(player->rect.x + player->rect.width/2), 600, 6, 25}, RED, speedProjectile, 0);
    projectile->active = false;

    GameObject*     UFO;
    UFO = gameobject_init(
             (Rectangle){800, 50, 75, 37}, BLUE, speedPlayer*1.5, 1);
    UFO->active = false;
    GameObject* powerup = powerup_init(UFO, speedProjectile);

    EnemyArmy       army = enemy_init(5, 11, speedEnemy, speedProjectile);
    ShieldsLineUp   oneShield = shields_init(4, 4, 4);
    UFO_PowerUp     bonus = bonus_init(UFO, powerup, 0, 0);

    ObjectList*     objectlist = object_list_init_gameobjects(player, projectile);

    object_list_init_other(objectlist, army, oneShield, bonus);

    return objectlist;
}

/*Update the entire game every frame*/
void            game_update(ObjectList* list, int screenwidth, GameStatus* currentStatus, Assets* sounds)
{
    // if (IsKeyPressed(KEY_E))
    // {
    //     list->player->active = false;
    // }                                               /*These Key are useful for testing*/
    // if (IsKeyPressed(KEY_S))
    // {
    //     list->projectile->score +=100;
    // }
    currentStatus->deltaTime = GetFrameTime();
    currentStatus->coolDown += currentStatus->deltaTime;
    if(currentStatus->coolDown > 2)
    {
        currentStatus->coolDown = 0;
    }
    if (currentStatus->isGamePaused == false && currentStatus->curentScreen == E_LVLONE)
    {
        add_delta(list, currentStatus);
        add_delta_animations(list, currentStatus);
        update_gameobject(list, screenwidth, currentStatus, sounds);
    }
    if (currentStatus->isGamePaused == true)
    {
        game_paused(currentStatus, list);
    }
}

/*Update the delta time every frame*/
void            add_delta(ObjectList* list, GameStatus* currentStatus)
{
    list->enemies.cd += currentStatus->deltaTime;
    if (list->enemies.cd > 1.01)
    {
        list->enemies.cd = 0;
        list->bonus.UFO->score += 1;
    }
    if (currentStatus->canPlayerShoot < 1)
    {
        currentStatus->canPlayerShoot += currentStatus->deltaTime;
    }

    int rand_time = Randoms(15, 25);
    if (list->bonus.UFO->score >= rand_time && list->enemies.enemy[0][0].rect.y > 90)
    {
        list->bonus.UFO->score = 0;
        list->bonus.UFO->rect.x = 800;
        list->bonus.UFO->active = true;
        list->bonus.rand = Randoms(50, 750);
        list->bonus.selected_power_up = Randoms(0, 2);
    }
}

/*Update the delta time used for animations every frame*/
void            add_delta_animations(ObjectList* list, GameStatus* currentStatus)
{
    if (list->player->animcoolDown >= 1 && list->player->animcoolDown <= 2)
    {
        list->player->animcoolDown += currentStatus->deltaTime;
    }
    else if (list->player->animcoolDown > 2)
    {
        list->player->animcoolDown = 0;
        if (list->player->life > 0)
        {
            list->player->rect.x = currentStatus->screenWIDTH/2 - list->player->rect.width/2;
        }
    }

    if (list->projectile->animcoolDown >= 1 && list->projectile->animcoolDown <= 2.5)
    {
        list->projectile->animcoolDown += currentStatus->deltaTime;
    }
    if(list->projectile->animcoolDown > 2.5)
    {
        list->projectile->animcoolDown = 0;
    }
    
    for (int i = 0; i < list->enemies.lines; i++)
    {
        for (int j = 0; j < list->enemies.columns; j++)
        {
            if (list->enemies.enemy[i][j].animcoolDown >= 1 
                && list->enemies.enemy[i][j].animcoolDown <= 2)
            {
                list->enemies.enemy[i][j].animcoolDown += currentStatus->deltaTime;
            }
            if (list->enemies.enemy_projectile[i][j].animcoolDown >= 1 
                && list->enemies.enemy_projectile[i][j].animcoolDown <= 2)
            {
                list->enemies.enemy_projectile[i][j].animcoolDown += currentStatus->deltaTime;
            }
        }
    }
}

/*Update all the GameObject every frame*/
void            update_gameobject(ObjectList* list, int screenWidth, GameStatus* currentStatus, Assets* sounds)
{
    if (list->player->animcoolDown < 1)     /*If the player has been hit, the game stop while the PlayerHit animation goes*/
    {
        player_update(list, screenWidth, currentStatus->deltaTime, sounds);
        projectile_update(list, currentStatus->deltaTime, currentStatus, sounds);
        enemy_update(list->enemies, currentStatus->deltaTime, &(currentStatus->goingRight), list->projectile, sounds);
        UFO_update(list, currentStatus, sounds);
    }

    loop_for_shield_update(list->shield, list->projectile, list->enemies, sounds);
    UFO_update(list, currentStatus, sounds);
}

/*Draw all the button we can use while the game is paused*/
void            game_paused(GameStatus* currentStatus, ObjectList* list)
{
    DrawText("PAUSED", 30, 645, 50, RED);
    DrawText("Press 'R' to restart", 520, 635, 20, WHITE);
    DrawText("Press 'Q' to quit", 520, 655, 20, WHITE);
    DrawText("Press 'P' to unpause", 520, 675, 20, WHITE);

    if(IsKeyPressed(KEY_Q))
    {
        currentStatus->exitGame = true;
    }
     if(IsKeyPressed(KEY_R))
    {
        player_reset(list);
        shield_reset(list);
        enemy_reset(list, 1);
        currentStatus->isGamePaused = false;
        currentStatus->curentScreen = E_LVLONE;
    }
    
}

/*Functions to get a random number between two numbers we send in parameters*/
int             Randoms(int lower, int upper)
{
    srand(time(0));
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

/*Draw the HUD*/
void            drawHUD(void)
{
    DrawRectangle(0, 0, 800, 5, GREEN);
    DrawRectangle(0, 40, 800, 5, GREEN);
    DrawRectangle(0, 630, 800, 5, GREEN);
    DrawRectangle(0, 695, 800, 5, GREEN);

    DrawRectangle(0, 0, 5, 700, GREEN);
    DrawRectangle(795, 0, 5, 700, GREEN);
}

/*Update the screen every frame*/
void            screen_update(ObjectList* list, GameStatus* currentStatus, Assets* sprite)
{
    UpdateMusicStream(sprite->Music_SpIn);
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentStatus->curentScreen)
    {
        case (E_TITLESCREEN):
        {
            update_titlescreen(currentStatus, sprite);
        }break;

        case (E_LVLONE):
        {
            drawHUD();
            update_lvlone(currentStatus, list, sprite);
        }break;

        case (E_ENDING):
        {
            update_ending(currentStatus, list, sprite);
        }break;

        default: break;
    }

    EndDrawing();
}

/*Update and draw the title screen if it's the currentScreen*/
void            update_titlescreen(GameStatus* currentStatus, Assets* sprite)
{
    currentStatus->tilt -= 0.01f;
    currentStatus->pan -= 0.01f;

    DrawTextureEx(sprite->background, (Vector2){currentStatus->pan, currentStatus->tilt}, 0.0f, 1.0f, WHITE);

    DrawText("SPACE INVADERS", 60, 200, 75, WHITE);
    if (currentStatus->coolDown < 1)
    {
        DrawText("Press any Key to start", 208, 500, 30, WHITE);
    }

    for (int key = 1; key < 348; key++)
    {
        if (IsKeyPressed(key))
        {
            currentStatus->curentScreen = E_LVLONE;
        }
    }
}

/*Update and draw the level one screen if it's the currentScreen*/
void            update_lvlone(GameStatus* currentStatus, ObjectList* list, Assets* sprite)
{
    if (IsKeyPressed(KEY_P))
    {
        currentStatus->isGamePaused = !currentStatus->isGamePaused;
    }

    if (list->player->active)
    {
        update_lvlone_playeractive(list, sprite);
    }
            
    if (!list->player->active)
    {
        StopMusicStream(sprite->Music_SpIn);
        update_high_score(list, currentStatus);
        currentStatus->curentScreen = E_ENDING;
    }

    if (enemy_alive(list->enemies) <= 0)
    {
        list->level += 1;
        enemy_reset(list, 1.15 *(list->level) - 1);
        shield_reset(list);
        currentStatus->curentScreen = E_LVLONE;
    }
    if (end_check(list->enemies, list->shield))
    {
        WaitTime(2);
        currentStatus->curentScreen = E_ENDING;
    }
}

/*Update and draw the level one screen if the player is alive*/
void            update_lvlone_playeractive(ObjectList* list, Assets* sprite)
{
    player_animation(list, sprite);
    DrawText("Score:", 50, 13, 20, WHITE);
    DrawText(TextFormat("%d", list->player->score), 125, 13, 20, WHITE);
    DrawTexturePro(sprite->UFO, (Rectangle){0, 0, 48, 24}, list->bonus.UFO->rect, (Vector2){0, 0}, 0, WHITE);

    for (unsigned int i = 0; i < list->player->life; i++)
    {
        Rectangle life = {i*75 + 550, 10, list->player->rect.width, list->player->rect.height};
        DrawTexturePro(sprite->player, (Rectangle){0, 0, 32, 16}, life, (Vector2){0, 0}, 0, WHITE);
    }

    if (list->projectile->active)
    {
        projectile_animation(list, sprite);
    }
    powerup_draw(list);

    loop_for_draw_enemy(list->enemies, sprite);
    shield_loop_for_draw(list->shield, sprite);
}

/*Update the High Score when the player's dead*/
void            update_high_score(ObjectList* list, GameStatus* currentStatus)
{
    high_score_init(currentStatus);
    FILE* high_score_file = fopen("INI.txt", "r+b");
    
    for (int i = 0; i < MAX_LINE; i++)
    {
        int scoreToCompare = alpha_to_number(currentStatus->HighScore[i]);

        if (list->player->score > scoreToCompare)
        {
            char* temp = number_to_alpha(list->player->score);
            int length = string_length(temp);
            fseek(high_score_file, MAX_DIGIT*(i+1) - (length+1), SEEK_SET);
            fputs(temp, high_score_file);
            for (int j = i ; j < MAX_LINE-1 ; j++)
            {
                fputc(10, high_score_file);
                fputs(currentStatus->HighScore[j], high_score_file);
            }
            free(temp);
            break;
        }
    }

    fclose(high_score_file);
}

/*Check if the ennemy army have reach the bottom every frame*/
bool            end_check(EnemyArmy arr, ShieldsLineUp shield)
{
    Rectangle   end = {50, 1000, 0, shield.shields[0][3][0].rect.y + 10};

    for (int i = 0; i < arr.lines; i++)
    {
        for (int j = 0; j < arr.columns; j++)
        {
            if ((arr.enemy[i][j].rect.y + arr.enemy[i][j].rect.height) > 
                (shield.shields[0][3][0].rect.y + shield.shields[0][3][0].rect.height) 
            &&  (arr.enemy[i][j].active) == true)
            {
                return true;
            }
        }
    }
    return false;
}

/*Update the ending screen if it's the current screen*/
void            update_ending(GameStatus* currentStatus, ObjectList* list, Assets* sprite)
{
    draw_ending(list, sprite, currentStatus);
    switch (currentStatus->selectedText)
    {
        case (E_Retry):
        {
            ending_retry(&currentStatus->selectedText, &currentStatus->curentScreen, list, sprite);
        }break;
        
        case (E_Quit):
        {
            ending_quit(&currentStatus->selectedText, &currentStatus->exitGame, list);
        }break;
    }
}

/*Draw the ending Screen if it's the current screen*/
void            draw_ending(ObjectList* list, Assets* sprite, GameStatus* currentStatus)
{
    high_score_init(currentStatus);
    ClearBackground(BLACK);
    DrawText("GAME OVER", 100,  100, 100, WHITE);
    DrawText("YOU DIED :(", 100,  200, 20, WHITE);
    DrawText("Retry", 150,  650, 50, WHITE);
    DrawText("Quit", 550, 650, 50, WHITE);
    DrawText("Your Score", 200,  260, 30, WHITE);
    for (int i = 0; i < MAX_LINE; i++)
    {
        DrawText("High Score :", 200,  300 + i* 40, 30, WHITE);
        DrawText(TextFormat("%s", currentStatus->HighScore[i]), 400, 300 + i* 40, 30, WHITE);
    }
    DrawText(TextFormat("%d", list->player->score), 400, 260, 30, WHITE);
    if(currentStatus->coolDown < 1 && currentStatus->coolDown > 0.5 || currentStatus->coolDown < 2 && currentStatus->coolDown > 1.5)
    {
        DrawTexturePro(sprite->playerDeath, (Rectangle){0, 0, 32, 16}, list->player->rect, (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(sprite->enemyTwo, (Rectangle){0, 0, 24, 18}, (Rectangle){600, 200, 72, 54}, (Vector2){0, 0}, 0, WHITE);
    }
    if(currentStatus->coolDown < 0.5 || currentStatus->coolDown > 1 && currentStatus->coolDown < 1.5)
    {
        DrawTexturePro(sprite->playerDeath, (Rectangle){32, 0, 32, 16}, list->player->rect, (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(sprite->enemyTwo, (Rectangle){24, 0, 24, 18}, (Rectangle){600, 200, 72, 54}, (Vector2){0, 0}, 0, WHITE);
    }
}

/*Check the input pressed if the selected text is "RETRY"*/
void            ending_retry(EndingText* selectedText, GameScreen* currentScreen, ObjectList* list, Assets* music)
{
    DrawRectangleRec((Rectangle){100, 660, 25, 25}, WHITE);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
    {
        *selectedText = E_Quit;
    }
    if (IsKeyDown(KEY_ENTER))
    {
        player_reset(list);
        enemy_reset(list, 1);
        shield_reset(list);
        *currentScreen = E_LVLONE;
        PlayMusicStream(music->Music_SpIn);
    }
}

/*Check the input pressed if the selected text is "RETRY"*/
void            ending_quit(EndingText* selectedText, bool* exitGame, ObjectList* list)
{
    DrawRectangleRec((Rectangle){500, 660, 25, 25}, WHITE);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT))
    {
        *selectedText = E_Retry;
    }
    if (IsKeyDown(KEY_ENTER))
    {
        *exitGame = true;
    }
}

/*Reset the player to default values*/
void            player_reset(ObjectList* list)
{
    list->player->life = 3;
    list->player->active = true;
    list->projectile->score = 0;
    list->player->animcoolDown = 0;
}

/*Reset the enemies to default values*/
void            enemy_reset(ObjectList* list, float speed)
{
    list->enemies.columns = 11;
    list->enemies.lines = 5;

    for (int i = 0; i < list->enemies.lines; i++)
    {
        for (int j = 0; j < list->enemies.columns; j++)
        {
            list->enemies.enemy[i][j] = (GameObject)
            {(Rectangle){j*62 + 77, i*62 + 50, 48, 36}, YELLOW, (list->player->speed/10)*speed, 1, true, 0};  
        }
    }
}

/*Reset the shields to default values*/
void            shield_reset(ObjectList* list)
{
    list->shield.numberOfShield = 4;
    list->shield.columns = 4;
    list->shield.lines = 4;

    for (unsigned int k = 0; k < list->shield.numberOfShield; k++)
    {
        for (unsigned int i = 0; i < list->shield.lines; i++)
        {
            for (unsigned int j = 0; j < list->shield.columns; j++)
            {
                list->shield.shields[k][i][j] = (GameObject)
                {(Rectangle){(k*200) + (j*25) + 55, i*25 + 475, 25, 25}, GREEN, 0, 5, true, 0};
            }
        }
    }

    shield_active_false(list->shield);
}

/*Get the length of a string*/
unsigned int    string_length(char *array)
{
    unsigned int count = 0;     

    while(*array != '\0')       
    {
        count++;                
        array++;                      
    }
    
    return count;               
}

/*Convert a string into a int number*/
int             alpha_to_number(char *toNumber)
{                                                       
    unsigned int length = string_length(toNumber);                              
    int minus = 1;                                                              
    int result = 0;
    
    for (int i = 0 ; i < length ; i++)                                          
    {
        if (!(toNumber[i] >= 48 && toNumber[i] <= 57) && toNumber[i] != 45)     
        {break;}

        if (toNumber[0] == 45)                                                   
        {minus *= -1;}

        if (toNumber[i] >= 48 && toNumber[i] <= 57)                              
        {
            result *= 10;                                                       
            result += toNumber[i] - 48;                                          
        }

        if(toNumber[i + 1] == 45)                                                
        {break;}
    }
    result *= minus;                                                            
    return result;                                                               
}

/*Convet an int number into a string*/
char*           number_to_alpha(int num)
{
    int digits = 1; 
    int negative = 0; 

    if (num < 0) 
    {
        num *= -1;
        negative = 1;
        digits++; 
    }

    int temp = num;

    while (temp >= 10)
    {
        digits++;
        temp /= 10;
    }

    char* array_number = (char*)malloc((digits+1) * sizeof(char)); 
    if (array_number == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < digits; i++)
    {
        array_number[i] = 48;
    }

    if (negative == 1)
    {
        array_number[0] = 45;
        for (int i = digits - 1; i > 0; i--)
        {
            array_number[i] = (num % 10) + 48;
            num /= 10;
        }
        array_number[digits] = '\0';
    }
    else
    {
        for (int i = digits - 1; i >= 0; i--)
        {
            array_number[i] = (num % 10) + 48;
            num /= 10;
        }
        array_number[digits] = '\0';
    }

    return array_number;
}

/*Free everything and close the windows*/
void            destroy_game(ObjectList* list, Assets* assets, GameStatus* currentStatus)
{
    destroy_enemy(list->enemies);
    shield_destroy(list->shield);
    free(assets);
    free(currentStatus);
    free(list->player);
    free(list->projectile);
    free(list->bonus.UFO);
    free(list->bonus.PowerUp);
    free(list);

    CloseAudioDevice();
    CloseWindow();
}