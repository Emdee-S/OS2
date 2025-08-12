#include "stats.h"

StatisticsComponent::StatisticsComponent() {
}

StatisticsComponent::~StatisticsComponent() {
}

void StatisticsComponent::update_allocation_stats(int nodes_traversed, bool allocation_successful) {
}

void StatisticsComponent::update_fragment_stats(int fragment_count) {
}

double StatisticsComponent::get_average_fragments() const {
    return 0.0;
}

double StatisticsComponent::get_average_allocation_time() const {
    return 0.0;
}

double StatisticsComponent::get_allocation_denial_percentage() const {
    return 0.0;
}

void StatisticsComponent::print_statistics(const std::string& technique_name) const {
}

void StatisticsComponent::reset() {
}
