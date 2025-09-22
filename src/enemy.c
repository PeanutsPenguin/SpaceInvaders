#include "enemy.h"
#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#define ARMY_SHOT_CD 1

/*Initializes the Enemy Army struct with a 2D array of enemies, a 2D array of projectiles, 
the number of lines and columns of those arrays, and the enemies shooting cooldown*/
EnemyArmy   enemy_init(unsigned int numberLines, unsigned int nummberColumns, float enemySpeed, float projectileSpeed)
{
    GameObject**    enemies = (GameObject**)malloc(sizeof(GameObject*)* numberLines);
    GameObject**    projectiles = (GameObject**)malloc(sizeof(GameObject*)* numberLines);

    for (unsigned int i = 0; i < numberLines; i++)
    {
        enemies[i] = (GameObject*)malloc(sizeof(GameObject)*nummberColumns);
        projectiles[i] = (GameObject*)malloc(sizeof(GameObject)*nummberColumns);

        for (unsigned int j = 0; j < nummberColumns; j++)
        {
            enemies[i][j] = (GameObject){(Rectangle){j*62 + 77, i*62 + 50, 48, 36}, YELLOW, enemySpeed, 1, true, 0};
            projectiles[i][j] = (GameObject){(Rectangle)
            {enemies[i][j].rect.x + enemies[i][j].rect.width/2, enemies[i][j].rect.y + enemies[i][j].rect.height, 5, 25}, 
            RED, projectileSpeed/2, 1, false, 0};
        }
    }

    EnemyArmy   arr;
    arr.enemy = enemies;
    arr.enemy_projectile = projectiles;
    arr.lines = numberLines;
    arr.columns = nummberColumns;
    arr.cd = 0;

    return arr;
}

/*Draws enemies if they are active or  their animation of death if they just died.
Also draws the enemies' projectile animation*/
void        loop_for_draw_enemy(EnemyArmy arr, Assets* sprite)
{
    for (unsigned int i = 0 ; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if (arr.enemy[i][j].active)
            {
                enemies_animation(arr.enemy[i][j], sprite, i, arr);
            }
            else if (!(arr.enemy[i][j].active) && arr.enemy[i][j].animcoolDown <= 2 && arr.enemy[i][j].animcoolDown >= 1 )
            {
                DrawTexturePro(sprite->enemyDeath, (Rectangle){0, 0, 28, 18}, arr.enemy[i][j].rect, (Vector2){0, 0}, 0, WHITE);
            }
            if (arr.enemy_projectile[i][j].active)
            {
                enemies_projectile_animations(arr.enemy_projectile[i][j], sprite);
            }
        }
    }
}

/*Updates all enemies and their projectile, and everything related to them each frame.*/
void        enemy_update(EnemyArmy arr, float delta, bool* goingRight, GameObject* projectile, Assets* sounds)
{
    int enemyCount = enemy_alive(arr);

    enemy_move(arr, delta, goingRight, sounds);

    int x = Randoms(0, 10); /*This random number will be the COLUMN in which an enemy will shoot*/
    
    can_enemy_shoot(arr, delta);
    
    if (arr.cd >= ARMY_SHOT_CD)
    {
        enemy_shoot(arr, delta, x, sounds);
    }
    projectile_move(arr, delta);

    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if (check_collision_enemy(arr.enemy[i][j], projectile))
            {
                arr.enemy[i][j].animcoolDown = 1;
                projectile->active = false;
                arr.enemy[i][j].active = false;
                PlaySound(sounds->Enemy_death);
                projectile->score += 35 - (5 * (i + 1));
                enemy_speedUp(arr, 1.015);  /*Each time an enemy dies, the other speed up*/
            }
        }
    }
}

/*Counts how many enemies are alive*/
int         enemy_alive(EnemyArmy arr)
{
    int count = 0;
    for (int i = 0; i < arr.lines; i++)
    {
        for (int j = 0; j < arr.columns; j++)
        {
            if (arr.enemy[i][j].active == true)
            {
                count++;
            }
        }
    }

    return count;
}

/*Returns the position of the right side of the column of enemies that is on the far right*/
float       rightest_enemy(EnemyArmy arr)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = arr.columns - 1; j >= 0; j--)
        {
            if (arr.enemy[i][j].active)
            {
                return (arr.enemy[i][j].rect.x + arr.enemy[i][j].rect.width);
            }
        }
    }   
    return 0;
}

/*Returns the position of the left side of the column of enemies that is on the far left*/
float       leftest_enemy(EnemyArmy arr)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if (arr.enemy[i][j].active)
            {
                return (arr.enemy[i][j].rect.x);
            }
        }
    }   
    return 0;
}

/*Moves the entire table of enemies at once, depending on where it is and which screen border it is heading to*/
void        enemy_move(EnemyArmy arr, float delta, bool* goingRight, Assets* sounds)
{
    if (rightest_enemy(arr) < 800 && *goingRight == true)
    {
        enemy_RIGHT(arr, delta);
    }
    else if (rightest_enemy(arr) >= 800 && *goingRight == true)
    {
        enemy_DOWN(arr, delta);
        PlaySound(sounds->Enemy_down);
        enemy_LEFT(arr, delta);
        *goingRight = false;
    }
    if (leftest_enemy(arr) > 0 && *goingRight == false)
    {
        enemy_LEFT(arr, delta);
    }
    else if (leftest_enemy(arr) <= 0 && *goingRight == false)
    {
        enemy_DOWN(arr, delta);
        PlaySound(sounds->Enemy_down);
        enemy_RIGHT(arr, delta);
        *goingRight = true;
    }
}

/*Moves the entire table of enemies up*/
void        enemy_UP(EnemyArmy arr, float delta)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            arr.enemy[i][j].rect.y -= arr.enemy[i][j].speed * delta;
        }
    }
}

/*Moves the entire table of enemies down*/
void        enemy_DOWN(EnemyArmy arr, float delta)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            arr.enemy[i][j].rect.y += arr.enemy[i][j].speed * delta * 1000;
        }
    }
}

/*Moves the entire table of enemies left*/
void        enemy_LEFT(EnemyArmy arr, float delta)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            arr.enemy[i][j].rect.x -= arr.enemy[i][j].speed * delta;
        }
    }
}

/*Moves the entire table of enemies right*/
void        enemy_RIGHT(EnemyArmy arr, float delta)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            arr.enemy[i][j].rect.x += arr.enemy[i][j].speed * delta;
        }
    }
}

/*Checks if each enemy collides with a game object*/
bool        check_collision_enemy(GameObject enemy, GameObject* projectile)
{
    if ((projectile->rect.x + projectile->rect.width) >= (enemy.rect.x)
    &&  (projectile->rect.x) <= (enemy.rect.x + enemy.rect.width)
    &&   projectile->active)
    {
        if ((projectile->rect.y) <= (enemy.rect.y + enemy.rect.height)
        &&  (projectile->rect.y + projectile->rect.height) >= (enemy.rect.y)
        &&   enemy.active)
        {
            DrawText("", 500, 500, 20, RED); /*For a reason only a superior being can decypher, the code doesn't work without this line so we kept it*/
            return true;
        }

        else
        {
            return false;
        }
    }

    return false;
}

/*Checks if each enemy projectile collides with a game object*/
bool        check_collision_projectile_enemy(GameObject enemy_projectile, GameObject object)
{
    if ((enemy_projectile.rect.x + enemy_projectile.rect.width) >= (object.rect.x)
    &&  (enemy_projectile.rect.x) <= (object.rect.x + object.rect.width)
    &&   enemy_projectile.active)
    {
        if ((enemy_projectile.rect.y) <= (object.rect.y + object.rect.height)
        &&  (enemy_projectile.rect.y + enemy_projectile.rect.height) >= (object.rect.y)
        &&   object.active)
        {
            return true;
        }

        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/*Checks if an enemy is the lowest of its column, and therefore can shoot*/
void        can_enemy_shoot(EnemyArmy arr, float delta)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if ((i == 0 && !arr.enemy[i+1][j].active && !arr.enemy[i+2][j].active && !arr.enemy[i+3][j].active && !arr.enemy[i+4][j].active)
            ||  (i == 1 && !arr.enemy[i+1][j].active && !arr.enemy[i+2][j].active && !arr.enemy[i+3][j].active)
            ||  (i == 2 && !arr.enemy[i+1][j].active && !arr.enemy[i+2][j].active)
            ||  (i == 3 && !arr.enemy[i+1][j].active)
            ||  (i == 4)&&  arr.enemy[i][j].active)
            {
                arr.enemy[i][j].color = RED;
                arr.enemy[i][j].life = 3;
            }
            else
                arr.enemy[i][j].color = GREEN;
        }
    }
}

/*If an enemy can shoot and is in the right column (c.f. the random number in enemy_update), an enemy projectile spawns at the position of the enemy*/
void        enemy_shoot(EnemyArmy arr, float delta, int rand, Assets* sounds)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if ((arr.enemy[i][j].life == 3) && (j == rand) 
            && (arr.enemy_projectile[i][j].active == false) && arr.enemy[i][j].active)
            {
                arr.enemy_projectile[i][j].animcoolDown = 1;
                arr.enemy_projectile[i][j].rect.x = arr.enemy[i][j].rect.x + arr.enemy[i][j].rect.width/2;
                arr.enemy_projectile[i][j].rect.y = arr.enemy[i][j].rect.y + arr.enemy[i][j].rect.height;
                arr.enemy_projectile[i][j].active = true;
                PlaySound(sounds->Enemy_shoot);
            }
        }
    } 
}

/*If an enemy projectile is active, drives it downward*/
void        projectile_move(EnemyArmy arr, float delta)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if (arr.enemy_projectile[i][j].active)
            {
                arr.enemy_projectile[i][j].rect.y += arr.enemy_projectile[i][j].speed * delta;

                if (arr.enemy_projectile[i][j].rect.y > 700)
                {
                    arr.enemy_projectile[i][j].active = false;
                }
            }
        }
    }
}

/*Frees the manually allocated memory of every enemy and every enemy projectile*/
void        destroy_enemy(EnemyArmy arr)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        free(arr.enemy_projectile[i]);
        free(arr.enemy[i]);
    }

    free(arr.enemy_projectile);
    free(arr.enemy);
}

/*Speeds up the enemies by a certain amount*/
void        enemy_speedUp(EnemyArmy arr, float speed)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            arr.enemy[i][j].speed *= speed;
        }
    }
}