#!/usr/bin/env bash

if [ "$#" -ne 2 ]; then
  echo "Usage: mpi_convex_hull <build_dir> <size>"
  exit 1
fi

readonly build_dir=$1
readonly size=$2
readonly executable="${build_dir}/mpi_convex_hull"

set -euxo pipefail

mkdir -p data
mpirun -np 4 "${executable}" "data/cloud_${size}.dat" "data/hull_${size}.dat" "data/benchmark_${size}.csv"
