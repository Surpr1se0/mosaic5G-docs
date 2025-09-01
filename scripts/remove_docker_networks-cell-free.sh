#!/bin/bash

redes=(
  'rfsim5g-oai-core-net'
  'rfsim5g-oai-e1-net'
  'rfsim5g-oai-e2-net'
  'rfsim5g-oai-f1c-net'
  'rfsim5g-oai-f1u-1-net'
  'rfsim5g-oai-traffic-net'
  'rfsim5g-oai-ue-net'
)

for rede in "${redes[@]}"; do
  echo "removing: $rede"
  docker network rm "$rede" 2>/dev/null || echo "Network $rede cannot be removed."
done
