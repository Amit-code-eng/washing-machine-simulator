#include <stdio.h>
#include "power.h"

/*
 * Handle power failure.
 *
 * Only a RUNNING machine is affected.
 *
 * The following are preserved:
 *
 *     mode
 *     remaining_time
 *     detergent_present
 *     start_requested
 *
 * The machine enters POWER_FAILURE and the door
 * remains locked.
 */
void power_failure(WashingMachine *machine)
{
    /*
     * Power failure only interrupts an active wash.
     */
    if (machine->state != RUNNING)
    {
        return;
    }

    /*
     * DO NOT modify remaining_time.
     *
     * It represents the unfinished portion of the
     * washing cycle.
     */

    /*
     * Stop the timer logically.
     *
     * The timer thread itself remains alive, but
     * timer_tick() will not execute because state
     * is no longer RUNNING.
     */
    machine->timer_running = 0;

    /*
     * Preserve the current washing state by changing
     * only the state to POWER_FAILURE.
     */
    machine->state = POWER_FAILURE;

    /*
     * Door remains locked during power failure.
     */
    machine->door_status = DOOR_LOCKED;

    printf("\nPower failure detected.\n");
    printf("Remaining time preserved: %d minutes.\n",
           machine->remaining_time);
}

/*
 * Restore power.
 *
 * Only POWER_FAILURE can be restored.
 *
 * The preserved remaining_time is not modified.
 */
void power_restore(WashingMachine *machine)
{
    /*
     * Restoration only matters if the machine is
     * currently in POWER_FAILURE.
     */
    if (machine->state != POWER_FAILURE)
    {
        return;
    }

    /*
     * Resume the unfinished washing cycle.
     */
    machine->state = RUNNING;

    /*
     * Door remains locked while washing resumes.
     */
    machine->door_status = DOOR_LOCKED;

    /*
     * Restart the timer thread's logical operation.
     *
     * remaining_time is intentionally untouched.
     */
    machine->timer_running = 1;

    printf("\nPower restored.\n");
    printf("Resuming washing cycle.\n");
    printf("Remaining time: %d minutes.\n",
           machine->remaining_time);
}