#include <stdlib.h>
#include "items-dict.h"

static int sorted = 0;
int comp(const void *a, const void *b)
{
  return ((DESC *)a)->code - ((DESC *)b)->code;
}

const DESC *dict_desc(int dictnum, uint16_t code)
{
  int i, bot, top;

  if (!sorted) {
    for (i = 0; i < sizeof(dicts) / sizeof(dicts[0]); i++)
      qsort(dicts[i].dict, dicts[i].dict_sz / dicts[i].dict_membsz,
	    dicts[i].dict_membsz, comp);
  }

  bot = 0;
  top = dicts[dictnum].dict_sz / dicts[dictnum].dict_membsz;
  while (top > bot) {
    i = (bot + top) / 2;
    if (code == dicts[dictnum].dict[i].code)
      break;
    if (code > dicts[dictnum].dict[i].code)
      bot = i;
    else
      top = i;
  }
  return &dicts[dictnum].dict[i];
}
