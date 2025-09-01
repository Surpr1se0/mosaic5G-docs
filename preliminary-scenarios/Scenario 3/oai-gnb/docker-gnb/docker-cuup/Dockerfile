FROM ubuntu:24.04

# Instalar dependências necessárias
RUN apt-get update && apt-get install -y \
    libpthread-stubs0-dev \
    libconfig-dev \
    libmnl-dev \
    libsctp-dev \
    iproute2 \
    net-tools \
    iputils-ping \
    vim \
    && apt-get clean

# Copiar binários compilados
COPY ./bin/nr-cuup /opt/oai-gnb/bin/
COPY ./etc/ /opt/oai-gnb/etc/

# Copiar Service Models (SMs)
COPY ./flexric /usr/local/lib/flexric

# Copiar a biblioteca libparams_libconfig.so
COPY ./bin/libparams_libconfig.so /usr/local/lib/

# Copiar outra biblioteca que gerou erro  xdddddd
COPY ./bin/lib/ /usr/local/lib

# Atualizar o LD_LIBRARY_PATH para incluir /usr/local/lib
ENV LD_LIBRARY_PATH="/usr/local/lib"

# Definir o ponto de entrada
ENTRYPOINT ["/opt/oai-gnb/bin/nr-cuup"]
