//---------------------------------------------------------------------------

#ifndef xTrackBarH
#define xTrackBarH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TxTrackBar : public TTrackBar
{
private:
    bool m_EnableSelRange;
    void __fastcall SetEnableSelRange(bool enabled);
protected:
    // Override CreateParams to fix the hard coding done in the VCL base class
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
public:
    __fastcall TxTrackBar(TComponent* Owner);
__published:
    __property bool EnableSelRange= {read=m_EnableSelRange,write=SetEnableSelRange,default=false};
};
//---------------------------------------------------------------------------
#endif
