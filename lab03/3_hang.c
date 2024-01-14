#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  int myrank, nprocs, name_len;
  char name[MPI_MAX_PROCESSOR_NAME];
  int recv_val;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Get_processor_name(name, &name_len);

  printf("%s[%d]: Hello\n", name, myrank);

  int prev = myrank ? (myrank - 1) : (nprocs - 1);

  MPI_Recv(&recv_val, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  printf("%s[%d]: Should never happen\n", name, myrank);

  MPI_Finalize();
  return 0;
}
