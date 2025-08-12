#include "mem_component.h"
#include <iostream>
#include <cstring>

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void MemoryComponent::cleanup_memory() {
    MemoryBlock* current = head;
    while (current != nullptr) {
        MemoryBlock* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    process_count = 0;
    memset(process_map, -1, sizeof(process_map));
}

void MemoryComponent::merge_adjacent_free_blocks() {
    MemoryBlock* current = head;
    
    while (current != nullptr && current->next != nullptr) {
        if (current->process_id == -1 && current->next->process_id == -1) {
            // Merge two adjacent free blocks
            current->size += current->next->size;
            MemoryBlock* to_delete = current->next;
            current->next = to_delete->next;
            delete to_delete;
        } else {
            current = current->next;
        }
    }
}

// ============================================================================
// FIRST FIT MEMORY IMPLEMENTATION
// ============================================================================

FirstFitMemory::FirstFitMemory() {
    // Initialize with one free block covering entire memory
    head = new MemoryBlock(0, TOTAL_MEMORY_UNITS);
    process_count = 0;
    memset(process_map, -1, sizeof(process_map));
}

FirstFitMemory::~FirstFitMemory() {
    cleanup_memory();
}

int FirstFitMemory::allocate_mem(int process_id, int num_units) {
    if (num_units < 3 || num_units > 10) {
        return -1;  // Invalid request size
    }
    
    MemoryBlock* current = head;
    int nodes_traversed = 0;
    
    // Search for first block that can accommodate the request
    while (current != nullptr) {
        nodes_traversed++;
        
        if (current->process_id == -1 && current->size >= num_units) {
            // Found a free block that can fit the request
            
            if (current->size == num_units) {
                // Exact fit - allocate entire block
                current->process_id = process_id;
                process_map[process_id] = nodes_traversed - 1;  // Store block index
                process_count++;
            } else {
                // Split the block
                MemoryBlock* new_block = new MemoryBlock(
                    current->start_address + num_units, 
                    current->size - num_units
                );
                
                current->size = num_units;
                current->process_id = process_id;
                new_block->next = current->next;
                current->next = new_block;
                
                process_map[process_id] = nodes_traversed - 1;
                process_count++;
            }
            
            return nodes_traversed;
        }
        
        current = current->next;
    }
    
    return -1;  // No suitable block found
}

int FirstFitMemory::deallocate_mem(int process_id) {
    if (process_map[process_id] == -1) {
        return -1;  // Process not found
    }
    
    // Find the block for this process
    MemoryBlock* current = head;
    int block_index = process_map[process_id];
    
    for (int i = 0; i < block_index && current != nullptr; i++) {
        current = current->next;
    }
    
    if (current == nullptr) {
        return -1;  // Block not found
    }
    
    current->process_id = -1;  // Mark as free
    process_map[process_id] = -1;
    process_count--;
    
    // Merge adjacent free blocks
    merge_adjacent_free_blocks();
    
    return 1;  // Success
}

int FirstFitMemory::fragment_count() {
    int fragments = 0;
    MemoryBlock* current = head;
    
    while (current != nullptr) {
        if (current->process_id == -1 && (current->size == 1 || current->size == 2)) {
            fragments++;
        }
        current = current->next;
    }
    
    return fragments;
}

// ============================================================================
// BEST FIT MEMORY IMPLEMENTATION
// ============================================================================

BestFitMemory::BestFitMemory() {
    // Initialize with one free block covering entire memory
    head = new MemoryBlock(0, TOTAL_MEMORY_UNITS);
    process_count = 0;
    memset(process_map, -1, sizeof(process_map));
}

BestFitMemory::~BestFitMemory() {
    cleanup_memory();
}

int BestFitMemory::allocate_mem(int process_id, int num_units) {
    if (num_units < 3 || num_units > 10) {
        return -1;  // Invalid request size
    }
    
    MemoryBlock* current = head;
    MemoryBlock* best_fit = nullptr;
    int best_fit_size = TOTAL_MEMORY_UNITS + 1;  // Initialize to larger than possible
    int nodes_traversed = 0;
    
    // Find the smallest free block that can accommodate the request
    while (current != nullptr) {
        nodes_traversed++;
        
        if (current->process_id == -1 && current->size >= num_units) {
            if (current->size < best_fit_size) {
                best_fit = current;
                best_fit_size = current->size;
            }
        }
        
        current = current->next;
    }
    
    if (best_fit == nullptr) {
        return -1;  // No suitable block found
    }
    
    // Allocate to the best fit block
    if (best_fit->size == num_units) {
        // Exact fit - allocate entire block
        best_fit->process_id = process_id;
        process_map[process_id] = nodes_traversed - 1;
        process_count++;
    } else {
        // Split the block
        MemoryBlock* new_block = new MemoryBlock(
            best_fit->start_address + num_units, 
            best_fit->size - num_units
        );
        
        best_fit->size = num_units;
        best_fit->process_id = process_id;
        new_block->next = best_fit->next;
        best_fit->next = new_block;
        
        process_map[process_id] = nodes_traversed - 1;
        process_count++;
    }
    
    return nodes_traversed;
}

int BestFitMemory::deallocate_mem(int process_id) {
    if (process_map[process_id] == -1) {
        return -1;  // Process not found
    }
    
    // Find the block for this process
    MemoryBlock* current = head;
    int block_index = process_map[process_id];
    
    for (int i = 0; i < block_index && current != nullptr; i++) {
        current = current->next;
    }
    
    if (current == nullptr) {
        return -1;  // Block not found
    }
    
    current->process_id = -1;  // Mark as free
    process_map[process_id] = -1;
    process_count--;
    
    // Merge adjacent free blocks
    merge_adjacent_free_blocks();
    
    return 1;  // Success
}

int BestFitMemory::fragment_count() {
    int fragments = 0;
    MemoryBlock* current = head;
    
    while (current != nullptr) {
        if (current->process_id == -1 && (current->size == 1 || current->size == 2)) {
            fragments++;
        }
        current = current->next;
    }
    
    return fragments;
}
