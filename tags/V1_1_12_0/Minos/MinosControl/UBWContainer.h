/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef UBWContainerH
#define UBWContainerH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "comport.h" 
//---------------------------------------------------------------------------
class USBComPort: public COMPort
{
   public:
      virtual void processRxBuff();
};
//---------------------------------------------------------------------------
class TUBWDM : public TDataModule
{
   __published: 	// IDE-managed Components
   private: 	// User declarations
      int speed;
      int parity;
      int bits;
      int stopbits;

      COMPort::MSPack msr;
      USBComPort aPort;
      long dig;
      bool parseLine( const std::string &R, int &portno, int &lineno );
      long getReadMask( const std::string &R );
   public: 		// User declarations
      __fastcall TUBWDM( TComponent* Owner );
      bool openPort();
      void setLineDirection( const std::string &portLineName, bool lineIn );
      bool closePort();
      std::string processRxBuff( const std::string &rxBuff );

      void setLines( long state );
      void setLine( const std::string &R, bool state );
      bool getLine( const std::string &R );
};
//---------------------------------------------------------------------------
extern PACKAGE TUBWDM *UBWDM;
//---------------------------------------------------------------------------
namespace UBW
{
   long openDevice( );
   void setLineDirection( const std::string &portLineName, bool lineIn );
   void closeDevice();
   long readAnalogChannel( const std::string &R );
   void readAllAnalog( long * Data1, long * Data2 );
   void outputAnalogChannel( const std::string &R, long Data );
   void outputAllAnalog( long Data1, long Data2 );
   void clearAnalogChannel( const std::string &R );
   void clearAllAnalog();
   void setAnalogChannel( const std::string &R );
   void setAllAnalog();
   void writeAllDigital( long Data );
   void clearAllDigital();
   void setDigitalChannel( const std::string &R, bool state );
   void setAllDigital();
   bool readDigitalChannel( const std::string &R );
}
#endif
