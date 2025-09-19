#pragma once
#include "raylib.h"
#include "game_manager.h"

typedef struct GameObject_s GameObject;
typedef struct EnemyArmy_s EnemyArmy;
typedef struct Shield_s ShieldsLineUp;
typedef struct ObjectList_s ObjectList;
typedef struct GameStatus_s GameStatus;
typedef struct Assets_s Assets;

GameObject* gameobject_init(Rectangle object_rect, Color object_color, float object_speed, unsigned int number_lives);

void        player_move(GameObject* player, int width_screen, float delta);

void        player_collisions(GameObject* player, struct EnemyArmy_s arr, Assets* sounds, GameObject* projectile);

void        player_update(ObjectList* list, int width_screen, float delta, Assets* sounds);

void        projectile_create(ObjectList* list, GameStatus* currentStatus, Assets* sounds);

void        projectile_update(ObjectList* list, float delta, GameStatus* currentStatus, Assets* sounds);