#!/bin/bash

HARDWARE=`uname -m`
if [ "$HARDWARE" = "armv6l" ] ; then
    echo Building for Pi 1
    rm Makefile
    cp Makefile_Pi1 Makefile
    make
elif [ "$HARDWARE" = "armv7l" ] ; then
    REVISION=`cat /proc/cpuinfo | grep Revision |  cut -d : -f2 | tr -d '[:space:]'`
    if [[ $REVISION =~ 1*a[02]1041 ]]; then
        echo Building for Pi 2
        rm Makefile
        cp Makefile_Pi2 Makefile
        make -j4
    else
        echo Building for Pi 3
        rm Makefile
        cp Makefile_Pi3 Makefile
        make -j4
    fi
fi
