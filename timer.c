#include "timer.h"
#include <stdio.h>
#include <unistd.h>

/*
 * Return the wash duration for the selected mode.
 *
 * Heavy  -> 45 simulated minutes
 * Normal -> 30 simulated minutes
 * Light  -> 20 simulated minutes
 * None   -> 0
 */
int get_mode_duration(WashMode mode)
{
    switch (mode)
    {
    case MODE_HEAVY:
        return 45;

    case MODE_NORMAL:
        return 30;

    case MODE_LIGHT:
        return 20;

    case MODE_NONE:
    default:
        return 0;
    }
}

/*
 * Decrement the washing timer by one simulated minute.
 *
 * One call to timer_tick() represents one simulated minute.
 *
 * Timer only operates when the machine is RUNNING.
 */
void timer_tick(WashingMachine *machine)
{
    /*
     * Do nothing unless the machine is actively washing.
     */
    if (machine->state != RUNNING)
    {
        return;
    }

    /*
     * Protect against invalid negative values.
     */
    if (machine->remaining_time <= 0)
    {
        machine->remaining_time = 0;

        /*
         * Washing has finished.
         */
        machine->timer_running = 0;
        machine->door_status = DOOR_OPEN;
        machine->state = COMPLETED;

        printf("\nWashing cycle completed.\n");

        return;
    }

    /*
     * Decrement one simulated minute.
     */
    machine->remaining_time--;

    /*
     * Timer reached zero.
     */
    if (machine->remaining_time == 0)
    {
        /*
         * Stop washing.
         */
        machine->timer_running = 0;

        /*
         * Washing is complete.
         */
        machine->state = COMPLETED;

        /*
         * Door is unlocked after completion.
         */
        machine->door_status = DOOR_OPEN;

        printf("\nWashing cycle completed.\n");
    }
}

/*
 * Background timer thread.
 *
 * One real second = one simulated minute.
 *
 * The thread runs independently of the main user-input
 * loop.
 */
void *timer_thread(void *arg)
{
    WashingMachine *machine = (WashingMachine *)arg;

    while (1)
    {
        /*
         * One real second represents one simulated minute.
         */
        sleep(1);

        /*
         * Only tick while the timer is active.
         *
         * timer_running is cleared when:
         *
         * - cycle completes
         * - cycle is aborted
         *
         * During POWER_FAILURE, timer_running remains
         * inactive because the machine is not RUNNING.
         */
        if (machine->timer_running &&
            machine->state == RUNNING)
        {
            timer_tick(machine);
        }
    }

    return NULL;
}