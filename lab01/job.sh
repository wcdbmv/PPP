#!/usr/bin/env bash

#SBATCH --job-name="Hello Kerimov"
#SBATCH --partition=debug
#SBATCH  --nodes=5
#SBATCH --time=0-00:05:00
#SBATCH --ntasks-per-node=1
#SBATCH --mem=1992

mpirun -np 5 cmake-build-debug/lab01/lab01
