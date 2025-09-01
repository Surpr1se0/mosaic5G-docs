FROM ubuntu:22.04

# install necessary packages
RUN apt-get update && apt-get install -y \
    libpthread-stubs0-dev \
    libconfig-dev \
    libmnl-dev \
    libsctp-dev \
    iproute2 \
    net-tools \
    iputils-ping \
    && apt-get clean

# copy compiled gNB binaries and configuration files
COPY ./bin/nr-softmodem /opt/oai-gnb/bin/
COPY ./etc /opt/oai-gnb/etc/

# Copy sms
COPY ./flexric /usr/local/lib/flexric

# copy library libparams_libconfig.so
COPY ./bin/libparams_libconfig.so /usr/local/lib/
COPY ./bin/lib /usr/local/lib


# Update LD_LIBRARY_PATH to include /usr/local/lib
ENV LD_LIBRARY_PATH="/usr/local/lib"

# define entrypoint
ENTRYPOINT ["/opt/oai-gnb/bin/nr-softmodem"]

