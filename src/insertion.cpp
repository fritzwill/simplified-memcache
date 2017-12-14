// insertion.cpp

#include <htcache/htcache.h>

#include <cassert>
#include <cstdio>
#include <random>
#include <thread>

#define N (1<<10)

int64_t Data[N] = {0};

void insertion_thread(HTCache<int64_t, int64_t> *cache) {
    for (size_t back = 1; back < N; back++) {
	for (size_t i = back; i > 0 && cache->get(i) < cache->get(i - 1); i--) {
	    int64_t curr = cache->get(i);
	    int64_t prev = cache->get(i - 1);
	    cache->put(i    , prev); Data[i]     = prev;
	    cache->put(i - 1, curr); Data[i - 1] = curr;
	}

	assert(Data[back - 1] <= Data[back]);
    }
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

    HTCache<int64_t, int64_t>	    cache(addrlen, pagesize, policy, [](const int64_t &key) { return Data[key]; });
    std::default_random_engine	    engine;
    std::uniform_int_distribution<> random(0, N);

    for (size_t i = 0; i < N; i++) {
    	Data[i] = random(engine);
    }

    std::thread t[nthreads];
    for (size_t i = 0; i < nthreads; i++) {
    	t[i] = std::thread(insertion_thread, &cache);
    }

    for (size_t i = 0; i < nthreads; i++) {
    	t[i].join();
    }

    for (size_t i = 1; i < N; i++) {
    	assert(Data[i - 1] <= Data[i]);
    }

    cache.stats(stdout);
    return EXIT_SUCCESS;
}
