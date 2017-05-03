CC = mpicc
CFLAGS = -O2 -Wall -g
LFLAGS = -ljpeg -lm

all:   SR_serial SR_serial_fine	SR_mpi

SR_serial:	SR_serial.o
		$(CC) -o SR_serial SR_serial.o $(LFLAGS)

SR_serial.o:	SR_serial.c
		$(CC) $(CFLAGS) -c SR_serial.c

SR_serial_fine:	SR_serial_fine.o
		$(CC) -o SR_serial_fine SR_serial_fine.o $(LFLAGS)

SR_serial_fine.o:	SR_serial_fine.c
		$(CC) $(CFLAGS) -c SR_serial_fine.c

SR_mpi:	SR_mpi.o
		$(CC) -o SR_mpi SR_mpi.o $(LFLAGS)

SR_mpi.o:	SR_mpi.c
		$(CC) $(CFLAGS) -c SR_mpi.c

clean:
	rm -f *~ *.o SR_serial SR_mpi SR_serial_fine

submit:
	tar -czvf SR_project.tar.gz Makefile README *.c
