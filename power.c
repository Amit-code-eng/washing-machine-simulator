#include <stdio.h>
#include "power.h"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

#define RESET "\033[0m"

/*
 * Handle power failure while the machine is RUNNING.
 *
 * Requirements:
 * - Preserve the current remaining time.
 * - Preserve the unfinished cycle.
 * - Keep the door locked.
 * - Move the machine to POWER_FAILURE state.
 */
void power_failure(WashingMachine *machine)
{
    /* TODO: Implement power failure logic */

    if (machine->state == RUNNING)
    {
        // machine->door_status = DOOR_LOCKED;

        machine->state = POWER_FAILURE;
        machine->timer_running = 0;

        printf(YELLOW
               "POWER FAILURE DETECTED.\n"
               "REMAINING TIME PRESERVED: %d MINUTES.\n" RESET,
               machine->remaining_time);
    }
    else
    {
        printf(CYAN "NO ACTIVE WASHING CYCLE.\n" RESET);
    }
}

/*
 * Handle restoration of power.
 *
 * Requirements:
 * - Power restoration is valid only from POWER_FAILURE.
 * - Resume the unfinished washing cycle.
 * - Preserve the remaining time.
 * - Keep the door locked.
 * - Return to RUNNING state.
 */
void power_restore(WashingMachine *machine)
{
    /* TODO: Implement power restoration logic */

    if (machine->state == POWER_FAILURE)
    {
        machine->door_status = DOOR_CLOSED;
        machine->timer_running = 1;
        machine->state = RUNNING;

        printf(GREEN
               "POWER RESTORED.\n"
               "RESUMING WASH CYCLE.\n"
               "REMAINING TIME: %d MINUTES.\n" RESET,
               machine->remaining_time);
    }
    else
    {
        printf(MAGENTA
               "MACHINE IS NOT IN POWER FAILURE STATE.\n" RESET);
    }
}