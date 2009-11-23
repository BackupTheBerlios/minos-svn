
#eror no longer used
#ifdef __cplusplus
namespace K8055
{
   extern "C"
   {
#endif

#define FUNCTION __declspec(dllimport)

      FUNCTION long __stdcall openDevice( long CardAddress );
      FUNCTION void __stdcall closeDevice();
      FUNCTION long __stdcall readAnalogChannel( long Channel );
      FUNCTION void __stdcall readAllAnalog( long * Data1, long * Data2 );
      FUNCTION void __stdcall outputAnalogChannel( long Channel, long Data );
      FUNCTION void __stdcall outputAllAnalog( long Data1, long Data2 );
      FUNCTION void __stdcall clearAnalogChannel( long Channel );
      FUNCTION void __stdcall clearAllAnalog();
      FUNCTION void __stdcall setAnalogChannel( long Channel );
      FUNCTION void __stdcall setAllAnalog();
      FUNCTION void __stdcall writeAllDigital( long Data );
      FUNCTION void __stdcall clearDigitalChannel( long Channel );
      FUNCTION void __stdcall clearAllDigital();
      FUNCTION void __stdcall setDigitalChannel( long Channel );
      FUNCTION void __stdcall setAllDigital();
      FUNCTION bool __stdcall readDigitalChannel( long Channel );
      FUNCTION long __stdcall readAllDigital();
      FUNCTION long __stdcall readCounter( long CounterNr );
      FUNCTION void __stdcall resetCounter( long CounterNr );
      FUNCTION void __stdcall setCounterDebounceTime( long CounterNr, long DebounceTime );

#ifdef __cplusplus

   }
}
#endif





