#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstddef>

struct CacheLine {
    bool valid = false;
    unsigned int tag = 0;
    int last_used_time = 0;
};

class CacheLevel {
private:
    int level_id;
    size_t total_size;
    size_t line_size;
    int associativity;
    int num_sets;
    
    std::vector<std::vector<CacheLine>> sets;
    int access_counter;
    
    // Statistics
    int hits;
    int misses;

public:
    CacheLevel(int id, size_t size, size_t l_size, int assoc);
    bool access(size_t address);
    void print_stats();
};

#endif