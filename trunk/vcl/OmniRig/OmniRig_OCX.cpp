// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// $Rev$
// File generated on 15/06/2011 17:42:47 from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\Program Files\Afreet\OmniRig\OmniRig.exe (1)
// LIBID: {4FE359C5-A58F-459D-BE95-CA559FB4F270}
// LCID: 0
// Helpfile: 
// HelpString: OmniRig Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "OmniRig_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Omnirig_tlb
{

IOmniRigXPtr& TOmniRigX::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TOmniRigX::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TOmniRigX::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TOmniRigX::Disconnect()
{
  if (m_DefaultIntf) {
    
    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TOmniRigX::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TOmniRigX::ConnectTo(IOmniRigXPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TOmniRigX::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_OmniRigX;
  sd.IntfIID = __uuidof(IOmniRigX);
  sd.EventIID= __uuidof(IOmniRigXEvents);
  ServerData = &sd;
}

void __fastcall TOmniRigX::InvokeEvent(int id, Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 1: {
      if (OnVisibleChange) {
        (OnVisibleChange)(this);
      }
      break;
      }
    case 2: {
      if (OnRigTypeChange) {
        (OnRigTypeChange)(this, params[0]);
      }
      break;
      }
    case 3: {
      if (OnStatusChange) {
        (OnStatusChange)(this, params[0]);
      }
      break;
      }
    case 4: {
      if (OnParamsChange) {
        (OnParamsChange)(this, params[0], params[1]);
      }
      break;
      }
    case 5: {
      if (OnCustomReply) {
        (OnCustomReply)(this, params[0], params[1], params[2]);
      }
      break;
      }
    default:
      break;
  }
}

long __fastcall TOmniRigX::get_InterfaceVersion(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_InterfaceVersion((long*)&Value));
  return Value;
}

long __fastcall TOmniRigX::get_SoftwareVersion(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_SoftwareVersion((long*)&Value));
  return Value;
}

Omnirig_tlb::IRigXPtr __fastcall TOmniRigX::get_Rig1(void)
{
  Omnirig_tlb::IRigXPtr Value;
  OLECHECK(GetDefaultInterface()->get_Rig1(&Value));
  return Value;
}

Omnirig_tlb::IRigXPtr __fastcall TOmniRigX::get_Rig2(void)
{
  Omnirig_tlb::IRigXPtr Value;
  OLECHECK(GetDefaultInterface()->get_Rig2(&Value));
  return Value;
}

VARIANT_BOOL __fastcall TOmniRigX::get_DialogVisible(void)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->get_DialogVisible((VARIANT_BOOL*)&Value));
  return Value;
}

void __fastcall TOmniRigX::set_DialogVisible(VARIANT_BOOL Value/*[in]*/)
{
  GetDefaultInterface()->set_DialogVisible(Value/*[in]*/);
}

IRigXPtr& TRigX::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TRigX::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TRigX::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TRigX::Disconnect()
{
  if (m_DefaultIntf) {
    
    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TRigX::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TRigX::ConnectTo(IRigXPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TRigX::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_RigX;
  sd.IntfIID = __uuidof(IRigX);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

VARIANT_BOOL __fastcall TRigX::IsParamReadable(Omnirig_tlb::RigParamX Param/*[in]*/)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->IsParamReadable(Param, (VARIANT_BOOL*)&Value));
  return Value;
}

VARIANT_BOOL __fastcall TRigX::IsParamWriteable(Omnirig_tlb::RigParamX Param/*[in]*/)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->IsParamWriteable(Param, (VARIANT_BOOL*)&Value));
  return Value;
}

void __fastcall TRigX::ClearRit(void)
{
  GetDefaultInterface()->ClearRit();
}

void __fastcall TRigX::SetSimplexMode(long Freq/*[in]*/)
{
  GetDefaultInterface()->SetSimplexMode(Freq/*[in]*/);
}

void __fastcall TRigX::SetSplitMode(long RxFreq/*[in]*/, long TxFreq/*[in]*/)
{
  GetDefaultInterface()->SetSplitMode(RxFreq/*[in]*/, TxFreq/*[in]*/);
}

long __fastcall TRigX::FrequencyOfTone(long Tone/*[in]*/)
{
  long Value;
  OLECHECK(GetDefaultInterface()->FrequencyOfTone(Tone, (long*)&Value));
  return Value;
}

void __fastcall TRigX::SendCustomCommand(VARIANT Command/*[in]*/, long ReplyLength/*[in]*/, 
                                         VARIANT ReplyEnd/*[in]*/)
{
  GetDefaultInterface()->SendCustomCommand(Command/*[in]*/, ReplyLength/*[in]*/, ReplyEnd/*[in]*/);
}

long __fastcall TRigX::GetRxFrequency(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->GetRxFrequency((long*)&Value));
  return Value;
}

long __fastcall TRigX::GetTxFrequency(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->GetTxFrequency((long*)&Value));
  return Value;
}

BSTR __fastcall TRigX::get_RigType(void)
{
  BSTR Value = 0;
  OLECHECK(GetDefaultInterface()->get_RigType((BSTR*)&Value));
  return Value;
}

long __fastcall TRigX::get_ReadableParams(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_ReadableParams((long*)&Value));
  return Value;
}

long __fastcall TRigX::get_WriteableParams(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_WriteableParams((long*)&Value));
  return Value;
}

Omnirig_tlb::RigStatusX __fastcall TRigX::get_Status(void)
{
  Omnirig_tlb::RigStatusX Value;
  OLECHECK(GetDefaultInterface()->get_Status((Omnirig_tlb::RigStatusX*)&Value));
  return Value;
}

BSTR __fastcall TRigX::get_StatusStr(void)
{
  BSTR Value = 0;
  OLECHECK(GetDefaultInterface()->get_StatusStr((BSTR*)&Value));
  return Value;
}

long __fastcall TRigX::get_Freq(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_Freq((long*)&Value));
  return Value;
}

void __fastcall TRigX::set_Freq(long Value/*[in]*/)
{
  GetDefaultInterface()->set_Freq(Value/*[in]*/);
}

long __fastcall TRigX::get_FreqA(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_FreqA((long*)&Value));
  return Value;
}

void __fastcall TRigX::set_FreqA(long Value/*[in]*/)
{
  GetDefaultInterface()->set_FreqA(Value/*[in]*/);
}

long __fastcall TRigX::get_FreqB(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_FreqB((long*)&Value));
  return Value;
}

void __fastcall TRigX::set_FreqB(long Value/*[in]*/)
{
  GetDefaultInterface()->set_FreqB(Value/*[in]*/);
}

long __fastcall TRigX::get_RitOffset(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_RitOffset((long*)&Value));
  return Value;
}

void __fastcall TRigX::set_RitOffset(long Value/*[in]*/)
{
  GetDefaultInterface()->set_RitOffset(Value/*[in]*/);
}

long __fastcall TRigX::get_Pitch(void)
{
  long Value;
  OLECHECK(GetDefaultInterface()->get_Pitch((long*)&Value));
  return Value;
}

void __fastcall TRigX::set_Pitch(long Value/*[in]*/)
{
  GetDefaultInterface()->set_Pitch(Value/*[in]*/);
}

Omnirig_tlb::RigParamX __fastcall TRigX::get_Vfo(void)
{
  Omnirig_tlb::RigParamX Value;
  OLECHECK(GetDefaultInterface()->get_Vfo((Omnirig_tlb::RigParamX*)&Value));
  return Value;
}

void __fastcall TRigX::set_Vfo(Omnirig_tlb::RigParamX Value/*[in]*/)
{
  GetDefaultInterface()->set_Vfo(Value/*[in]*/);
}

Omnirig_tlb::RigParamX __fastcall TRigX::get_Split(void)
{
  Omnirig_tlb::RigParamX Value;
  OLECHECK(GetDefaultInterface()->get_Split((Omnirig_tlb::RigParamX*)&Value));
  return Value;
}

void __fastcall TRigX::set_Split(Omnirig_tlb::RigParamX Value/*[in]*/)
{
  GetDefaultInterface()->set_Split(Value/*[in]*/);
}

Omnirig_tlb::RigParamX __fastcall TRigX::get_Rit(void)
{
  Omnirig_tlb::RigParamX Value;
  OLECHECK(GetDefaultInterface()->get_Rit((Omnirig_tlb::RigParamX*)&Value));
  return Value;
}

void __fastcall TRigX::set_Rit(Omnirig_tlb::RigParamX Value/*[in]*/)
{
  GetDefaultInterface()->set_Rit(Value/*[in]*/);
}

Omnirig_tlb::RigParamX __fastcall TRigX::get_Xit(void)
{
  Omnirig_tlb::RigParamX Value;
  OLECHECK(GetDefaultInterface()->get_Xit((Omnirig_tlb::RigParamX*)&Value));
  return Value;
}

void __fastcall TRigX::set_Xit(Omnirig_tlb::RigParamX Value/*[in]*/)
{
  GetDefaultInterface()->set_Xit(Value/*[in]*/);
}

Omnirig_tlb::RigParamX __fastcall TRigX::get_Tx(void)
{
  Omnirig_tlb::RigParamX Value;
  OLECHECK(GetDefaultInterface()->get_Tx((Omnirig_tlb::RigParamX*)&Value));
  return Value;
}

void __fastcall TRigX::set_Tx(Omnirig_tlb::RigParamX Value/*[in]*/)
{
  GetDefaultInterface()->set_Tx(Value/*[in]*/);
}

Omnirig_tlb::RigParamX __fastcall TRigX::get_Mode(void)
{
  Omnirig_tlb::RigParamX Value;
  OLECHECK(GetDefaultInterface()->get_Mode((Omnirig_tlb::RigParamX*)&Value));
  return Value;
}

void __fastcall TRigX::set_Mode(Omnirig_tlb::RigParamX Value/*[in]*/)
{
  GetDefaultInterface()->set_Mode(Value/*[in]*/);
}

Omnirig_tlb::IPortBitsPtr __fastcall TRigX::get_PortBits(void)
{
  Omnirig_tlb::IPortBitsPtr Value;
  OLECHECK(GetDefaultInterface()->get_PortBits(&Value));
  return Value;
}

IPortBitsPtr& TPortBits::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TPortBits::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TPortBits::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TPortBits::Disconnect()
{
  if (m_DefaultIntf) {
    
    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TPortBits::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TPortBits::ConnectTo(IPortBitsPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TPortBits::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_PortBits;
  sd.IntfIID = __uuidof(IPortBits);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

VARIANT_BOOL __fastcall TPortBits::Lock(void)
{
  VARIANT_BOOL Ok;
  OLECHECK(GetDefaultInterface()->Lock((VARIANT_BOOL*)&Ok));
  return Ok;
}

void __fastcall TPortBits::Unlock(void)
{
  GetDefaultInterface()->Unlock();
}

VARIANT_BOOL __fastcall TPortBits::get_Rts(void)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->get_Rts((VARIANT_BOOL*)&Value));
  return Value;
}

void __fastcall TPortBits::set_Rts(VARIANT_BOOL Value/*[in]*/)
{
  GetDefaultInterface()->set_Rts(Value/*[in]*/);
}

VARIANT_BOOL __fastcall TPortBits::get_Dtr(void)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->get_Dtr((VARIANT_BOOL*)&Value));
  return Value;
}

void __fastcall TPortBits::set_Dtr(VARIANT_BOOL Value/*[in]*/)
{
  GetDefaultInterface()->set_Dtr(Value/*[in]*/);
}

VARIANT_BOOL __fastcall TPortBits::get_Cts(void)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->get_Cts((VARIANT_BOOL*)&Value));
  return Value;
}

VARIANT_BOOL __fastcall TPortBits::get_Dsr(void)
{
  VARIANT_BOOL Value;
  OLECHECK(GetDefaultInterface()->get_Dsr((VARIANT_BOOL*)&Value));
  return Value;
}


};     // namespace Omnirig_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Omnirig_ocx
{

void __fastcall PACKAGE Register()
{
  // [3]
  TComponentClass cls_svr[] = {
                              __classid(Omnirig_tlb::TOmniRigX), 
                              __classid(Omnirig_tlb::TRigX), 
                              __classid(Omnirig_tlb::TPortBits)
                           };
  RegisterComponents("ActiveX", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

};     // namespace Omnirig_ocx
