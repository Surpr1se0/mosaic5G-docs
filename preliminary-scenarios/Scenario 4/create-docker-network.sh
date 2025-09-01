#!/bin/bash

#Scriptt to create docker networks for Scenario 4
networks=(
  "rfsim5g-oai-core-net 192.168.71.0/24"
  "rfsim5g-oai-traffic-net 192.168.80.0/24"
  "rfsim5g-oai-f1c-net 192.168.72.0/24"
  "rfsim5g-oai-f1u-1-net 192.168.73.0/24"
  "rfsim5g-oai-f1u-2-net 192.168.74.0/24"
  "rfsim5g-oai-f1u-3-net 192.168.76.0/24"
  "rfsim5g-oai-e1-net 192.168.77.0/24"
  "rfsim5g-oai-ue-net 192.168.78.0/24"
  "rfsim5g-oai-e2-net 192.168.70.0/24"
)

for net in "${networks[@]}"; do
  name=$(echo $net | awk '{print $1}')
  subnet=$(echo $net | awk '{print $2}')
  echo "A criar rede $name com subnet $subnet"
  docker network create --scope=swarm --driver overlay --attachable --ipv4 --subnet="$subnet" "$name"
done
