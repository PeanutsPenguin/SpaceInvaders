#include "animation.h"

//Animate and draw the player 
void    player_animation(ObjectList* list, Assets* sprite)
{
    if (list->player->animcoolDown >= 1 && list->player->animcoolDown <= 1.25 
        || list->player->animcoolDown >= 1.5 && list->player->animcoolDown <= 1.75)
    {
        DrawTexturePro(sprite->playerDeath, (Rectangle){0, 0, 32, 16}, 
        list->player->rect, (Vector2){0, 0}, 0, WHITE);
    }
    else if (list->player->animcoolDown >= 1.25 && list->player->animcoolDown <= 1.5 
            || list->player->animcoolDown >= 1.75 && list->player->animcoolDown <= 2)
    {
        DrawTexturePro(sprite->playerDeath, (Rectangle){32, 0, 32, 16}, 
        list->player->rect, (Vector2){0, 0}, 0, WHITE);
    }
    else 
    {
        DrawTexturePro(sprite->player, (Rectangle){0, 0, 32, 16}, list->player->rect, (Vector2){0, 0}, 0, WHITE);
    }
}

//Animate and draw the player projectile if his active
void    projectile_animation(ObjectList* list, Assets* sprite)
{
    if (list->projectile->animcoolDown >= 1 && list->projectile->animcoolDown <= 1.20 
        || list->projectile->animcoolDown >= 1.40 && list->projectile->animcoolDown <= 1.60
        || list->projectile->animcoolDown >= 1.80 && list->projectile->animcoolDown <= 2
        || list->projectile->animcoolDown >= 2.20 && list->projectile->animcoolDown <= 2.40)
    {
        DrawTexturePro(sprite->projectile, (Rectangle){0, 0, 6, 12}, 
        list->projectile->rect, (Vector2){3, 6}, 0, WHITE);
    }
    else if (list->projectile->animcoolDown >= 1.20 && list->projectile->animcoolDown <= 1.40 
            || list->projectile->animcoolDown >= 1.60 && list->projectile->animcoolDown <= 1.80
            || list->projectile->animcoolDown >= 2 && list->projectile->animcoolDown <= 2.20
            || list->projectile->animcoolDown >= 2.40 && list->projectile->animcoolDown <= 2.50)
    {
        DrawTexturePro(sprite->projectile, (Rectangle){0, 0, 6, 12}, 
        list->projectile->rect, (Vector2){3, 6}, 180, WHITE);
    } 
}

//Animate and draw the enemies 
void    enemies_animation(GameObject enemy, Assets* sprite, int lines, EnemyArmy army)
{
    if (lines >= 3)
    {
        if (army.cd < 0.5)
        {
            DrawTexturePro(sprite->enemyOne, (Rectangle){24, 0, 24, 18}, enemy.rect, (Vector2){0, 0}, 0, WHITE);
        }
        else 
        {
            DrawTexturePro(sprite->enemyOne, (Rectangle){0, 0, 24, 18}, enemy.rect, (Vector2){0, 0}, 0, WHITE);
        }
    }
    else if (lines >= 1 && lines < 3)
    {
        if (army.cd < 0.5)
        {
            DrawTexturePro(sprite->enemyTwo, (Rectangle){24, 0, 24, 18}, enemy.rect, (Vector2){0, 0}, 0, WHITE);
        }
        else 
        {
            DrawTexturePro(sprite->enemyTwo, (Rectangle){0, 0, 24, 18}, enemy.rect, (Vector2){0, 0}, 0, WHITE);
        }
    }
    else if (lines < 1)
    {
        if (army.cd < 0.5)
        {
            DrawTexturePro(sprite->enemyThree, (Rectangle){24, 0, 24, 18}, enemy.rect, (Vector2){0, 0}, 0, WHITE);
        }
        else 
        {
            DrawTexturePro(sprite->enemyThree, (Rectangle){0, 0, 24, 18}, enemy.rect, (Vector2){0, 0}, 0, WHITE);
        }
    }
}

//Animate and draw the enemies projectile if his active
void    enemies_projectile_animations(GameObject projectile, Assets* sprite)
{
    if (projectile.animcoolDown >= 1 && projectile.animcoolDown <= 1.25 
        || projectile.animcoolDown >= 1.50 && projectile.animcoolDown <= 1.75
        || projectile.animcoolDown >= 2 && projectile.animcoolDown <= 2.25)
    {
        DrawTexturePro(sprite->projectile, (Rectangle){0, 0, 6, 12}, 
        projectile.rect, (Vector2){3, 6}, 0, RED);
    }
    else if (projectile.animcoolDown >= 1.25 && projectile.animcoolDown <= 1.50 
            || projectile.animcoolDown >= 1.75 && projectile.animcoolDown <= 2
            || projectile.animcoolDown >= 2.25 && projectile.animcoolDown <= 2.50)
    {
        DrawTexturePro(sprite->projectile, (Rectangle){0, 0, 6, 12}, 
        projectile.rect, (Vector2){3, 6}, 180, RED);
    } 
}