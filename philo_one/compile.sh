#!/bin/sh

clang -Wall -Werror -Wextra -pthread -o philo_one -g -O0 \
    src/*.c