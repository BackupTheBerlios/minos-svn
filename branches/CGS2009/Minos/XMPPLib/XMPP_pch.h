/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef XMPP_pchH
#define XMPP_pchH

#define WIN32_LEAN_AND_MEAN

#include "vcl.h"
#include <except.h>
#include <winsock2.h>
#include <sysutils.hpp>

#include <deque>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>

#include <fstream>

#include <process.h>
#include <stdio.h>
#include "boost\format.hpp"
#include "boost\shared_ptr.hpp"

#include "tinyxml.h"
#include "tinyutils.h"

#include "UTF8Conversion.h"
#include "stringcast.h"

#include "RCVersion.h"
#include "WSAGuard.h"

#include "GJVThreads.h"
#include "LogEvents.h"
#include "MTrace.h"
#include "MLogFile.h"

#include "PortIds.h"

#include "ServerEvent.h"
#include "XMPPEvents.h"

#include "Dispatcher.h"

#include "XMPPRPCParams.h"
#include "XMPPStanzas.h"
#include "XMPPRPCObj.h"
#include "RPCPubSub.h"
#include "PubSubClient.h"

#endif
