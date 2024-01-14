#!/usr/bin/env bash

if [ "$#" -ne 2 ]; then
  echo "Usage: generate_point_cloud <build_dir> <size>"
  exit 1
fi

readonly build_dir=$1
readonly size=$2
readonly generator="${build_dir}/convex_hull/gen/generate_point_cloud"

set -euxo pipefail

mkdir -p data
${generator} "${size}" "data/cloud_${size}.dat" "data/reference_hull_${size}.dat"
