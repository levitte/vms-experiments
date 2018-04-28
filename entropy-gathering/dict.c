#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"

#include <descrip.h>
#include <dvidef.h>
#include <jpidef.h>
#include <rmidef.h>
#include <syidef.h>
#include <stsdef.h>
#include <ssdef.h>
#include <starlet.h>
#include <efndef.h>
#include <iosbdef.h>
#include <lib$routines.h>

typedef struct desc_st {
  int code;
  int length;
  const char name[40];
  int istime;
} DESC;

#define desc_number(name, words) \
  { (name), (words) * 4, #name, 0 }
#define desc_time(name) \
  { (name), 8, #name, 1 }

static DESC DVI_dict[] = {
  desc_number(DVI$_ERRCNT,		1),
  desc_number(DVI$_REFCNT,		1),
};
#define DVI_num (sizeof(DVI_dict)/sizeof(DVI_dict[0]))
static DESC JPI_dict[] = {
  desc_number(JPI$_BUFIO,		1),
  desc_number(JPI$_CPUTIM,		1),
  desc_number(JPI$_DIRIO,		1),
  desc_number(JPI$_IMAGECOUNT,		1),
  desc_time(JPI$_LAST_LOGIN_I),
  desc_time(JPI$_LOGINTIM),
  desc_number(JPI$_PAGEFLTS,		1),
  desc_number(JPI$_PID,			1),
  desc_number(JPI$_PPGCNT,		1),
  desc_number(JPI$_WSPEAK,		1),
  desc_number(JPI$_FINALEXC,		4),
};
#define JPI_num (sizeof(JPI_dict)/sizeof(JPI_dict[0]))
static DESC RMI_dict[] = {
  desc_number(RMI$_COLPG,		1),
  desc_number(RMI$_MWAIT,		1),
  desc_number(RMI$_CEF,			1),
  desc_number(RMI$_PFW,			1),
  desc_number(RMI$_LEF,			1),
  desc_number(RMI$_LEFO,		1),
  desc_number(RMI$_HIB,			1),
  desc_number(RMI$_HIBO,		1),
  desc_number(RMI$_SUSP,		1),
  desc_number(RMI$_SUSPO,		1),
  desc_number(RMI$_FPG,			1),
  desc_number(RMI$_COM,			1),
  desc_number(RMI$_COMO,		1),
  desc_number(RMI$_CUR,			1),
#if defined __alpha
  desc_number(RMI$_FRLIST,		1),
  desc_number(RMI$_MODLIST,		1),
#elif defined __ia64
  desc_number(RMI$_FRLIST,		2),
  desc_number(RMI$_MODLIST,		2),
#endif
  desc_number(RMI$_FAULTS,		1),
  desc_number(RMI$_PREADS,		1),
  desc_number(RMI$_PWRITES,		1),
  desc_number(RMI$_PWRITIO,		1),
  desc_number(RMI$_PREADIO,		1),
  desc_number(RMI$_GVALFLTS,		1),
  desc_number(RMI$_WRTINPROG,		1),
  desc_number(RMI$_FREFLTS,		1),
  desc_number(RMI$_DZROFLTS,		1),
  desc_number(RMI$_SYSFAULTS,		1),
  desc_number(RMI$_ISWPCNT,		1),
  desc_number(RMI$_DIRIO,		1),
  desc_number(RMI$_BUFIO,		1),
  desc_number(RMI$_MBREADS,		1),
  desc_number(RMI$_MBWRITES,		1),
  desc_number(RMI$_LOGNAM,		1),
  desc_number(RMI$_FCPCALLS,		1),
  desc_number(RMI$_FCPREAD,		1),
  desc_number(RMI$_FCPWRITE,		1),
  desc_number(RMI$_FCPCACHE,		1),
  desc_number(RMI$_FCPCPU,		1),
  desc_number(RMI$_FCPHIT,		1),
  desc_number(RMI$_FCPSPLIT,		1),
  desc_number(RMI$_FCPFAULT,		1),
  desc_number(RMI$_ENQNEW,		1),
  desc_number(RMI$_ENQCVT,		1),
  desc_number(RMI$_DEQ,			1),
  desc_number(RMI$_BLKAST,		1),
  desc_number(RMI$_ENQWAIT,		1),
  desc_number(RMI$_ENQNOTQD,		1),
  desc_number(RMI$_DLCKSRCH,		1),
  desc_number(RMI$_DLCKFND,		1),
  desc_number(RMI$_NUMLOCKS,		1),
  desc_number(RMI$_NUMRES,		1),
  desc_number(RMI$_ARRLOCPK,		1),
  desc_number(RMI$_DEPLOCPK,		1),
  desc_number(RMI$_ARRTRAPK,		1),
  desc_number(RMI$_TRCNGLOS,		1),
  desc_number(RMI$_RCVBUFFL,		1),
  desc_number(RMI$_ENQNEWLOC,		1),
  desc_number(RMI$_ENQNEWIN,		1),
  desc_number(RMI$_ENQNEWOUT,		1),
  desc_number(RMI$_ENQCVTLOC,		1),
  desc_number(RMI$_ENQCVTIN,		1),
  desc_number(RMI$_ENQCVTOUT,		1),
  desc_number(RMI$_DEQLOC,		1),
  desc_number(RMI$_DEQIN,		1),
  desc_number(RMI$_DEQOUT,		1),
  desc_number(RMI$_BLKLOC,		1),
  desc_number(RMI$_BLKIN,		1),
  desc_number(RMI$_BLKOUT,		1),
  desc_number(RMI$_DIRIN,		1),
  desc_number(RMI$_DIROUT,		1),
#if 0
  desc_number(RMI$_MSCP_EVERYTHING,    35),
  desc_number(RMI$_DDTM_ALL,	       38),
  desc_number(RMI$_TMSCP_EVERYTHING,   20),
#endif
  desc_number(RMI$_LCKMGR_REQCNT,	2),
  desc_number(RMI$_LCKMGR_REQTIME,	2),
  desc_number(RMI$_LCKMGR_SPINCNT,	2),
  desc_number(RMI$_LCKMGR_SPINTIME,	2),
  desc_number(RMI$_LPZ_PAGCNT,		1),
  desc_number(RMI$_LPZ_HITS,		1),
  desc_number(RMI$_LPZ_MISSES,		1),
  desc_number(RMI$_LPZ_EXPCNT,		1),
  desc_number(RMI$_LPZ_ALLOCF,		1),
  desc_number(RMI$_LPZ_ALLOC2,		1),
  desc_number(RMI$_ACCESS,		1),
  desc_number(RMI$_ALLOC,		1),
  desc_number(RMI$_FCPCREATE,		1),
  desc_number(RMI$_VOLWAIT,		1),
  desc_number(RMI$_FCPTURN,		1),
  desc_number(RMI$_FCPERASE,		1),
  desc_number(RMI$_OPENS,		1),
  desc_number(RMI$_FIDHIT,		1),
  desc_number(RMI$_FIDMISS,		1),
  desc_number(RMI$_FILHDR_HIT,		1),
  desc_number(RMI$_DIRFCB_HIT,		1),
  desc_number(RMI$_DIRFCB_MISS,		1),
  desc_number(RMI$_DIRDATA_HIT,		1),
  desc_number(RMI$_EXTHIT,		1),
  desc_number(RMI$_EXTMISS,		1),
  desc_number(RMI$_QUOHIT,		1),
  desc_number(RMI$_QUOMISS,		1),
  desc_number(RMI$_STORAGMAP_HIT,	1),
  desc_number(RMI$_VOLLCK,		1),
  desc_number(RMI$_SYNCHLCK,		1),
  desc_number(RMI$_SYNCHWAIT,		1),
  desc_number(RMI$_ACCLCK,		1),
  desc_number(RMI$_XQPCACHEWAIT,	1),
  desc_number(RMI$_DIRDATA_MISS,	1),
  desc_number(RMI$_FILHDR_MISS,		1),
  desc_number(RMI$_STORAGMAP_MISS,	1),
  desc_number(RMI$_CPUINTSTK,		2),
  desc_number(RMI$_CPUMPSYNCH,		2),
  desc_number(RMI$_CPUKERNEL,		2),
  desc_number(RMI$_CPUEXEC,		2),
  desc_number(RMI$_CPUSUPER,		2),
  desc_number(RMI$_CPUUSER,		2),
  desc_number(RMI$_PROCCNTMAX,		1),
  desc_number(RMI$_PROCBATCNT,		1),
  desc_number(RMI$_PROCINTCNT,		1),
  desc_number(RMI$_PROCNETCNT,		1),
  desc_number(RMI$_PROCSWITCHCNT,	1),
  desc_number(RMI$_PROCBALSETCNT,	1),
  desc_number(RMI$_PROCLOADCNT,		1),
  desc_number(RMI$_BADFLTS,		1),
  desc_number(RMI$_EXEFAULTS,		1),
  desc_number(RMI$_HDRINSWAPS,		1),
  desc_number(RMI$_HDROUTSWAPS,		1),
  desc_number(RMI$_IOPAGCNT,		1),
  desc_number(RMI$_ISWPCNTPG,		1),
  desc_number(RMI$_OSWPCNT,		1),
  desc_number(RMI$_OSWPCNTPG,		1),
  desc_number(RMI$_RDFAULTS,		1),
  desc_number(RMI$_TRANSFLTS,		1),
  desc_number(RMI$_WRTFAULTS,		1),
#if defined __alpha
  desc_number(RMI$_USERPAGES,		1),
#elif defined __ia64
  desc_number(RMI$_USERPAGES,		2),
#endif
  desc_number(RMI$_VMSPAGES,		1),
  desc_number(RMI$_TTWRITES,		1),
  desc_number(RMI$_BUFOBJPAG,		1),
  desc_number(RMI$_BUFOBJPAGPEAK,	1),
  desc_number(RMI$_BUFOBJPAGS01,	1),
  desc_number(RMI$_BUFOBJPAGS2,		1),
  desc_number(RMI$_BUFOBJPAGMAXS01,	1),
  desc_number(RMI$_BUFOBJPAGMAXS2,	1),
  desc_number(RMI$_BUFOBJPAGPEAKS01,	1),
  desc_number(RMI$_BUFOBJPAGPEAKS2,	1),
  desc_number(RMI$_BUFOBJPGLTMAXS01,	1),
  desc_number(RMI$_BUFOBJPGLTMAXS2,	1),
  desc_number(RMI$_DLCK_INCMPLT,	1),
  desc_number(RMI$_DLCKMSGS_IN,		1),
  desc_number(RMI$_DLCKMSGS_OUT,	1),
  desc_number(RMI$_TQETOTAL,		2),
  desc_number(RMI$_TQESYSUB,		2),
  desc_number(RMI$_TQEUSRTIMR,		2),
  desc_number(RMI$_TQEUSRWAKE,		2),
  desc_number(RMI$_MCHKERRS,		1),
  desc_number(RMI$_MEMERRS,		1),
};
#define RMI_num (sizeof(RMI_dict)/sizeof(RMI_dict[0]))
static DESC SYI_dict[] = {
  desc_number(SYI$_PAGEFILE_FREE,	1),
};
#define SYI_num (sizeof(SYI_dict)/sizeof(SYI_dict[0]))

/* These maps get filled in run-time, then sorted by code */
static struct map_st {
  int code;
  DESC *desc;
} DVI_map[DVI_num],
  JPI_map[JPI_num],
  RMI_map[RMI_num],
  SYI_map[SYI_num];

/* These lists get filled in run-time */
static ile3 DVI_ile3[DVI_num + 1],
  JPI_ile3[JPI_num + 1],
  RMI_ile3[RMI_num + 1],
  SYI_ile3[SYI_num + 1];

/* List of dictionaries with associated stuff */
static struct {
  DESC *dict;
  size_t dict_members;
  ile3 *items;
  struct map_st *code2desc_map;
} dicts[] = {
  /* The position of each row MUST correspond to the dictnum given in dict.h */
  { DVI_dict, DVI_num, DVI_ile3, DVI_map},
  { JPI_dict, JPI_num, JPI_ile3, JPI_map },
  { RMI_dict, RMI_num, RMI_ile3, RMI_map },
  { SYI_dict, SYI_num, SYI_ile3, SYI_map },
};

/*
 * We store everything as 32-bit words, even though some results are 64
 * bits in size.  The latter will simply occupy two slots in this array.
 */
static uint32_t *data = NULL;
/* Size of |data|, measured in sizeof(uint32_t) */
static size_t data_sz = 0;

static void cleanup_items(void)
{
  free(data);
}

int comp(const void *a, const void *b)
{
  return ((struct map_st *)a)->code - ((struct map_st *)b)->code;
}
void init_items()
{
  /*
   * Go through all item lists and construct an array of uint32_t big enough
   * to hold all the data.  ONLY if data isn't already created.
   */
  if (data == NULL) {
    size_t i, j, pos;

    atexit(cleanup_items);

    /* First, figure out the total data and item list size of all items */
    data_sz = 0;
    for (i = 0; i < sizeof(dicts) / sizeof(dicts[0]); i++) {
      for (j = 0; j < dicts[i].dict_members; j++)
	data_sz += dicts[i].dict[j].length;
    }

    /* Allocate a buffer to take all the values */
    assert((data = malloc(data_sz * sizeof(uint32_t))) != NULL);

    /* Now, edit the dicts to point into data */
    pos = 0;
    for (i = 0; i < sizeof(dicts) / sizeof(dicts[0]); i++) {
      for (j = 0; j < dicts[i].dict_members; j++) {
	dicts[i].items[j].ile3$w_code = dicts[i].dict[j].code;
	dicts[i].items[j].ile3$w_length = dicts[i].dict[j].length;
	dicts[i].items[j].ile3$ps_bufaddr = &data[pos];
	/* We aren't interested in a size we already know */
	dicts[i].items[j].ile3$ps_retlen_addr = NULL;

	pos += dicts[i].items[j].ile3$w_length / sizeof(uint32_t);

	dicts[i].code2desc_map[j].code = dicts[i].dict[j].code;
	dicts[i].code2desc_map[j].desc = &dicts[i].dict[j];
      }
      /* "NULL" entry at the end */
      dicts[i].items[j].ile3$w_code = 0;
      dicts[i].items[j].ile3$w_length = 0;
      dicts[i].items[j].ile3$ps_bufaddr = NULL;
      dicts[i].items[j].ile3$ps_retlen_addr = NULL;

      qsort(dicts[i].code2desc_map,
	    dicts[i].dict_members,
	    sizeof(dicts[i].code2desc_map[0]),
	    comp);
    }
  }
}

size_t num_items(int dictnum)
{
  if (dictnum < 0 && dictnum >= sizeof(dicts))
    return 0;
  init_items();
  return dicts[dictnum].dict_members;
}

ile3 *items(int dictnum)
{
  if (dictnum < 0 && dictnum >= sizeof(dicts))
    return 0;
  init_items();
  return dicts[dictnum].items;
}

static const DESC *find_desc(int dictnum, uint16_t code)
{
  int i, bot, top;

  if (dictnum < 0 && dictnum >= sizeof(dicts))
    return 0;
  init_items();

  bot = 0;
  top = dicts[dictnum].dict_members;
  while (top > bot) {
    i = (bot + top) / 2;
    if (code == dicts[dictnum].code2desc_map[i].code)
      break;
    if (code > dicts[dictnum].code2desc_map[i].code)
      bot = i;
    else
      top = i;
  }
  if (code == dicts[dictnum].code2desc_map[i].code)
    return dicts[dictnum].code2desc_map[i].desc;
  return NULL;
}

const char *item_name(int dictnum, uint16_t code)
{
  const DESC *desc = find_desc(dictnum, code);

  if (desc)
    return desc->name;
  return NULL;
}

int item_istime(int dictnum, uint16_t code)
{
  const DESC *desc = find_desc(dictnum, code);

  if (desc)
    return desc->istime;
  return 0;
}

int gather_system_data(void)
{
#define errchk_ret(arg) if (!($VMS_STATUS_SUCCESS((arg)))) return (arg)
  uint32_t status;
  uint32_t efn;
  iosb iosb;
  const $DESCRIPTOR(SYSDEVICE,"SYS$SYSDEVICE:");

  memset(&iosb, 0, sizeof(iosb));
#ifdef DEBUG
  fprintf(stderr, "DEBUG: gathering DVI items\n");
#endif
  status =
    sys$getdviw(EFN$C_ENF, 0, &SYSDEVICE, items(dictnum_dvi), 0, 0, 0, 0, 0);
  errchk_ret(status);
#ifdef DEBUG
  fprintf(stderr, "DEBUG: gathering JPI items\n");
#endif
  status = sys$getjpiw(EFN$C_ENF, 0, 0, items(dictnum_jpi), 0, 0, 0);
  errchk_ret(status);
#ifdef DEBUG
  fprintf(stderr, "DEBUG: gathering SYI items\n");
#endif
  status = sys$getsyiw(EFN$C_ENF, 0, 0, items(dictnum_syi), 0, 0, 0);
  errchk_ret(status);
  /*
   * The RMI service is a bit special, as there is no synchronous variant,
   * so we MUST create an event flag to synchronise on.
   */
#ifdef DEBUG
  fprintf(stderr, "DEBUG: gathering RMI items\n");
#endif
  status = lib$get_ef(&efn);
  errchk_ret(status);
  status = sys$getrmi(efn, 0, 0, items(dictnum_rmi), &iosb, 0, 0);
  errchk_ret(status);
  status = sys$synch(efn, &iosb);
  errchk_ret(status);
  errchk_ret(iosb.iosb$l_getxxi_status);
  status = lib$free_ef(&efn);
  errchk_ret(status);

#ifdef DEBUG
  fprintf(stderr, "DEBUG: done gathering (total %lu bytes)\n", data_sz);
#endif
  /*
   * Special handling of JPI$_FINALEXC
   * The value received is the address to a list of 4 32-bit words
   * We know that we reserved 4 slots for this, so simply replace the
   * address and the next 3 slots with those words.
   */
  {
    ile3 *item;

    for(item = items(dictnum_jpi); item->ile3$w_code != 0; item++)
      if (item->ile3$w_code == JPI$_FINALEXC) {
	uint32_t *data = item->ile3$ps_bufaddr;
	uint32_t *ptr = (uint32_t *)*data;
	int j;

	for (j = 0; j < 4; j++)
	  data[j] = ptr[j];

	break;
      }
  }
  return SS$_NORMAL;
}

