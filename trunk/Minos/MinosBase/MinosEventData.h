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
   EN_ReportOverstrike,
   EN_AfterLogContact,
   EN_AfterSelectContact,
   EN_NextContactDetailsOnLeft,
   EN_ContestDetails,
   EN_GoToSerial,
   EN_MakeEntry,
   EN_NextUnfilled,
   EN_FormKey,
   EN_SetMode,
   EN_SetFreq,
   EN_EditMatchContact,
   EN_ScreenContactChanged,

   EN_ReplaceLogList,
   EN_ReplaceListList,
   EN_ScrollToCountry,
   EN_ScrollToDistrict,
   EN_MatchStarting,

   EN_CountrySelect,
   EN_DistrictSelect,
   EN_LocatorSelect,

   EN_Op1Change,
   EN_Op2Change,
   EN_ShowOperators,
   //   EN_ThisMatchColumnsChanged,
//   EN_OtherMatchColumnsChanged,
 //  EN_ArchiveMatchColumnsChanged,


	EN_Max      // Keep this last
};
//---------------------------------------------------------------------------
#endif

