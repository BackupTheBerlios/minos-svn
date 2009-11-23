/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef FindUSBComPortH
#define FindUSBComPortH 
//---------------------------------------------------------------------------

#define PIBUBW       "SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_04d8&Pid_000a\\"
bool FindUSBComPort( String manKey, String &port );
#endif
