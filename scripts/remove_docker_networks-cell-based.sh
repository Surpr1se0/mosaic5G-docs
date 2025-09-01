#!/bin/bash

redes=(
  'rfsim5g-oai-core-net'
  'rfsim5g-oai-e1-net'
  'rfsim5g-oai-e2-net'
  'rfsim5g-oai-f1c-net'
  'rfsim5g-oai-f1u-1-net'
  'rfsim5g-oai-f1u-2-net'
  'rfsim5g-oai-f1u-3-net'
  'rfsim5g-oai-f1u-4-net'
  'rfsim5g-oai-f1u-5-net'
  'rfsim5g-oai-traffic-net'
  'rfsim5g-oai-ue-net1'
  'rfsim5g-oai-ue-net2'
  'rfsim5g-oai-ue-net3'
  'rfsim5g-oai-ue-net4'
  'rfsim5g-oai-ue-net5'
)

for rede in "${redes[@]}"; do
  echo "Removing: $rede"
  docker network rm "$rede" 2>/dev/null || echo "network $rede cannot be removed."
done
