#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
  echo "Usage: mpi_convex_hull_all <build_dir>"
  exit 1
fi

readonly build_dir=$1
readonly sizes=(10 20 50 100 200 500 1000 2000 5000 10000 20000 50000 100000 200000 50000 1000000)

set -euxo pipefail

for size in "${sizes[@]}"; do
  ./scripts/mpi_convex_hull.sh "${build_dir}" "${size}"
done
