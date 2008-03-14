/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef WaitCursorH
 #define WaitCursorH
class TWaitCursor
{

   public:
      TWaitCursor() : oldc( Screen->Cursor )
      {
         Screen->Cursor = crHourGlass;
      }
      ~TWaitCursor()
      {
         Screen->Cursor = oldc;
      }
   private:
      TCursor oldc;
};
#endif
