/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BandMapH
#define BandMapH 
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "VirtualTrees.hpp" 
//---------------------------------------------------------------------------
class BMEntry
{
   public:
      std::string freq;
      std::string UTC;
      std::string csCs;
      std::string loc;
      std::string qth;
      std::string comments;
      bool operator<( const BMEntry& rhs ) const;
};
struct ltbm
{
   bool operator() ( const BMEntry* s1, const BMEntry* s2 ) const;
};
inline bool ltbm::operator() ( const BMEntry* s1, const BMEntry* s2 ) const
{
   bool res = ( *s1 < *s2 );
   return res;
}
inline bool BMEntry::operator<( const BMEntry& rhs ) const
{
   int ret = stricmp( freq.c_str(), rhs.freq.c_str() );
   return ret < 0;
}

typedef codeproject::sorted_vector < BMEntry *, false, ltbm > BMList;
typedef BMList::iterator BMIterator;
//---------------------------------------------------------------------------

class TBandMapForm : public TForm
{
   __published:  	// IDE-managed Components
      TTimer *Timer1;
      TTimer *LogTimer;
      TLabel *Label1;
      TLabel *FreqLabel;
      TLabel *Label3;
      TLabel *LogLabel;
      TVirtualStringTree *BMTree;
      TTimer *StartupTimer;
      void __fastcall Timer1Timer( TObject *Sender );
      void __fastcall LogTimerTimer( TObject *Sender );
	  void __fastcall BMTreeGetNodeDataSize( TBaseVirtualTree *Sender,
											 int &NodeDataSize );
	  void __fastcall FormShow( TObject *Sender );
	  void __fastcall StartupTimerTimer( TObject *Sender );
	  void __fastcall FormClose( TObject *Sender, TCloseAction &Action );
	  void __fastcall FormResize( TObject *Sender );
	void __fastcall BMTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		  TVSTTextType TextType, UnicodeString &CellText);
   private:  	// User declarations
	  bool connected;
      bool subscribed;
      bool saveResize;
      static void makeRPCObjects();
      void bandMapClientCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void bandMapServerCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void notifyCallback( bool err, MinosRPCObj *mro, const std::string &from );
      void showBM();

      std::string FreqLabelStr;
      std::string LogLabelStr;
      BMList bmlist;
   public:  		// User declarations
      __fastcall TBandMapForm( TComponent* Owner );
      void __fastcall WmMove( TMessage &Msg );
      BEGIN_MESSAGE_MAP
      MESSAGE_HANDLER( WM_MOVE, TMessage, WmMove )
      END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE TBandMapForm *BandMapForm;
//---------------------------------------------------------------------------
#endif
