#!/bin/bash

# Script para criar redes overlay no Docker Swarm

networks=(
  "rfsim5g-oai-core-net 192.168.71.0/24"
  "rfsim5g-oai-traffic-net 192.168.80.0/24"
  "rfsim5g-oai-f1c-net 192.168.72.0/24"
  "rfsim5g-oai-f1u-1-net 192.168.73.0/24"
  "rfsim5g-oai-e1-net 192.168.77.0/24"
  "rfsim5g-oai-ue-net 192.168.78.0/24"
  "rfsim5g-oai-e2-net 192.168.70.0/24"
)

for net in "${networks[@]}"; do
  name=$(echo $net | awk '{print $1}')
  subnet=$(echo $net | awk '{print $2}')
  
  base_prefix=$(echo "$subnet" | cut -d. -f1-3)

  # Define um intervalo pequeno no final da subnet para uso interno do Docker
  ip_range="${base_prefix}.240/28"  # => .240 a .255 (16 IPs)
  
  echo "A criar rede $name com subnet $subnet e ip-range $ip_range"
  docker network create --scope=swarm --driver overlay --attachable --ipv4 --subnet="$subnet" --ip-range="$ip_range" "$name"
done

