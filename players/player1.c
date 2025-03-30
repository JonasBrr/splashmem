//RED PLAYER
#include "actions.h"
#include <stdio.h>
#define MAX_PLAY_ACTION 9

char play_actions[] = {
    ACTION_MOVE_L, ACTION_DASH_U, ACTION_TELEPORT_L, ACTION_SPLASH, ACTION_TELEPORT_U, ACTION_TELEPORT_R, ACTION_SPLASH, ACTION_TELEPORT_D, ACTION_MOVE_D,
};

char get_action()
{
    static int i = 0;
    char ret_val = 0;

    ret_val = play_actions[i];
    i++;
    if (i >= MAX_PLAY_ACTION)
    {
        i = 0;
    } 
    return (ret_val);
}