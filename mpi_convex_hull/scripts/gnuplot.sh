#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
  echo "Usage: gnuplot <size>"
  exit 1
fi

readonly size=$1

set -euxo pipefail

gnuplot -e "cloud='data/cloud_${size}.dat'; hull='data/hull_${size}.dat'" scripts/gnuplot.plg
