#!/usr/bin/env bash

DIR="$(dirname "${BASH_SOURCE[0]}")"

mkdir -p "$DIR"/out/
mpicc main.c -cc=gcc -o "$DIR"/out/broadcast -std=c99