/*
	Derived from BCB sample code
*/
//---------------------------------------------------------------------------


#pragma hdrstop

#include "smpreg.h"

//---------------------------------------------------------------------------

#include "cspin.h"
#include "ccalendr.h"


namespace Smpreg
{
   void __fastcall PACKAGE Register()
   {
	 TComponentClass classes[3] = {__classid(TCSpinButton), __classid(TCSpinEdit),
								   __classid(TCCalendar)};

	 ActivateClassGroup(__classid(Controls::TControl));

	 GroupDescendentsWith(__classid(TCSpinButton), __classid(Controls::TControl));
	 GroupDescendentsWith(__classid(TCSpinEdit), __classid(Controls::TControl));
	 GroupDescendentsWith(__classid(TCCalendar), __classid(Controls::TControl));

	 RegisterComponents("MJGAddons", classes, (sizeof(classes)/sizeof(classes[0])) - 1);
  }
}




#pragma package(smart_init)
