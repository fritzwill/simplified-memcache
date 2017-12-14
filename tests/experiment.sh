#!/bin/bash

TIMEFORMAT=%R

echo "Performance Test Address Length Increase ... "

time ./bin/fibonacci 6 16 0 1 > /dev/null
time ./bin/fibonacci 8 16 0 1 > /dev/null
time ./bin/fibonacci 10 16 0 1 > /dev/null
time ./bin/fibonacci 12 16 0 1 > /dev/null
time ./bin/fibonacci 14 16 0 1 > /dev/null
time ./bin/fibonacci 16 16 0 1 > /dev/null
time ./bin/fibonacci 18 16 0 1 > /dev/null


echo "Performance Test Page Length Increase ... "

time ./bin/fibonacci 12 2 0 1 > /dev/null
time ./bin/fibonacci 12 4 0 1 > /dev/null
time ./bin/fibonacci 12 8 0 1 > /dev/null
time ./bin/fibonacci 12 16 0 1 > /dev/null
time ./bin/fibonacci 12 32 0 1 > /dev/null
time ./bin/fibonacci 12 64 0 1 > /dev/null
time ./bin/fibonacci 12 128 0 1 > /dev/null


echo "Performance Test Eviction Policy ... "

echo -n "FIFO:   "
time ./bin/fibonacci 6 16 0 1 > /dev/null
echo -n "FIFO:   "
time ./bin/fibonacci 6 8 0 1 > /dev/null
echo -n "Random:   "
time ./bin/fibonacci 6 16 1 1 > /dev/null
echo -n "Random:   "
time ./bin/fibonacci 6 8 1 1 > /dev/null
echo -n "LRU:   "
time ./bin/fibonacci 6 16 2 1 > /dev/null
echo -n "LRU:   "
time ./bin/fibonacci 6 8 2 1 > /dev/null
echo -n "Clock:   "
time ./bin/fibonacci 6 16 3 1 > /dev/null
echo -n "Clock:   "
time ./bin/fibonacci 6 8 3 1 > /dev/null


echo "Performance Test Thread Increase ... "
time ./bin/fibonacci 6 16 0 1 > /dev/null
time ./bin/fibonacci 6 16 0 2 > /dev/null
time ./bin/fibonacci 6 16 0 3 > /dev/null
time ./bin/fibonacci 6 16 0 4 > /dev/null
time ./bin/fibonacci 6 16 0 5 > /dev/null
time ./bin/fibonacci 6 16 0 6 > /dev/null
time ./bin/fibonacci 6 16 0 7 > /dev/null
