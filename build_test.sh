#!/bin/bash
set -e

gcc -std=c99 -o ledscape_test -I . ledscape_test.c -L . -L am335x/app_loader/lib/ -lprussdrv -lledscape
