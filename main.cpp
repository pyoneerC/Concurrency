#include <iostream>
#include <new>

int main() {
    constexpr std::size_t min_offset_to_avoid_false_sharing = std::hardware_destructive_interference_size;
    constexpr std::size_t max_offset_to_promote_true_sharing = std::hardware_constructive_interference_size;

    alignas(min_offset_to_avoid_false_sharing) int variable1 = 0;  // Aligned to avoid false sharing
    alignas(max_offset_to_promote_true_sharing) int variable2 = 0; // Aligned to promote true sharing

    std::cout << "Cache line size for destructive interference: " << min_offset_to_avoid_false_sharing << " bytes\n";
    std::cout << "Cache line size for constructive interference: " << max_offset_to_promote_true_sharing << " bytes\n";

    return 0;
}
