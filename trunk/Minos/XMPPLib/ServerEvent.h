/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

#ifndef ServerEventH
#define ServerEventH 
//---------------------------------------------------------------------------
extern std::string makeUuid();
extern std::string getServerId();
extern HANDLE makeServerEvent( bool create );
extern bool checkServerReady();
extern HANDLE makeServerShowEvent( );
extern bool getShowServers();
extern void setShowServers(bool state);
#endif

