FROM ubuntu:jammy

ARG USER=ci
ARG UID=1000
ARG GID=1000

ENV DEBIAN_FRONTEND=noninteractive
RUN dpkg --add-architecture i386
RUN apt-get update \
 && apt-get install -y sudo locales vim screen libncurses-dev \
    build-essential git python3-minimal cmake \
    gcc-aarch64-linux-gnu \
    doxygen graphviz plantuml \
    clang llvm clang-tidy \
    qemu-user-static \
    libc6:i386 \
    rsync \
 && rm -rf /var/lib/apt/lists/*

# build and install musl for aarch64
RUN mkdir -p /opt/src/musl \
 && git clone git://git.musl-libc.org/musl /opt/src/musl \
 && cd /opt/src/musl \
 && git checkout 85e0e3519655220688e757b9d5bfd314923548bd \
 && CROSS_COMPILE=aarch64-linux-gnu- CFLAGS="-march=armv8-a+crc -mcpu=cortex-a53" \
    ./configure --prefix=/usr/local/aarch64-musl --target=aarch64-linux-gnu --disable-shared \
 && make -j$(nproc) \
 && make install \
 && rm -rf /opt/src/musl

# symlink kernel headers to musl include dir
RUN for i in linux asm asm-generic ; do \
      ln -s /usr/aarch64-linux-gnu/include/$i /usr/local/aarch64-musl/include/ ; \
    done

# build and install libmbedtls/crypto for aarch64+musl
COPY mbedtls/mbedtls-user-config.h /opt/src/
RUN mkdir -p /opt/src/mbedtls \
 && git clone https://github.com/ARMmbed/mbedtls.git /opt/src/mbedtls \
 && cd /opt/src/mbedtls \
 && git checkout 8df2f8e7b9c7bb9390ac74bb7bace27edca81a2b \
 && CC="/usr/local/aarch64-musl/bin/musl-gcc" \
    CFLAGS='-O2 -Werror -mcpu=cortex-a53 -march=armv8-a+crc -ffunction-sections -fdata-sections \
    -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
    -DMBEDTLS_USER_CONFIG_FILE=\"/opt/src/mbedtls-user-config.h\"' \
    make -j$(nproc) no_test \
 && make install DESTDIR=/usr/local/aarch64-musl \
 && rm -rf /opt/src/mbedtls-user-config.h /opt/src/mbedtls

# build and install cmocka as a static lib for aarch64+musl
COPY aarch64-cmocka-toolchain.cmake /opt/src/
RUN mkdir -p /opt/src/cmocka \
 && git clone https://git.cryptomilk.org/projects/cmocka.git /opt/src/cmocka \
 && cd /opt/src/cmocka \
 && git checkout 5a4b15870efa2225e6586fbb4c3af05ff0659434 \
 && mkdir -p build \
 && cd build \
 && cmake \
        -DCMAKE_TOOLCHAIN_FILE=/opt/src/aarch64-cmocka-toolchain.cmake \
        -DPICKY_DEVELOPER=ON \
        -DBUILD_SHARED_LIBS=OFF \
        -DWITH_EXAMPLES=OFF \
        -DCMAKE_INSTALL_PREFIX=/usr/local/aarch64-musl \
        .. \
 && make \
 && make install \
 && rm -rf /opt/src/cmocka

RUN locale-gen en_US.UTF-8 \
 && groupadd -g $GID -o ci \
 && useradd -m -u $UID -g $GID -o -s /bin/bash $USER \
 && echo "$USER ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER $USER
ENV LC_ALL=en_US.UTF-8
ENV DOCKERBUILD=1

CMD /bin/bash

