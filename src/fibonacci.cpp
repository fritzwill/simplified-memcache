// fibonacci.cpp

#include <htcache/htcache.h>
#include <iomanip>
#include <cstdio>
#include <thread>
HTCache<int64_t, unsigned long long> *CachePointer = nullptr;

unsigned long long Fib(int64_t n){
	if (n <= 1){
		return n;
	}

	return CachePointer->get(n);
}

void Fib_Thread(size_t tid, HTCache<int64_t, unsigned long long> *cache){
	cache->put(0, 0);
	cache->put(1, 1);
	for (int64_t i = 0; i <= 99; i++){
		std::cout << " 0. Fibonacci(" << std::setw(2) << i << ") = " << cache->get(i) << std::endl;
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

    HTCache<int64_t, unsigned long long>	    cache(addrlen, pagesize, policy, [](const int64_t &key) { return Fib(key-1)+Fib(key-2); });
	CachePointer = &cache;

    std::thread t[nthreads];
    for (size_t i = 0; i < nthreads; i++) {
    	t[i] = std::thread(Fib_Thread, i, &cache);
    }
    for (size_t i = 0; i < nthreads; i++) {
    	t[i].join();
    }
    
    cache.stats(stdout);
    return EXIT_SUCCESS;
}
