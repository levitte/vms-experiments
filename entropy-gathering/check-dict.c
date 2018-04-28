#include <stdio.h>
#include <inttypes.h>

#include <descrip.h>
#include <stsdef.h>
#include <ssdef.h>
#include <starlet.h>
#include <efndef.h>
#include <iledef.h>
#include <iosbdef.h>
#include <lib$routines.h>
#include "dict.h"

#define errchk_ret(arg) if (!($VMS_STATUS_SUCCESS((arg)))) return (arg)

int main()
{
  int i, j;
  ile3 null_item;
  const $DESCRIPTOR(SYSDEVICE,"SYS$SYSDEVICE:");

  null_item.ile3$w_code = 0;
  null_item.ile3$w_length = 0;
  null_item.ile3$ps_bufaddr = NULL;
  null_item.ile3$ps_retlen_addr = NULL;

  for (i = 0; i < dicts(); i++)
    for (j = 0; j < num_items(i); j++) {
      uint32_t status;
      uint32_t efn;
      iosb iosb;
      ile3 itms[2];

      itms[0] = items(i)[j];
      itms[1] = null_item;

      switch(i) {
      case 0:			/* DVI */
	status = sys$getdviw(EFN$C_ENF, 0, &SYSDEVICE, itms, 0, 0, 0, 0, 0);
	break;
      case 1:			/* JPI */
	status = sys$getjpiw(EFN$C_ENF, 0, 0, itms, 0, 0, 0);
	break;
      case 2:			/* RMI */
	{
	  uint32_t status2;
	  /*
	   * The RMI service is a bit special, as there is no synchronous
	   * variant, so we MUST create an event flag to synchronise on.
	   */
	  status2 = lib$get_ef(&efn);
	  errchk_ret(status2);
	  status2 = sys$getrmi(EFN$C_ENF, 0, 0, itms, &iosb, 0, 0);
	  errchk_ret(status2);
	  status2 = sys$synch(efn, &iosb);
	  errchk_ret(status2);
	  status = iosb.iosb$l_getxxi_status;
	  status2 = lib$free_ef(&efn);
	  errchk_ret(status2);
	}
	break;
      case 3:			/* SYI */
	status = sys$getsyiw(EFN$C_ENF, 0, 0, itms, 0, 0, 0);
	break;
      }

#ifndef DEBUG
      if (!($VMS_STATUS_SUCCESS(status)))
#endif
	fprintf(stderr, "%s (%u) : %u\n",
		item_name(i, itms[0].ile3$w_code), itms[0].ile3$w_code,
		status);
    }
}
