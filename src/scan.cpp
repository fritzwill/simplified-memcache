// scan.cpp

#include <htcache/htcache.h>

#include <cassert>
#include <cstdio>
#include <thread>
#include <iostream>

#define N (1<<10)

int64_t Data[N] = {0};

void scan_thread(size_t tid, HTCache<int64_t, int64_t> *cache) {
    int64_t total = 0;
    for(size_t j = 0; j < 10; j++) {
	for(size_t i = 0; i < N; i++) {
	    total += cache->get(i);
	}
    }

    assert(total == 1305600);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
    	fprintf(stderr, "Usage: %s AddressLength PageSize EvictionPolicy Threads\n", argv[0]);
    	return EXIT_FAILURE;
    }

    size_t addrlen        = strtol(argv[1], NULL, 10);
    size_t pagesize       = strtol(argv[2], NULL, 10);
    EvictionPolicy policy = static_cast<EvictionPolicy>(strtol(argv[3], NULL, 10));
    size_t nthreads	  = strtol(argv[4], NULL, 10);

    HTCache<int64_t, int64_t> cache(addrlen, pagesize, policy, [](const int64_t &key) { return Data[key]; });

    for (size_t i = 0; i < N; i++) {
    	Data[i] = i % 256;
    }
    std::thread t[nthreads];
    for (size_t i = 0; i < nthreads; i++) {
    	t[i] = std::thread(scan_thread, i, &cache);
    }
    for (size_t i = 0; i < nthreads; i++) {
    	t[i].join();
    }
    
    cache.stats(stdout);
    return EXIT_SUCCESS;
}
