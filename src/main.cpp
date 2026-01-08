#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "../include/MemoryManager.h"
#include "../include/Cache.h"

using namespace std;

class SystemSimulator {
private:
    MemoryManager* mem;
    CacheLevel* L1;
    CacheLevel* L2;

public:
    SystemSimulator() {
        mem = nullptr;
        // L1: 64B, 16B Block, Direct Mapped [cite: 72]
        L1 = new CacheLevel(1, 64, 16, 1); 
        // L2: 256B, 16B Block, 2-Way Assoc [cite: 74]
        L2 = new CacheLevel(2, 256, 16, 2); 
    }

    void process_command(string line) {
        stringstream ss(line);
        string cmd;
        ss >> cmd;

        if (cmd == "init") {
            size_t size;
            ss >> size;
            if (mem) delete mem;
            mem = new MemoryManager(size);
            cout << "Initialized memory with " << size << " bytes." << endl;
        }
        else if (!mem) {
            cout << "Error: Memory not initialized. Run 'init <size>' first." << endl;
            return;
        }
        else if (cmd == "allocator") {
            string type, fit;
            ss >> type >> fit; 
            if (type == "first") mem->set_strategy(FIRST_FIT);
            else if (type == "best") mem->set_strategy(BEST_FIT);
            else if (type == "worst") mem->set_strategy(WORST_FIT);
            else cout << "Unknown strategy." << endl;
        }
        else if (cmd == "malloc") {
            size_t size;
            ss >> size;
            mem->malloc(size);
        }
        else if (cmd == "free") {
            int id;
            ss >> id;
            mem->free(id);
        }
        else if (cmd == "dump") {
            mem->dump();
        }
        else if (cmd == "stats") {
            mem->show_stats();
            L1->print_stats();
            L2->print_stats();
        }
        else if (cmd == "access") { 
            int id;
            ss >> id;
            size_t addr = mem->get_address_of_id(id);
            if (addr == (size_t)-1) {
                cout << "Invalid Block ID." << endl;
            } else {
                cout << "CPU accessing Block " << id << " (Addr: " << addr << ")... ";
                if (L1->access(addr)) {
                    cout << "L1 HIT!" << endl;
                } else {
                    cout << "L1 MISS -> Accessing L2... ";
                    if (L2->access(addr)) {
                        cout << "L2 HIT!" << endl;
                    } else {
                        cout << "L2 MISS -> Fetch from RAM." << endl;
                    }
                }
            }
        }
        else if (cmd == "exit") {
            exit(0);
        }
        else {
            cout << "Unknown command." << endl;
        }
    }
};

int main() {
    SystemSimulator sim;
    string line;
    
    cout << "=== Memory Management Simulator ===" << endl;
    cout << "> ";
    while (getline(cin, line)) {
        if (!line.empty()) sim.process_command(line);
        cout << "> ";
    }
    return 0;
}