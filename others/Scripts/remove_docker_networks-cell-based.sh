#!/bin/bash

# Lista de redes a remover
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

# Remoção das redes Docker
for rede in "${redes[@]}"; do
  echo "A remover rede: $rede"
  docker network rm "$rede" 2>/dev/null || echo "Rede $rede não pôde ser removida ou não existe."
done
