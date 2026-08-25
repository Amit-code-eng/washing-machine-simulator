#include "machine.h"
#include "timer.h"
#include <stdio.h>

/*
 * Initialize the washing machine.
 */
void machine_init(WashingMachine *machine)
{
    machine->mode = MODE_NONE;
    machine->state = IDLE;
    machine->door_status = DOOR_OPEN;

    machine->remaining_time = 0;
    machine->detergent_present = 0;
    machine->start_requested = 0;
    machine->timer_running = 0;
}

/*
 * Select the wash mode.
 *
 * A mode can only be selected/changed while the machine
 * is in IDLE state.
 */
void machine_select_mode(WashingMachine *machine, WashMode mode)
{
    if (machine->state != IDLE)
    {
        return;
    }

    /*
     * Only valid wash modes are accepted.
     */
    if (mode == MODE_HEAVY ||
        mode == MODE_NORMAL ||
        mode == MODE_LIGHT)
    {
        machine->mode = mode;

        printf("Wash mode selected.\n");
    }
}

/*
 * Start the washing machine.
 *
 * Requirements:
 *
 * 1. Machine must be IDLE.
 * 2. A valid wash mode must be selected.
 * 3. Door must be closed.
 * 4. If detergent is missing, remember the Start request
 *    and move to WAITING_FOR_DETERGENT.
 * 5. If detergent is present, initialize the timer,
 *    lock the door and start washing.
 */
void machine_start(WashingMachine *machine)
{
    /*
     * Start has no effect while washing or during
     * any other non-IDLE state.
     */
    if (machine->state != IDLE)
    {
        return;
    }

    /*
     * A valid mode is mandatory.
     */
    if (machine->mode == MODE_NONE)
    {
        printf("Select a valid wash mode first.\n");
        return;
    }

    /*
     * Door must be closed before starting.
     */
    if (machine->door_status == DOOR_OPEN)
    {
        printf("Close the door before starting.\n");
        return;
    }

    /*
     * Detergent missing:
     *
     * Remember the Start request and wait.
     */
    if (!machine->detergent_present)
    {
        machine->start_requested = 1;
        machine->state = WAITING_FOR_DETERGENT;

        printf("Detergent missing. Start request pending.\n");

        return;
    }

    /*
     * Detergent is available.
     *
     * Initialize the simulated timer.
     */
    machine->remaining_time = get_mode_duration(machine->mode);

    /*
     * A valid mode guarantees a non-zero duration.
     */
    if (machine->remaining_time <= 0)
    {
        machine->remaining_time = 0;
        machine->state = IDLE;

        printf("Invalid wash mode.\n");

        return;
    }

    /*
     * Start washing.
     */
    machine->state = RUNNING;

    /*
     * Door must remain locked while washing.
     */
    machine->door_status = DOOR_LOCKED;

    /*
     * Timer thread will now process the countdown.
     */
    machine->timer_running = 1;

    /*
     * Start request has been consumed.
     */
    machine->start_requested = 0;

    printf("Washing cycle started.\n");
}

/*
 * Abort the current washing cycle.
 *
 * Abort is meaningful while the machine is actively
 * washing. The cycle is stopped immediately and the
 * door is unlocked.
 */
void machine_abort(WashingMachine *machine)
{
    /*
     * Abort only an active washing cycle.
     */
    if (machine->state != RUNNING)
    {
        return;
    }

    /*
     * Stop the timer.
     */
    machine->timer_running = 0;

    /*
     * Clear the current cycle.
     */
    machine->remaining_time = 0;

    /*
     * No pending start request remains.
     */
    machine->start_requested = 0;

    /*
     * Unlock the door.
     */
    machine->door_status = DOOR_OPEN;

    /*
     * Mark cycle as aborted.
     */
    machine->state = ABORTED;

    /*
     * No wash mode is currently selected for the
     * aborted cycle.
     */
    machine->mode = MODE_NONE;

    printf("Washing cycle aborted.\n");
}

/*
 * Open the washing-machine door.
 *
 * The door cannot be opened while:
 *
 *     RUNNING
 *     POWER_FAILURE
 *
 * In both states the door is locked.
 */
void machine_open_door(WashingMachine *machine)
{
    if (machine->state == RUNNING)
    {
        printf("Cannot open door while washing.\n");
        return;
    }

    if (machine->state == POWER_FAILURE)
    {
        printf("Cannot open door during power failure.\n");
        return;
    }

    /*
     * Door can be opened in all safe states.
     */
    machine->door_status = DOOR_OPEN;

    printf("Door opened.\n");
}

/*
 * Close the washing-machine door.
 */
void machine_close_door(WashingMachine *machine)
{
    /*
     * A locked door is already closed.
     */
    if (machine->door_status == DOOR_LOCKED)
    {
        return;
    }

    machine->door_status = DOOR_CLOSED;

    printf("Door closed.\n");
}

/*
 * Fill the detergent compartment.
 *
 * If a Start request is pending, the machine should
 * automatically start, provided the door is still closed.
 */
void machine_fill_detergent(WashingMachine *machine)
{
    machine->detergent_present = 1;

    printf("Detergent filled.\n");

    /*
     * No pending Start request.
     */
    if (!machine->start_requested)
    {
        return;
    }

    /*
     * Start was requested previously, but the door may
     * have been opened after that request.
     *
     * Do not start with an open door.
     */
    if (machine->door_status != DOOR_CLOSED)
    {
        printf("Door is open. Close the door to start.\n");
        return;
    }

    /*
     * The request is valid and the door is closed.
     *
     * Initialize timer and start automatically.
     */
    machine->remaining_time = get_mode_duration(machine->mode);

    if (machine->remaining_time <= 0)
    {
        machine->start_requested = 0;
        machine->state = IDLE;

        printf("Select a valid wash mode first.\n");

        return;
    }

    machine->state = RUNNING;
    machine->door_status = DOOR_LOCKED;
    machine->timer_running = 1;
    machine->start_requested = 0;

    printf("Detergent detected. Washing cycle started automatically.\n");
}