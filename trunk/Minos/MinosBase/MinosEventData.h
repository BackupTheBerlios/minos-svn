/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2006 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#ifndef MinosEventDataH
#define MinosEventDataH
//---------------------------------------------------------------------------
enum MinosEventNumber
{
	EN_Unknown, // Keep this first

   EN_ContestPageChanged,
   EN_LogColumnsChanged,
   EN_SplittersChanged,
   EN_ValidateError,
   EN_ShowErrorList,

//   EN_ThisMatchColumnsChanged,
//   EN_OtherMatchColumnsChanged,
 //  EN_ArchiveMatchColumnsChanged,


	EN_Max      // Keep this last
};
//---------------------------------------------------------------------------
#endif

