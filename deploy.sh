#!/bin/sh
PICO_PATH=""

if [[ -f /media ]]; then
    PICO_PATH="/media/**/RP2350"
else
    PICO_PATH="/run/media/**/RP2350"
fi

cp build/wolvsec_badge.uf2 $PICO_PATH

