FROM ubuntu:24.04

# install dependencies
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

# copy compiled CU/UP binary and configuration files
COPY ./bin/nr-cuup /opt/oai-gnb/bin/
COPY ./etc/ /opt/oai-gnb/etc/

# copy sms
COPY ./flexric /usr/local/lib/flexric

# copy library libparams_libconfig.so
COPY ./bin/libparams_libconfig.so /usr/local/lib/
COPY ./bin/lib/ /usr/local/lib

# update library path
ENV LD_LIBRARY_PATH="/usr/local/lib"

# Define entry point
ENTRYPOINT ["/opt/oai-gnb/bin/nr-cuup"]
