/*
 * An experiment to show how much entropy one can gather from diverse
 * system and process counters.
 *
 * (with gratefulness to the folks on comp.os.vms, who kindly fed me ideas)
 */

/*
 * The idea is to pick all the data into an internal state and measure how
 * how much they change over small periods of time.  A third order delta is
 * calculated for every iteration, and the position of the highest set bit
 * from that delta will be used as entropy estimate.  Items with no entropy
 * (i.e. third order delta == 0) will not be displayed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <assert.h>

/* VMSy stuff */
#ifdef __VMS
# include <descrip.h>
# include <stsdef.h>
# include <ssdef.h>
# include <starlet.h>
# include <lib$routines.h>
# ifdef __DECC
#  pragma message disable DOLLARID
# endif

# include "dict.h"
# include "entropy.h"

size_t total_bits = 0;
size_t total_entropy = 0;
size_t total_rounds = 0;
size_t data_sz = 0;

void final_entropy()
{
  if (total_bits)
    printf("Total bits / entropy: %d / %d (ratio = %f)\n",
	   total_bits, total_entropy, (double)total_entropy / total_bits);
  else
    printf("No data gathered yet\n");
  printf("Data was gathered in %d byte chunks\n", data_sz * sizeof(uint32_t));
}
void sigint(int x)
{
  exit(0);
}
void test_entropy(FILE *binout, size_t maxbytes)
{
  /* Flag for the first run... */
  int first = 1;
  size_t bytes = 0;

  {
    size_t i, j;

    /* Calculate total data size */
    for (i = 0; i < dicts(); i++) {
      ile3 *itms = items(i);
      size_t num_itms = num_items(i);

      for (j = 0; j < num_itms; j++)
	data_sz += itms[j].ile3$w_length / sizeof(uint32_t);
    }
    init_entropy_state(data_sz);
  }

  atexit(final_entropy);
  signal(SIGINT, sigint);
  do {
    size_t round_entropy = 0;
    size_t round_bits = 0;
    size_t pos = 0;
    size_t i, j;
    uint32_t status = gather_system_data();
    if (!($VMS_STATUS_SUCCESS(status)))
      lib$signal(status);

    for (i = 0; i < dicts(); i++) {
      for (j = 0; j < num_items(i); j++) {
	size_t sz = items(i)[j].ile3$w_length / sizeof(uint32_t);
	size_t entropy = 0;
	size_t k;
	char buffer[1024];

	for (k = 0; k < sz; k++) {
	  entropy +=
	    calculate_entropy(((uint32_t *)(items(i)[j].ile3$ps_bufaddr))[k],
			      pos++);
	}
	bytes += fwrite(items(i)[j].ile3$ps_bufaddr,
			items(i)[j].ile3$w_length, 1,
			binout)
	  * items(i)[j].ile3$w_length;

	/* We must count all bits for fair measurements */
	round_bits += items(i)[j].ile3$w_length * 8;
	round_entropy += entropy;

	/*
	 * There's no point in using the entropy on the first run,
	 * as there isn't enough saved state.
	 */
	if (first)
	  continue;

	/* We don't care to display zero entropy items */
	if (entropy == 0)
	  continue;

	if (item_istime(i, items(i)[j].ile3$w_code)) {
	  size_t timlen = 0;

	  $DESCRIPTOR(timbuf, buffer);
	  sys$asctim(&timlen, &timbuf, items(i)[j].ile3$ps_bufaddr, 0);
	  buffer[timlen] = '\0';
	} else {
	  size_t buflen = 0;

	  if (sz > 2) {
	    strcpy(buffer, "\n  ");
	    buflen = strlen(buffer);
	  }
	  for (k = 0; k < sz; k++) {
	    size_t itemlen = 0;

	    if (k > 0) {
	      if (k % 4 == 0) {
		strcpy(buffer + buflen, ",\n  ");
		buflen += strlen(buffer + buflen);
	      } else {
		strcpy(buffer + buflen, ", ");
		buflen += strlen(buffer + buflen);
	      }
	    }
	    itemlen = snprintf(buffer + buflen, sizeof(buffer) - buflen,
			       "0x%08X",
			       ((uint32_t *)(items(i)[j].ile3$ps_bufaddr))[k]);
	    buflen += itemlen;
	  }
	  buffer[buflen] = '\0';
	}
	printf("%s: (%d) ", item_name(i, items(i)[j].ile3$w_code), entropy);
	puts(buffer);
      }
    }
    first = 0;
    total_rounds++;
    total_bits += round_bits;
    total_entropy += round_entropy;
    if (round_bits)
      printf("Total bits / entropy this round: %d / %d (ratio = %f)\n",
	     round_bits, round_entropy, (double)round_entropy / round_bits);
    else
      printf("No data gathered this round\n");

    printf("%lu bytes written to file so far\n", bytes);
    if (bytes >= maxbytes)
      break;
    printf("%lu bytes still needed\n", maxbytes - bytes);

    {
      int sleeptime = rand() % 10 + 1;
      printf("DEBUG: sleep %d seconds\n", sleeptime);
      sleep(sleeptime);
    }
  } while (1);
}
#endif

/********************************************************************/
int main()
{
  FILE *binout = NULL;

  srand((unsigned int)time(NULL));
  binout = fopen("entropy-stats.bin", "w", "ctx=bin");
  test_entropy(binout, 1024 * 1024);
  fclose(binout);
}

