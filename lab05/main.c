#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

int main(int argc, char** argv) {
  int rank, size;
  int num_array[ARRAY_SIZE];
  int local_max, global_max;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
      num_array[i] = (i + 1) * 100 + rank;
    }
  }

  MPI_Bcast(num_array, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

  local_max = 0;
  for (int i = rank; i < ARRAY_SIZE; i += size) {
    if (num_array[i] % 3 == 0 && num_array[i] > local_max) {
      local_max = num_array[i];
    }
  }

  MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    if (global_max == 0) {
      printf("В массиве нет числа, кратного 3.\n");
    } else {
      printf("Максимальное число, кратное 3: %d\n", global_max);
    }
  }

  MPI_Finalize();
  return 0;
}
