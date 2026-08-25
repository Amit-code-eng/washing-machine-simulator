#include "timer.h"
#include <stdio.h>
#include <unistd.h>

#define GREEN "\033[32m"

#define RESET "\033[0m"

/*
 * Return the wash duration for the selected mode:
 * Heavy  -> 45 minutes
 * Normal -> 30 minutes
 * Light  -> 20 minutes
 */
int get_mode_duration(WashMode mode)
{
    /* TODO: Implement mode duration logic */

    return (mode == MODE_HEAVY) ? 45 : (mode == MODE_NORMAL) ? 30
                                   : (mode == MODE_LIGHT)    ? 20
                                                             : 0;
}

/*
 * Timer operates only while the machine is RUNNING.
 *
 * Decrease the remaining time and handle completion
 * when the timer reaches zero.
 */
void timer_tick(WashingMachine *machine)
{
    /* TODO: Implement timer logic */

    if (machine->state != RUNNING)
        return;

    else if (machine->remaining_time == 1)
    {
        machine->state = COMPLETED;
        machine->remaining_time = 0;
        machine->timer_running = 0;
        machine->door_status = DOOR_CLOSED;
        machine->detergent_present = 0;
        machine->mode = MODE_NONE;

        printf(GREEN "\nWASHING CYCLE COMPLETED.\n" RESET);
    }
    else
    {
        machine->remaining_time--;
    }
}

/*
 * Background timer thread.
 *
 * One real second represents one simulated minute.
 * The timer should continue running independently
 * of user input.
 */
void *timer_thread(void *arg)
{
    /* TODO: Implement background timer logic */

    WashingMachine *machine = (WashingMachine *)arg;

    while (1)
    {
        sleep(1);
        timer_tick(machine);
    }

    return NULL;
}