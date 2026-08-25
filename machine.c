#include "machine.h"
#include "timer.h"
#include <stdio.h>

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
 * Mode can only be changed while the machine is in IDLE.
 */
void machine_select_mode(WashingMachine *machine, WashMode mode)
{
    /* TODO: Implement mode selection logic */
}


/*
 * Start the washing machine.
 *
 * Conditions:
 * - Machine must be IDLE
 * - A valid mode must be selected
 * - Door must be closed
 * - If detergent is absent, start request must remain pending
 */
void machine_start(WashingMachine *machine)
{
    /* TODO: Implement start logic */
}


/*
 * Abort the current washing cycle.
 * Aborting should stop the cycle and unlock the door.
 */
void machine_abort(WashingMachine *machine)
{
    /* TODO: Implement abort logic */
}


/*
 * Open the door.
 * The door cannot be opened while it is locked.
 */
void machine_open_door(WashingMachine *machine)
{
    /* TODO: Implement door opening logic */
}


/*
 * Close the door.
 */
void machine_close_door(WashingMachine *machine)
{
    /* TODO: Implement door closing logic */
}


/*
 * Fill detergent.
 * If a start request is already pending,
 * the machine should automatically start.
 */
void machine_fill_detergent(WashingMachine *machine)
{
    /* TODO: Implement detergent logic */
}