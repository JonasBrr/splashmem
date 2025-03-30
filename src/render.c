#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "actions.h"
#include "param.h"
#include "splash.h"
#include "world.h"

/* ------------------------------------------------------------------------- */
/*                                                                           */
/* ------------------------------------------------------------------------- */
void render_map()
{
    int      i = 0;
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = SQUARE_SIZE;
    rect.h = SQUARE_SIZE;

    screenSurface = SDL_GetWindowSurface(window);

    for (i = 0; i < SPOT_NUM; i++)
    {
        rect.x = (i % MAP_SIZE) * SQUARE_SIZE;
        rect.y = (i / MAP_SIZE) * SQUARE_SIZE;
        SDL_FillRect(screenSurface, &rect, colors[mapmem[i]]);
    }
    SDL_UpdateWindowSurface(window);
}

/* ------------------------------------------------------------------------- */
/*                                                                           */
/* ------------------------------------------------------------------------- */
int check_balance(int i, char action)
{
    if (action == ACTION_DASH_L || action == ACTION_DASH_R || action == ACTION_DASH_U || action == ACTION_DASH_D)
    {
        if (players[i]->credits < 10)
        {
            return 0;
        }
        players[i]->credits -= 10;
        return 1;
    }
    if (action == ACTION_BOMB)
    {
        if (players[i]->credits < 9)
        {
            return 0;
        }
        players[i]->credits -= 9;
        return 1;
    }

    if (action == ACTION_SPLASH)
    {
        if (players[i]->credits < 8)
        {
            return 0;
        }
        players[i]->credits -= 8;
        return 1;
    }
    if (action == ACTION_TELEPORT_L || action == ACTION_TELEPORT_R || action == ACTION_TELEPORT_U || action == ACTION_TELEPORT_D)
    {
        if (players[i]->credits < 2)
        {
            return 0;
        }
        players[i]->credits -= 2;
        return 1;
    }

    if (action == ACTION_MOVE_L || action == ACTION_MOVE_R || action == ACTION_MOVE_U || action == ACTION_MOVE_D || action == ACTION_STILL)
    {
        if (players[i]->credits < 1)
        {
            return 0;
        }
        players[i]->credits -= 1;
        return 1;
    }
    return 0;
}

void action_player(char action, int i)
{
    int x_cord, y_cord;

    if (check_balance(i, action) == 0)
    {
        printf("Solde insuffisant !\n");
        action = ACTION_STILL;
    }
    // On regarde si les bombes ne doivent pas exploser et si y en a on les décrémentes
    for (int j = 0; j < 5; j++){
        if (players[i]->actions[j][0] != 0){
            players[i]->actions[j][0] -= 1;
            if (players[i]->actions[j][0] == 0){
                for (int ind_j = -1; ind_j < 2; ind_j++){
                    for (int ind_i =-1; ind_i < 2; ind_i++){
                        x_cord = ind_j + players[i]->actions[j][1];
                        y_cord = ind_i + players[i]->actions[j][2];
                        if (x_cord < 0)
                            x_cord = MAP_SIZE - 1;
                        if (x_cord >= MAP_SIZE)
                            x_cord = 0;
                        if (y_cord < 0)
                            y_cord = MAP_SIZE - 1;
                        if (y_cord >= MAP_SIZE)
                            y_cord = 0;
                        world_paint_spot(x_cord, y_cord, i+1);
                    }
                }
            }
        }
    }



    switch (action)
    {
    case ACTION_MOVE_L:
        players[i]->x += -1;
        if (players[i]->x == -1)
        {
            players[i]->x = MAP_SIZE - 1;
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;

    case ACTION_MOVE_R:
        players[i]->x += 1;
        if (players[i]->x >= MAP_SIZE)
        {
            players[i]->x = 0;
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    case ACTION_MOVE_U:
        players[i]->y -= 1;
        if (players[i]->y == -1)
        {
            players[i]->y = MAP_SIZE - 1;
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    case ACTION_MOVE_D:
        players[i]->y += 1;
        if (players[i]->y == MAP_SIZE)
        {
            players[i]->y = 0;
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    case ACTION_DASH_L:
        for (int j = 0; j < 8; j++){
            players[i]->x += -1;
            if (players[i]->x == -1)
            {
                players[i]->x = MAP_SIZE - 1;
            }
            world_paint_spot(players[i]->x, players[i]->y, i+1);
        }
        break;
    case ACTION_DASH_R:
        for (int j = 0; j < 8; j++){
            players[i]->x += 1;
            if (players[i]->x >= MAP_SIZE)
            {
                players[i]->x = 0;
            }
            world_paint_spot(players[i]->x, players[i]->y, i+1);
        }
        break;
    case ACTION_DASH_U:
        for (int j = 0; j < 8; j++){
            players[i]->y -= 1;
            if (players[i]->y == -1)
            {
                players[i]->y = MAP_SIZE - 1;
            }
            world_paint_spot(players[i]->x, players[i]->y, i+1);
        }
        break;
    case ACTION_DASH_D:
        for (int j = 0; j < 8; j++){
            players[i]->y += 1;
            if (players[i]->y == MAP_SIZE)
            {
                players[i]->y = 0;
            }
            world_paint_spot(players[i]->x, players[i]->y, i+1);
        }
        break;
    case ACTION_TELEPORT_D:
            for (int j = 0; j < 8; j++){
            players[i]->y += 1;
            if (players[i]->y == MAP_SIZE)
            {
                players[i]->y = 0;
            }
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    case ACTION_TELEPORT_U:
        for (int j = 0; j < 8; j++){
            players[i]->y -= 1;
            if (players[i]->y == -1)
            {
                players[i]->y = MAP_SIZE - 1;
            }
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    case ACTION_TELEPORT_L:
        for (int j = 0; j < 8; j++){
            players[i]->x += -1;
            if (players[i]->x == -1)
            {
                players[i]->x = MAP_SIZE - 1;
            }
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    case ACTION_TELEPORT_R:
        for (int j = 0; j < 8; j++){
            players[i]->x += 1;
            if (players[i]->x >= MAP_SIZE)
            {
                players[i]->x = 0;
            }
        }
        world_paint_spot(players[i]->x, players[i]->y, i+1);
        break;
    
    case ACTION_SPLASH:
        for (int j_ind = -1; j_ind < 2; j_ind++){
            for (int i_ind =-1; i_ind < 2; i_ind++){
                x_cord = j_ind + players[i]->x;
                y_cord = i_ind + players[i]->y;
                if (x_cord < 0)
                    x_cord = MAP_SIZE - 1;
                if (x_cord >= MAP_SIZE)
                    x_cord = 0;
                if (y_cord < 0)
                    y_cord = MAP_SIZE - 1;
                if (y_cord >= MAP_SIZE)
                    y_cord = 0;
                world_paint_spot(x_cord, y_cord, i+1);
            }
        }
        break;
    case ACTION_BOMB:
        int indice_bombe = 0;
        while (players[i]->actions[indice_bombe][0] != 0 && indice_bombe < 5)
            indice_bombe++;
        
        players[i]->actions[indice_bombe][0] = 5;
        players[i]->actions[indice_bombe][1] = players[i]->x;
        players[i]->actions[indice_bombe][2] = players[i]->y;
        break;
    case ACTION_STILL:
        printf("ACTION_STILL\n");
        break;
    default:
        break;
    }
}

void render_player()
{
    int i         = 0;
    screenSurface = SDL_GetWindowSurface(window);
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = SQUARE_SIZE;
    rect.h = SQUARE_SIZE;

    for (i = 0; i < SPOT_NUM; i++)
    {
        rect.x = (i % MAP_SIZE) * SQUARE_SIZE;
        rect.y = (i / MAP_SIZE) * SQUARE_SIZE;
        SDL_FillRect(screenSurface, &rect, colors[mapmem[i]]);
    }
    SDL_UpdateWindowSurface(window);
}

/* ------------------------------------------------------------------------- */
/*                                                                           */
/* ------------------------------------------------------------------------- */
void main_loop()
{
    void *handle1, *handle2, *handle3, *handle4;
    char (*player1action)(char *);
    char (*player2action)(char *);
    char (*player3action)(char *);
    char (*player4action)(char *);
    char *error;

    handle1 = dlopen("./build/players/libplayer1.so", RTLD_LAZY);
    if (!handle1)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    player1action = dlsym(handle1, "get_action");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    handle2 = dlopen("./build/players/libplayer2.so", RTLD_LAZY);
    if (!handle2)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    player2action = dlsym(handle2, "get_action");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    handle3 = dlopen("./build/players/libplayer3.so", RTLD_LAZY);
    if (!handle3)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    player3action = dlsym(handle3, "get_action");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    handle4 = dlopen("./build/players/libplayer4.so", RTLD_LAZY);
    if (!handle4)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    player4action = dlsym(handle4, "get_action");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    while (!quitting)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quitting = 1;
            }
        }

        action_player(player1action("NULL"), 0);
        action_player(player2action("Test"), 1);
        action_player(player3action("Test"), 2);
        action_player(player4action("Test"), 3);

        render_map();

        SDL_Delay(100);
    }
    dlclose(handle1);
    dlclose(handle2);
    dlclose(handle3);
    dlclose(handle4);
}
