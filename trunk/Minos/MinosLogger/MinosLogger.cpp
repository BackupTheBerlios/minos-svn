/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "logger_pch.h"
#pragma hdrstop 
//---------------------------------------------------------------------------
USEFORM("BundleFrame.cpp", SettingBundleFrame);
USEFORM("ClockDialog.cpp", ClockDlg);
USEFORM("ConfigDM.cpp", MConfigDM); /* TDataModule: File Type */
USEFORM("ConfigElementFrame.cpp", ConfigElementFrm); /* TFrame: File Type */
USEFORM("ConfigMainFrame.cpp", TConfigFrame); /* TFrame: File Type */
USEFORM("ContestDetails.cpp", ContestEntryDetails);
USEFORM("enqdlg.cpp", EnquireDlg);
USEFORM("ForceLog.cpp", ForceLogDlg);
USEFORM("ListDetails.cpp", ContactListDetails);
USEFORM("loccalc.cpp", LocCalcForm);
USEFORM("LogFrame.cpp", SingleLogFrame);
USEFORM("LoggerAbout.cpp", AboutBox);
USEFORM("LogMain.cpp", LogContainer);
USEFORM("MinosHelp.cpp", MinosHelpForm);
USEFORM("OptionsFrame.cpp", OptionFrame); /* TFrame: File Type */
USEFORM("QSOEdit.cpp", QSOEditDlg);
USEFORM("QSOEditFrame.cpp", GJVQSOEditFrame);
USEFORM("QSOLogFrame.cpp", GJVQSOLogFrame);
USEFORM("QSOFrame.cpp", GJVEditFrame);
USEFORM("SendRPCDM.cpp", SendDM);
USEFORM("SettingsEditor.cpp", SettingsEditDlg);
USEFORM("TCalendarForm.cpp", CalendarForm);
USEFORM("TMinosBandChooser.cpp", MinosBandChooser);
USEFORM("TManageListsDlg.cpp", ManageListsDlg);
//---------------------------------------------------------------------------
WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
   GJV_thread::setThreadName( "Main" );
   try
   {
      Application->Initialize();
      Application->Title = "Minos Contest Logger";
      Application->CreateForm(__classid(TLogContainer), &LogContainer);
       Application->Run();
   }
   catch ( Exception & exception )
   {
      Application->ShowException( &exception );
   }
   catch ( ... )
   {
      try
      {
         throw Exception( "" );
      }
      catch ( Exception & exception )
      {
         Application->ShowException( &exception );
      }
   }
   return 0;
}
//---------------------------------------------------------------------------
