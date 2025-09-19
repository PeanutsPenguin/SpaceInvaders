#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "game_manager.h"
#define PLAYER_SPEED 300
#define PROJECTILE_SPEED 500
#define ENEMY_SPEED 30
#define SCREENWIDTH 800
#define SCREENHEIGHT 700

void    main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Space Invaders");
    InitAudioDevice();

    GameStatus* currentStatus = game_status_init(SCREENWIDTH);
    Assets* assets = assets_init();
    ObjectList* objectlist = game_init(PLAYER_SPEED, PROJECTILE_SPEED, ENEMY_SPEED, SCREENWIDTH);

    PlayMusicStream(assets->Music_SpIn);


    while   (!WindowShouldClose() && !currentStatus->exitGame)
    {
        screen_update(objectlist, currentStatus, assets);
        game_update(objectlist, SCREENWIDTH, currentStatus, assets);
    }
     
    destroy_game(objectlist, assets, currentStatus);
}