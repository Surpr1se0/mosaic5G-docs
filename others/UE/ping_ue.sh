#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Uso: $0 <lista de UEs separados por vírgula> (ex: 1,2,3,4)"
  exit 1
fi

UE_LIST=$(echo "$1" | tr ',' ' ')

for UE in $UE_LIST; do
  echo "A pingar o UE $UE (rfsim5g-oai-nr-ue$UE)..."
  docker exec -it rfsim5g-oai-nr-ue$UE ping -I oaitun_ue1 -c 1 12.1.1.1
  echo "----------------------------------------------"
done

