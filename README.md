*This activity has been created as part of the 42 curriculum by llafforg.*

# Codexion

## Summary

- [Descritpion](#description)
- [Features](#features)
- [Instructions](#instructions)
- [Thread Synchronization Mechanisms](#thread-synchronization-mechanisms)
- [Blocking cases handled](#blocking-cases-handled)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Technical Choices](#technical-choices)
- [AI Usage](#ai-usage)
- [Resources](#resources)

## Description

Codexion is a concurrency and synchronization project written in C.

The program simulates several coders working simultaneously in a circular co-working space. Each coder is represented by a POSIX thread and repeatedly goes through three stages:

1. **Compiling**
2. **Debugging**
3. **Refactoring**

Compilation requires two USB dongles: one on the coder's left and one on their right. There are exactly as many dongles as coders, so several coders may compete for the same resources.

The main objective of the project is to coordinate concurrent access to these shared resources while preventing race conditions, deadlocks, starvation, and burnout.

The simulation ends when either:

* one coder burns out because they did not start compiling before their deadline;
* or every coder has completed the required number of compilations.

The project supports two scheduling policies:

* `fifo` — First In, First Out
* `edf` — Earliest Deadline First

The project is designed to practice POSIX threads, mutexes, condition variables, synchronization, resource arbitration, timing, and concurrent programming in C.

---

## Features

* One POSIX thread per coder.
* One dongle between each pair of coders.
* Two dongles required simultaneously to compile.
* Per-dongle mutex protection.
* Condition variables for waiting on shared dongles.
* Mandatory dongle cooldown.
* FIFO and EDF scheduling modes.
* Dedicated burnout monitoring thread.
* Thread-safe logging.
* Automatic simulation termination.
* Command-line argument validation.
* Memory cleanup.
* Makefile with compilation and test targets.

---

## Instructions

The project is written in C and uses POSIX threads.

Compile the project with:

```bash
make
```

The Makefile uses:

```text
-Wall -Wextra -Werror -pthread
```

The main executable is:

```text
codexion
```

Other available Makefile targets include:

```bash
make clean
make fclean
make re
make test
make test_big
make valgrind
```


### Usage

The program requires exactly eight arguments:

```text
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Arguments

| Argument                      | Description                                                              |
| ----------------------------- | ------------------------------------------------------------------------ |
| `number_of_coders`            | Number of coders and number of dongles                                   |
| `time_to_burnout`             | Maximum time, in milliseconds, between the beginning of two compilations |
| `time_to_compile`             | Duration of the compilation stage                                        |
| `time_to_debug`               | Duration of the debugging stage                                          |
| `time_to_refactor`            | Duration of the refactoring stage                                        |
| `number_of_compiles_required` | Number of compilations required from every coder                         |
| `dongle_cooldown`             | Time, in milliseconds, during which a released dongle cannot be reused   |
| `scheduler`                   | Scheduling policy: `fifo` or `edf`                                       |

All numeric arguments must be valid positive integers.

### Example

```bash
./codexion 5 100 10 5 10 3 5 fifo
```

EDF can be selected with:

```bash
./codexion 5 100 10 5 10 3 5 edf
```

---

## Thread Synchronization Mechanisms

The project uses POSIX synchronization primitives to coordinate concurrent access to shared resources.

### `pthread_mutex_t`

Each dongle owns a mutex protecting its state.

The mutex prevents multiple coder threads from modifying the same dongle simultaneously.

For example, a coder must acquire the dongle mutex before checking or changing its availability.

This prevents race conditions such as two coders observing the same dongle as available and both attempting to take it.

Mutexes are also used to protect shared coder and simulation state.

### `pthread_cond_t`

Condition variables allow coder threads to wait without continuously polling a dongle while it is unavailable.

A typical waiting sequence is:

```c
pthread_mutex_lock(&dongle->lock);

while (!condition_is_satisfied)
    pthread_cond_wait(&dongle->available, &dongle->lock);

pthread_mutex_unlock(&dongle->lock);
```

The condition is always checked again after waking because a condition variable does not itself represent a boolean state.

The mutex protects the state associated with the condition.

### Monitor thread

A dedicated monitor thread periodically checks the coders' deadlines and the simulation termination conditions.

It is responsible for detecting burnout and stopping the simulation when necessary.

The monitor also checks whether every coder has reached the required number of compilations.

### Logging synchronization

Log output must not be interleaved by concurrent coder threads.

A logging mutex is therefore used to ensure that a state-change message is printed atomically.

This guarantees that two coder messages cannot be mixed on the same line.

---

## Blocking Cases Handled

Concurrency introduces several situations that must be handled carefully.

### Shared dongles

A dongle cannot be assigned to two coders simultaneously.

Its state is protected by a mutex and access is coordinated through a condition variable.

### Cooldown

After a dongle is released, it cannot immediately be reused.

The dongle stores its cooldown deadline and waiting coders must wait until the cooldown has expired.

This prevents a released dongle from being reused too early.

### Deadlock prevention

A coder requires two dongles to compile.

The implementation acquires the two neighbouring dongles in a deterministic order based on their identifiers rather than allowing every coder to acquire them in an arbitrary order.

This reduces the risk of circular resource acquisition and addresses the classic deadlock conditions associated with shared resources.

The four Coffman conditions are:

1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

The deterministic dongle acquisition order is used to prevent circular wait.

### Starvation

The scheduler is intended to prevent a coder from being indefinitely ignored when several coders are competing for the same dongle.

FIFO preserves request arrival order.

EDF prioritizes the coder with the earliest burnout deadline.

A complete scheduler must apply this arbitration consistently to every competing request.

### Burnout detection

A dedicated monitor thread checks coder deadlines independently from the coder threads.

When a coder exceeds its allowed time without starting another compilation, the simulation is stopped and the burnout event is logged.

### Simulation termination

The simulation can terminate for two reasons:

* a coder burns out;
* all coders reach the required number of compilations.

Shared termination state is protected by synchronization mechanisms so that coder threads and the monitor can safely observe the simulation state.

### Log serialization

All state-change messages are protected so that concurrent threads cannot interleave their output.

---

## Project Structure

The main source files are organized according to their responsibilities:

```text
.
├── Makefile
├── README.md
├── codexion.c
├── codexion.h
├── init.c
├── prints.c
├── stages.c
├── strategies.c
├── threads.c
├── utils.c
├── utils_free.c
└── tester.sh
```

### Main responsibilities

| File           | Responsibility                                  |
| -------------- | ----------------------------------------------- |
| `codexion.c`   | Program entry point and general orchestration   |
| `codexion.h`   | Structures and function declarations            |
| `init.c`       | Initialization of simulation structures         |
| `threads.c`    | Creation, monitoring and joining of threads     |
| `stages.c`     | Compile, debug, refactor and dongle acquisition |
| `strategies.c` | Dongle arbitration and scheduling logic         |
| `prints.c`     | Simulation logging                              |
| `utils.c`      | General utility functions                       |
| `utils_free.c` | Resource cleanup                                |
| `tester.sh`    | Test scenarios                                  |

---

## Testing

The Makefile contains predefined scenarios for:

* valid FIFO execution;
* valid EDF execution;
* invalid arguments;
* single-coder behaviour;
* larger simulations;
* Valgrind memory checking.

Run the standard test set with:

```bash
make test
```

Run larger scenarios with:

```bash
make test_big
```

Run Valgrind with:

```bash
make valgrind
```

Additional test cases can be added to `tester.sh`.

---

## Technical Choices

### POSIX threads

`pthread_create()` is used to represent each coder as an independent thread.

`pthread_join()` is used to wait for the termination of the simulation threads.

### Mutex-protected resources

Each dongle has its own mutex.

This provides fine-grained synchronization and avoids protecting all dongles with one global lock.

### Condition variables

Condition variables allow threads waiting for dongles to sleep until the resource state changes instead of continuously consuming CPU time.

### Timing

The simulation uses millisecond timestamps to calculate compilation deadlines and dongle cooldown periods.

The project uses the timing functions permitted by the subject, including `gettimeofday()` / `clock_gettime()` and `usleep()` where appropriate.

---

## AI Usage

AI tools were used as a learning and debugging aid during the development of this project.

They were used for:

* explaining POSIX thread concepts;
* understanding mutexes and condition variables;
* discussing synchronization patterns;
* identifying potential race conditions and deadlocks;


---

## Resources

The following resources were used to understand the concepts involved in the project:

* **42 Codexion subject** — official project specification.
* **POSIX Threads (`pthread`) documentation** — thread creation, joining and synchronization.
* **Codexion Vizualizer**(#https://codexion-visualizer.sacha-dev.me/)
