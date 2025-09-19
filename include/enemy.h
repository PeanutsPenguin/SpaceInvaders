#pragma once
#include "game_manager.h"

typedef struct GameObject_s GameObject;
typedef struct EnemyArmy_s EnemyArmy;
typedef struct Assets_s Assets;

EnemyArmy   enemy_init(unsigned int numberLines, unsigned int nummberColumns, float enemySpeed, float projectileSpeed);

void        loop_for_draw_enemy(EnemyArmy arr, Assets* sprite);

void        draw_enemy(GameObject enemy, Assets* sprite, int lines, EnemyArmy army);

void        enemy_update(EnemyArmy arr, float delta, bool* goingRight, GameObject* projectile, Assets* sounds);

int         enemy_alive(EnemyArmy arr);

float       rightest_enemy(EnemyArmy arr);

float       leftest_enemy(EnemyArmy arr);

void        enemy_move(EnemyArmy arr, float delta, bool* goingRight, Assets* sounds);

void        enemy_UP(EnemyArmy arr, float delta);

void        enemy_DOWN(EnemyArmy arr, float delta);

void        enemy_LEFT(EnemyArmy arr, float delta);

void        enemy_RIGHT(EnemyArmy arr, float delta);

bool        check_collision_enemy(GameObject enemy, GameObject* projectile);

bool        check_collision_projectile_enemy(GameObject enemy_projectile, GameObject object);

void        can_enemy_shoot(EnemyArmy arr, float delta);

void        enemy_shoot(EnemyArmy arr, float delta, int rand, Assets* sounds);

void        projectile_move(EnemyArmy arr, float delta);

void        destroy_enemy(EnemyArmy arr);

void        enemy_speedUp(EnemyArmy arr, float speed);