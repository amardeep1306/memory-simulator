#include "../../include/MemoryManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

MemoryManager::MemoryManager(size_t size) : total_memory(size), strategy(FIRST_FIT) {
    head = new Block();
    head->id = 0;
    head->start_addr = 0;
    head->size = size;
    head->payload_size = 0;
    head->is_free = true;
    head->next = nullptr;
    next_block_id = 1;
}

MemoryManager::~MemoryManager() {
    Block* curr = head;
    while (curr) {
        Block* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

void MemoryManager::set_strategy(AllocationStrategy s) {
    strategy = s;
    string name = (s == FIRST_FIT) ? "First Fit" : (s == BEST_FIT) ? "Best Fit" : "Worst Fit";
    cout << "[SYSTEM] Allocator strategy set to: " << name << endl;
}

int MemoryManager::malloc(size_t size) {
    // ALIGNMENT LOGIC for Internal Fragmentation [cite: 60]
    size_t aligned_size = (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

    Block* best = nullptr;
    Block* curr = head;

    // Strategy Selection [cite: 21-24]
    while (curr) {
        if (curr->is_free && curr->size >= aligned_size) {
            if (strategy == FIRST_FIT) {
                best = curr;
                break;
            }
            else if (strategy == BEST_FIT) {
                if (!best || curr->size < best->size) best = curr;
            }
            else if (strategy == WORST_FIT) {
                if (!best || curr->size > best->size) best = curr;
            }
        }
        curr = curr->next;
    }

    if (!best) {
        cout << "ERROR: Not enough memory." << endl;
        return -1;
    }

    // Splitting [cite: 27]
    if (best->size > aligned_size) {
        Block* new_free = new Block();
        new_free->id = 0;
        new_free->start_addr = best->start_addr + aligned_size;
        new_free->size = best->size - aligned_size;
        new_free->payload_size = 0;
        new_free->is_free = true;
        new_free->next = best->next;

        best->size = aligned_size;
        best->next = new_free;
    }

    best->is_free = false;
    best->id = next_block_id++;
    best->payload_size = size; // Store actual request for stats
    
    cout << "Allocated block id=" << best->id << " at address 0x" 
         << hex << uppercase << best->start_addr << dec << endl;
    
    return best->id;
}

void MemoryManager::free(int id) {
    Block* curr = head;
    bool found = false;

    while (curr) {
        if (curr->id == id && !curr->is_free) {
            curr->is_free = true;
            curr->id = 0;
            curr->payload_size = 0;
            found = true;
            cout << "Block " << id << " freed." << endl;
            break;
        }
        curr = curr->next;
    }

    if (!found) {
        cout << "ERROR: Invalid Block ID." << endl;
        return;
    }

    // Coalescing [cite: 32] - Fixed Logic
    curr = head;
    while (curr && curr->next) {
        if (curr->is_free && curr->next->is_free) {
            Block* temp = curr->next;
            curr->size += temp->size;
            curr->next = temp->next;
            delete temp;
            cout << " (Merged adjacent free blocks)" << endl;
            // Do NOT advance 'curr' here; check the new neighbor in next iteration
        } else {
            curr = curr->next;
        }
    }
}

void MemoryManager::dump() {
    cout << "\n--- MEMORY DUMP ---" << endl;
    Block* curr = head;
    while (curr) {
        cout << "[0x" << hex << curr->start_addr << " - 0x" << (curr->start_addr + curr->size - 1) << dec << "] ";
        if (curr->is_free) cout << "FREE ";
        else cout << "USED (ID=" << curr->id << ") ";
        cout << "| Size: " << curr->size << endl;
        curr = curr->next;
    }
    cout << "-------------------" << endl;
}

void MemoryManager::show_stats() {
    size_t free_mem = 0;
    size_t used_mem = 0;
    size_t max_free_block = 0;
    size_t internal_frag_total = 0;

    Block* curr = head;
    while (curr) {
        if (curr->is_free) {
            free_mem += curr->size;
            if (curr->size > max_free_block) max_free_block = curr->size;
        } else {
            used_mem += curr->size;
            // Calc internal fragmentation [cite: 60]
            if (curr->payload_size > 0)
                internal_frag_total += (curr->size - curr->payload_size);
        }
        curr = curr->next;
    }

    float ext_frag = 0.0f;
    if (free_mem > 0) {
        ext_frag = 1.0f - ((float)max_free_block / free_mem);
    }

    cout << "\n--- STATISTICS ---" << endl;
    cout << "Total Memory: " << total_memory << endl;
    cout << "Used Memory:  " << used_mem << endl;
    cout << "Free Memory:  " << free_mem << endl;
    cout << "Internal Fragmentation: " << internal_frag_total << " bytes" << endl;
    cout << "External Fragmentation: " << (ext_frag * 100.0) << "%" << endl;
}

size_t MemoryManager::get_address_of_id(int id) {
    Block* curr = head;
    while(curr) {
        if (curr->id == id) return curr->start_addr;
        curr = curr->next;
    }
    return -1;
}