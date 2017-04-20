NVCC = nvcc
CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =

all:	saxpy saxpy_c 


saxpy:	saxpy.cu
	$(NVCC) -o saxpy  saxpy.cu


saxpy_c:	saxpy_c.o
	$(CC) $(LFLAGS) -o saxpy_c saxpy_c.o

saxpy_c.o:	saxpy_c.c
	$(CC) $(CFLAGS) -c saxpy_c.c



clean:	
	rm -f *~ *.o saxpy saxpy_c

