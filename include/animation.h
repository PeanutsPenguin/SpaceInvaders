#pragma once 

#include "raylib.h"
#include "game_manager.h"

typedef struct GameObject_s GameObject;
typedef struct EnemyArmy_s EnemyArmy;
typedef struct Shield_s ShieldsLineUp;
typedef struct ObjectList_s ObjectList;
typedef struct GameStatus_s GameStatus;
typedef struct Assets_s Assets;

void    player_animation(ObjectList* list, Assets* sprite);

void    projectile_animation(ObjectList* list, Assets* sprite);

void    enemies_animation(GameObject enemy, Assets* sprite, int lines, EnemyArmy army);

void    enemies_projectile_animations(GameObject projectile, Assets* sprite);