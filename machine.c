#include "machine.h"
#include "timer.h"
#include <stdio.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

#define RESET "\033[0m"

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

    if (((machine->state == RUNNING) ||
         (machine->state == POWER_FAILURE)))
    {
        printf(MAGENTA "WASH MODE CANNOT BE SELECTED .\n" RESET);

        return;
    }
    machine->mode = mode;
    machine->state = IDLE;
    machine->remaining_time = 0;
    machine->timer_running = 0;
    printf(CYAN "WASH MODE SELECTED .\n" RESET);
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

    if ((machine->state != IDLE) && (machine->state != WAITING_FOR_DETERGENT) && (machine->state != COMPLETED) && (machine->state != ABORTED))
    {
        printf(RED "Machine is Not Ready to Start.\n" RESET);
        return;
    }

    if (machine->mode == MODE_NONE)
    {
        printf(RED "Invalide Mode Selected.\n" RESET);
        return;
    }

    if (machine->door_status == DOOR_OPEN)
    {
        printf(RED "Door is Not CLosed.\n" RESET);
        return;
    }

    if (!machine->detergent_present)
    {
        machine->start_requested = 1;
        machine->state = WAITING_FOR_DETERGENT;
        printf(RED "Detergent is Empty.\n" RESET);
        return;
    }

    machine->remaining_time = get_mode_duration(machine->mode);
    machine->door_status = DOOR_LOCKED;
    machine->timer_running = 1;
    machine->state = RUNNING;
    printf(GREEN "Washing Maching is Running.\n" RESET);
}

/*
 * Abort the current washing cycle.
 * Aborting should stop the cycle and unlock the door.
 */
void machine_abort(WashingMachine *machine)
{
    /* TODO: Implement abort logic */
    if (machine->state != RUNNING)
    {
        printf(YELLOW "No Washing Cycle is Currently Running.\n" RESET);
        return;
    }

    machine->state = ABORTED;
    machine->timer_running = 0;
    machine->start_requested = 0;
    machine->mode = MODE_NONE;
    machine->door_status = DOOR_CLOSED;

    printf(MAGENTA "Washing Cycle Aborted.\n" RESET);
}

/*
 * Open the door.
 * The door cannot be opened while it is locked.
 */
void machine_open_door(WashingMachine *machine)
{
    /* TODO: Implement door opening logic */
    if (machine->door_status == DOOR_LOCKED)
    {
        printf(MAGENTA "Door is Locked and Cannot be Opened.\n" RESET);
        return;
    }

    machine->door_status = DOOR_OPEN;
    machine->remaining_time = 0;
    machine->timer_running = 0;
    machine->state = IDLE;
    machine->start_requested = 0;
    printf(CYAN "Door Opended.\n" RESET);
}

/*
 * Close the door.
 */

void machine_close_door(WashingMachine *machine)
{
    /* TODO: Implement door closing logic */
    if (machine->door_status == DOOR_LOCKED)
    {
        printf(YELLOW "Door is Already Locked.\n" RESET);
        return;
    }
    machine->door_status = DOOR_CLOSED;
    printf(BLUE "Door Closed.\n" RESET);
}

/*
 * Fill detergent.
 * If a start request is already pending,
 * the machine should automatically start.
 */
void machine_fill_detergent(WashingMachine *machine)
{
    /* TODO: Implement detergent logic */
    machine->detergent_present = 1;
    printf(CYAN "Detergent Filled.\n" RESET);

    if (machine->start_requested)
    {
        machine->start_requested = 0;

        machine_start(machine);
        return;
    }
}