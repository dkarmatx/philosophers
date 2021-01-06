#!/bin/sh

clang -Wall -Werror -Wextra -pthread -o philo_one -O2 \
    src/*.c