#include "UFO.h"
#include <stdlib.h>

/*Initializes the 3 powerups linked to the UFO*/
GameObject* powerup_init(GameObject* UFO, int speedProjectile)
{
    GameObject* powerup = (GameObject*)malloc(3*sizeof(GameObject));

    powerup[0] = (GameObject){(Rectangle){(UFO->rect.x + UFO->rect.width/2), 87, 6, 25}, YELLOW, speedProjectile/4, 0, false, 0, 0};
    powerup[1] = (GameObject){(Rectangle){(UFO->rect.x + UFO->rect.width/2), 87, 6, 25}, BLUE, speedProjectile/4, 0, false, 0, 0};
    powerup[2] = (GameObject){(Rectangle){(UFO->rect.x + UFO->rect.width/2), 87, 6, 25}, GREEN, speedProjectile/4, 0, false, 0, 0};

    return powerup;
}

/*Initializes the UFO_PowerUp struct with the UFO, an array of powerups,
 a random position in which the UFO will spawn the powerups and the default selected powerup that will spawn*/
UFO_PowerUp bonus_init(GameObject* UFO, GameObject* powerups, int rand, int selected_powerup)
{
    UFO_PowerUp bonus;
    bonus.UFO = UFO;
    bonus.PowerUp = powerups;
    bonus.rand = rand;
    bonus.selected_power_up = selected_powerup;

    return bonus;
}

/*Updates the UFO and the powerups each frame*/
void        UFO_update(ObjectList* list, GameStatus* currentStatus, Assets* sounds)
{
    if (list->bonus.UFO->active && ((list->bonus.UFO->rect.x + list->bonus.UFO->rect.width) > 0))
    {
        list->bonus.UFO->rect.x -= (list->bonus.UFO->speed * currentStatus->deltaTime);
        powerup_create(list, sounds);
    }
    if (powerup_active(list))
    {
        powerup_move(list, currentStatus);
    }

    power_player_up(list, sounds);
}

/*Spawns a powerup if there are none active when the UFO reaches a previously randomly generated position*/
void        powerup_create(ObjectList* list, Assets* sounds)
{
    int index = list->bonus.selected_power_up;
    if (!powerup_active(list)
    && list->bonus.UFO->rect.x <= list->bonus.rand + 25 
    && list->bonus.UFO->rect.x >= list->bonus.rand - 25)
    {
        int selected = list->bonus.selected_power_up;
        list->bonus.PowerUp[selected].rect.x = (list->bonus.UFO->rect.x + list->bonus.UFO->rect.width/2) - list->bonus.PowerUp[selected].rect.width/2;
        list->bonus.PowerUp[selected].rect.y = list->bonus.UFO->rect.y - list->bonus.PowerUp[selected].rect.height;
        list->bonus.PowerUp[selected].active = true;
        PlaySound(sounds->PowerUp);
    }
}

/*Checks if there is any powerup currently active*/
bool        powerup_active(ObjectList* list)
{
    for (int i = 0; i < 3; i++)
    {
        if (list->bonus.PowerUp[i].active)
        {
            return true;
        }
    }
    return false;
}

/*Empowers the player when it collides with a power up.
The gained power depends on the color of the powerup*/
void        power_player_up(ObjectList* list, Assets* sounds)
{
    if (check_collision_enemy(list->bonus.PowerUp[0], list->player))
    {
        list->player->speed += 150;
        list->bonus.PowerUp[0].active = false;
    }
    if (check_collision_enemy(list->bonus.PowerUp[1], list->player))
    {
        if (list->player->life < 3)
        {
            list->player->life += 1;
        }
        list->bonus.PowerUp[1].active = false;
    }
    if (check_collision_enemy(list->bonus.PowerUp[2], list->player))
    {
        shield_reset(list);
        list->bonus.PowerUp[2].active = false;
    }
    if (list->bonus.UFO->score >= 10 && list->player->speed > 300)
    {
        list->player->speed = 300;
    }
}

/*If a powerup is active, drives it downward*/
void        powerup_move(ObjectList* list, GameStatus* currentStatus)
{
    int selected = list->bonus.selected_power_up;

    list->bonus.PowerUp[selected].rect.y += list->bonus.PowerUp[selected].speed * currentStatus->deltaTime;

    if  ((list->bonus.PowerUp[selected].rect.y + list->bonus.PowerUp[selected].rect.height) > 700)
    {
        list->bonus.PowerUp[selected].active = false;
    }
}

/*Draws the powerups when they are active*/
void        powerup_draw(ObjectList* list)
{
    if (powerup_active(list))
    {
        int selected = list->bonus.selected_power_up;
        DrawRectangleRec(list->bonus.PowerUp[selected].rect, list->bonus.PowerUp[selected].color);
    }
}