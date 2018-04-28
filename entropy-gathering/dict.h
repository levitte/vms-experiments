#include <inttypes.h>
#include <iledef.h>

#define dicts() (4)
enum {
  dictnum_dvi = 0,
  dictnum_jpi = 1,
  dictnum_rmi = 2,
  dictnum_syi = 3,
};

size_t num_items(int dictnum);
ile3 *items(int dictnum);
const char *item_name(int dictnum, uint16_t code);
int item_istime(int dictnum, uint16_t code);

/*
 * Use system services to gather all the data we want.  Nothing happens
 * unless |data| has been created.
 *
 * Return VMS status SS$_NORMAL when everything went well, SS$_NODATA if
 * |data| hasn't been created yet, or whatever other status the called
 * system services returned.
 */
int gather_system_data(void);
