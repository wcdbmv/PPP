#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  int rc = MPI_Init(&argc, &argv);
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

  int* buffer = malloc(sizeof(int) * BUFFER_SIZE * 1024 + 100);
  if (!buffer) {
    rc = EXIT_FAILURE;
    goto exit;
  }

  MPI_Status status;
  if (comm_rank % 2 == 0) {
    if (comm_rank < comm_size - 1) {
      for (int i = 0; i < BUFFER_SIZE * 1024; ++i) {
        buffer[i] = i + 10;
      }

      int size = BUFFER_SIZE;
      for (int i = 0; i <= 10; ++i) {
        double time = MPI_Wtime();
        for (int j = 0; j < 100; ++j) {
          MPI_Send(buffer, size, MPI_INT, comm_rank + 1, 10, MPI_COMM_WORLD);
          MPI_Recv(buffer, size + 100, MPI_INT, comm_rank + 1, 20,
                   MPI_COMM_WORLD, &status);
        }
        time = MPI_Wtime() - time;
        printf("[%d] Time = %lf, Data = %9.0f KByte\n", comm_rank, time,
               size * sizeof(int) * 200.0 / 1024);
        printf("[%d] Bandwith[%d] = %lf MByte/sec\n", comm_rank, i,
               size * sizeof(int) * 200 / (time * 1024 * 1024));
        size *= 2;
      }
    } else {
      printf("[%d] Idle\n", comm_rank);
    }
  } else {
    int size = BUFFER_SIZE;
    for (int i = 0; i <= 10; ++i) {
      for (int j = 0; j < 100; ++j) {
        MPI_Recv(buffer, size + 100, MPI_INT, comm_rank - 1, 10, MPI_COMM_WORLD,
                 &status);
        MPI_Send(buffer, size, MPI_INT, comm_rank - 1, 20, MPI_COMM_WORLD);
      }
      size *= 2;
    }
  }

free_buffer:
  free(buffer);
  buffer = NULL;

exit:
  MPI_Finalize();

  return rc;
}
