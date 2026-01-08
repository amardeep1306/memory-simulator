#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef> 

struct Block {
    int id;               
    size_t start_addr;    // Physical start address
    size_t size;          
    size_t payload_size;  
    bool is_free;
    Block* next;
};

#endif