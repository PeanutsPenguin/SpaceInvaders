#pragma once
#include "raylib.h"
#include "game_manager.h"

typedef struct GameObject_s GameObject;
typedef struct EnemyArmy_s EnemyArmy;
typedef struct Shield_s ShieldsLineUp;
typedef struct UFO_PowerUp_s UFO_PowerUp;
typedef struct ObjectList_s ObjectList;
typedef struct GameStatus_s GameStatus;
typedef struct Assets_s Assets;

GameObject* powerup_init(GameObject* UFO, int speedProjectile);

UFO_PowerUp bonus_init(GameObject* UFO, GameObject* powerups, int rand, int selected_powerup);

void        UFO_update(ObjectList* list, GameStatus* currentStatus, Assets* sounds);

void        powerup_create(ObjectList* list, Assets* sounds);

bool        powerup_active(ObjectList* list);

void        power_player_up(ObjectList* list, Assets* sounds);

void        powerup_move(ObjectList* list, GameStatus* currentStatus);

void        powerup_draw(ObjectList* list);