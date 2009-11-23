/////////////////////////////////////////////////////////////////////////////
// $Id$
//
// PROJECT NAME 		Minos Amateur Radio Control and Logging System
//
// COPYRIGHT         (c) M. J. Goodey G0GJV 2005 - 2008
//
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
#ifndef tspanelH
#define tspanelH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE CompDetail;
class PACKAGE TSPanel : public TCustomPanel
{
	typedef TCustomPanel inherited;

private:
	TRect *panelpos;
	CompDetail *compdetails;

    bool fDesignDrag;
    bool fLimitSize;
    int ccount;
    bool fGrowButtons;

    bool __fastcall GetDesignDrag();
    void __fastcall SetDesignDrag(bool s);
	 virtual void __fastcall WndProc(Messages::TMessage &Message);
protected:
__published:
	__property Align ;
	__property Alignment ;
	__property BevelInner ;
	__property BevelOuter ;
	__property BevelWidth ;
	__property BorderWidth ;
	__property BorderStyle ;
	__property DragCursor ;
	__property DragMode ;
	__property Enabled ;
   __property Canvas;
	__property Caption ;
	__property Color ;
	__property Ctl3D ;
	__property Font ;
	__property Locked ;
	__property ParentColor ;
	__property ParentCtl3D ;
	__property ParentFont ;
	__property ParentShowHint ;
	__property PopupMenu ;
	__property ShowHint ;
	__property TabOrder ;
	__property TabStop ;
	__property Visible ;
	__property OnClick ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnResize ;
	__property OnStartDrag ;

    __property bool DesignDrag = {read = GetDesignDrag, write = SetDesignDrag};
    __property bool LimitSize = {read = fLimitSize, write = fLimitSize};
    __property bool GrowButtons = {read =  fGrowButtons, write = fGrowButtons, default = true};
public:
	/* TCustomPanel.Create */ __fastcall virtual TSPanel(Classes::TComponent* AOwner);
	/* TCustomControl.Destroy */ __fastcall virtual ~TSPanel(void);
	/* TWinControl.CreateParented */ __fastcall TSPanel(HWND ParentWindow);

	virtual void __fastcall AlignControls(Controls::TControl* AControl, Types::TRect &Rect);
	void __fastcall GetCompDetails(void);
   DYNAMIC void __fastcall Resize(void);
	virtual void __fastcall Loaded(void);

//	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation	);
};
//---------------------------------------------------------------------------
#endif
