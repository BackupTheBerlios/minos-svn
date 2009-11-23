/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef K8055ContainerH
#define K8055ContainerH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <vector> 
//---------------------------------------------------------------------------
class TDMK8055 : public TForm
{
   __published:  	// IDE-managed Components
      TTimer *Timer1;
      void __fastcall FormShow( TObject *Sender );
      void __fastcall Timer1Timer( TObject *Sender );
   private:  	// User declarations
   public:  		// User declarations
//      std::vector <TJvHidDevice *> devList;
//      TJvHidDevice *theDev;
      bool timed;
      bool viewed;
      bool deviceFound;

      unsigned int shared[ 100 ];
      unsigned char bufTx[ 65 ];
      unsigned char bufRx[ 65 ];
      int deviceNo;

      void __fastcall doRead();
      void __fastcall doWrite();
      void __fastcall setShareData ( unsigned int address, unsigned int data );
      void __fastcall doAction( unsigned int k0, unsigned int k1, unsigned int k2 );

      __fastcall TDMK8055( TComponent* Owner );
      __fastcall ~TDMK8055();
};
//---------------------------------------------------------------------------
extern PACKAGE TDMK8055 *DMK8055;
namespace K8055
{
   long openDevice( long CardAddress );
   void closeDevice();
   long readAnalogChannel( long Channel );
   void readAllAnalog( long * Data1, long * Data2 );
   void outputAnalogChannel( long Channel, long Data );
   void outputAllAnalog( long Data1, long Data2 );
   void clearAnalogChannel( long Channel );
   void clearAllAnalog();
   void setAnalogChannel( long Channel );
   void setAllAnalog();
   void writeAllDigital( long Data );
   void clearDigitalChannel( long Channel );
   void clearAllDigital();
   void setDigitalChannel( long Channel );
   void setAllDigital();
   bool readDigitalChannel( long Channel );
   long readAllDigital();
   long readCounter( long CounterNr );
   void resetCounter( long CounterNr );
   void setCounterDebounceTime( long CounterNr, long DebounceTime );
}
//---------------------------------------------------------------------------
#endif
