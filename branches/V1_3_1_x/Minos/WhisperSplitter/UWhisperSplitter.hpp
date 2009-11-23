// CodeGear C++Builder
// Copyright (c) 1995, 2007 by CodeGear
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Uwhispersplitter.pas' rev: 11.00

#ifndef UwhispersplitterHPP
#define UwhispersplitterHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Extctrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Uwhispersplitter
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TExtSplitter;
class PASCALIMPLEMENTATION TExtSplitter : public Extctrls::TSplitter 
{
	typedef Extctrls::TSplitter inherited;
	
__published:
	__property OnClick ;
	__property OnMouseEnter ;
	__property OnMouseLeave ;
	__property OnMoved ;
	__property OnPaint ;
public:
	#pragma option push -w-inl
	/* TSplitter.Create */ inline __fastcall virtual TExtSplitter(Classes::TComponent* AOwner) : Extctrls::TSplitter(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TSplitter.Destroy */ inline __fastcall virtual ~TExtSplitter(void) { }
	#pragma option pop
	
};


class DELPHICLASS TWhisperSplitter;
class PASCALIMPLEMENTATION TWhisperSplitter : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Extctrls::TSplitter* fSplitter;
	Extctrls::TPanel* fPanel;
	bool fAutoRestore;
	Graphics::TBitmap* fBitmap;
	int fBitmapInterations;
	Graphics::TColor fColor;
	Graphics::TColor fHighlightColor;
	bool fMinimized;
	int fRestoreSize;
	int fPanelSize;
	Classes::TNotifyEvent FOnSplitterMoved;
	void __fastcall SplitterClick(System::TObject* Sender);
	void __fastcall SplitterMouseEnter(System::TObject* Sender);
	void __fastcall SplitterMouseLeave(System::TObject* Sender);
	void __fastcall SplitterMoved(System::TObject* Sender);
	void __fastcall SplitterPaint(System::TObject* Sender);
	
__published:
	__property bool AutoRestore = {read=fAutoRestore, write=fAutoRestore, nodefault};
	__property Graphics::TBitmap* Bitmap = {read=fBitmap, write=fBitmap};
	__property int BitmapInterations = {read=fBitmapInterations, write=fBitmapInterations, nodefault};
	__property Graphics::TColor HighlightColor = {read=fHighlightColor, write=fHighlightColor, nodefault};
	__property bool Minimized = {read=fMinimized, nodefault};
	__property Classes::TNotifyEvent OnSplitterMoved = {read=FOnSplitterMoved, write=FOnSplitterMoved};
	
public:
	void __fastcall Restore(void);
	void __fastcall Minimize(void);
	__fastcall TWhisperSplitter(Extctrls::TSplitter* pSplitter, Extctrls::TPanel* pPanel);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TWhisperSplitter(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Uwhispersplitter */
using namespace Uwhispersplitter;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Uwhispersplitter
