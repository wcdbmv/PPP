#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(int argc, char* argv[]) {
  const int rc = MPI_Init(&argc, &argv);
  if (rc != MPI_SUCCESS) {
    return rc;
  }

  int comm_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  int comm_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  char proc_name[MPI_MAX_PROCESSOR_NAME];
  int proc_name_len = 0;
  MPI_Get_processor_name(proc_name, &proc_name_len);

  printf("HELLO from processor \"%s[%d]\", communicator %d of %d\n", proc_name,
         proc_name_len, comm_rank, comm_size);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
