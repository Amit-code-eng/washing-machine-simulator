#include <stdio.h>
#include "power.h"


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
}