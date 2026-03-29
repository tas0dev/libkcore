#!/usr/bin/env sh

CFLAGS="-std=c99 -O0 -g -fsanitize=undefined,address"
CC=gcc

build_example() {
    mkdir -p output

    for file in *.c; do
        name=$(basename "$file" .c)
        out="output/$name"

        $CC $CFLAGS -I ../src \
						"$file" \
            ../src/*.c \
            -o "$out"
    done
}

build_example
