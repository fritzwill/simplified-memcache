// prime.cpp

#include <htcache/htcache.h>

#include <cassert>
#include <cstdio>
#include <thread>

#define N (1<<10)

void prime_thread(size_t tid, HTCache<int64_t, bool> *cache) {
    size_t total;

    total = 0;
    for (int64_t i = 0; i < N; i++) {
	total += cache->get(i);
    }
    assert(total == 172);
    
    total = 0;
    for (int64_t i = 0; i < N; i++) {
	total += cache->get(i);
    }
    assert(total == 172);
    
    total = 0;
    for (int64_t i = 0; i < 2*N; i++) {
	total += cache->get(i);
    }
    assert(total == 309);
}

bool prime_handler(const int64_t &key) {
    if (key <= 1) {
	return false;
    }

    for (int64_t i = 2; i <= key / 2; i++) {
	if ((key % i) == 0) {
	    return false;
	}
    }
    return true;
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

    HTCache<int64_t, bool> cache(addrlen, pagesize, policy, prime_handler);

    std::thread t[nthreads];
    for (size_t i = 0; i < nthreads; i++) {
    	t[i] = std::thread(prime_thread, i, &cache);
    }

    for (size_t i = 0; i < nthreads; i++) {
    	t[i].join();
    }
    
    cache.stats(stdout);
    return EXIT_SUCCESS;
}
