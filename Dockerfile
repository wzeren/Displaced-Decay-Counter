# DCC (Displaced Decay Counter) - multi-stage build
#
# Build runtime image:
#   docker build -t dcc --target runtime .
#
# Build dev image (for devcontainer / Cursor attach):
#   docker build -t dcc:dev --target dev .
#   docker run -d --name dcc-dev -v $(pwd):/dcc -w /dcc dcc:dev
#
# Run: docker run --rm dcc [args]. With no args, main prints usage.

ARG DEBIAN_FRONTEND=noninteractive
ARG HOME=/home/jsk
ARG HEPMC_VER=2.06.11
ARG HEPMC_TAG=HEPMC_02_06_11
ARG PYTHIA_VER=8317
ARG PYTHIA_DIR=pythia83

# -----------------------------------------------------------------------------
# Stage 1: build dependencies and DCC binary
# -----------------------------------------------------------------------------
FROM ubuntu:22.04 AS builder

ARG HOME
ARG HEPMC_VER
ARG HEPMC_TAG
ARG PYTHIA_VER
ARG PYTHIA_DIR

RUN apt-get update && apt-get install -y --no-install-recommends \
    autoconf \
    automake \
    ca-certificates \
    coreutils \
    g++ \
    gzip \
    libtool \
    make \
    rsync \
    wget \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/* \
    && useradd -m -d $HOME jsk

ENV HOME=$HOME

# HepMC2 (newest 2.06.11 from GitHub mirror)
RUN wget -q "https://github.com/hep-mirrors/hepmc/archive/refs/tags/${HEPMC_TAG}.tar.gz" -O /tmp/hepmc.tar.gz \
    && tar -xzf /tmp/hepmc.tar.gz -C /tmp \
    && cd /tmp/hepmc-${HEPMC_TAG} && autoreconf -i \
    && ./configure --prefix=$HOME/hepmc --with-momentum=GEV --with-length=MM \
    && make -j$(nproc) && make install && rm -rf /tmp/hepmc-${HEPMC_TAG} /tmp/hepmc.tar.gz

# Pythia 8.317 (with HepMC2 and zlib)
RUN wget -q "https://pythia.org/download/${PYTHIA_DIR}/pythia${PYTHIA_VER}.tgz" -O /tmp/pythia.tgz \
    && mkdir -p /tmp/pythia && tar -xzf /tmp/pythia.tgz -C /tmp/pythia --strip-components=1 \
    && cd /tmp/pythia && ./configure --prefix=$HOME/pythia${PYTHIA_VER} \
        --with-hepmc=$HOME/hepmc --with-hepmcversion=${HEPMC_VER} --with-gzip \
    && make -j$(nproc) && make install && rm -rf /tmp/pythia /tmp/pythia.tgz

WORKDIR /dcc
COPY . .

# Make example inputs work as-is: DDC_Directory → /dcc
RUN find /dcc \( -name "*.dat" -o -name "*.cmnd" \) -exec sed -i "s|'DDC_Directory'|/dcc|g" {} \; 2>/dev/null || true

# Clean host-built artifacts so we build only inside the container
RUN make clean || true
RUN make -j$(nproc) main

# -----------------------------------------------------------------------------
# Stage 2: minimal runtime image
# -----------------------------------------------------------------------------
FROM debian:trixie-slim AS runtime

ARG HOME=/home/jsk
ARG PYTHIA_VER=8317

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    zlib1g \
    && rm -rf /var/lib/apt/lists/* \
    && useradd -m -d $HOME jsk

ENV HOME=$HOME

COPY --from=builder $HOME/hepmc $HOME/hepmc
COPY --from=builder $HOME/pythia${PYTHIA_VER} $HOME/pythia${PYTHIA_VER}
COPY --from=builder /dcc /dcc
RUN mkdir -p /dcc/Logs

WORKDIR /dcc
ENTRYPOINT ["./bin/main"]
CMD []

# -----------------------------------------------------------------------------
# Stage 3: development image (build tools, for devcontainer)
# -----------------------------------------------------------------------------
FROM builder AS dev
RUN apt-get update && apt-get install -y --no-install-recommends \
    clang \
    clang-tidy \
    cppcheck \
    git \
    && rm -rf /var/lib/apt/lists/*
ENTRYPOINT []
CMD ["sleep", "infinity"]
