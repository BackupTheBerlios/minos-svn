/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef ConfigDMH
#define ConfigDMH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp> 
//---------------------------------------------------------------------------
extern bool terminated;
class TConfigElement
{
   private:  	// User declarations
   public:  		// User declarations
      String name;
      std::string outfname;
      //   std::string progname;
      std::string rundir;
      std::string commandLine;
      std::string params;

      bool run;

      TConfigElement();
      bool initialise( String sect );

      void setRun(bool state);
};
class TMConfigDM : public TDataModule
{
   __published:  	// IDE-managed Components
   private:  	// User declarations
      static TMConfigDM *thisDM;
      std::vector <GJV_thread *> guardv;
      std::string circleOfHell;
      __fastcall TMConfigDM( TComponent* Owner );
   public:  		// User declarations
      __fastcall ~TMConfigDM();

      std::vector <TConfigElement *> elelist;

      void start();
      void stop();
      void setCircleOfHell( const std::string &circle );
      std::string getCircleOfHell();

      static TMConfigDM *getConfigDM( TComponent* Owner );
};
//---------------------------------------------------------------------------
#endif
