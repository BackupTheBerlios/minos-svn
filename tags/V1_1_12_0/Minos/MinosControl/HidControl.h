//---------------------------------------------------------------------------

#ifndef HidControlH
#define HidControlH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <setupapi.h>
//---------------------------------------------------------------------------
class THidControlFM : public TForm
{
__published:	// IDE-managed Components
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	HANDLE HidDevHandle;
	bool HidAttached;
	HDEVNOTIFY hDevNotify;					/* notification results */
public:		// User declarations
	__fastcall THidControlFM(TComponent* Owner);
	__fastcall ~THidControlFM();
	bool bHidDeviceNotify();
	bool bOpenHidDevice(USHORT vid, USHORT pid);
	bool ReadFile(char *buff, unsigned long toread, unsigned long &read);
	bool WriteFile(char *buff, unsigned long towrite, unsigned long &written);

	void __fastcall WmDeviceChange( TMessage &Msg );
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER( WM_DEVICECHANGE, TMessage, WmDeviceChange )
	END_MESSAGE_MAP( TForm )
};
//---------------------------------------------------------------------------
extern PACKAGE THidControlFM *HidControlFM;
//---------------------------------------------------------------------------
#endif

