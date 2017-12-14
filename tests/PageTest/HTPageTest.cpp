#include "htpage.h"
#include <iostream>
#include <assert.h>

int CanYouCatchAFunction(HTPage<int, int> TestPage, int KeyIndex){	// An extra function to make sure the catch still recieves the exception
	return TestPage.get(KeyIndex, KeyIndex);
}

int main(){
	// Create a page that holds 4 entries and uses fifo
	HTPage<int, int> SinglePageTest(4, static_cast<EvictionPolicy>(0));
	try{
		int ThrowException = CanYouCatchAFunction(SinglePageTest, 0);
	}
	catch (std::out_of_range &e){
		std::cout << "Couldn't get key because out_of_index reached\n";
	}
	// Add four entries in order to fill the page
	SinglePageTest.put(0, 4, 0);
	SinglePageTest.put(1, 5, 1);
	SinglePageTest.put(2, 6, 2);
	SinglePageTest.put(3, 7, 3);

	for (int i = 0; i < 3; i++){
		std::cout << SinglePageTest.get(i, i) << "->";
		assert(SinglePageTest.get(i, i) == i+4);
	}
	std::cout << SinglePageTest.get(3, 3) << std::endl;

	// Add a new entry causing key 0 to get evicted
	SinglePageTest.put(14, 8, 2); // should evict index 0
	try{
		int ThrowException = CanYouCatchAFunction(SinglePageTest, 0);
	}
	catch (std::out_of_range &e){
		std::cout << "Couldn't get key because out_of_index reached: 4 is no longer stored in the cache\n";
		std::cout << "The first page must have been evicted!\n";
	}
	std::cout << SinglePageTest.get(14, 2) << "->";
	assert(SinglePageTest.get(14, 2) == 8);
	for (int i = 1; i < 3; i++){
		std::cout << SinglePageTest.get(i, i) << "->";
		assert(SinglePageTest.get(i, i) == i+4);
	}
	std::cout << SinglePageTest.get(3, 3) << std::endl;
}
