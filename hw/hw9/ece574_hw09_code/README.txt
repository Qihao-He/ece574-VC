ece574-10 hw9
Name:Qihao He
================================================================================
3. CUDA vs C (2 pts)
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 1
y[100]=1000.000000

real	0m0.080s
user	0m0.040s
sys	0m0.036s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 4
y[100]=1600.000000

real	0m0.131s
user	0m0.084s
sys	0m0.044s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 16
y[100]=13000.000000

real	0m0.251s
user	0m0.224s
sys	0m0.024s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 64
y[100]=202600.000000

real	0m0.506s
user	0m0.456s
sys	0m0.048s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 256
y[100]=3265000.000000

real	0m1.508s
user	0m1.496s
sys	0m0.008s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 1024
y[100]=52378600.000000

real	0m6.028s
user	0m5.996s
sys	0m0.032s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy_c 2048
y[100]=209612032.000000

real	0m11.321s
user	0m11.276s
sys	0m0.044s

--------------------------------------------------------------------------------
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 1
Size: 31250
y[100]=1000.000000 1010.000000

real	0m0.240s
user	0m0.052s
sys	0m0.184s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 4
Size: 31250
y[100]=1600.000000 1616.000000

real	0m0.247s
user	0m0.056s
sys	0m0.188s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 16
Size: 31250
y[100]=13000.000000 13130.000000

real	0m0.240s
user	0m0.072s
sys	0m0.164s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 64
Size: 31250
y[100]=202600.000000 204626.000000

real	0m0.331s
user	0m0.136s
sys	0m0.192s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 256
Size: 31250
y[100]=3265000.000000 3297650.000000

real	0m0.587s
user	0m0.376s
sys	0m0.208s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 1024
Size: 31250
y[100]=52378600.000000 52902164.000000

real	0m1.759s
user	0m1.316s
sys	0m0.440s
ece574-10@Quadro:~/QH_repository/ece574-VC/hw/hw9/ece574_hw09_code$ time ./saxpy 2048
Size: 31250
y[100]=209612032.000000 211709200.000000

real	0m3.283s
user	0m2.332s
sys	0m0.948s
--------------------------------------------------------------------------------
(a) Why is the C version faster with only 1 repetition?

$ time ./saxpy_c 1
y[100]=1000.000000
real	0m0.080s

$ time ./saxpy 1
Size: 31250
y[100]=1000.000000 1010.000000
real	0m0.240s

The CPU process the serial code with low latency and low through put, so at the
low through put condition as the 1 repetition, the C version is faster.
--------------------------------------------------------------------------------
(b) Why is the Cuda version faster for 2048 repetitions?

$ time ./saxpy_c 2048
y[100]=209612032.000000
real	0m11.321s

$ time ./saxpy 2048
Size: 31250
y[100]=209612032.000000 211709200.000000
real	0m3.283s

The GPU process the data with parallel computing, it is high latency and high
through put, so when there is huge amount of data that is 2048 repetitions, it
is doing way faster than the CPU.
--------------------------------------------------------------------------------
(c) What is the crossover point where Cuda is faster than C?

$ time ./saxpy_c 16
y[100]=13000.000000
real	0m0.251s

$ time ./saxpy 16
Size: 31250
y[100]=13000.000000 13130.000000
real	0m0.240s

At the repetitions around 16, the Cuda is faster than c.
--------------------------------------------------------------------------------
(d) How could you improve the performance of the C version?
In this program, the loop order cannot improve the performance that will
lower the cache misses. But the C version currently is using 1 core calculating,
it can use OpenCL and fully utilize the other cores to improve the performance.
To do that, it should be making the code into parallel code instead of serial code.

================================================================================
4. Power/Energy (4 points)

$ perf stat -a -e cycles,power/energy-pkg/ ./saxpy_c 2048
y[100]=209612032.000000

 Performance counter stats for 'system wide':

    46,298,891,954        cycles
            333.19 Joules power/energy-pkg/

      11.603618695 seconds time elapsed
--------------------------------------------------------------------------------
$ perf stat -a -e cycles,power/energy-pkg/ ./saxpy 2048
Size: 31250
y[100]=209612032.000000 211709200.000000

 Performance counter stats for 'system wide':

    13,728,890,417        cycles
             83.29 Joules power/energy-pkg/

       3.251979013 seconds time elapsed
--------------------------------------------------------------------------------
 nvidia-smi --query-gpu=utilization.gpu,power.draw --format=csv -lms 100
utilization.gpu [%], power.draw [W]
 42 %, 10.23 W
 42 %, 14.09 W
 100 %, 14.10 W
 100 %, 16.67 W
 100 %, 16.67 W
 100 %, 16.68 W
 100 %, 16.68 W
 100 %, 16.69 W
 100 %, 16.70 W
 100 %, 16.72 W
 100 %, 16.71 W
 100 %, 16.71 W
 100 %, 16.72 W
 100 %, 16.73 W
 100 %, 16.73 W
 100 %, 16.75 W
 100 %, 16.73 W
 100 %, 16.75 W
 100 %, 16.74 W
 100 %, 16.75 W
 100 %, 16.75 W
 100 %, 16.76 W
 100 %, 16.75 W
 100 %, 16.77 W
 100 %, 16.75 W
 100 %, 16.76 W
 100 %, 16.77 W
 100 %, 16.77 W
 100 %, 16.81 W
 100 %, 16.96 W
 100 %, 16.95 W
 100 %, 3.30 W
 2 %, 3.24 W

 AVG=15.9578125
--------------------------------------------------------------------------------
(a) How much total CPU energy is consumed by the C implementation?
333.19 Joules
--------------------------------------------------------------------------------
(b) How much total CPU+GPU energy is consumed by the GPU implementation?
83.29 Joules,15.9578125*3.2=51.065, total:83.29+51.07=134.36 Joules
--------------------------------------------------------------------------------
(c) Which implementation would you choose if speed were most important? If Energy were most
important? If Energy delay were most important?
The GPU one saxpy.cu would be the priority one if the speed is the most important.
saxpy.cu is also a good choice for Energy were most important. Energy delay is
the most important would also make the saxpy.cu a better choice.

================================================================================
5. Reliability / Checkpointing (2 points)
(a) Why might a cluster located at an observatory at the top of Mauna Kea in Hawaii have a higher
failure rate than an identical cluster located at UMaine?
It should be the Radiation. Locating at an observatory at the top of Mauna Kea
in Hawaii should be receiving a higher rate of catching alpha particles and cause
flip bits.
--------------------------------------------------------------------------------
(b) List a benefit to using application-level checkpointing in your code.
If a failure takes down a program or hardware node, you can restore to last
checkpoint rather than starting from scratch.
--------------------------------------------------------------------------------
(c) List a downside to using application-level checkpointing in your code.
Checkpoints have high overhead. It takes much additional time to run it.

================================================================================
6. Big Data / Hadoop (2 points)
(a) What two major operations are used by Hadoop?
Distributed Processing and Distributed storage on commodity clusters.

--------------------------------------------------------------------------------
(b) What language is used when writing Hadoop code?
Java
--------------------------------------------------------------------------------
(c) Name one benefit of a distributed filesystem (such as the hadoop HDFS
filesystem) over a centralized filesystem such as NFS.
Keeps working in face of hardware failures.
It can handle big data storage, however, NFS cannot handle big data storage.
It has the function of distributed processing and distributed storage that can
fully utilize the processors.
