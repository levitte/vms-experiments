#include <inttypes.h>

/* size is number of uint32_t */
void init_entropy_state(size_t size);
/* Returns the estimated entropy for an item in slot |index| */
size_t calculate_entropy(uint32_t data, size_t index);
