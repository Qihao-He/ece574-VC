This is the log file.
________________________________________________________________________________
3/31/2017
searching for PI cluster machine application.

________________________________________________________________________________
4/8/2017
Reading article for building RPi cluster.

Build your own supercomputer out of Raspberry Pi boards
Who says you need a few million bucks to build a supercomputer? Joshua Kiepert put together a Linux-powered Beowulf cluster with Raspberry Pi computers for less than $2,000.
 Steven J. Vaughan-Nichols
By Steven J. Vaughan-Nichols for Linux and Open Source | May 23, 2013 -- 21:43 GMT (14:43 PDT) | Topic: SMBs
http://www.zdnet.com/article/build-your-own-supercomputer-out-of-raspberry-pi-boards/
________________________________________________________________________________
4/9/2017 4/10/2017
RPi's matrix multiplication.

Raspberry Pi
Performance Benchmarking
Justin Moore
Salish Kootenai College
http://opensky.ucar.edu/islandora/object/siparcs%3A132/datastream/PDF/view

Page 5:
Performance – Floating Point Operations
• How we measure performance
• Busy CPU? Speed?
• What is a floating point operation (FLOP)?
• Arithmetic operation
• Formats
• Single
• Double
• Performance measurement by FLOPS
• How do we measure FLOPS?
• General Matrix Multiplication (GEMM)
• High computational intensity with an increase in matrix size

Page 6:
Performance Benchmarking
• Single Raspberry Pi
• BLAS - Basic Linear Algebra Subprograms
• ATLAS -Automatically Tuned Linear Algebra Software
• Auto tunes BLAS for any system
• Raspberry Pi Cluster
• MPI - Message Passing Interface
• Standard API for inter-process communication
• Facilitates parallel programming
• MPICH 2-1.4.1p1
• HPL - High Performance LINPACK
• Tuned MPI
• Combined with ATLAS
• Wrote Custom code
• ATLAS
• Added parallel capability
• Compared with HPL

Page7-8 introduce Naive implementation of GEMM and the Naive pitfalls.
Page 9 Introducing block matrix multiplication:
* Matrix is split into smaller matrices/blocks
* Shrink matrix size to allow both A & B into fast memory.
Page 10 Cluster software Tuning
* How do we distribute the matrix multiplication?
** MPI used to distribute blocks to nodes(in RPi cluster, we have 23 nodes)
* MyGEMM allows for experimentation on block size
--------------------------------------------------------------------------------
Single Node:

Computational intensity
single precision, single node MyGEMM

Blocking efficiency
single precision,single node w/MyGemm
Display of N=512, 1024

Variation of GPU footprint
single precision, single node W/MyGemm

Clock Speed
QUESTION: How does the clock speed varies changes the MFLOPS?

Performance Comparison
Single and Double MFLOPS Comparison.

--------------------------------------------------------------------------------
RPi cluster

Double precision
HPL W/ATLAS
HPL W/BLAS
MPI W/MyGemm
--------------------------------------------------------------------------------
Conclusion:
Perfotmance is dependent on key factors:
* Matrix size
* Block size
* RAM size
* CPU Speed

Reading
RPi 3 Cluster Test
Raspberry Pi 3 Cluster Test

 02:07:00  JORDI CORBILLA  ALGEBRA, MATHS, MATRIX, PERFORMANCE, PYTHON, RPI3, TESTING
 http://thundaxsoftware.blogspot.com/2016/08/raspberry-pi-3-cluster-test.html
The test will play with 2 main variables:
a) the size of the matrix and
b) the number of nodes to use to perform the calculations.

Notice: Better performance can be reached when increasing the CPUs each node.
RPi has 4 CPUs and we can control the number of CPU used through the machinefile
and MPI.

/*------There is a sample script to grab the cpu usage for linux.---------*/

#!/bin/bash
# @Author: Jordi Corbilla
while true; do
  (echo "%CPU %MEM ARGS $(date)" && ps -e -o pcpu,pmem,args --sort=pcpu | cut -d" " -f1-5 | tail) >> cpu.log; sleep 0.1;
done

#Example results;
#%CPU %MEM ARGS Fri 12 Aug 19:11:20 UTC 2016
#87.0  1.8 python /home/pi/mpi4py-2.0.0/demo/matrixmultiplication.py
#90.0  1.8 python /home/pi/mpi4py-2.0.0/demo/matrixmultiplication.py
#90.5  1.9 python /home/pi/mpi4py-2.0.0/demo/matrixmultiplication.py
#92.5  1.8 python /home/pi/mpi4py-2.0.0/demo/matrixmultiplication.py

/*----------------- end of sample script --------------------------*/

Study of the highest throughput. The time is not linear in this case, overhead
should be taken into consideration. The throughput is achieved by splitting the
matrix using as many nodes as possible.
 Speedup comparison.

Results:
The results of the execution can be seen below (time in seconds):

Test	Sequential Time	CPU1	CPU2	CPU3	CPU4	CPU1	CPU2	CPU3	CPU4	CPU1	CPU2	CPU3	CPU4	Max	Speedup
12x12	0.01111	0.00525				0.0037				0.0037				0.00525	2.116190476
12x12	0.01111	0.00292	0.00202			0.00208	0.00303			0.00206	0.00211			0.00303	3.666666667
12x12	0.01111	0.00197	0.00175	0.00129		0.00205	0.00182	0.00122		0.00218	0.00183	0.00127		0.00218	5.096330275
12x12	0.01111	0.00214	0.00233	0.00212	0.00194	0.00202	0.00234	0.00267	0.00185	0.00201	0.00222	0.00209	0.00241	0.00267	4.161048689
60x60	1.29955	0.4778				0.36717				0.36728				0.4778	2.719861867
60x60	1.29955	0.35127	0.27552			0.20697	0.34767			0.27474	0.207			0.35127	3.699575825
60x60	1.29955	0.21966	0.22068	0.15385		0.22046	0.22196	0.15476		0.22054	0.22058	0.15436		0.22196	5.854883763
60x60	1.29955	0.18464	0.18589	0.18397	0.18379	0.18283	0.1851	0.18473	0.18374	0.18373	0.18427	0.18396	0.18381	0.18589	6.990962397
144x144	15.02287	5.08906				4.99491				4.93309				5.08906	2.951993099
144x144	15.02287	2.78648	2.90336			2.67886	2.78634			2.89459	2.68409			2.90336	5.174304943
144x144	15.02287	3.11941	2.69587	2.022		3.12694	2.69165	2.0254		3.12809	2.68855	2.01999		3.12809	4.802569619
144x144	15.02287	2.37735	2.3879	2.2758	2.38267	2.37713	2.27037	2.38764	2.37814	2.25782	2.38018	2.38604	2.27187	2.3879	6.29124754
216x216	51.69995	17.26683				17.08989				16.66551				17.26683	2.994177275
216x216	51.69995	9.67602	8.63827			8.60859	9.65453			8.62195	8.62782			9.67602	5.343100779
216x216	51.69995	10.52823	9.46254	7.30951		10.51718	9.44692	7.30529		10.54083	9.48649	7.28061		10.54083	4.90473236
216x216	51.69995	8.13615	8.12379	8.00465	8.13003	8.13992	7.94381	8.14352	8.1071	8.00023	8.13681	8.14298	7.999	8.14352	6.348599868


 Creating a Raspberry Pi 3 Cluster - "Supercomputer", for parallel computing.

 22:53:00  JORDI CORBILLA  ARCHITECTURE, COMPUTING, LINUX, NETWORK, PARALLEL, RASPBERRY PI, RPI3, Supercomputer
 http://thundaxsoftware.blogspot.com/2016/07/creating-raspberry-pi-3-cluster.html


 (best benchmark for cluster performance is linpack)

 What is the point of cluster computing?

 Post a reply   22 posts
 by toneboy1 » Fri Jun 28, 2013 6:56 pm
 Hi,
 So I'm interested in Cluster computing with some Pis, but what can I do with a cluster computer? Can I run them as one desktop? Can I run them as one XBMC device? (sharing the load so they run smoother and cooler)


by TideMan » Fri Jun 28, 2013 9:47 pm
 Also, learning about and experimenting with parallelisation can develop a useful programming skill that could be valuable.
Some problems lend themselves to parallelisation, such as Monte Carlo simulations where the runs can simply be divided amongst processors and the results assembled at the end. Other applications are problematical, such as matrix decomposition, where the time it takes for processors to communicate with each other can exceed the time saved by splitting the computations between processors.
The best way to develop knowledge of this sort is by trying it out, and a cluster of RPis would give you this opportunity.


by Jim Manley » Fri Jun 28, 2013 10:59 pm
The performance of the Pi's ARM CPU and network interface are low enough that if you're doing serious parallel computing work where throughput really matters, you're much better off using multi-core based systems on a cost-per-cycle basis. However, if you just want to learn the principles of multiple-processor technology, you could configure a cluster of Pii to use Linux-based Message Passing Interface (MPI) or Parallel Virtual Machine (PVM) tools such as Beowulf, Rocks, OpenMPI, MPICH, Oracle (nee Sun) Grid Engine, Open Grid Engine, etc.

I don't know how many of these have been ported to an ARM Linux, and specifically a Linux distro that's been ported to the Pi's ARMv6 flavor. Massively parallel, high-performance computing systems with thousands of computing nodes generally run on multi-core Intel or AMD x86 architectures, but source code is available for all but Oracle Grid Engine.

To get you started, here's a link to PhD student Joshua Kiepert's report on how he set up a 32-node Pi-based Beowulf cluster at Boise State University:

http://coen.boisestate.edu/ece/files/2013/05/Creating.a.Raspberry.Pi-Based.Beowulf.Cluster_v2.pdf

Here's how Professor Simon Cox built an MPI-based Pi cluster at the University of Southampton:

http://www.southampton.ac.uk/~sjc/raspberrypi/pi_supercomputer_southampton.htm

A different use of clustering is for improving web server performance by using multiple, coordinated, computing devices, and Interworx has ported their product to the Pi, for which a free demo license is available for experimentation:

http://pi.interworx.com

Here's a 24-node Pi cluster running WSO2 middleware to allow for higher-performance app services operations:

http://www.raspberrypi.org/phpBB3/viewtopic.php?f=24&t=41449

More can be found with a Google search for terms like "Raspberry Pi" and "cluster". Oh, and enthusiasts have come to call Pi clusters "Raspberry Brambles" :)
The best things in life aren't things ... but, a Pi comes pretty darned close! :D
"Education is not the filling of a pail, but the lighting of a fire." -- W.B. Yeats
In theory, theory & practice are the same - in practice, they aren't!!!

________________________________________________________________________________
4/11/2017
Pi-Crust: A Raspberry Pi Cluster Implementation
http://jorgehc.com/files/pi_crust_paper.pdf

The performance of a 10 nodes Pi-Crust RPi-cluster evaluation.

• Random reboots: This problem happened occasionally
while performing long benchmark runs. It may have to
do with processor overheating. However, the reboots were
not that frequent and did not recur after the first night.
If heat ever did become a problem it would be a simple
thing to attach small fans (even USB) or even heat sinks
to the processors.

________________________________________________________________________________
4/12/2017
Reading
How I setup a Raspberry Pi 3 Cluster Using The New Docker Swarm Mode In 29 Minutes
Malcolm JonesFollow
TONY DARK. Black Tony Stark. DevOps Engineer @Behance (acq’d by @Adobe). NUPE, WAHOO. Commonly referred to as #heron, SON OF BABYLON, Le #Hyena. I build shit.
Jul 10, 2016
https://medium.com/@bossjones/how-i-setup-a-raspberry-pi-3-cluster-using-the-new-docker-swarm-mode-in-29-minutes-aa0e4f3b1768
________________________________________________________________________________
4/13/2017
My setup: Hardware
RPi * N, microUSB * N, N-ports USB 2.0 HUB, N-ports Ethernet switch router,
Ethernet cable * N.

EZ pre-setup for SD card:
Preparation Step — Flash Micro SD Cards With Hypriot v0.8.0
This little guy takes care of image customization very efficiently before the Raspberry Pi is even turned on. Here you can predefine a proper hostname, WIFI settings, and more. On top of that, it has error handling built into it, handles unmounting the SD card for writing, and prompts you when everything is done. Must have tool. For each SD card, I ran the command below with the following host names, scarlett-kub-master, scarlett-kub-slave1, scarlett-kub-slave2, scarlett-kub-slave3. (NOTE: Hostnames remain the same as when I was working on k8s).

SSH connection (essential)
When dealing w/ multiple nodes and SSH Access, using SSH keys to connect to remote machines is a must.
To set up passwordless authentication I did the following (NOTE: This assumes that you have a private key created already; if you don’t, see this guide on how to get setup http://www.tecmint.com/ssh-passwordless-login-using-ssh-keygen-in-5-easy-steps/):

Now to test that everything works, I ran:
$ ssh pirate@scarlett-kub-master.local

 (Optional) Set up SSH Autocomplete
QUESTION: What does a Autocomlete do?

Installation Of Docker 1.12.0-rc3 On Each Node
QUESTION: What does a docker do in the cluser for all the nodes.

SSH to machine, run docker version and you should see something like this:
Looks pretty eazy for grabing the information of the system and the software.

Bootstrap The Master!
$ ssh pirate@scarlett-kub-master.local docker swarm init
Swarm initialized: current node (1njlvzi9rk2syv3xojw217o0g) is now a manager.

$ docker node ls
ID HOSTNAME MEMBERSHIP STATUS AVAILABILITY MANAGER STATUS
al55dh1yjho2wojhagzksdqwu * scarlett-kub-master Accepted Ready Active Leader

Time To Bootstrap Some Nodes!
$ function getip() { (traceroute $1 2>&1 | head -n 1 | cut -d\( -f 2 | cut -d\) -f 1) }
$ export MASTER=$(getip scarlett-kub-master.local)
$ ssh pirate@scarlett-kub-slave1.local docker swarm join $MASTER:2377

Let’s bring up a container that simply pings google’s DNS server:

Scale up the number of tasks running! How about 10 of these bad boys? No problem, let’s run this:

 Sit Back In Awe.

QUESTION: When trying to add a remote node to VW's cluster, is the HypriotOS and Docker
matters for using different docker and HypriotOS(preset the RPis)? I think it
should maintain the same docker, but the hypriotOS is not that strictly required.
________________________________________________________________________________
4/14/2017
Raspberry Pi
Performance Benchmarking
Justin Moore
Salish Kootenai College
In Page 10, the cluster software tuning.
How do we distribute the matrix multiplication?
• MPI used to distribute blocks to nodes
It briefly says that each node will get a part of the matrix evenly divided.

________________________________________________________________________________
4/16/2017
Reading:
Build a Compact 4 Node Raspberry Pi Cluster
By Alasdair Allan Time Required: 3-8 HoursDifficulty: Moderate
http://makezine.com/projects/build-a-compact-4-node-raspberry-pi-cluster/

Building the hardware (now I am having 1RPi, there is 1 resting in the lab, may
  need a another one, need to ask Bruce.)
KEY POINT: N RPIS, USB HUB POWER, ROUTER, Ethernet

Configuring the cluster:
allocate the ip address
generate ssh keys for ssh between the nodes without having to repetitively type
my password all the time.

NOTE: file system for the cluster is very important.
Adding a disk:
mount a usb stick, and enable the automatically mount on boot.
However I wanted to go a bit further and make this disk available to all four of the nodes. To do this I used NFS and autofs. On all four of the nodes you’ll need to go ahead and install the NFS client software,
NOTE: Network File System (NFS) is a distributed file system protocol
Master node should be installed with the NFS server software.
Slave nodes should be installed with the NFS client software, also need to create
a mount point.

to add the three compute nodes. After doing that you’ll need to restart the RPC services,
NOTE: RPC
In distributed computing a remote procedure call (RPC) is when a computer program causes a procedure to execute in another address space,
autofs is a program for automatically mounting directories on an as-needed basis. Auto-mounts are mounted only as they are accessed, and are unmounted after a period of inactivity.

Instead of directly connecting the Ethernet switch to the external network, and having my home router allocate IP addresses for each of the nodes, as a next step I’m going to add a USB Ethernet adaptor to the head node.
The first will connect to the external network, giving the head node — and hence the cluster — an ‘external’ IP address.The second will connect to the cluster’s Ethernet switch. We can then configure the head node as a DHCP server for other three ‘internal’ nodes attached to the switch, creating a second network visible only to the cluster.

Add a LCD
Add a second Ethernet connection.

Monitoring and testing:
From here there are a couple of things we could do, the most obvious next step would be to add some SNMP monitoring, and an external facing ‘status’ dashboard on the head node to monitor the cluster health.
________________________________________________________________________________
4/17/2017
MPI for RPi cluster:

TECHNOLOGY
Installing MPI for Python on a Raspberry Pi Cluster
26 Jan 2015 3:59pm, by akshay pai
https://thenewstack.io/installing-mpi-python-raspberry-pi-cluster-runs-docker/

Installing MPI for Python

Checking the MPI version:
$ mpiexec --version
HYDRA build details:
    Version:                                 3.1
    Release Date:                            Thu Feb 20 11:41:13 CST 2014
    CC:                              gcc -D_FORTIFY_SOURCE=2 -g -O2 -fstack-protector-strong -Wformat -Werror=format-security -Wl,-z,relro
    CXX:                             g++ -D_FORTIFY_SOURCE=2 -g -O2 -fstack-protector-strong -Wformat -Werror=format-security -Wl,-z,relro
    F77:                             gfortran -g -O2 -fstack-protector-strong -Wl,-z,relro
    F90:                             gfortran -g -O2 -fstack-protector-strong -Wl,-z,relro
    Configure options:                       '--disable-option-checking' '--prefix=/usr' '--build=arm-linux-gnueabihf' '--includedir=${prefix}/include' '--mandir=${prefix}/share/man' '--infodir=${prefix}/share/info' '--sysconfdir=/etc' '--localstatedir=/var' '--libdir=${prefix}/lib/arm-linux-gnueabihf' '--libexecdir=${prefix}/lib/arm-linux-gnueabihf' '--disable-maintainer-mode' '--disable-dependency-tracking' '--enable-shared' '--enable-fc' '--disable-rpath' '--disable-wrapper-rpath' '--sysconfdir=/etc/mpich' '--libdir=/usr/lib/arm-linux-gnueabihf' '--includedir=/usr/include/mpich' '--docdir=/usr/share/doc/mpich' '--with-hwloc-prefix=system' 'build_alias=arm-linux-gnueabihf' 'MPICHLIB_CFLAGS=-g -O2 -fstack-protector-strong -Wformat -Werror=format-security' 'MPICHLIB_CXXFLAGS=-g -O2 -fstack-protector-strong -Wformat -Werror=format-security' 'MPICHLIB_FFLAGS=-g -O2 -fstack-protector-strong' 'MPICHLIB_FCFLAGS=-g -O2 -fstack-protector-strong' 'CFLAGS=-g -O2 -fstack-protector-strong -Wformat -Werror=format-security -g -O2 -fstack-protector-strong -Wformat -Werror=format-security -O2' 'LDFLAGS=-Wl,-z,relro ' 'CPPFLAGS=-D_FORTIFY_SOURCE=2 -I/build/mpich-nW7vHl/mpich-3.1/src/mpl/include -I/build/mpich-nW7vHl/mpich-3.1/src/mpl/include -I/build/mpich-nW7vHl/mpich-3.1/src/openpa/src -I/build/mpich-nW7vHl/mpich-3.1/src/openpa/src -I/build/mpich-nW7vHl/mpich-3.1/src/mpi/romio/include' 'CXXFLAGS=-g -O2 -fstack-protector-strong -Wformat -Werror=format-security -g -O2 -fstack-protector-strong -Wformat -Werror=format-security' 'F77=gfortran' 'FFLAGS=-g -O2 -fstack-protector-strong -g -O2 -fstack-protector-strong -O2' 'FC=gfortran' 'FCFLAGS=-g -O2 -fstack-protector-strong -g -O2 -fstack-protector-strong' '--cache-file=/dev/null' '--srcdir=.' 'CC=gcc' 'LIBS=-lrt -lpthread '
    Process Manager:                         pmi
    Launchers available:                     ssh rsh fork slurm ll lsf sge manual persist
    Topology libraries available:            hwloc
    Resource management kernels available:   user slurm ll lsf sge pbs cobalt
    Checkpointing libraries available:       blcr
    Demux engines available:                 poll select
________________________________________________________________________________
4/25/2017
Reading:
Article
A Raspberry Pi Cluster Instrumented for Fine-Grained Power Measurement †
Michael F. Cloutier, Chad Paradis and Vincent M. Weaver *
Keywords: Raspberry Pi; embedded supercomputers; GFLOPS/W; cluster construction; power measurement

Fine-Grained Power Measurement is essential for accurate results.
________________________________________________________________________________
4/26/2017
ODE Matlab work reference, also using the MPI to spread the work to clusters.

________________________________________________________________________________
5/3/2017
Working with one thread.
1 thread
Appr pi: 3.141592652923515
True pi: 3.141592653589793

2 threads:
Appr pi: 6.283185305847029
True pi: 3.141592653589793

4 threads:
Appr pi: 12.566370611694058
True pi: 3.141592653589793

problem find:
SR_f() start and end are using 0 to 1 which is recompute the range.
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
________________________________________________________________________________
