// htcache.h: Hash Tree Cache

#pragma once

#include <htcache/htpage.h>

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

template <typename KeyType, typename ValueType>
class HTCache {
    typedef ValueType (*HandlerFunc)(const KeyType&);
    typedef std::vector<HTPage<KeyType, ValueType>> TableType;

private:
    size_t	    AddressLength;  // Length of virtual address
    size_t	    PageSize;	    // Number of entries per page
    EvictionPolicy  Policy;	    // Eviction policy
    HandlerFunc	    Handler;	    // Handler for misses
    TableType	    PageTable;	    // Page table

    size_t	    Addresses;	    // Number of addresses
    size_t	    Pages;	    // Number of pages
    size_t	    VPNShift;	    // VPN Bit Shift
    size_t	    VPNMask;	    // VPN Bit Mask
    size_t	    OffsetMask;	    // Offset Bit Mask

    size_t	    Hits;	    // Number of cache hits
    size_t	    Misses;	    // Number of cache misses

    std::mutex	    Lock;	    // Lock

public:
    HTCache(size_t addrlen, size_t pagesize, EvictionPolicy policy, HandlerFunc handler) {
		std::lock_guard<std::mutex> LG(Lock); // Lock until out of scope
    	if (addrlen == 0) {
    	    return;
	    }

        // init given values
        AddressLength = addrlen;
        PageSize = pagesize;
        Policy = policy;
        Handler = handler;

	    // TODO: Determine Addresses, Pages, VPNShift, VPNMask, OffsetMask

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// USE ADDRESSLENGTH AND PAGESIZE TO CALCULATE THE TOTAL ADDRESSES AND PAGES ALONG WITH THEIR NEEDED MASKS //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        Addresses = pow(2.0, AddressLength);
        Pages = Addresses / PageSize;
        VPNShift = (size_t)log2(PageSize);
        OffsetMask = ((1 << VPNShift) - 1);
        VPNMask = (((1 << AddressLength) - 1) & ~OffsetMask);

	    // TODO: Initialize PageTable

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// PUSH BACK EMPTY PAGES INTO THE PAGE TABLE AND SET THE HITS AND MISSES TO ZERO //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	    for (size_t i = 0; i < Pages; i++)
            PageTable.push_back(HTPage<KeyType, ValueType>(PageSize, Policy));
		Hits = 0;
		Misses = 0;
    }

    ValueType get(const KeyType &key) {
    	if (PageTable.empty()) {
    	    return Handler(key);
	    }

	    // TODO: Determine virtual address, VPN, offset
	    size_t virtualAddress, VPN, offset;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// GENERATE A HASH BASED ON THE KEY AND MOD IT WITH THE TOTAL ADDRESSES //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        std::hash<KeyType> hasher;
        virtualAddress = hasher(key);
        virtualAddress %= Addresses;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// MASK AND SHIFT TO GET THE VPN(INDEX) AND OFFSET(INDEX) //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        VPN = ((virtualAddress & VPNMask) >> VPNShift);
        offset = (virtualAddress & OffsetMask);
	    // TODO: Retrieve value from HTPage
	    ValueType val;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// EITHER GET A HIT OR GET A MISS //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        try {
			std::lock_guard<std::mutex> LG(Lock); // Lock until out of scope
            val = PageTable[VPN].get(key, offset);
            Hits++;
        }
        catch (std::out_of_range &e){
			Lock.lock();
            Misses++;
			Lock.unlock();
            val = Handler(key);
            put(key, val);
        }
    
    	return val;
    }

    void put(const KeyType &key, const ValueType &value) {
		std::lock_guard<std::mutex> LG(Lock); // Lock until out of scope
    	if (PageTable.empty()) {
    	    return;
	    }

	    // TODO: Determine virtual address, VPN, offset
        size_t virtualAddress, VPN, offset;
        std::hash<KeyType> hasher;
        virtualAddress = hasher(key);
        virtualAddress %= Addresses;
        VPN = ((virtualAddress & VPNMask) >> VPNShift);
        offset = (virtualAddress & OffsetMask); 
	    // TODO: Set key, value in HTPage
	    PageTable[VPN].put(key, value, offset);
    }

    void stats(FILE *stream) {
		fprintf(stream, "Addresses : %lu\n"  , Addresses);
		fprintf(stream, "Pages     : %lu\n"  , Pages);
		fprintf(stream, "VPNShift  : %lu\n"  , VPNShift);
		fprintf(stream, "VPNMask   : 0x%lX\n", VPNMask);
		fprintf(stream, "OffsetMask: 0x%lX\n", OffsetMask);
    	fprintf(stream, "Hits      : %lu\n"  , Hits);
    	fprintf(stream, "Misses    : %lu\n"  , Misses);
    }
};
