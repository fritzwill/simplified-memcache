Thread-safe cache for in-memoyr key-value store.

'src' contains examples of using the library

'include' contians the implemntation of the classes needed for the library

'bin' is the target directory for the Makefile to place the binaries compiled from src

'tests' contains unit tests for the classes along with a script to collect useful time statistics on the implementation regarding which eviction policy was chosen

Example of object in code:

HTCache<int64_t, bool> cache(addrelen, pagesize, policy, handler)

addrlen = address space in terms of number of bits
pagesize = size of each page in terms of number of entries
eviction = eviction policy (FIFO, Random, LRU, Clock) 
handler = function to call when requested item is missing from cache

maps int64_t to bool value
