#!/bin/bash

# destination directory
output_dir="/mnt/results/se/cb/1"

# Time between executions (seconds)
interval=15

# reps
max_reps=30

# make directory if it does not exist
mkdir -p "$output_dir"

echo "start of the cycle..."
for i in $(seq -f "%03g" 1 $max_reps); do
  filename="rep_${i}.log"
  echo "[$(date)] Execution $i of $max_reps..."

  # Select the xApp
  ./xapp_se_ul > "${output_dir}/${filename}"

  if [[ "$i" -lt "$max_reps" ]]; then
    echo "waiting  $interval seconds..."
    sleep $interval
  fi
done

echo "Finalized $max_reps executions."