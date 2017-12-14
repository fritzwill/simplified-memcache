// This should give different output each time it is run.
#include "htpage.h"
#include <iostream>
#include <assert.h>

int main(){
	// Create a page that holds 8 entries and uses random
	HTPage<int, int> SinglePageTest(8, static_cast<EvictionPolicy>(1));

	// Add eight entries in order to fill the page
	SinglePageTest.put(0, 10, 0);
	SinglePageTest.put(1, 11, 1);
	SinglePageTest.put(2, 12, 2);
	SinglePageTest.put(3, 13, 3);
	SinglePageTest.put(4, 14, 4);
	SinglePageTest.put(5, 15, 5);
	SinglePageTest.put(6, 16, 6);
	SinglePageTest.put(7, 17, 7);

	// Add a ninth entry and evict a random one
	SinglePageTest.put(8, 18, 8);
	for (int i = 0; i < 9; i++){
		try{
			SinglePageTest.get(i, i);
		}
		catch (std::out_of_range &e){
			std::cout << "Entry: " << i << " was evicted.\n";
			break;
		}
	}

	// Add eight entries in order to fill the page
	SinglePageTest.put(0, 10, 0);
	SinglePageTest.put(1, 11, 1);
	SinglePageTest.put(2, 12, 2);
	SinglePageTest.put(3, 13, 3);
	SinglePageTest.put(4, 14, 4);
	SinglePageTest.put(5, 15, 5);
	SinglePageTest.put(6, 16, 6);
	SinglePageTest.put(7, 17, 7);

	// Add a ninth entry and evict a random one
	SinglePageTest.put(8, 18, 8);
	for (int i = 0; i < 9; i++){
		try{
			SinglePageTest.get(i, i);
		}
		catch (std::out_of_range &e){
			std::cout << "Entry: " << i << " was evicted.\n";
			break;
		}
	}

	// Add eight entries in order to fill the page
	SinglePageTest.put(0, 10, 0);
	SinglePageTest.put(1, 11, 1);
	SinglePageTest.put(2, 12, 2);
	SinglePageTest.put(3, 13, 3);
	SinglePageTest.put(4, 14, 4);
	SinglePageTest.put(5, 15, 5);
	SinglePageTest.put(6, 16, 6);
	SinglePageTest.put(7, 17, 7);

	// Add a ninth entry and evict a random one
	SinglePageTest.put(8, 18, 8);
	for (int i = 0; i < 9; i++){
		try{
			SinglePageTest.get(i, i);
		}
		catch (std::out_of_range &e){
			std::cout << "Entry: " << i << " was evicted.\n";
			break;
		}
	}
}
