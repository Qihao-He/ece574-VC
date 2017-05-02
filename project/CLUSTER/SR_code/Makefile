CC = mpicc
CFLAGS = -O2 -Wall -g
LFLAGS = -ljpeg -lm

# all:   sobel_before sobel_coarse SR_serial
all:   SR_serial SR_serial_fine
	# SR_mpi

# sobel_before:	sobel_before.o
# 		$(CC) -o sobel_before sobel_before.o $(LFLAGS)
#
# sobel_before.o:	sobel_before.c
# 		$(CC) $(CFLAGS) -c sobel_before.c


# sobel_coarse:	sobel_coarse.o
# 		$(CC) -o sobel_coarse sobel_coarse.o $(LFLAGS)
#
# sobel_coarse.o:	sobel_coarse.c
# 		$(CC) $(CFLAGS) -c sobel_coarse.c


SR_serial:	SR_serial.o
		$(CC) -o SR_serial SR_serial.o $(LFLAGS)

SR_serial.o:	SR_serial.c
		$(CC) $(CFLAGS) -c SR_serial.c


SR_mpi:	SR_mpi.o
		$(CC) -o SR_mpi SR_mpi.o $(LFLAGS)

SR_mpi.o:	SR_mpi.c
		$(CC) $(CFLAGS) -c SR_mpi.c


SR_serial_fine:	SR_serial_fine.o
		$(CC) -o SR_serial_fine SR_serial_fine.o $(LFLAGS)

SR_serial_fine.o:	SR_serial_fine.c
		$(CC) $(CFLAGS) -c SR_serial_fine.c

clean:   #  rm -f *~ *.o sobel_coarse SR_serial sobel_before out.jpg
				 rm -f *~ *.o SR_serial SR_mpi SR_serial_fine


submit:
	tar -czvf SR_submit.tar.gz SR_serial.c SR_mpi.c Makefile README *.c
