#include "htcache.h"
#include <iostream>
#include <assert.h>

int main(){
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// CREATE A CACHE THAT CAN HOLD 8 ENTRIES WITH 4 PAGES USING FIFO //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	HTCache<int, int> CacheTest(3, 2, static_cast<EvictionPolicy>(0), [](const int &key) { return 42; });
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// TEST GET MISSES/HITS CORRECTLY AND UPDATES AN ENTRY CORRECTLY //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	std::cout << "Looking for key 0 (Should miss)...\n";
	std::cout << "Found: " << CacheTest.get(0) <<std::endl;
	CacheTest.stats(stdout);
	std::cout << "Looking for key 0 (Should hit)...\n";
	std::cout << "Found " << CacheTest.get(0) <<std::endl;
	CacheTest.stats(stdout);
	assert(CacheTest.get(0) == 42);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// TEST THAT PUT WORKS FOR FILLING THE CACHE //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	std::cout << "Adding elements 1 - 7 to the cache...\n";
	for (int i = 1; i < 8; i++) CacheTest.put(i, i);
	std::cout << "Cache is holding:";
	for (int i = 0; i < 8; i++) std::cout << " " << CacheTest.get(i);
	std::cout << std::endl;
	CacheTest.stats(stdout);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// TEST THAT PUT WILL EVICT AN ENTRY ONCE FULL //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	std::cout << "Adding new entry and evicting one...\n";
	CacheTest.put(8, 10);
	std::cout << "Cache now holding:";
	for (int i = 1; i < 9; i++) std::cout << " " << CacheTest.get(i);
	std::cout << std::endl;
	std::cout << "Should now have 18 total hits.\n";
	CacheTest.stats(stdout);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// A GET FOR KEY 0 SHOULD NOW RESULT IN A MISS AND EVICT KEY 1 //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	std::cout << CacheTest.get(1) <<std::endl;
	std::cout << "Checking for key 0 (should miss)...\n";
	std::cout << "Found: " << CacheTest.get(0) << std::endl;
	CacheTest.stats(stdout);
	assert(CacheTest.get(1) == 42); // Show that key 1 got evicted when we tried to get the missing key 0 and had to then add it to our cache
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// SHOW THAT FUTURE MISSES WILL UPDATE WITH THE HANDLER FUNCTION //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	std::cout << "Looking at keys 0-8 to see which eight are actually in the cache...\nAfter key 0, any value of 42 should represent a miss...\nFound:";
	for (int i = 0; i <= 8; i++) std::cout << " " << CacheTest.get(i);
	std::cout << std::endl;
	CacheTest.stats(stdout);

}
