/* File:     ring.c
 * Purpose:  This program prints a hello message at each process. Modify it so
 *           that it communicates in a ring fashion.
 *
 * Compile:  mpicc -g -Wall -o ring ring.c
 * Run:      mpirun -n <num_procs> ./ring
 * Run:      mpirun -n <num_procs> --hostfile host_file.txt ./ring
 */

#include <mpi.h>
#include <stdio.h>

int main(int argc, char ** argv) {
    MPI_Init(NULL, NULL);

    /* Total number of processes in this MPI communicator */
    int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get the rank of this process */
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Hostname */
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int name_sz;
    MPI_Get_processor_name(hostname, &name_sz);


    /* We need to modify this file so that we send point-to-point messages
     * between processes. We'll use MPI_Send and MPI_Recv... */

    /* MPI_Send example:
     *
     * char buffer[1000];
     * sprintf(buffer, "Hello world from %d\n", rank);
     * MPI_Send(buffer, 1000, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
     */

     if(rank == 0){
         char buffer[1000];
         sprintf(buffer, "Hello world from %d\n", rank);
         MPI_Send(buffer, 1000, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
     }
     else if (rank == comm_sz-1) {
       /* Leader */
       char buffer[1000];
       MPI_Recv(buffer, 1000, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       sprintf(buffer, "%sHello world from %d\n", buffer, rank);
       printf("%s\n", buffer);
   } else {
       /* Followers */
       char buffer[1000];
       MPI_Recv(buffer, 1000, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

       sprintf(buffer, "%sHello world from %d\n", buffer, rank);
       MPI_Send(buffer, 1000, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
   }

    /* Clean up */



    MPI_Finalize();
    return 0;
}
