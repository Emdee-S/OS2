// This file defines the interface for mem allocation, deallocation, and fragment count
#ifndef MEM_COMPONENT_H
#define MEM_COMPONENT_H

// Memory block structure for linked list
struct MemoryBlock {
    int start_address;      // Starting address of the block
    int size;              // Size of the block in units
    int process_id;        // Process ID if allocated, -1 if free
    MemoryBlock* next;     // Pointer to next block
    
    MemoryBlock(int start, int block_size, int pid = -1) 
        : start_address(start), size(block_size), process_id(pid), next(nullptr) {}
};

// Memory allocation/deallocation technique interface
class MemoryComponent {
public:
    virtual ~MemoryComponent() = default;
    
    // Allocates num_units units of memory to a process
    // Returns number of nodes traversed if successful, -1 otherwise
    virtual int allocate_mem(int process_id, int num_units) = 0;
    
    // Deallocates memory allocated to a process
    // Returns 1 if successful, -1 otherwise
    virtual int deallocate_mem(int process_id) = 0;
    
    // Returns the number of holes (fragments of sizes 1 or 2 units)
    virtual int fragment_count() = 0;
    
    // Get the head pointer for memory layout printing
    MemoryBlock* get_head() const { return head; }

protected:
    MemoryBlock* head;                    // Head of the linked list
    int process_map[1000];               // Simple array to map process_id to block index
    int process_count;                   // Number of active processes
    const int TOTAL_MEMORY_UNITS = 128;  // 256 KB / 2 KB = 128 units
    
    void cleanup_memory();
    void merge_adjacent_free_blocks();
};

// First Fit memory allocation technique
class FirstFitMemory : public MemoryComponent {
public:
    FirstFitMemory();
    ~FirstFitMemory();
    
    int allocate_mem(int process_id, int num_units) override;
    int deallocate_mem(int process_id) override;
    int fragment_count() override;
};

// Best Fit memory allocation technique
class BestFitMemory : public MemoryComponent {
public:
    BestFitMemory();
    ~BestFitMemory();
    
    int allocate_mem(int process_id, int num_units) override;
    int deallocate_mem(int process_id) override;
    int fragment_count() override;
};

#endif // MEM_COMPONENT_H
