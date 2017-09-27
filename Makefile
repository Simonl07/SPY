ring: ring.c
	mpicc -g -Wall ring.c -o ring

clean:
	rm -f ring
