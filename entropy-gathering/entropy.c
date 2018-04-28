#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "entropy.h"

/*
 * Saved state from last run.  Initially, this will contain nothing.
 */
struct state_st {
  uint32_t value;		/* Saved value */
  int64_t deltas[2];		/* Saved 1st and 2nd order delta */
} *state = NULL;
size_t state_sz = 0;

void init_entropy_state(size_t size)
{
  if (state == NULL) {
    state_sz = size;
    assert((state = malloc(sizeof(*state) * state_sz)) != NULL);
    memset(state, 0, sizeof(*state) * state_sz);
  }
}

static int msb(uint32_t x);
size_t calculate_entropy(uint32_t data, size_t index)
{
  int64_t d1, d2, d3;

  d1 = data - state[index].value;
  state[index].value = data;
  d2 = d1 - state[index].deltas[0];
  state[index].deltas[0] = d1;
  d3 = d2 - state[index].deltas[1];
  state[index].deltas[1] = d2;

  /* Make sure the values are absolute */
  if (d1 < 0) d1 = -d1;
  if (d2 < 0) d2 = -d2;
  if (d3 < 0) d3 = -d3;

  /* Find the smallest delta, store it in d1 */
  if (d1 > d2) d1 = d2;
  if (d1 > d3) d1 = d3;

  /*
   * d1 now describes how much has changed, msb(d1) gives us an estimate
   * of the number of entropy bits delivered with the new value
   */
  return msb(d1);
}

/********************************************************************/

/* This is clz3 from https://en.wikipedia.org/wiki/Find_first_set#CLZ */
static int clz3(uint32_t x)
{
  int n = 0;

  if (x == 0)
    return 32;

  if ((x & 0xFFFF0000) == 0) { n = n + 16; x = x << 16; }
  if ((x & 0xFF000000) == 0) { n = n +  8; x = x <<  8; }
  if ((x & 0xF0000000) == 0) { n = n +  4; x = x <<  4; }
  if ((x & 0xC0000000) == 0) { n = n +  2; x = x <<  2; }
  if ((x & 0x80000000) == 0) { n = n +  1; }
  return n;
}

/* msb is simply the "opposite" value of clz3 */
static int msb(uint32_t x)
{
  return 32 - clz3(x);
}

#ifdef TEST
/********************************************************************/
int main()
{
  int i;

  for (i = 0; i < 32; i++) {
    uint32_t v1 = 1 << i;			   /* Simplest */
    uint32_t v0 = v1 - 1;
    uint32_t v2 = v1 | 1;			   /* Lowest always 1 */
    uint32_t v3 = v1 | (i > 0 ? 1 << (i - 1) : 0); /* Midway bit always 1 */
    uint32_t v4 = v2 | v3;			   /* Combine all */

    printf("highest bit position in %u is %d\n", v0, msb(v0));
    printf("highest bit position in %u is %d\n", v1, msb(v1));
    printf("highest bit position in %u is %d\n", v2, msb(v2));
    printf("highest bit position in %u is %d\n", v3, msb(v3));
    printf("highest bit position in %u is %d\n", v4, msb(v4));
  }
}
#endif
