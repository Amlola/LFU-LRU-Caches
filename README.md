# Caches

## Description

This project features three cache implementations: 

1. LRU cache (least recently used)
2. LFU cache (least frequently used)
3. Perfect cache (page is pre-rendered)

## Dependencies
1. g++
2. CMake 3.10 version (or higher)
3. GTest (for testing)

## Compiling 

### Simple mode

To compile all three caches and get three executable files, use:

``` cmd
$ mkdir build
$ cmake -S ./ -B build/Release -DDEBUG=OFF
$ cmake --build build/Release
```

### Debug mode

Сompilation in debug mode is also available:

``` cmd
$ mkdir build
$ cmake -S ./ -B build/Debug -DDEBUG=ON
$ cmake --build build/Debug
```

### Test mode

To compile Google tests you need the ```gtest``` library:

``` cmd
$ cd test
$ mkdir build
$ cd build
$ сmake ..
$ make
```

### Benchmark

Running benchmark is only available on Linux:

``` cmd
$ cd benchmark
$ mkdir build
$ cd build
$ сmake ..
$ make
```

## Run the program:

The binaries are in the build folder

### LRU cache: 
``` cmd
$ ./build/lru_cache arg
```
or for base mode:
```
$ ./build/lru_cache
```

```arg``` may contain one of two commands: ```len```, ```fib```. The first is used exclusively for debugging, with ```fib``` you can calculate the Fibbonacci number using caching

### Caches: 
* Debug
``` cmd
$ cd build/Debug/bin/
$ ./lfu_cache
$ ./lru_cache
$ ./perfect_cache
```

* Release
``` cmd
$ cd build/Release/bin/
$ ./lfu_cache
$ ./lru_cache
$ ./perfect_cache
```

### Test:
``` cmd
$ ./Test
```

### Benchmark:
``` cmd
$ cd ..
$ sh bench.sh
```
