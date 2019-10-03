/**************************************************************************
*
* Copyright (C) 2012 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************/
#ifndef BACNET_H
#define BACNET_H

/** @file bacnet.h  This file is designed to reference the entire BACnet stack library */

/* core files */
#include "version.h"
#include "config.h"
#include "address.h"
#include "apdu.h"
#include "bacapp.h"
#include "bacdcode.h"
#include "bacint.h"
#include "bacreal.h"
#include "bacstr.h"
#include "bacdef.h"
#include "bacenum.h"
#include "bacerror.h"
#include "bactext.h"
#include "datalink.h"
#include "indtext.h"
#include "npdu.h"
#include "reject.h"
#include "tsm.h"

/* services */
#include "arf.h"
#include "awf.h"
#include "cov.h"
#include "dcc.h"
#include "iam.h"
#include "ihave.h"
#include "rd.h"
#include "rp.h"
#include "rpm.h"
#include "timesync.h"
#include "whohas.h"
#include "whois.h"
#include "wp.h"
#include "event.h"
#include "lso.h"
#include "alarm_ack.h"

/* required object - note: developer must supply the device.c file
   since it is not included in the library.  However, the library
   references the device.c members via the device.h API. */
#include "device.h"

/* demo objects */
#include "ai.h"
#include "ao.h"
#include "av.h"
#include "bacfile.h"
#include "bi.h"
#include "bo.h"
#include "bv.h"
#include "lc.h"
#include "lsp.h"
#include "mso.h"

/* demo handlers */
#include "txbuf.h"
#include "client.h"
#include "handlers.h"


#endif
