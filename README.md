# Washing Machine Simulator — Lab Assignment

## 1. Project Overview

This project is a **C-based Washing Machine Simulator** developed as part of a lab assignment.

The simulator models the control logic of a washing machine using:

- Modular C programming
- Enumerated machine states
- User input handling
- Wash-mode selection
- Door-state management
- Detergent handling
- Background timer execution using POSIX threads
- Power-failure and power-restoration handling
- State-based control logic
- Console status and event messages

The project demonstrates how a real-world embedded/control-system problem can be represented using a software state machine.

---

## 2. Objectives

The main objectives of this lab assignment are to:

1. Implement a washing-machine control system using C.
2. Divide the application into multiple source and header files.
3. Model machine behavior using explicit states and transitions.
4. Implement different washing modes with different durations.
5. Handle user inputs and invalid inputs safely.
6. Simulate a background timer using a separate thread.
7. Handle power failure while preserving the unfinished washing cycle.
8. Automatically start a pending wash request after detergent is filled.
9. Prevent unsafe operations such as opening a locked door.
10. Build and execute the complete application using GCC and Make.

---

## 3. System Requirements

### Software

The project can be built using:

- GCC
- GNU Make
- POSIX threads (`pthread`)
- MSYS2 UCRT64 on Windows
- Linux with GCC and Make

### Recommended Windows Environment

For Windows, the recommended environment is:

- Visual Studio Code
- MSYS2
- MSYS2 UCRT64 terminal
- GCC
- GNU Make

Install the required packages in the MSYS2 UCRT64 terminal:

```bash
pacman -Syu
```

If MSYS2 requests a terminal restart, close and reopen the UCRT64 terminal and continue the update.

Then install GCC and Make:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
```

Verify the installation:

```bash
gcc --version
make --version
```

---

## 4. Project Structure

```text
washing-machine-simulator/
├── main.c
├── machine.c
├── machine.h
├── input.c
├── input.h
├── display.c
├── display.h
├── timer.c
├── timer.h
├── power.c
├── power.h
├── Makefile
├── reference_washing_machine.exe
├── washing_machine.exe
├── Washing Machine Simulator.pdf
├── Washing_Machine_Simulator_PPT_Lab1.pdf
└── README.md
```

---

## 5. Module Description

### `main.c`

Application entry point.

Responsibilities:

- Initialize the washing-machine object.
- Create the background timer thread.
- Read user commands.
- Dispatch commands to the appropriate module.
- Keep the simulator running until the user selects Exit.

---

### `machine.c` / `machine.h`

Contains the main washing-machine state and control logic.

Responsibilities:

- Machine initialization
- Wash-mode selection
- Starting a washing cycle
- Aborting a washing cycle
- Door opening and closing
- Detergent handling
- Pending start-request handling

The central data structure is:

```c
WashingMachine
```

It stores:

- Current wash mode
- Current machine state
- Door status
- Remaining wash time
- Detergent status
- Pending start request
- Timer status

---

### `input.c` / `input.h`

Handles console input.

Available user operations include:

```text
1.  Start
2.  Abort
3.  Select Heavy Mode
4.  Select Normal Mode
5.  Select Light Mode
6.  Open Door
7.  Close Door
8.  Fill Detergent
9.  Power Off
10. Power On
11. Show Status
12. Exit
```

Invalid input is detected and discarded before returning `INPUT_INVALID`.

---

### `display.c` / `display.h`

Provides console display functionality.

The status display shows:

- Selected mode
- Current machine state
- Door state
- Remaining time
- Detergent status
- Pending start request

---

### `timer.c` / `timer.h`

Implements the simulated washing timer.

The timer runs in a background thread.

The simulation uses:

```text
1 real second = 1 simulated minute
```

Wash durations:

| Mode | Duration |
|---|---:|
| Heavy | 45 minutes |
| Normal | 30 minutes |
| Light | 20 minutes |

When the remaining time reaches zero, the machine enters the `COMPLETED` state.

---

### `power.c` / `power.h`

Handles simulated power failure and power restoration.

During a power failure:

- The current washing cycle is preserved.
- Remaining time is preserved.
- Timer execution is stopped.
- The machine enters `POWER_FAILURE`.

When power is restored:

- The unfinished cycle resumes.
- Remaining time is preserved.
- Timer execution resumes.
- The machine returns to `RUNNING`.

---

## 6. Washing Machine States

The simulator defines the following machine states:

```c
typedef enum
{
    IDLE,
    WAITING_FOR_DETERGENT,
    RUNNING,
    POWER_FAILURE,
    COMPLETED,
    ABORTED
} MachineState;
```

### State Description

| State | Description |
|---|---|
| `IDLE` | Machine is not currently washing and is ready for a valid operation. |
| `WAITING_FOR_DETERGENT` | Start was requested, but detergent is unavailable. |
| `RUNNING` | Washing cycle is active and timer is running. |
| `POWER_FAILURE` | Power has failed during an active washing cycle. |
| `COMPLETED` | Washing cycle has reached zero remaining time. |
| `ABORTED` | Current washing cycle has been manually aborted. |

---

## 7. Door States

The door has three possible states:

```c
typedef enum
{
    DOOR_OPEN,
    DOOR_CLOSED,
    DOOR_LOCKED
} DoorStatus;
```

### Door Behavior

- The machine starts with the door open.
- The door must be closed before starting a wash cycle.
- The door becomes locked when washing starts.
- A locked door cannot be opened.
- The door is unlocked after the washing cycle is aborted or completed.
- The door remains unavailable for opening while the machine is running.

---

## 8. Wash Modes

Three washing modes are supported:

```text
Heavy
Normal
Light
```

Their simulated durations are:

```text
Heavy  = 45 minutes
Normal = 30 minutes
Light  = 20 minutes
```

Since one simulated minute equals one real second, the actual execution times are:

```text
Heavy  -> 45 seconds
Normal -> 30 seconds
Light  -> 20 seconds
```

---

## 9. Start Conditions

A washing cycle can start only when the required conditions are satisfied.

### Required conditions

1. A valid wash mode must be selected.
2. The machine must be in a valid startable state.
3. The door must be closed.
4. Detergent must be available.

If detergent is missing when Start is selected:

```text
State -> WAITING_FOR_DETERGENT
```

The start request is preserved.

When detergent is subsequently filled, the simulator automatically attempts to start the pending cycle.

---

## 10. Timer Operation

The timer is implemented using a background POSIX thread.

The main program creates the thread using:

```c
pthread_create(
    &timer_thread_id,
    NULL,
    timer_thread,
    &machine
);
```

The timer thread executes independently of the user-input loop.

Conceptually:

```text
Main Thread
    |
    +-- Read user input
    |
    +-- Process machine commands


Timer Thread
    |
    +-- Wait 1 second
    |
    +-- Update remaining time
    |
    +-- Detect completion
```

This allows the user to interact with the simulator while the washing timer continues running in the background.

---

## 11. Power Failure Behavior

If power is turned off while the machine is running:

```text
RUNNING
   |
   | Power Off
   v
POWER_FAILURE
```

The remaining time is preserved.

Example:

```text
Remaining Time = 17 minutes
Power Failure
Remaining Time = 17 minutes
```

When power is restored:

```text
POWER_FAILURE
      |
      | Power On
      v
   RUNNING
```

The washing cycle resumes from the preserved remaining time.

---

## 12. Basic State Flow

A simplified normal washing cycle is:

```text
          Select Mode
              |
              v
            IDLE
              |
       Close Door + Detergent
              |
              v
           RUNNING
              |
              | Timer reaches 0
              v
          COMPLETED
```

If detergent is missing:

```text
IDLE
 |
 | Start
 v
WAITING_FOR_DETERGENT
 |
 | Fill Detergent
 v
RUNNING
```

If power fails during washing:

```text
RUNNING
   |
   | Power Off
   v
POWER_FAILURE
   |
   | Power On
   v
RUNNING
```

If the user aborts:

```text
RUNNING
   |
   | Abort
   v
ABORTED
```

---

## 13. Build Instructions

### Using Make

From the project directory, run:

```bash
make
```

The Makefile uses:

```text
gcc
-Wall
-Wextra
-std=c11
-pthread
```

The resulting executable is:

```text
washing_machine
```

On Windows/MSYS2 it may be generated as:

```text
washing_machine.exe
```

### Clean Build

To remove the generated executable:

```bash
make clean
```

Then rebuild:

```bash
make
```

---

## 14. Manual GCC Build

The complete project can also be compiled directly:

```bash
gcc -Wall -Wextra -std=c11 -pthread \
    main.c machine.c input.c display.c timer.c power.c \
    -o washing_machine
```

On Windows/MSYS2:

```bash
gcc -Wall -Wextra -std=c11 -pthread \
    main.c machine.c input.c display.c timer.c power.c \
    -o washing_machine.exe
```

---

## 15. Running the Simulator

Linux:

```bash
./washing_machine
```

Windows/MSYS2:

```bash
./washing_machine.exe
```

After starting, the simulator displays the available commands.

Example:

```text
====================================
       WASHING MACHINE SIMULATOR
====================================
1.  Start
2.  Abort
3.  Select Heavy Mode
4.  Select Normal Mode
5.  Select Light Mode
6.  Open Door
7.  Close Door
8.  Fill Detergent
9.  Power Off
10. Power On
11. Show Status
12. Exit
```

---

## 16. Example Test Sequence

A basic successful wash can be tested using:

```text
1. Select Normal Mode
2. Fill Detergent
3. Close Door
4. Start
5. Show Status
```

The expected state after starting should be approximately:

```text
Mode            : Normal
State           : RUNNING
Door            : LOCKED
Remaining Time  : 30 minutes
Detergent       : PRESENT
```

Because the simulator uses real seconds as simulated minutes, the cycle completes after approximately 30 seconds.

---

## 17. Power-Failure Test

To test power-failure recovery:

```text
1. Select Normal Mode
2. Fill Detergent
3. Close Door
4. Start
5. Wait several seconds
6. Power Off
7. Show Status
8. Power On
9. Show Status
```

The remaining time should be preserved across the simulated power failure.

---

## 18. Detergent-Pending Test

To test the pending-start mechanism:

```text
1. Select Normal Mode
2. Close Door
3. Start
4. Fill Detergent
```

Expected behavior:

```text
Start
  |
  v
WAITING_FOR_DETERGENT
  |
  | Fill Detergent
  v
RUNNING
```

---

## 19. Door-Safety Test

To verify door protection:

```text
1. Select a wash mode
2. Fill detergent
3. Close door
4. Start
5. Attempt to open the door
```

The simulator should reject the door-opening request while the door is locked.

---

## 20. Reference Executable

The project contains:

```text
reference_washing_machine.exe
```

This executable is provided as a reference implementation for comparing expected simulator behavior.

It can be executed from the MSYS2 UCRT64 terminal using:

```bash
./reference_washing_machine.exe
```

Use the provided assignment/problem-statement material as the primary specification when validating behavior.

---

## 21. Assignment Reference Material

The project includes the following reference documents:

```text
Washing Machine Simulator.pdf
Washing_Machine_Simulator_PPT_Lab1.pdf
```

These documents contain the assignment requirements, system behavior, state information, and other material required for implementation and testing.

---

## 22. Important Notes

- The simulator is a console-based software model; it does not control physical washing-machine hardware.
- The timer is simulated rather than connected to a hardware timer peripheral.
- `pthread` is used to provide background timer execution.
- The project uses modular source/header separation.
- Compiler warnings should be enabled during development using `-Wall -Wextra`.
- The generated executable should not be considered the source of truth; the assignment specification and implemented source code define the project.

---

## 23. Author / Lab Information

**Project:** Washing Machine Simulator  
**Course/Lab:** Lab Assignment  
**Language:** C  
**Build System:** GNU Make  
**Compiler:** GCC  
**Threading:** POSIX Threads (`pthread`)  
**Platform:** Linux / Windows with MSYS2 UCRT64
