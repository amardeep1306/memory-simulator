#include "../../include/Cache.h"
#include <iostream>

using namespace std;

CacheLevel::CacheLevel(int id, size_t size, size_t l_size, int assoc) 
    : level_id(id), total_size(size), line_size(l_size), associativity(assoc) {
    
    num_sets = total_size / (line_size * associativity);
    sets.resize(num_sets, vector<CacheLine>(associativity));
    access_counter = 0;
    hits = 0;
    misses = 0;
}

bool CacheLevel::access(size_t address) {
    access_counter++;
    size_t block_addr = address / line_size;
    size_t set_index = block_addr % num_sets;
    unsigned int tag = block_addr / num_sets;

    // 1. Check for Hit
    for (auto& line : sets[set_index]) {
        if (line.valid && line.tag == tag) {
            line.last_used_time = access_counter; 
            hits++;
            return true;
        }
    }

    misses++;
    
    int victim_idx = -1;
    int min_time = 2147483647;
    
    for (int i = 0; i < associativity; ++i) {
        if (!sets[set_index][i].valid) {
            victim_idx = i;
            break; 
        }
        if (sets[set_index][i].last_used_time < min_time) {
            min_time = sets[set_index][i].last_used_time;
            victim_idx = i;
        }
    }

    // Replace
    sets[set_index][victim_idx].valid = true;
    sets[set_index][victim_idx].tag = tag;
    sets[set_index][victim_idx].last_used_time = access_counter;

    return false;
}

void CacheLevel::print_stats() {
    cout << "L" << level_id << " Cache: Hits=" << hits 
         << " Misses=" << misses 
         << " Hit Rate=" << (hits + misses > 0 ? (float)hits/(hits+misses)*100.0 : 0.0) << "%" << endl;
}