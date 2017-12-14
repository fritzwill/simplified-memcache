// This should show that the entry with a key of three will get evicted..
#include "htpage.h"
#include <iostream>
#include <assert.h>

int main(){
	// Create a page that holds 8 entries and uses LRU
	HTPage<int, int> SinglePageTest(8, static_cast<EvictionPolicy>(2));

	// Add eight entries in order to fill the page
	SinglePageTest.put(0, 10, 0);
	SinglePageTest.put(1, 11, 1);
	SinglePageTest.put(2, 12, 2);
	SinglePageTest.put(3, 13, 3);
	SinglePageTest.put(4, 14, 4);
	SinglePageTest.put(5, 15, 5);
	SinglePageTest.put(6, 16, 6);
	SinglePageTest.put(7, 17, 7);

	// Access some entries to change the LRU
	SinglePageTest.get(0, 0);
	SinglePageTest.get(1, 1);
	SinglePageTest.get(2, 2);

	// Add a ninth entry and evict the LRU (key 3)
	SinglePageTest.put(8, 18, 8);
	for (int i = 0; i < 9; i++){
		try{
			SinglePageTest.get(i, i);
		}
		catch (std::out_of_range &e){
			std::cout << "Entry: " << i << " was evicted.\n";
			assert(i == 3);
			break;
		}
	}

}
