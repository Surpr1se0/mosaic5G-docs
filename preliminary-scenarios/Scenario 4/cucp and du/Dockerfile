# install necessary dependencies
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

# copy SMs 
COPY ./flexric /usr/local/lib/flexric

# copy libparams library
COPY ./bin/libparams_libconfig.so /usr/local/lib/

# copy other lbibraries
COPY ./bin/lib /usr/local/lib


# Update  LD_LIBRARY_PATH to include /usr/local/lib
ENV LD_LIBRARY_PATH="/usr/local/lib"

RUN chmod -R 755 /opt/oai-gnb/bin \
    && chmod -R 755 /usr/local/lib/flexric \
    && chmod -R 755 /usr/local/lib/

# Define entrypoint
USER root
ENTRYPOINT ["/opt/oai-gnb/bin/nr-softmodem"]

