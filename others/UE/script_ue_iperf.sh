#!/bin/bash

# Lista dos nomes dos UEs
UE_LIST=(
    rfsim5g-oai-nr-ue1
    rfsim5g-oai-nr-ue2
    rfsim5g-oai-nr-ue3
    rfsim5g-oai-nr-ue4
    rfsim5g-oai-nr-ue5
    rfsim5g-oai-nr-ue6
    rfsim5g-oai-nr-ue7
    rfsim5g-oai-nr-ue8
    rfsim5g-oai-nr-ue9
    rfsim5g-oai-nr-ue10
)


# IP do servidor iperf3
SERVER_IP="12.1.1.1"

# Lançar o iperf3 em paralelo nos 10 UEs
for i in "${UE_LIST[@]}"; do
    echo "Iniciando iperf3 no UE $i..."
    docker exec -it $i iperf -c 12.1.1.1 -u &
done

# Esperar que todos terminem
wait

echo "Todos os testes iperf3 foram lançados."

