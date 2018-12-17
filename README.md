# Thread-Safe Cache for in-memory Key-Value Store.
## Overview
This is meant to be a simplified version of [memcached](https://www.memcached.org/). This library is also **thread-safe** meaning that the this implementation supports **concurrent** access to the cache

## Directory Structure
**src/**: Contains examples of using the library. Each of these files utilizes the cache by importing the library and using its functionality within the program

**include/**: Contians the implemntation of the classes needed for the library

**bin/**: This is the target directory for the Makefile to place the binaries compiled from src

**tests/**: Contains unit tests for the classes along with a script to collect useful time statistics when using the cache while calculating fibonacci numbers (see 'experiments.sh')

## Using the Cache in Code
```C
// Declaring a HTCache object in code
HTCache<int64_t, bool> cache(addrelen, pagesize, policy, handler)

// Storing key-value
cache.put(key, value);

// Retrieve value from key
cache.get(key);
```
addrlen = address space in terms of number of bits
pagesize = size of each page in terms of number of entries
eviction = eviction policy (FIFO, Random, LRU, Clock) 
handler = function to call when requested item is missing from cache

maps int64_t to bool value
