#pragma once
#include "player.h"
#include "enemy.h"
#include "shield.h"
#include "UFO.h"
#include "animation.h"
#define MAX_LINE 5
#define MAX_DIGIT 6

typedef enum GameScreen_e
{
    E_TITLESCREEN,
    E_LVLONE,
    E_ENDING
}GameScreen;

typedef enum EndingText_e
{
    E_Retry,
    E_Quit
}EndingText;

typedef struct GameStatus_s
{
    GameScreen curentScreen;
    EndingText selectedText;
    bool exitGame;
    bool goingRight;
    bool isGamePaused;
    float tilt;
    float pan;
    float canPlayerShoot;
    float deltaTime;
    float coolDown;
    int screenWIDTH;
    char HighScore [MAX_LINE][MAX_DIGIT];
}GameStatus;

typedef struct GameObject_s
{
    Rectangle rect;
    Color color;
    float speed;
    unsigned int life;
    bool active;
    unsigned int score;
    float animcoolDown;
}GameObject;

typedef struct EnemyArmy_s
{
    GameObject** enemy;
    GameObject** enemy_projectile;
    unsigned int lines;
    unsigned int columns;
    float cd;
}EnemyArmy;

typedef struct Shield_s
{
    GameObject*** shields;
    unsigned int lines;
    unsigned int columns;
    unsigned int numberOfShield;
}ShieldsLineUp;

typedef struct UFO_PowerUp_s
{
    GameObject* UFO;
    GameObject* PowerUp;
    int rand;
    int selected_power_up;
}UFO_PowerUp;

typedef struct ObjectList_s
{
    GameObject* player;
    GameObject* projectile;
    UFO_PowerUp bonus;
    EnemyArmy enemies;
    ShieldsLineUp shield;
    unsigned int level;
}ObjectList;

typedef struct Assets_s
{
    Texture2D player;
    Texture2D enemyOne;
    Texture2D enemyTwo;
    Texture2D enemyThree;
    Texture2D projectile;
    Texture2D titleScreen;
    Texture2D background;
    Texture2D Shield;
    Texture2D playerDeath;
    Texture2D UFO;
    Texture2D enemyDeath;
    Sound Player_shoot;
    Sound Enemy_shoot;
    Sound Shield_Damage;
    Sound Enemy_down;
    Sound Player_hit;
    Sound Enemy_death;
    Sound PowerUp;
    Music Music_SpIn;
}Assets;

Assets*         assets_init();

GameStatus*     game_status_init(int SCREENWIDTH);

ObjectList*     object_list_init_gameobjects(GameObject* player, GameObject* projectile);

void            high_score_init(GameStatus* currentStatus);

void            object_list_init_other(ObjectList* list, EnemyArmy enemies, ShieldsLineUp shield, UFO_PowerUp bonus);

ObjectList*     game_init(int speedPlayer, int speedProjectile, int speedEnemy, int screenWidth);

void            game_update(ObjectList* list, int screenwidth, GameStatus* currentScreen, Assets* sounds);

void            add_delta(ObjectList* list, GameStatus* currentStatus);

void            add_delta_animations(ObjectList* list, GameStatus* currentStatus);

void            update_gameobject(ObjectList* list, int screenWidth, GameStatus* currentStatus, Assets* sounds);

void            game_paused(GameStatus* currentStatus, ObjectList* list);

int             Randoms(int lower, int upper);

void            drawHUD(void);

void            screen_update(ObjectList* list, GameStatus* currentScreen, Assets* sprite);

void            update_titlescreen(GameStatus* currentStatus, Assets* sprite);

void            update_lvlone(GameStatus* currenStatus, ObjectList* list, Assets* sprite);

void            update_lvlone_playeractive(ObjectList* list, Assets* sprite);

void            update_high_score(ObjectList* list, GameStatus* currentStatus);

bool            end_check(EnemyArmy arr, ShieldsLineUp shield);

void            update_ending(GameStatus* currentStatus, ObjectList* list, Assets* sprite);

void            draw_ending(ObjectList* list, Assets* sprite,  GameStatus* currentStatus);

void            ending_retry(EndingText* selectedText, GameScreen* currentScreen, ObjectList* list, Assets* music);

void            ending_quit(EndingText* selectedText, bool* exitGame, ObjectList* list);

void            player_reset(ObjectList* list);

void            enemy_reset(ObjectList* list, float speed);

void            shield_reset(ObjectList* list);

unsigned int    string_length(char *array);

int             alpha_to_number(char *toNumber);

char*           number_to_alpha(int number);

void            destroy_game(ObjectList* list, Assets* assets, GameStatus* currentStatus);