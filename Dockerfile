FROM ubuntu:20.04

# Skip timezone setting
RUN DEBIAN_FRONTEND=noninteractive apt-get update -y && apt-get upgrade -y && apt-get install -y tzdata

RUN apt-get update -y && apt-get upgrade -y && apt-get install -y \
    gcc \
    make \
    cmake \
    git \
    libpng-dev

RUN mkdir /workspace
VOLUME /workspace
WORKDIR /workspace
