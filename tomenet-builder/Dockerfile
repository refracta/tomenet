FROM ubuntu:24.10

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install nano git build-essential gcc-mingw-w64-i686 wine32 wget unzip -y

WORKDIR /usr/src

RUN echo "INSTALL SDL2" && \
    wget https://github.com/libsdl-org/SDL/releases/download/release-2.30.7/SDL2-devel-2.30.7-mingw.tar.gz && \
    tar --strip-components=1 -xzvf SDL2-devel-2.30.7-mingw.tar.gz -C /usr SDL2-2.30.7/i686-w64-mingw32 && \
    rm -rf SDL2-devel-2.30.7-mingw.tar.gz && \
    \
    echo "INSTALL SDL2_MIXER" && \
    wget https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.0/SDL2_mixer-devel-2.8.0-mingw.tar.gz && \
    tar --strip-components=1 -xzvf SDL2_mixer-devel-2.8.0-mingw.tar.gz -C /usr SDL2_mixer-2.8.0/i686-w64-mingw32 && \
    rm -rf SDL2_mixer-devel-2.8.0-mingw.tar.gz && \
    \
    echo "INSTALL LIBGNURX" && \
    git clone https://github.com/coapp-packages/libgnurx && \
    cd libgnurx && \
    export CC=i686-w64-mingw32-gcc && \
    export CXX=i686-w64-mingw32-g++ && \
    chmod -R 777 configure && \
    ./configure --host=i686-w64-mingw32 --prefix=/usr/i686-w64-mingw32 --enable-shared && \
    make && \
    make install && \
    cd .. && \
    rm -rf libgnurx
