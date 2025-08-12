#ifndef STATS_H
#define STATS_H

#include <string>

// Statistics tracking and reporting component
class StatisticsComponent {
public:
    StatisticsComponent();
    ~StatisticsComponent();
    
    // Update statistics after each allocation/deallocation request
    void update_allocation_stats(int nodes_traversed, bool allocation_successful);
    void update_fragment_stats(int fragment_count);
    
    // Get current performance metrics
    double get_average_fragments() const;
    double get_average_allocation_time() const;
    double get_allocation_denial_percentage() const;
    
    // Print final statistics
    void print_statistics(const std::string& technique_name) const;
    
    // Reset statistics for new simulation run
    void reset();

private:
    int total_allocations;
    int successful_allocations;
    int failed_allocations;
    long total_nodes_traversed;
    long total_fragments;
    int request_count;
};

#endif // STATS_H
