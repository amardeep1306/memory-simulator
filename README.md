![Language](https://img.shields.io/badge/language-C%2B%2B11-blue)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20(WSL)-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-Completed-brightgreen)
#  Memory Management Simulator

A robust, C++ based simulation of an Operating System's memory management unit (MMU). This project models physical memory allocation, fragmentation handling, and multilevel CPU caching architectures.

Designed to demonstrate core OS concepts including **Dynamic Memory Allocation**, **Coalescing**, **Fragmentation Analysis**, and **Cache Hit/Miss Logic**.

---
## 🎥 Project Demo

Watch the complete simulation workflow in action:
https://github.com/user-attachments/assets/1db76efe-13a4-4c92-aeee-7cebc856ffb0

##  Features

### 1. Physical Memory Management
- **Contiguous Memory Model**: Simulates a configurable block of physical RAM.
- **Dynamic Allocation**: Splits large memory blocks to satisfy specific user requests.
- **Automatic Coalescing**: Immediately merges adjacent free blocks upon deallocation to minimize external fragmentation.
- **Internal Fragmentation**: Simulates realistic memory alignment (4-byte alignment), tracking wasted space within allocated blocks.

### 2. Allocation Strategies
Implements three standard algorithms to manage free space:
- **First Fit**: Allocates the first free block that fits the request. Fast but can cause fragmentation.
- **Best Fit**: Searches the entire list for the smallest block that fits. Minimizes wasted space but is slower.
- **Worst Fit**: Allocates the largest available block. Leaves large remaining chunks but can lead to external fragmentation.

### 3. Multilevel Cache Simulation
- **Hierarchy**: Simulates a two-level cache system (L1 -> L2 -> Main Memory).
- **L1 Cache**: Small, fast, Direct Mapped cache.
- **L2 Cache**: Larger, Set-Associative cache.
- **LRU Policy**: Implements Least Recently Used (LRU) replacement policy using access counters.
- **Metrics**: Tracks hits, misses, and hit rates per level.

---

##  Project Structure

Following a modular industry-standard layout:

```text
memory-simulator/
├── Makefile                  
├── include/                   
│   ├── Block.h                
│   ├── MemoryManager.h        
│   └── Cache.h                
├── src/                       
│   ├── main.cpp               
│   ├── allocator/             
│   │   └── MemoryManager.cpp
│   ├── cache/                 
│   │   └── Cache.cpp
│   ├── buddy/                 
│   └── virtual_memory/       
├── tests/                     
└── docs/                     
```

## 🛠️ Build & Run Instructions

### Prerequisites
- **Operating System**: Linux (Ubuntu/Kali), macOS, or **Windows via WSL** (Windows Subsystem for Linux).
- **Compiler**: GCC/G++ (supporting C++11 or later).
- **Build Tool**: GNU Make.

### Compilation
Open your terminal in the project root directory and run:

```bash
make
```

This will compile the source code and generate the `memsim` executable.

### Running the Simulator
Start the interactive Command Line Interface (CLI):

```bash
./memsim
```

## 📖 Usage Guide & Commands

Once inside the simulator, use the following commands to interact with the system:

| Command | Description | Example |
| :--- | :--- | :--- |
| `malloc <size>` | Request to allocate memory block of specified size (in bytes). | `malloc 128` |
| `free <id>` | Free an allocated block using its Block ID. | `free 1` |
| `access <id>` | Simulate a CPU read/write to a block. This triggers the Cache logic (L1/L2 check) and Virtual Memory translation. | `access 1` |
| `dump` | Display the current memory map showing which blocks are Free vs Used. | `dump` |
| `stats` | Show detailed statistics (Memory usage, Cache Hit/Miss rates). | `stats` |
| `exit` | Exit the simulator. | `exit` |

### Example Workflow
Here is a sample session demonstrating Best Fit allocation and Coalescing:

```text
> init 1000
Initialized memory with 1000 bytes.

> allocator best fit
[SYSTEM] Allocator strategy set to: Best Fit

> malloc 100
Allocated block id=1 at address 0x0

> malloc 500
Allocated block id=2 at address 0x64

> malloc 100
Allocated block id=3 at address 0x258

> free 2
Block 2 freed.

> malloc 400
Allocated block id=4 at address 0x64
(Note: Best Fit placed this in the gap left by Block 2)

> dump
[0x64 - 0x1F3] USED (ID=4) | Size: 400 ...

> stats
Internal Fragmentation: 0 bytes
External Fragmentation: 0%

```

##  Design Decisions

- **Memory Alignment**: The system enforces **4-byte alignment**. For example, if a user requests 10 bytes, the system allocates 12 bytes. This realistic behavior ensures proper addressing and allows the calculation of **Internal Fragmentation** metrics.
- **Coalescing Strategy**: The system implements **Immediate Coalescing** rather than Lazy Coalescing. As soon as a block is freed, the allocator checks its immediate left and right neighbors. If they are free, they are instantly merged into a single larger contiguous block to minimize external fragmentation.
- **Cache Tagging**: The cache simulation utilizes **Physical Addresses** generated by the Memory Manager. These addresses are used to calculate tags and set indices, simulating how a real CPU interacts with the Memory Management Unit (MMU).

##  Future Improvements

- Implementation of the **Buddy System** allocator for power-of-two allocations.
- Integration of **Virtual Memory** simulation, including Page Tables and Page Fault handling.
- Adding **Thread-safety** (Mutex locks) to support concurrent memory allocation requests.

##  References & Acknowledgments

This simulator was designed based on concepts from standard Operating System textbooks and modern development tools:

- **Operating System Concepts** - Silberschatz, Galvin, Gagne
- **Modern Operating Systems** - Andrew S. Tanenbaum
- **Project Specifications**: Provided course material on Memory Management Unit (MMU) design.
- **AI Assistance**: Utilized AI tools (LLMs) for code refactoring, debugging, and generating documentation.

##  License

This project is developed for educational purposes as part of an Operating Systems curriculum.
