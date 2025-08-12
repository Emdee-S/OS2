#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mem_component.h"

// Function to print memory layout
void print_memory_layout(MemoryComponent* mem, const std::string& name) {
    std::cout << "\n" << name << " Memory Layout:" << std::endl;
    std::cout << "==================" << std::endl;
    
    MemoryBlock* current = mem->get_head();
    int block_num = 0;
    
    while (current != nullptr) {
        std::cout << "Block " << block_num << ": ";
        if (current->process_id == -1) {
            std::cout << "FREE  - Address: " << current->start_address 
                     << ", Size: " << current->size << " units" << std::endl;
        } else {
            std::cout << "PROC " << current->process_id 
                     << " - Address: " << current->start_address 
                     << ", Size: " << current->size << " units" << std::endl;
        }
        current = current->next;
        block_num++;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Memory Allocation Simulation Program - Memory Component Test" << std::endl;
    std::cout << "==========================================================" << std::endl << std::endl;
    
    // Create memory components for both techniques
    FirstFitMemory firstFit;
    BestFitMemory bestFit;
    
    std::cout << "Memory components created successfully!" << std::endl;
    std::cout << "Initial memory state: 128 units (256 KB) all free" << std::endl << std::endl;
    
    // Test 1: Basic allocation
    std::cout << "=== Test 1: Basic Allocation ===" << std::endl;
    int result1 = firstFit.allocate_mem(1, 5);
    int result2 = bestFit.allocate_mem(1, 5);
    
    std::cout << "First Fit: Process 1 allocated 5 units, nodes traversed: " << result1 << std::endl;
    std::cout << "Best Fit:  Process 1 allocated 5 units, nodes traversed: " << result2 << std::endl;
    
    print_memory_layout(&firstFit, "First Fit");
    print_memory_layout(&bestFit, "Best Fit");
    
    // Test 2: Multiple allocations
    std::cout << "=== Test 2: Multiple Allocations ===" << std::endl;
    int result3 = firstFit.allocate_mem(2, 8);
    int result4 = bestFit.allocate_mem(2, 8);
    
    std::cout << "First Fit: Process 2 allocated 8 units, nodes traversed: " << result3 << std::endl;
    std::cout << "Best Fit:  Process 2 allocated 8 units, nodes traversed: " << result4 << std::endl;
    
    print_memory_layout(&firstFit, "First Fit");
    print_memory_layout(&bestFit, "Best Fit");
    
    // Test 3: Fragment counting
    std::cout << "=== Test 3: Fragment Counting ===" << std::endl;
    int frags1 = firstFit.fragment_count();
    int frags2 = bestFit.fragment_count();
    
    std::cout << "First Fit fragments: " << frags1 << std::endl;
    std::cout << "Best Fit fragments: " << frags2 << std::endl;
    
    // Test 4: Deallocation
    std::cout << "\n=== Test 4: Deallocation ===" << std::endl;
    int dealloc1 = firstFit.deallocate_mem(1);
    int dealloc2 = bestFit.deallocate_mem(1);
    
    std::cout << "First Fit: Process 1 deallocated, result: " << dealloc1 << std::endl;
    std::cout << "Best Fit:  Process 1 deallocated, result: " << dealloc2 << std::endl;
    
    print_memory_layout(&firstFit, "First Fit (after deallocation)");
    print_memory_layout(&bestFit, "Best Fit (after deallocation)");
    
    // Test 5: Fragment counting after deallocation
    std::cout << "=== Test 5: Fragment Counting After Deallocation ===" << std::endl;
    frags1 = firstFit.fragment_count();
    frags2 = bestFit.fragment_count();
    
    std::cout << "First Fit fragments: " << frags1 << std::endl;
    std::cout << "Best Fit fragments: " << frags2 << std::endl;
    
    // Test 6: Edge cases
    std::cout << "\n=== Test 6: Edge Cases ===" << std::endl;
    
    // Try to allocate more than available
    int result5 = firstFit.allocate_mem(3, 200);
    std::cout << "First Fit: Process 3 tried to allocate 200 units, result: " << result5 << std::endl;
    
    // Try invalid size
    int result6 = firstFit.allocate_mem(4, 2);
    std::cout << "First Fit: Process 4 tried to allocate 2 units (invalid), result: " << result6 << std::endl;
    
    // Try to deallocate non-existent process
    int dealloc3 = firstFit.deallocate_mem(999);
    std::cout << "First Fit: Tried to deallocate process 999, result: " << dealloc3 << std::endl;
    
    // Test 7: Fill memory and test fragmentation
    std::cout << "\n=== Test 7: Memory Filling and Fragmentation ===" << std::endl;
    
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Allocate remaining memory in small chunks
    int process_id = 10;
    int total_allocated = 0;
    
    while (total_allocated < 128) {
        int size = 3 + (std::rand() % 8);  // Random size 3-10
        if (total_allocated + size <= 128) {
            int result = firstFit.allocate_mem(process_id, size);
            if (result > 0) {
                std::cout << "Process " << process_id << " allocated " << size 
                         << " units, nodes traversed: " << result << std::endl;
                total_allocated += size;
                process_id++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    std::cout << "Total memory allocated: " << total_allocated << " units" << std::endl;
    print_memory_layout(&firstFit, "First Fit (Memory Filled)");
    
    // Final fragment count
    frags1 = firstFit.fragment_count();
    std::cout << "Final fragment count: " << frags1 << std::endl;
    
    std::cout << "\nMemory component testing completed!" << std::endl;
    return 0;
}
