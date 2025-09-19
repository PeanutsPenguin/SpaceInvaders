#include "shield.h"
#include "raylib.h"
#include <stdlib.h>

//Initialize the shields with the values we send in parameters
ShieldsLineUp   shields_init(unsigned int lines, unsigned int columns, unsigned int numberOfShield)
{
    GameObject***   shields = (GameObject***)malloc(sizeof(GameObject**)* numberOfShield);

    /*Using a triple 'for' loop to create*/
    for (unsigned int k = 0; k < numberOfShield; k++)
        {
            shields[k] = (GameObject**)malloc(sizeof(GameObject*)*lines);
            for (unsigned int i = 0; i < lines; i++)
            {
                shields[k][i] = (GameObject*)malloc(sizeof(GameObject)*columns);
                for (unsigned int j = 0; j < columns; j++)
                {
                    shields[k][i][j] = (GameObject){(Rectangle){(k*200) + (j*25) + 55, i*25 + 475, 25, 25},
                    GREEN, 0, 5, true, 0};
                }
            }
        }

    ShieldsLineUp oneShield;
    oneShield.shields = shields;
    oneShield.lines = lines;
    oneShield.columns = columns;
    oneShield.numberOfShield = numberOfShield;

    shield_active_false(oneShield);

    return oneShield;
}

//Delete some blocks of the shield so he has the right form
void            shield_active_false(ShieldsLineUp oneShield)
{
    for (unsigned int i = 0; i < oneShield.numberOfShield; i++)
    {
        oneShield.shields[i][0][0].active = false;
        oneShield.shields[i][0][3].active = false;

        for (unsigned int j = 2; j < 4; j++)
        {
            for (unsigned int k = 1; k < 3; k++)
            {
                oneShield.shields[i][j][k].active = false;
            }
        }
    }
}

//Create a triple loop to draw every block of every shields
void            shield_loop_for_draw(ShieldsLineUp shieldArr, Assets* sprite)
{
    for (unsigned int k = 0; k < shieldArr.numberOfShield; k++)
    {
        for (unsigned int i = 0; i < shieldArr.lines; i++)
        {
            for (unsigned int j = 0; j < shieldArr.columns; j++)
            {
                if (shieldArr.shields[k][i][j].active)
                {
                    shield_draw(shieldArr.shields[k][i][j], sprite);
                }
            }
        }
    }
}

//Draw a specific block of a shield in function of his life
void            shield_draw(GameObject shield, Assets* sprite)
{
    if(shield.life == 5)
    {
        DrawRectangleRec(shield.rect, shield.color);
    }
    if(shield.life == 4)
    {
       DrawTexturePro(sprite->Shield, (Rectangle){165, 0, 55, 33}, shield.rect, (Vector2){0, 0}, 0, WHITE);
    }
    if(shield.life == 3)
    {
        DrawTexturePro(sprite->Shield, (Rectangle){0, 0, 55, 33}, shield.rect, (Vector2){0, 0}, 0, WHITE);
    }
    if(shield.life == 2)
    {
        DrawTexturePro(sprite->Shield, (Rectangle){110, 0, 55, 33}, shield.rect, (Vector2){0, 0}, 0, WHITE);
    }
    if(shield.life == 1)
    {
        DrawTexturePro(sprite->Shield, (Rectangle){55, 0, 55, 33}, shield.rect, (Vector2){0, 0}, 0, WHITE);
    }
}

//Return 'true' if a collision is happening between the two GameObjects we send in parameters
bool            shield_check_collision(GameObject shields, GameObject projectile)
{
    if ((projectile.rect.x + projectile.rect.width) >= (shields.rect.x)
    &&  (projectile.rect.x) <= (shields.rect.x + shields.rect.width)
    &&   projectile.active)
    {
        if ((projectile.rect.y) <= (shields.rect.y + shields.rect.height)
        &&  (projectile.rect.y + projectile.rect.height) >= (shields.rect.y)
        &&   shields.active)
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

//Create a loop so we can apply a function to every block of every shield
void            loop_for_shield_update(ShieldsLineUp oneShield, GameObject* projectile, EnemyArmy enemy, Assets* sounds)
{
    for (unsigned int k = 0; k < oneShield.numberOfShield; k++)
    {
        for (unsigned int i = 0;  i < oneShield.lines; i++)
        {
            for (unsigned int j = 0; j < oneShield.columns; j++)
            {
                shield_update(&oneShield.shields[k][i][j], projectile, enemy, sounds);
            }
        }
    }
}

//Update the the shield status every frame 
void            shield_update(GameObject* shield, GameObject* projectile, EnemyArmy army, Assets* sounds)
{
    if  (shield_check_collision(*shield, *projectile))
    {
        PlaySound(sounds->Shield_Damage);
        shield->life -= 1;
        projectile->active = false;
    }

    if  (shield->life <= 0)
    {
        shield->active = false;
    }

    for (unsigned int l = 0; l < army.lines; l++)
    {
        for (unsigned int m = 0; m < army.columns; m++)
        {
            if  (shield_check_collision(*shield, army.enemy[l][m]))
            {
                shield->life -= 1;
            }  
            if (check_collision_projectile_enemy(army.enemy_projectile[l][m], *shield))
            {
                army.enemy_projectile[l][m].active = false;
                shield->life -= 1;
                PlaySound(sounds->Shield_Damage);
            }
        }
    }
}

//Destroy the shield once the game is ending 
void            shield_destroy(ShieldsLineUp shield)
{
    for (int i = 0; i < shield.numberOfShield; i++)
    {
        for (int j = 0; j < shield.lines; j++)
        {
           free(shield.shields[i][j]);
        }
        
        free(shield.shields[i]);
    }

    free(shield.shields);
}