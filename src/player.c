#include <stdlib.h>
#include <stdbool.h>
#include "player.h"

/*Initialize the Player struct with the value we send in parameters*/
GameObject* gameobject_init(Rectangle object_rect, Color object_color, float object_speed, unsigned int number_lives)
{
    GameObject* object = (GameObject*)malloc(sizeof(GameObject));

    object->rect = object_rect;
    object->color = object_color;
    object->speed = object_speed;
    object->life = number_lives;
    object->active = true;
    object->score = 0;
    object->animcoolDown = 0;

    return object;
}

/*Move the player if special Inputs are pressed */
void        player_move(GameObject* player, int width_screen, float delta)
{
    if  (IsKeyDown(KEY_RIGHT) && (player->rect.x + player->rect.width < width_screen))
    {
        player->rect.x += player->speed * delta;
    }
    if  (IsKeyDown(KEY_LEFT) && player->rect.x > 0)
    {
        player->rect.x -= player->speed * delta;
    }
}

/*Check if there is a collision between the player and an enemy Projectile*/
void        player_collisions(GameObject* player, EnemyArmy arr, Assets* sounds, GameObject* projectile)
{
    for (unsigned int i = 0; i < arr.lines; i++)
    {                                                    /*We're using a double 'for' loop to check collisions with all the enemies*/
        for (unsigned int j = 0; j < arr.columns; j++)
        {
            if (check_collision_projectile_enemy(arr.enemy_projectile[i][j], *player))
            {
                projectile->active = false;
                player->animcoolDown = 1;               /*This 'float' number is used to play the Player hit animation*/
                arr.enemy_projectile[i][j].active = false;
                player->life -= 1;
                PlaySound(sounds->Player_hit);
                
            }
        }
    }
}

/*Update the player status every frame*/
void        player_update(ObjectList* list, int width_screen, float delta, Assets* sounds)
{
    if  (!list->player->active)
    {
        return;
    }

    player_move(list->player, width_screen, delta);

    player_collisions(list->player, list->enemies, sounds, list->projectile);

    if (list->player->life == 0)
    {
        list->player->active = false;
    }
}

/*If the input to shoot is pressed, this function create the projectile*/
void        projectile_create(ObjectList* list, GameStatus* currentStatus, Assets* sounds)
{
    if (IsKeyPressed(KEY_SPACE) && !(list->projectile->active) && (currentStatus->canPlayerShoot >= 1))
    {
        list->projectile->animcoolDown = 1;             /*This 'float' number is used to animate the projectile*/
        list->projectile->rect.x = (list->player->rect.x + list->player->rect.width/2) 
                                    - list->projectile->rect.width/2;
        list->projectile->rect.y = list->player->rect.y - list->projectile->rect.height;
        list->projectile->active = true;
        currentStatus->canPlayerShoot = 0;              /*This 'float' number is used to delay the player shooting*/
        PlaySound(sounds->Player_shoot);
    }
}

/*If a projectile is created, this function update his status every frame*/
void        projectile_update(ObjectList* list, float delta, GameStatus* currentStatus, Assets* sounds)
{
    projectile_create(list, currentStatus, sounds);

    if  (list->projectile->active)
    {
        list->projectile->rect.y -= list->projectile->speed * delta;

        if  ((list->projectile->rect.y + list->projectile->rect.height) < 0)
        {
            list->projectile->active = false;
        }

        for (unsigned int i = 0; i < list->enemies.lines; i++)
        {                                                               /*We use a double 'for' loop to check collisions with all the enemies*/
            for (unsigned int j = 0; j < list->enemies.columns; j++)
            {
                if  (check_collision_projectile_enemy(list->enemies.enemy_projectile[i][j], *(list->projectile)))
                {
                    list->projectile->active = false;
                    list->enemies.enemy_projectile[i][j].active = false;
                }
            }
        }
    }
    list->player->score = list->projectile->score;
}