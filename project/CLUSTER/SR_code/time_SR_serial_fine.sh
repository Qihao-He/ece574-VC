#!/bin/bash

#SBATCH -p general	# partition (queue)
#SBATCH -t 0-2:00	# time (D-HH:MM)
#SBATCH -o slurm.coarse.%N.%j.out # STDOUT
#SBATCH -e slurm.coarse.%N.%j.err # STDERR

time mpirun ./SR_serial_fine
