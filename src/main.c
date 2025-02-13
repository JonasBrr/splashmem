#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "actions.h"
#include "param.h"
#include "splash.h"
#include "world.h"

int          quitting      = 0;
SDL_Window*  window        = NULL;
SDL_Surface* screenSurface = NULL;

/* ------------------------------------------------------------------------- */
/*                                                                           */
/* ------------------------------------------------------------------------- */
int SDLCALL watch(void* userdata, SDL_Event* event)
{
    if (event->type == SDL_APP_WILLENTERBACKGROUND)
    {
        quitting = 1;
    }

    return 1;
}

/* ------------------------------------------------------------------------- */
/*                                                                           */
/* ------------------------------------------------------------------------- */
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SplashMem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_SIZE,
                              WIN_SIZE, SDL_WINDOW_SHOWN);
    SDL_AddEventWatch(watch, NULL);

    if (argc != 5)
    {
        printf("Wrong argument number\n");
        return 1;
    }
    inits(argc, argv);

    main_loop();

    SDL_DelEventWatch(watch, NULL);
    SDL_DestroyWindow(window);
    SDL_Quit();

    exit(0);

} // main
