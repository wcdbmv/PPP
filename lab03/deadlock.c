#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

int main(int argc, char** argv) {
  int myrank, nprocs, len, dest, i;
  char name[MPI_MAX_PROCESSOR_NAME];
  int *buf0, *buf1;
  MPI_Status st;
  MPI_Request rst, rst1;
  // char *abuf;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Get_processor_name(name, &len);

  printf("Hello from processor %s[%d] %d of %d  \n", name, len, myrank, nprocs);

  buf0 = (int*)malloc(sizeof(int) * (SIZE * 1024 + 100));
  buf1 = (int*)malloc(sizeof(int) * (SIZE * 1024 + 100));

  for (i = 0; i < SIZE * 1024; i++)
    buf0[i] = (i + 1) * 100 + myrank;
  dest = (nprocs == myrank + 1) ? 0 : myrank + 1;
  // abuf = (char*) malloc( 1024*1024 );

  // MPI_Buffer_attach( abuf, 1024*1024 );

  printf("My[%2d] Buf = %8d %8d %8d ...\n", myrank, buf0[0], buf0[1], buf0[2]);

  MPI_Irecv(buf1, 300, MPI_INT, MPI_ANY_SOURCE, 10, MPI_COMM_WORLD, &rst);

  printf("My[%2d] Buf[??] = %8d %8d %8d ...\n", myrank, buf1[0], buf1[1],
         buf1[2]);

  MPI_Issend(buf0, 300, MPI_INT, dest, 10, MPI_COMM_WORLD, &rst1);

  MPI_Wait(&rst, &st);

  printf("My[%2d] Buf[%2d] = %8d %8d %8d ...\n", myrank, st.MPI_SOURCE, buf1[0],
         buf1[1], buf1[2]);

  MPI_Wait(&rst1, &st);

  printf("My[%2d] Success\n", myrank);

  MPI_Finalize();
  return 0;
}
