// htpage.h: Hash Tree Page

#pragma once

#include <iostream>
#include <cstdlib>
#include <mutex>
#include <queue>
#include <random>
#include <stdexcept>
#include <vector>
#include <ctime>

enum EvictionPolicy {
    EVICT_FIFO,
    EVICT_RANDOM,
    EVICT_LRU,
    EVICT_CLOCK,
};

template <typename KeyType, typename ValueType>
struct HTPageEntry {
    KeyType	Key;
    ValueType	Value;
    // TODO: Add bookkeeping for eviction
    int dirtyBit;
	bool isEmpty;
};

template <typename KeyType, typename ValueType>
class HTPage {
    typedef std::vector<HTPageEntry<KeyType, ValueType>> EntriesType;

    private:
    EntriesType		Entries;    // Individual entries
    EvictionPolicy	Policy;	    // Eviction policy
    mutable std::mutex	Lock;	    // Lock

    // TODO: Add bookkeeping for eviction
	int ClockHand;
	std::queue<size_t> FifoOffsets;
	std::vector<size_t> LRUOffests;

    size_t evict_fifo(size_t offset) {
        // TODO: Implement FIFO eviction policy
		offset = FifoOffsets.front(); FifoOffsets.pop();
		return offset;
    }

    size_t evict_random(size_t offset) {
        // TODO: Implement random eviction policy
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, Entries.size()-1);
        offset = dist(gen);
        return offset;
    }

    size_t evict_lru(size_t offset) {
        // TODO: Implement LRU eviction policy

        return LRUOffests.front();
    }

    size_t evict_clock(size_t offset) {
        // TODO: Implement clock eviction policy
        bool foundOne = false;
        for (size_t i = offset; i < Entries.size(); i++){
            if (Entries[i].dirtyBit == 1) Entries[i].dirtyBit = 0;
            else { // dirtyBit == 0 aka found the one
                offset = i;
                foundOne = true;
                break;
            }
        }
        if (!foundOne){
            for (size_t i = 0; i < offset; i ++){
                if (Entries[i].dirtyBit == 1) Entries[i].dirtyBit = 0;
                else {
                    offset = i;
                    break;
                }
            }
        }
        return offset;
    }

    public:
    HTPage(size_t n, EvictionPolicy p) {
        // TODO: Initialize Entries
        std::lock_guard<std::mutex> LG(Lock);	// Lock until out of scope
        Policy = p;
        for (size_t i = 0; i < n; i++){			// Make n empty entries
            HTPageEntry<KeyType, ValueType> NullEntry;
            NullEntry.isEmpty = true;
            Entries.push_back(NullEntry);
        }
		ClockHand = 0;							// Set the clockhand to a starting value
    }

    HTPage(const HTPage<KeyType, ValueType>& other) {
        // TODO: Copy instance variables
        std::lock_guard<std::mutex> LG(Lock);	// Lock until out of scope
        Entries = other.Entries;
        Policy = other.Policy;
		ClockHand = other.ClockHand;
		FifoOffsets = other.FifoOffsets;
		LRUOffests = other .LRUOffests;
    }

    ValueType get(const KeyType &key, size_t offset) {
        // TODO: Use linear probing to locate key
        std::lock_guard<std::mutex> LG(Lock);	// Lock until out of scope
        ValueType val;
        bool foundOne = false;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LINEAR SEARCH FROM OFFSET TO END //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        for (size_t i = offset; i < Entries.size(); i++){
            if (Entries[i].isEmpty) break;
            else if (Entries[i].Key == key){
                foundOne = true;
                val = Entries[i].Value;
				if (Policy == EVICT_LRU){
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// FIND IF THE OFFSET IS ALREADY IN THE USE LIST //
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					std::vector<size_t>::iterator it;
					for (it = LRUOffests.begin(); it != LRUOffests.end(); it++){
						if (*it == i) break;
					}
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// DELETE THE CURRENT RECORDING OF ITS USE //
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					if (it != LRUOffests.end()){
						LRUOffests.erase(it);
					}
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					// ADD THE ACCESSED OFFSET TO THE END OF THE USE LIST //
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
					LRUOffests.push_back(i);
				}
                break;
            }
        }
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LINEAR SEARCH FROM BEINGING TO OFFSET //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        if (!foundOne){
            for (size_t i = 0; i < offset; i++){
                if (Entries[i].isEmpty) break;
                else if (Entries[i].Key == key){
                    foundOne = true;
                    val = Entries[i].Value;
					if (Policy == EVICT_LRU){
						//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
						// FIND IF THE OFFSET IS ALREADY IN THE USE LIST //
						//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
						std::vector<size_t>::iterator it;
						for (it = LRUOffests.begin(); it != LRUOffests.end(); it++){
							if (*it == i) break;
						}
						//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
						// DELETE THE CURRENT RECORDING OF ITS USE //
						//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
						if (it != LRUOffests.end()){
							LRUOffests.erase(it);
						}
						//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
						// ADD THE ACCESSED OFFSET TO THE END OF THE USE LIST //
						//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
						LRUOffests.push_back(i);
					}
                    break;
                }
            }
        }

        if (!foundOne) throw std::out_of_range ("Could not find in Entries");
        return val;

    }

    void put(const KeyType &key, const ValueType &value, size_t offset) {
        // TODO: Use linear probing to locate key
        std::lock_guard<std::mutex> LG(Lock);	// Lock until out of scope
        bool foundOne = false;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LINEAR SEARCH FROM OFFSET TO END //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        for (size_t i = offset; i < Entries.size(); i++){
            if (Entries[i].isEmpty){ // new
                foundOne = true;
				Entries[i].isEmpty = false;
                Entries[i].Key = key;
                offset = i;
				FifoOffsets.push(offset);
                break;
            }
            else if (Entries[i].Key == key){ //update existing
                foundOne = true;
                offset = i;
                break;
            }
        }
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// LINEAR SEARCH FROM BEGINNING TO OFFSET //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        if (!foundOne){
            for (size_t i = 0; i < offset; i++){
                if (Entries[i].isEmpty){ // new
                    foundOne = true;
					Entries[i].isEmpty = false;
                    Entries[i].Key = key;
                    offset = i;
					FifoOffsets.push(offset);
                    break;
                }
                else if (Entries[i].Key == key){ //update existing
                    foundOne = true;
                    offset = i;
                    break;
                }
            }
        }

        // TODO: Evict an entry if HTPage is full
        if (!foundOne){
            switch (Policy){
                case EVICT_FIFO:
                    offset = evict_fifo(offset);
                    break;
                case EVICT_RANDOM:
                    offset = evict_random(offset);
                    break;
                case EVICT_LRU:
                    offset = evict_lru(offset);
                    break;
                case EVICT_CLOCK:
                    offset = evict_clock(ClockHand);
					ClockHand = offset;
                    break;
            }
			Entries[offset].Key = key;
			FifoOffsets.push(offset);
        }
        // TODO: Update entry
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// FOR LRU MOVE THE OFFSET OF THE ENTRY BEING ACCESSED TO THE END OF A LIST //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		if (Policy == EVICT_LRU){
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// FIND IF THE OFFSET IS ALREADY IN THE USE LIST //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			std::vector<size_t>::iterator it;
			for (it = LRUOffests.begin(); it != LRUOffests.end(); it++){
				if (*it == offset) break;
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// DELETE THE CURRENT RECORDING OF ITS USE //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			if (it != LRUOffests.end()){
				LRUOffests.erase(it);
			}
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			// ADD THE ACCESSED OFFSET TO THE END OF THE USE LIST //
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
			LRUOffests.push_back(offset);
		}

        Entries[offset].Value = value;
        Entries[offset].dirtyBit = 1;
    }
};
