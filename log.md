This is a log file of the ece574.
--------------------------------------------------------------------------------
3/23/2017
Reading the lec13 notes about the MPI program.
MPI code
Rank

custom Data types
You can creat custom data types that aren't the MPI default, sort of like
structures.

QUESTION: Can you just cast your data into integers and uncast on the other
side.
I think that because the data structure can be customized, so the data can be
cast into integers and uncast on the other side.

running the MPI code is:
$mpiexec -n 4 ./test_mpi
You'll often see mpirun instead. Some implementations have that, but it's not
the official standard way.

Send Example
mpi_send.c
Run with
$mpirun -np 4 ./mpi_send

Sends 1 millon integers(each with value of 1) to each node.
Each adds up 1/4th then sends only the sum(a single int) back.
Notice this is a lot like pthreads where we have to go a lot of work manually.

mpi_wtime.c
Same as previous example, but with timing.
Unlike PAPI, the time is returned as a floating point value.

QUESTION
PAPI returns a long integer that is showing the time, but the mpi_wtime.c
returns the the floating point. Are they measuring in cycles? or in Wall clock
time?

lec12
The Linux limitations makes the Distributed System.
MPI-Message Passing Interface

lec11
Scope
static and dynamic schedule
QUESTION: Why is the Dynamic not working as fast as the static schedule in the
test?

Results:
openmp static schedule.c
• Creates 100 threads with chunksize of 1.
• Threads are assigned loop indices at compile time.
• In example, thread 0 is fastest and 4 the slowest.
• You can see thread 0 runs through its assignment fast
and then sits around doing nothing while the rest slowly
finish.

Dynamic schedule
openmp dynamic schedule.c
• Creates 100 threads with chunksize of 1.
• Threads are assigned loop indices dynamically.
• Each thread starts with one, but zero runs all the rest
because it is so fast.

QUESTION:
If this intuitive theory would lead to the process of handling data time
consuming, why is the static and dynamic schedule in our HW showing the opposite
results.
Why is the thread zero runs all the rest because it is so fast? Isn't the
thread are sharing the processor evenly?

Changing Chunksize
Threads are assignned same amount of time to run.
Spread mostly evenly but the last set of chunks, only two threads get assigned
while the others have nothing to do.
Switch to "guided" and the chunksize decreases over time and the ending is a bit
more balanced.

QUESTION: Why is the dynamic allocate the chunksize is leaving only two threads
assigned and reast doing nothing. Why is the guided helping the allocating?

Critical section
Use for race condition.
section
reduction directive
simd reduction
offload

lec10
OpenMP
Shared memory multi-processing interface.
Goal: parallelize serial code by just adding a few compiler directives here and
there.
No need to totally re-write code like you would with pthread or MPI.
OpenMP interface:
    compiler directives
        Spawning a parallel region
        Dividing blocks of code among threads
        Distributing loop iterations between threads
        Serializing sections of code
        Synchronization of work among threads
    library routines
        • Need to #include <omp.h>
        • Getting and setting the number of threads
        • Getting a thread’s ID
        • Getting and setting threads features
        • Checking in in parallel region
        • Checking nested parallelism
        • Locking
        • Wall clock measurements
    environment variables
        • Setting number of threads
        • Configuring loop iteration division
        • Processor bindings
        • Nested Parallelism settings
        • Dynamic thread settings
        • Stack size
        • Wait policy

parallel directives
variable scopes
work-sharing constructs
For
scheduling
data dependencies
reductions
OMP sections
synchronization
flush directive
nested parallelism

--------------------------------------------------------------------------------
3/29/2017
Deciding for final project title. Maybe making cluster machine using PI.
Maybe joining Nicolas team. Alternative option would be using PI GPU for FFT
algorithm enhancement.

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
