ARG REPO
ARG UBUNTU_RELEASE
FROM ${REPO}${REPO:+/}ubuntu:${UBUNTU_RELEASE}

ENV DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture i386
RUN apt-get update \
 && apt-get install -y \
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
 libtss2-dev \
 llvm \
 locales \
 npm \
 pkg-config \
 plantuml \
 python3-minimal \
 rsync \
 screen \
 shfmt \
 software-properties-common \
 sudo \
 vim \
 wget \
 && rm -rf /var/lib/apt/lists/* \
 && ln -fs /usr/share/zoneinfo/UTC /etc/localtime

RUN add-apt-repository -y ppa:elos-team/ppa \
    && apt-get update \
    && apt-get install -y cmocka-extensions-dev \
    && rm -rf /var/lib/apt/lists/*

 # Install doctoc
RUN npm install doctoc -g

 # Install commitlint too with conventional commit config and dependencies.
RUN npm install n -g \
&& n stable
RUN npm install @commitlint/cli -g \
&& npm install @commitlint/config-conventional -g

ARG USER=ci
ARG UID=1000
ARG GID=1000

RUN locale-gen en_US.UTF-8 \
 && groupadd -g $GID ci \
 && useradd -m -u $UID -g $GID -s /bin/bash $USER \
 && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER $USER
ENV LC_ALL=en_US.UTF-8
ENV DOCKERBUILD=1

CMD /bin/bash
