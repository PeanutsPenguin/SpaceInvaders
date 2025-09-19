#pragma once 
#include "game_manager.h"

typedef struct GameObject_s GameObject;
typedef struct EnemyArmy_s EnemyArmy;
typedef struct Shield_s ShieldsLineUp;
typedef struct Assets_s Assets;

ShieldsLineUp   shields_init(unsigned int lines, unsigned int columns, unsigned int numberOfShield);

void            shield_active_false(ShieldsLineUp oneShield);

void            shield_loop_for_draw(ShieldsLineUp shield, Assets* sprite);

void            shield_draw(GameObject shield, Assets* sprite);

bool            shield_check_collision(GameObject shields, GameObject projectile);

void            loop_for_shield_update(ShieldsLineUp oneShield, GameObject* projectile, EnemyArmy enemy, Assets* sounds);

void            shield_update(GameObject* shield, GameObject* projectile, EnemyArmy army, Assets* sounds);

void            shield_destroy(ShieldsLineUp shield);