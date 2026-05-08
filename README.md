# SimpleMultithreader - Using Multithreading with Ease

## Design Document for Operating Systems (OS) Assignment 5

---

# Group Members & Contribution

## 2024138 - Atharva Singh Velpula

### Contributions

```cpp id="r41mry"
void parallel_for(int low, int high, std::function &&lambda, int numThreads);
```

---

## 2024343 - Mayank Yadav

### Contributions

```cpp id="7wsp7e"
void parallel_for(int low1, int high1, int low2, int high2, std::function &&lambda, int numThreads);
```

---

## Joint Contribution

Both members contributed to:

* Error handling
* Bug fixing

---

# Implementation Details

* The codebase is well documented using:

  * Comments
  * Appropriate variable names
  * Meaningful function names

---

# `simple-multithreader.h`

---

# `struct pfor1_args`

## Functionality

This structure stores:

* Lower limit of the loop
* Upper limit of the loop
* Lambda function to execute inside the loop

---

## Stored Data

```text id="dfjlwm"
- lower bound
- upper bound
- lambda function
```

---

# `void* pfor1_func(void* ptr)`

## Functionality

This function is passed to each thread for execution.

---

## Workflow

1. Unpacks `ptr`
2. Retrieves:

   * Loop lower limit
   * Loop upper limit
   * Lambda function
3. Executes the loop
4. Calls the lambda function for every iteration

---

## Purpose

* Serves as the worker function for pthread execution.
* Enables concurrent execution of loop iterations.

---

# `void parallel_for(int low, int high, std::function &&lambda, int numThreads);`

## Functionality

Implements parallel execution for a **1D loop** using multithreading.

---

# Work Distribution

## Chunk Size Calculation

The function:

* Divides the loop range across threads.
* Determines chunk size using:

```text id="z7r3f6"
(high - low) / numThreads
```

---

## Handling Extra Iterations

If the loop range is not perfectly divisible by `numThreads`:

* Extra iterations are distributed among the first few threads.

This ensures:

* Balanced workload distribution.

---

# Execution Flow

## Timing

* Records loop start time before execution.
* Records loop end time after completion.
* Prints total execution duration.

---

## Thread Creation

Creates:

```text id="gr2exu"
numThreads - 1
```

new threads so that:

* Total threads including the main thread equals `numThreads`.

---

## Thread Tasks

Each thread:

* Receives:

  * `pfor1_func`
  * Appropriate argument structure
* Executes its assigned loop chunk.

---

## Main Thread Participation

* The final chunk of work is executed by the main thread itself.
* Prevents unnecessary thread creation overhead.

---

## Synchronization

The main thread:

* Waits for all created threads to finish execution using:

```cpp id="kw08gw"
join
```

---

# Cleanup

After execution:

* Stored pthread IDs are cleaned up.
* Dynamically allocated argument structures are cleaned up.

---

# `void parallel_for(int low1, int high1, int low2, int high2, std::function &&lambda, int numThreads);`

## Functionality

Implements parallel execution for a **2D loop**.

---

# Design Approach

Instead of implementing separate threading logic:

* The 2D version reuses the 1D `parallel_for()` implementation.

---

# Parallelization Strategy

## Outer Loop Parallelization

The outer loop:

* Executes in parallel using the 1D implementation.

---

## Inner Loop Execution

A new lambda function is created:

* Matches the signature required by the 1D implementation.
* Executes the original lambda:

```text id="6vhfdg"
(high2 - low2)
```

times for each iteration of the outer loop.

---

# Advantages

## Code Modularity

This design:

* Avoids duplication of threading logic.
* Keeps implementation modular and maintainable.

---

## Reusability

The 2D implementation:

* Directly reuses:

  * Thread management
  * Synchronization
  * Work distribution

from the 1D implementation.

---

# Parallel Execution Workflow

## 1D Parallel For

```text id="t5sj6o"
Loop Range
    ↓
Divide into Chunks
    ↓
Create Threads
    ↓
Execute Lambda in Parallel
    ↓
Join Threads
    ↓
Print Execution Time
```

---

## 2D Parallel For

```text id="pnk5po"
Outer Loop
    ↓
Parallelized using 1D parallel_for
    ↓
Inner Loop executed inside lambda
    ↓
Combined 2D execution
```

---

# Threading Model

| Component        | Purpose                        |
| ---------------- | ------------------------------ |
| `pthread`        | Thread creation and management |
| `join`           | Synchronization                |
| Lambda Functions | User-defined parallel work     |
| Chunking Logic   | Balanced workload distribution |

---

# Key Features

* Parallel execution of loops
* 1D and 2D loop support
* Dynamic workload balancing
* Modular implementation
* Reusable threading logic
* Execution time measurement
* Main-thread participation optimization

---

# Key Concepts Used

* Multithreading
* Parallel programming
* Lambda functions
* Work partitioning
* Thread synchronization
* Modular software design

---

# Notes

* The implementation ensures efficient utilization of threads by balancing workload across all threads.
* Using the main thread for execution reduces unnecessary overhead.
* The 2D implementation builds on top of the 1D implementation to maintain modularity.
* Timing functionality allows performance comparison between serial and parallel execution.
* The implementation is designed to simplify parallel programming using an easy-to-use interface.
