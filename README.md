# LFU-Cache

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
$ cd build
$ сmake ..
$ make
```

if you need to compile a specific cache (for example lru_cache):

``` cmd
$ mkdir build
$ cd build
$ сmake ..
$ make lru_cache
```

### Debug mode

Сompilation in debug mode is also available:

``` cmd
$ mkdir build
$ cd build
$ сmake -DDEBUG=ON ..
$ make
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

```arg``` must contain one of three commands: ```num```, ```len```, ```fib```. The first two are used exclusively for debugging, with ```fib``` you can calculate the Fibbonacci number using caching

### LFU cache: 
``` cmd
$ ./build/lfu_cache
```

### Perfect cache:
``` cmd
$ ./build/perfect_cache
```

### Test:
``` cmd
$ ./Test
```

### Benchmark:
``` cmd
$ cd ..
$ ./sh bench.sh
```
