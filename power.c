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
        printf(YELLOW "Power Failure Detected.\nRemaining time preserved: %d minutes.\n" RESET, machine->remaining_time);
    }
    else
    {
        printf(CYAN "No Active Washing Cycle.\n" RESET);
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
        machine->state = RUNNING;
        printf(GREEN "Power Restored.\nResuming Wash Cycle.\nRemaining Time: %d minutes.\n" RESET, machine->remaining_time);
    }
    else
    {
        printf(MAGENTA "Machine is Not in Power Failure State.\n" RESET);
    }
}