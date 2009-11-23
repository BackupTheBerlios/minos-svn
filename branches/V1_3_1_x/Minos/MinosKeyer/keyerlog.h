/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _KEYERLOG_H_
#define _KEYERLOG_H_
#include <fstream.h>

extern unsigned long MORSEINTCOUNT;
extern unsigned long currTick;
extern unsigned long basetick;

#define TARGET_RESOLUTION 55U         // 55-millisecond target resolution
#define TIMER_INTERVAL 55U         // 55-millisecond target interval
extern bool sblog;
extern bool sbintrlog;
#endif
