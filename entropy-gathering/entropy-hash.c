/*
 * An experiment to show how much entropy one can gather from diverse
 * system and process counters by hashing them all together with SHA256.
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

#include <openssl/evp.h>

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

void final_entropy()
{
  if (total_bits)
    printf("Total bits / entropy: %d / %d (ratio = %f)\n",
	   total_bits, total_entropy, (double)total_entropy / total_bits);
  else
    printf("No data gathered yet\n");
}
void sigint(int x)
{
  exit(0);
}
void test_entropy(FILE *binout, size_t maxbytes)
{
  /* Flag for the first run... */
  int first = 1;
  const EVP_MD *md = EVP_sha512();
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  uint32_t hash[16];	/* 512 bits */
  size_t bytes = 0;

  init_entropy_state(sizeof(hash) / sizeof(hash[0]));

  atexit(final_entropy);
  signal(SIGINT, sigint);

  do {
    size_t i, j;
    size_t round_entropy = 0;
    size_t round_bits = 0;
    uint32_t status = gather_system_data();
    if (!($VMS_STATUS_SUCCESS(status)))
      lib$signal(status);

    EVP_MD_CTX_reset(ctx);
    EVP_DigestInit(ctx, md);

    for (i = 0; i < dicts(); i++) {
      for (j = 0; j < num_items(i); j++) {
	void *b = items(i)[j].ile3$ps_bufaddr;
	int l = items(i)[j].ile3$w_length;
	EVP_DigestUpdate(ctx, b, l);
      }
    }

    EVP_DigestFinal(ctx, (unsigned char *)hash, NULL);

    do {
      size_t entropy = 0;
      size_t k, pos;
      char *hexbuf;

      for (k = 0, pos = 0; k < sizeof(hash) / sizeof(hash[0]); k++) {
	entropy += calculate_entropy(hash[k], pos++);
      }
      bytes += fwrite(hash, sizeof(hash), 1, binout) * sizeof(hash);

      /*
       * There's no point in using the entropy on the first run,
       * as there isn't enough saved state.
       */
      if (first)
	continue;

      /* We don't care about zero entropy items */
      if (entropy == 0)
	continue;

      round_entropy += entropy;
      round_bits += sizeof(hash) * 8;

      hexbuf = OPENSSL_buf2hexstr((unsigned char *)hash, sizeof(hash));
      printf("%s: (%d)\n", hexbuf, entropy);
    } while(0);

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
#ifdef DEBUG
      fprintf(stderr, "DEBUG: sleep %d seconds\n", sleeptime);
#endif
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
  binout = fopen("entropy-hash.bin", "w", "ctx=bin");
  test_entropy(binout, 1024 * 1024);
  fclose(binout);
}

