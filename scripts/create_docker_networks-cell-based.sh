#!/bin/bash

networks=(
  "rfsim5g-oai-core-net 192.168.71.0/24"
  "rfsim5g-oai-traffic-net 192.168.80.0/24"
  "rfsim5g-oai-f1c-net 192.168.72.0/24"
  "rfsim5g-oai-f1u-1-net 192.168.73.0/24"
  "rfsim5g-oai-f1u-2-net 192.168.74.0/24"
  "rfsim5g-oai-f1u-3-net 192.168.75.0/24"
  "rfsim5g-oai-f1u-4-net 192.168.76.0/24"
  "rfsim5g-oai-f1u-5-net 192.168.85.0/24"
  "rfsim5g-oai-e1-net 192.168.77.0/24"
  "rfsim5g-oai-ue-net1 192.168.78.0/24"
  "rfsim5g-oai-ue-net2 192.168.79.0/24"
  "rfsim5g-oai-ue-net3 192.168.81.0/24"
  "rfsim5g-oai-ue-net4 192.168.82.0/24"
  "rfsim5g-oai-ue-net5 192.168.83.0/24"
  "rfsim5g-oai-e2-net 192.168.70.0/24"
)

for net in "${networks[@]}"; do
  name=$(echo $net | awk '{print $1}')
  subnet=$(echo $net | awk '{print $2}')

  base_prefix=$(echo "$subnet" | cut -d. -f1-3)

  ip_range="${base_prefix}.240/28"

  echo "Creating $name with subnet $subnet and ip-range $ip_range"
  docker network create --scope=swarm --driver overlay --attachable --ipv4 --subnet="$subnet" --ip-range="$ip_range" "$name"
done

