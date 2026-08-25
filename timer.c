#include "timer.h"
#include <stdio.h>
#include <unistd.h>

/*
 * Return the wash duration for the selected mode:
 * Heavy  -> 45 minutes
 * Normal -> 30 minutes
 * Light  -> 20 minutes
 */
int get_mode_duration(WashMode mode)
{
    /* TODO: Implement mode duration logic */
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
}