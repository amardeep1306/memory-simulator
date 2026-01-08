#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "Block.h"
#include <cstddef>

enum AllocationStrategy {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

class MemoryManager {
private:
    size_t total_memory;
    Block* head;
    AllocationStrategy strategy;
    int next_block_id;
    const size_t ALIGNMENT = 4; // 4-byte alignment to create internal fragmentation

public:
    MemoryManager(size_t size);
    ~MemoryManager();

    void set_strategy(AllocationStrategy s);
    int malloc(size_t size);
    void free(int id);
    void dump();
    void show_stats();
    size_t get_address_of_id(int id);
};

#endif