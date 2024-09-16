#!/bin/bash


echo "TEST LFU-PERFECT CACHE:\n" > result/lfu-perfect.txt
echo "TEST LRU-PERFECT CACHE:\n" > result/lru-perfect.txt


echo "TEST 1: with cache_size = 25 and num_elements = 1000000. Numbers = [0; 50]" >> result/lfu-perfect.txt
./build/bench_lfu "data/big_data.txt" >> result/lfu-perfect.txt

echo "TEST 2: with cache_size = 1000001 and num_elements = 1000000. Numbers = [0; 50]" >> result/lfu-perfect.txt
./build/bench_lfu "data/big_data2.txt" >> result/lfu-perfect.txt

echo "TEST 3: with cache_size = 1000001 and num_elements = 1000000. Numbers = [0; 1000000] (without replays)" >> result/lfu-perfect.txt
./build/bench_lfu "data/big_data3.txt" >> result/lfu-perfect.txt


echo "TEST 1: with cache_size = 25 and num_elements = 1000000" >> result/lru-perfect.txt
./build/bench_lru "data/big_data.txt" >> result/lru-perfect.txt

echo "TEST 2: with cache_size = 1000001 and num_elements = 1000000. Numbers = [0; 50]" >> result/lru-perfect.txt
./build/bench_lru "data/big_data2.txt" >> result/lru-perfect.txt

echo "TEST 3: with cache_size = 1000001 and num_elements = 1000000. Numbers = [0; 1000000] (without replays)" >> result/lru-perfect.txt
./build/bench_lru "data/big_data3.txt" >> result/lru-perfect.txt