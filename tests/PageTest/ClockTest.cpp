// This should show that the entry with a key of three will get evicted.
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

	// Add a new entry, causing the clock to do a full cycle of setting each dirty bit to 0
	SinglePageTest.put(8, 18, 8);

	// Update the values of keys 1 and 2 so that their dirty bit is reset
	SinglePageTest.put(1, 21, 1);
	SinglePageTest.put(2, 22, 2);

	// Add a ninth entry and evict the next option on the clock (key 3)
	SinglePageTest.put(9, 18, 9);
	for (int i = 1; i < 10; i++){
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
