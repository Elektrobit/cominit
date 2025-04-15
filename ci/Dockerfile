ARG REPO
ARG UBUNTU_RELEASE
FROM ${REPO}${REPO:+/}ubuntu:${UBUNTU_RELEASE}

ARG USER=ci
ARG UID=1000
ARG GID=1000

ENV DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture i386
RUN apt-get update \
 && apt-get install -y \
 autoconf \
 autoconf-archive \
 automake \
 build-essential \
 clang \
 clang-format \
 clang-tidy \
 cmake \
 debhelper \
 devscripts \
 doxygen \
 equivs \
 fakeroot \
 git \
 git-buildpackage \
 graphviz \
 libc6:i386 \
 libcmocka-dev \
 libmbedtls-dev \
 libncurses-dev \
 libssl-dev \
 libtool \
 libtss2-dev \
 llvm \
 locales \
 npm \
 pkg-config \
 plantuml \
 python3-minimal \
 qemu-user-static \
 rsync \
 screen \
 shfmt \
 software-properties-common \
 sudo \
 uthash-dev \
 vim \
 wget \
 && rm -rf /var/lib/apt/lists/* \
 && ln -fs /usr/share/zoneinfo/UTC /etc/localtime

 # Install doctoc
RUN npm install doctoc -g

RUN locale-gen en_US.UTF-8 \
 && groupadd -g $GID -o ci \
 && useradd -m -u $UID -g $GID -o -s /bin/bash $USER \
 && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

 # Install commitlint too with conventional commit config and dependencies.
RUN npm install n -g \
&& n stable
RUN npm install @commitlint/cli -g \
&& npm install @commitlint/config-conventional -g

USER $USER
ENV LC_ALL=en_US.UTF-8
ENV DOCKERBUILD=1

CMD /bin/bash

