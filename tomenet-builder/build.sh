#!/bin/bash

docker run -v $(pwd):/usr/src -i --rm refracta/tomenet-builder /bin/bash -c "
    cd /usr/src && \
    git clone https://github.com/refracta/tomenet-ko && \
    cd tomenet-ko/src && \
    make -f makefile.mingw
"
