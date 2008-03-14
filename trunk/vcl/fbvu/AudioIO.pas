{
  File Name: AudioIO.PAS  V 4.00
  Created: 5-Oct-96 by John Mertus on the IBM PC
  Revision #1: 5-Oct-22 by John Mertus
                                        -John Mertus
  Version 1.00 Initial Release

}
{
  There are three Sound Components, the first is the base Component,
  TAudioIO.  This defines the sampling rates, buffers and some of the
  common events.

  The second component is AudioOut, which started just loops playing out
  buffers.

  The third component is AudioIN, which, when started, just loops filling
  buffer with digital data.

  See AudioIO.Hlp for detailed explaination.
}

unit AudioIO;
{$ObjExportAll On}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, MMSystem;

const
  MAXBUFFERS = 16;

type
  TBufferEvent = function(Buffer: pChar; var Size: Integer): Boolean of object;

  TAudioOut = class;
  TAudioIn = class;
  TAudioIO = class;

  TCallBackWinOut = class(TWinControl)
  private
    { Private declarations }
    AudioComponent: TAudioOut;
    procedure BufferDone(var Msg: TMessage); message MM_WOM_DONE;
    procedure WaveOpen(var Msg: TMessage); message MM_WOM_OPEN;
    procedure WaveClose(var Msg: TMessage); message MM_WOM_CLOSE;
  protected
    { Protected declarations }
  public
    { Public declarations }
  published
    { Published declarations }
  end;

  TCallBackWinIn = class(TWinControl)
  private
    { Private declarations }
    AudioComponent: TAudioIn;
    procedure BufferFinished(var Msg: TMessage); message MM_WIM_DATA;
    procedure WaveOpenIn(var Msg: TMessage); message MM_WIM_OPEN;
    procedure WaveCloseIn(var Msg: TMessage); message MM_WIM_CLOSE;
  end;

{---------------------------TAudioIO Component-----------------------------}
  TAudioIO = class(TComponent)
  private
    FBufferSize: Integer;
    FRequestedBufferSize: Integer;
    FNumBuffers: Integer;
    FPaused: Boolean;

    FWaveFmtEx: TWaveFormatEx;
    FonOpen: TNotifyEvent;
    FonClose: TNotifyEvent;
    FWaveDevice: DWord;
    hWaveHeader	: array[0..MAXBUFFERS - 1] of THANDLE;
    WaveHdr: array[0..MAXBUFFERS - 1] of PWAVEHDR;
    WaveBuffer: array[0..MAXBUFFERS - 1] of lpstr;
    hWaveBuffer: array[0..MAXBUFFERS - 1] of THANDLE;
    BufIndex: Integer;
    ContinueProcessing: Boolean; { Set to TRUE to start FALSE to abort
                                         after filled buffers are done }

    { Property Functions }
    procedure SetNumBuffers(Value: Integer);
    procedure SetBufferSize(Value: Integer);
    procedure SetFrameRate(Value: Integer);
    procedure SetStereo(Value: Boolean);
    procedure SetBits(Value: Word);
    function GetFrameRate: Integer;
    function GetStereo: Boolean;
    procedure MakeWaveFmtConsistent;

  protected
    { Protected declarations }
    function InitWaveHeaders: Boolean;
    function AllocPCMBuffers: Boolean;
    function FreePCMBuffers: Boolean;
    function AllocWaveHeaders: Boolean;
    procedure FreeWaveHeaders;

  public
    { Public declarations }
    ErrorMessage: string;
    Active: Boolean;
    FilledBuffers : Integer;
    QueuedBuffers : integer;
    ProcessedBuffers: Integer;

    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure StopAtOnce; virtual;
    procedure StopGracefully; virtual;

  published
    { Published declarations }
    property BufferSize: Integer read FBufferSize write SetBufferSize default 8192;
    property NumBuffers: Integer read FNumBuffers write SetNumBuffers default MAXBUFFERS;
    property FrameRate: Integer read GetFrameRate write SetFrameRate default 22055;
    property Stereo: Boolean read GetStereo write SetStereo default False;
    property Quantization: Word read FWaveFmtEx.wBitsPerSample write SetBits default 16;
    property WaveDevice: Cardinal read FWaveDevice write FWaveDevice default WAVE_MAPPER;
    property OnStart: TNotifyEvent read FOnOpen write FOnOpen;
    property OnStop: TNotifyEvent read FOnClose write FOnClose;
  end;

{---------------------------TAudioOut Component-----------------------------}
  TAudioOut = class(TAudioIO)
  private
    { Private declarations }
    WaveDeviceOpen: Boolean;
    CallBackWin: TCallBackWinOut;
    FOnFillBuffer: TBufferEvent;

    function QueueBuffer: Boolean;
    function ReadBuffer(Idx, N: Integer): Boolean; virtual;
    procedure SetPaused(Value: Boolean);
    procedure CloseWaveDevice; virtual;
    function Setup: Boolean; virtual;
    function StartIt: Boolean;

  protected
    { Protected declarations }

  public
    { Public declarations }
    WaveHandle: HWaveOut; { Waveform output handle }

    function Start: Boolean;
    procedure StopAtOnce; override;
    procedure StopGracefully; override;
    function ElapsedTime: Real;

  published
    { Published declarations }
    property Paused: Boolean read FPaused write SetPaused default FALSE;
    property OnFillBuffer: TBufferEvent read FOnFillBuffer write FOnFillBuffer;
  end;

{---------------------------TAudioIn Component-----------------------------}
  TAudioIn = class(TAudioIO)
  private
    { Private declarations }
    WaveDeviceOpen: Boolean;
    CallBackWin: TCallBackWinIn;
    FOnBufferFilled: TBufferEvent;

    function QueueBuffer: Boolean;
    function ProcessBuffer(B: lpstr; N: Integer): Boolean;
    procedure CloseWaveDevice;
    function Setup: Boolean;
    function StartIt: Boolean;

  protected
    { Protected declarations }

  public
    { Public declarations }
    WaveHandle: HWaveOut; { Waveform output handle }
    function Start: Boolean;
    procedure StopAtOnce; override;
    procedure StopGracefully; override;
    function ElapsedTime: Real;

  published
    { Published declarations }
    property OnBufferFilled: TBufferEvent read FOnBufferFilled write FOnBufferFilled;
  end;


procedure Register;

implementation

{$R *.res}

function TWaveOutGetErrorText(iErr: Integer): string;
{  This just gets the error text assocated with the output error ierr.  }
var
  ErrorMsgC: array[0..255] of Char;
begin
  waveOutGetErrorText(iErr, ErrorMsgC, Sizeof(ErrorMsgC));
  Result := StrPas(ErrorMsgC);
end;

function TWaveInGetErrorText(iErr: Integer): string;
{  This just gets the error text assocated with the output error ierr.  }
var
  ErrorMsgC: array[0..255] of Char;
begin
  waveInGetErrorText(iErr, ErrorMsgC, Sizeof(ErrorMsgC));
  Result := StrPas(ErrorMsgC);
end;

procedure Register;
begin
  RegisterComponents('Snd', [TAudioOut, TAudioIn]);
end;

procedure TAudioIO.SetBufferSize(Value: Integer);
{  This just set the buffersize, making sure it is too small.           }
begin
  if (Value < 512) then Value := 512;
  { make the wave buffer size a multiple of the block align... }
  FRequestedBufferSize := Value;
  MakeWaveFmtConsistent;
  FreePCMBuffers;
  AllocPCMBuffers;
end;

procedure TAudioIO.SetNumBuffers(Value: Integer);
{  This just set the numbers of buffers making sure it is between       }
{ and MaxNumberBuffers                                                  }
begin
  if (Value < 2) then Value := 2;
  if (Value > MAXBUFFERS) then Value := MAXBUFFERS;
  FNumBuffers := Value;
end;

procedure TAudioIO.SetStereo(Value: Boolean);
{  This just set the numbers of channels, True 2, false 1.              }
begin
  if Value then
    FWaveFmtEx.nChannels := 2
  else
    FWaveFmtEx.nChannels := 1;
  MakeWaveFmtConsistent;
end;

procedure TAudioIO.SetBits(Value: Word);
{  This just set the numbers of buffers making sure it is between       }
{ and MaxNumberBuffers                                                  }
begin
  if (Value < 8) then Value := 8;
  if (Value > 8) then Value := 16;
  FWaveFmtEx.wBitsPerSample := Value;
  MakeWaveFmtConsistent;
end;

procedure TAudioIO.SetFrameRate(Value: Integer);
{  This just set the frame rate for sampling.                           }
begin
  FWaveFmtEx.nSamplesPerSec := Value;
  MakeWaveFmtConsistent;
end;

function TAudioIO.GetFrameRate: Integer;
{  This just returns the framerate for the current header.              }
begin
  Result := FWaveFmtEx.nSamplesPerSec;
end;

function TAudioIO.GetStereo: Boolean;
{  This just returns the True if stereo, e.g. 2 channels                }
begin
  Result := (FWaveFmtEx.nChannels = 2);
end;

constructor TAudioIO.Create(AOwner: TComponent);
{  This just set the numbers of buffers making sure it is between       }
{ and MaxNumberBuffers                                                  }
var
  i: Integer;
begin
  inherited Create(AOwner);
  FNumBuffers := MAXBUFFERS;
  FRequestedBufferSize := 8192;
  Active := FALSE;
  FPaused := FALSE;
  FWaveDevice := WAVE_MAPPER;
  ErrorMessage := '';
  { Set the indendent sampling rates }
  FWaveFmtEx.wFormatTag := WAVE_FORMAT_PCM;
  FWaveFmtEx.wBitsPerSample := 16;
  FWaveFmtEx.nchannels := 1;
  FWaveFmtEx.nSamplesPerSec := 22050;
  MakeWaveFmtConsistent;

  { Now make sure we know buffers are not allocated }
  for i := 0 to MAXBUFFERS - 1 do WaveBuffer[i] := nil;

  AllocWaveHeaders;
  AllocPCMBuffers;
end;

destructor TAudioIO.Destroy;
{  This cleans up the buffers.                                          }
begin
  FreePCMBuffers;
  FreeWaveHeaders;
  inherited Destroy;
end;

procedure TAudioIO.MakeWaveFmtConsistent;
{  This just trys to find the correct avgbytes and blockalign that      }
{ one needs to use for the format.  I DO NOT UNDERSTAND WHY MICROSOFT   }
{ did this.                                                             }
begin
  with FWaveFmtEx do
  begin
    nBlockAlign := (wBitsPerSample div 8) * nchannels;
    nAvgBytesPerSec := nSamplesPerSec * nBlockAlign;
  end;
  FBufferSize := FRequestedBufferSize - (FRequestedBufferSize mod FWaveFmtEx.nBlockAlign);
end;

function TAudioIO.InitWaveHeaders: Boolean;
{ This just initalizes the waveform headers, no memory allocated        }
var
  i: Integer;
begin
  { This should not be necessary, but to be safe... }
  MakeWaveFmtConsistent;
  { Set the wave headers }
  for i := 0 to FNumBuffers - 1 do
    with WaveHdr[i]^ do
    begin
      lpData := WaveBuffer[i]; // address of the waveform buffer
      dwBufferLength := FBufferSize; // length, in bytes, of the buffer
      dwBytesRecorded := 0; // see below
      dwUser := 0; // 32 bits of user data
      dwFlags := 0; // see below
      dwLoops := 0; // see below
      lpNext := nil; // reserved; must be zero
      reserved := 0; // reserved; must be zero
    end;

  InitWaveHeaders := TRUE;
end;

function TAudioIO.AllocPCMBuffers: Boolean;
{ Allocate and lock the waveform memory.                                }
var
  i: Integer;
begin
  for i := 0 to fNumBuffers - 1 do
  begin
    hWaveBuffer[i] := GlobalAlloc(GMEM_MOVEABLE or GMEM_SHARE, fBufferSize);
    if (hWaveBuffer[i] = 0) then
    begin
      FreePCMBuffers;
      ErrorMessage := 'Error allocating wave buffer memory';
      AllocPCMBuffers := False;
      Exit;
    end;
    WaveBuffer[i] := GlobalLock(hWaveBuffer[i]);
    if (WaveBuffer[i] = nil) then
    begin
      FreePCMBuffers;
      ErrorMessage := 'Error Locking wave buffer memory';
      AllocPCMBuffers := False;
      Exit;
    end;
    WaveHdr[i].lpData := WaveBuffer[i];
  end;

  AllocPCMBuffers := TRUE;
end;

function TAudioIO.FreePCMBuffers: Boolean;
{ Free up the meomry AllocPCMBuffers used.                              }
var
  i: Integer;
begin
  Result := FALSE;
  for i := 0 to MaxBuffers - 1 do
  begin
    if (hWaveBuffer[i] <> 0) then
    begin
      GlobalUnlock(hWaveBuffer[i]);
      GlobalFree(hWaveBuffer[i]);
      hWaveBuffer[i] := 0;
      WaveBuffer[i] := nil;
      Result := TRUE;
    end;
  end;

end;

function TAudioIO.AllocWaveHeaders: Boolean;
{ Allocate and lock header memory                                       }
var
  i: Integer;
begin
  for i := 0 to MAXBUFFERS - 1 do
  begin
    hwaveheader[i] := GlobalAlloc(GMEM_MOVEABLE or GMEM_SHARE or GMEM_ZEROINIT, sizeof(TWAVEHDR));
    if (hwaveheader[i] = 0) then
    begin
      FreeWaveHeaders;
      ErrorMessage := 'Error allocating wave header memory';
      AllocWaveHeaders := FALSE;
      Exit;
    end;

    WaveHdr[i] := GlobalLock(hwaveheader[i]);
    if (WaveHdr[i] = nil) then
    begin
      FreeWaveHeaders;
      ErrorMessage := 'Could not lock header memory for recording';
      AllocWaveHeaders := FALSE;
      Exit;
    end;

  end;

  AllocWaveHeaders := TRUE;
end;

procedure TAudioIO.FreeWaveHeaders;
{ Just free up the memory AllocWaveHeaders allocated.                   }
var
  i: Integer;
begin
  for i := 0 to MAXBUFFERS - 1 do
  begin
    if (hWaveHeader[i] <> 0) then
    begin
      GlobalUnlock(hwaveheader[i]);
      GlobalFree(hwaveheader[i]);
      hWaveHeader[i] := 0;
      WaveHdr[i] := nil;
    end
  end;
end;

procedure TAudioIO.StopAtOnce;
{ Write the buffer to the wave device and toggel buffer index.          }
begin
  Active := False;
  ContinueProcessing := FALSE;
end;

procedure TAudioIO.StopGracefully;
{ Write the buffer to the wave device and toggel buffer index.          }
begin
  ContinueProcessing := FALSE;
end;

function TAudioOut.ElapsedTime: Real;
{ This function returns the time since start of playout.                }
var
  pmmt: TMMTime;
begin
  if (Active) then
  begin
    pmmt.wType := TIME_SAMPLES;
    if (waveOutGetPosition(WaveHandle, @pmmt, Sizeof(TMMTime)) <> 0) then
      Result := 0
    else
      Result := pmmt.Sample / FrameRate;
  end
  else
    Result := 0;
end;

procedure TAudioOut.SetPaused(Value: Boolean);
{  This pauses or restarts the output.                                  }
begin
  FPaused := Value;
  if (not Active) then Exit;
  if FPaused then
    WaveOutPause(WaveHandle)
  else
    WaveOutReStart(WaveHandle);
end;

procedure TAudioOut.CloseWaveDevice;
{ Closes the wave output device.                                        }
var
  i: Integer;
begin
{ unprepare the headers }
  Active := FALSE;
  Paused := FALSE;
  for i := 0 to FNumBuffers - 1 do
    waveOutUnprepareHeader(WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));

{ close the device }
  waveOutClose(WaveHandle);
  WaveDeviceOpen := FALSE;

end;

function TAudioOut.Setup : Boolean;
{ This function just sets up the board for output.                      }
var
  iErr: Integer;
  i: Integer;
begin
  { if the device is still open, return error }
  if (WaveDeviceOpen) then
  begin
    ErrorMessage := 'Wave output device is already open';
    Result := FALSE;
    Exit;
  end;

  BufIndex := 0;

  { Now create the window component to handle the processing }
  CallBackWin := TCallBackWinOut.CreateParented(TWinControl(Owner).Handle);
  CallBackWin.Visible := FALSE;
  CallBackWin.AudioComponent := self;

  { Open the device for playout }
  { Either go via interrupt or window }
  iErr := WaveOutOpen(@WaveHandle, FWaveDevice, @FWaveFmtEx, Integer(CallBackWin.Handle),
    0, CALLBACK_WINDOW or WAVE_ALLOWSYNC);

  if (iErr <> 0) then
  begin
    ErrorMessage := TWaveOutGetErrorText(iErr);
    Result := FALSE;
    Exit;
  end;

  WaveDeviceOpen := TRUE;

  { Setup the buffers and headers }
  if (not InitWaveHeaders) then
  begin
    Result := FALSE;
    Exit;
  end;

  { Now Prepare the buffers for output }
  for i := 0 to FNumBuffers - 1 do
  begin
    iErr := WaveOutPrepareHeader(WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));
    if (iErr <> 0) then
    begin
      ErrorMessage := TWaveOutGetErrorText(iErr);
      CloseWaveDevice;
      Result := FALSE;
      Exit;
    end;
  end;

  { Read in the buffers }
  QueuedBuffers := 0;
  ProcessedBuffers := 0;
  FilledBuffers := 0;
  ContinueProcessing := TRUE;
  Active := TRUE;

  if (not ReadBuffer(0, FBufferSize)) then
  begin
    CloseWaveDevice;
    ErrorMessage := 'There must be at least one filled buffer';
    Result := FALSE;
    Exit;
  end;

  for i := 1 to FNumBuffers - 1 do ReadBuffer(i, FBufferSize);

  Result := TRUE;
end;

function TAudioOut.QueueBuffer: Boolean;
{ Write the buffer to the wave device and toggel buffer index.          }
var
  iErr: Integer;
begin
 { reset flags field (remove WHDR_DONE attribute) }
  WaveHdr[bufindex].dwFlags := WHDR_PREPARED;

 { now queue the buffer for output }
  iErr := waveOutWrite(WaveHandle, WaveHdr[bufindex], sizeof(TWAVEHDR));
  if (iErr <> 0) then
  begin
    ErrorMessage := TwaveOutGetErrorText(iErr);
    StopGracefully;
    Result := FALSE;
    Exit;
  end;

  { Advance index }
  bufindex := (bufindex + 1) mod FNumBuffers;

  Result := TRUE;
end;

function TAudioOut.StartIt: Boolean;
{ This function just starts the waveform playing                        }
var
  i: Integer;
begin
  Active := TRUE;

  if (FPaused) then WaveOutPause(WaveHandle);
  { Now we are ready to start the output }
  if (not QueueBuffer) then
  begin
    CloseWaveDevice;
    Result := FALSE;
    Exit;
  end;

  for i := 0 to FNumBuffers - 2 do QueueBuffer;
  Result := TRUE;
end;

function TAudioOut.Start: Boolean;
{ This function first sets up the output and then starts it.            }
begin
  Result := Setup;
  if (not Result) then Exit;
  Result := StartIt;
  if (not Result) then Exit;
end;

function TAudioOut.ReadBuffer(Idx, N: Integer): Boolean;
{ This is called whenver move buffer data is needed.                    }
var
  NSize: Integer;
begin
  { Do not call the read buffer routine if we want to stop }
  if (not ContinueProcessing) then
  begin
    Result := FALSE;
    Exit;
  end;

  { If assigned, process the buffer, Notice that the Size returned may not
    be the size sent,  so reset the output size }
  if Assigned(FOnFillBuffer) then
  begin
    NSize := N;
    Result := FOnFillBuffer(WaveBuffer[idx], NSize);
    WaveHdr[idx].dwBufferLength := NSize;
  end
  else
    Result := FALSE;

  { On a filled buffer, increment it }
  if (Result) then FilledBuffers := FilledBuffers + 1;
  QueuedBuffers := FilledBuffers - ProcessedBuffers;
end;

procedure TAudioOut.StopAtOnce;
{ Write the buffer to the wave device and toggel buffer index.          }
begin
  { if the device isn't open, just return }
  if (not WaveDeviceOpen) then Exit;

  Active := False;
  ContinueProcessing := FALSE;

  { stop playing }
  waveOutReset(WaveHandle);
  CloseWaveDevice;
end;

procedure TAudioOut.StopGracefully;
{ Write the buffer to the wave device and toggel buffer index.          }
begin
  { if the device isn't open, just return }
  if (not WaveDeviceOpen) then Exit;
  ContinueProcessing := FALSE;
end;

procedure TCallBackWinOut.BufferDone(var Msg: TMessage);
{ This is called when a buffer si done playing                          }
begin
  with AudioComponent do
  begin
    ProcessedBuffers := ProcessedBuffers + 1;
    QueuedBuffers := FilledBuffers - ProcessedBuffers;
    Active := (QueuedBuffers > 0);
    if (ReadBuffer(BufIndex, FBufferSize)) then QueueBuffer;
    if (not Active) then
    begin
      ContinueProcessing := FALSE;
      CloseWaveDevice;
    end;
  end;
end;

procedure TCallBackWinOut.WaveOpen(var Msg: TMessage);
{ This is called at the termination of each buffer.                     }
begin
  if Assigned(AudioComponent.FonOpen) then AudioComponent.FonOpen(Self);
end;

procedure TCallBackWinOut.WaveClose(var Msg: TMessage);
{ This is called at the termination of each buffer.                     }
begin
  if Assigned(AudioComponent.FonClose) then AudioComponent.FonClose(Self);
end;

function TAudioIn.ElapsedTime: Real;
{ This function returns the time since start of playout.                }
var
  pmmt: TMMTime;
begin
  if (Active) then
  begin
    pmmt.wType := TIME_SAMPLES;
    if (waveInGetPosition(WaveHandle, @pmmt, Sizeof(TMMTime)) <> 0) then
      Result := 0
    else
      Result := pmmt.sample / FrameRate;
  end
  else
    Result := 0;
end;

procedure TAudioIn.CloseWaveDevice;
{ Closes the wave output device.                                        }
var
  i: Integer;
begin
{ unprepare the headers }
  Active := FALSE;
  for i := 0 to FNumBuffers - 1 do
    waveInUnprepareHeader(WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));

{ close the device }
  waveInReset(WaveHandle);
  waveInClose(WaveHandle);
  WaveDeviceOpen := FALSE;
end;

function TAudioIn.Setup: Boolean;
{ This function just sets up the board for output.                      }
var
  iErr: Integer;
  i: Integer;

begin
  { if the device is still open, return error }
  if (WaveDeviceOpen) then
  begin
    ErrorMessage := 'Wave Input device is already open';
    Result := FALSE;
    Exit;
  end;

  BufIndex := 0;
  { Now create the window component to handle the processing }
  CallBackWin := TCallBackWinIn.CreateParented(TWinControl(Owner).Handle);
  CallBackWin.Visible := FALSE;
  CallBackWin.AudioComponent := self;

  { Open the device for playout }
  { Either go via interrupt or window }
  iErr := WaveInOpen(@WaveHandle, FWaveDevice, @FWaveFmtEx, Integer(CallBackWin.Handle),
    0, CALLBACK_WINDOW or WAVE_ALLOWSYNC);

  if (iErr <> 0) then
  begin
    ErrorMessage := TWaveInGetErrorText(iErr);
    Result := FALSE;
    Exit;
  end;

  WaveDeviceOpen := TRUE;

  { Setup the buffers and headers }
  if (not InitWaveHeaders) then
  begin
    Result := FALSE;
    Exit;
  end;

  { Now Prepare the buffers for output }
  for i := 0 to FNumBuffers - 1 do
  begin
    iErr := WaveInPrepareHeader(WaveHandle, WaveHdr[i], sizeof(TWAVEHDR));
    if (iErr <> 0) then
    begin
      ErrorMessage := TWaveInGetErrorText(iErr);
      CloseWaveDevice;
      Result := FALSE;
      Exit;
    end;
  end;

  { Read in the buffers }
  QueuedBuffers := 0;
  ProcessedBuffers := 0;
  FilledBuffers := 0;
  ContinueProcessing := TRUE;
  Active := TRUE;

  Result := TRUE;
end;

function TAudioIn.QueueBuffer: Boolean;
{ Write the buffer to the wave device and toggel buffer index.          }
var
  iErr: Integer;
begin
 { reset flags field (remove WHDR_DONE attribute) }
  WaveHdr[bufindex].dwFlags := WHDR_PREPARED;

 { now queue the buffer for output }
  iErr := waveInAddBuffer(WaveHandle, WaveHdr[bufindex], sizeof(TWAVEHDR));
  if (iErr <> 0) then
  begin
    ErrorMessage := TWaveInGetErrorText(iErr);
    StopGracefully;
    Result := FALSE;
    Exit;
  end;

  { Advance index }
  bufindex := (bufindex + 1) mod FNumBuffers;
  QueuedBuffers := QueuedBuffers + 1;

  Result := TRUE;
end;

function TAudioIn.StartIt: Boolean;
{ This function just starts the waveform playing                        }
var
  i, iErr: Integer;
begin
  { start recording to first buffer }
  iErr := WaveInStart(WaveHandle);
  if (iErr <> 0) then
  begin
    CloseWaveDevice;
    ErrorMessage := 'Error starting wave record: ' + TWaveInGetErrorText(iErr);
    Result := FALSE;
    Exit;
  end;

  Active := TRUE;

  { Now we are ready to start the output }
  for i := 0 to FNumBuffers - 1 do
    if (not QueueBuffer) then
    begin
      CloseWaveDevice;
      Result := FALSE;
      Exit;
    end;

  Result := TRUE;
end;

function TAudioIn.Start: Boolean;
{ This function first sets up the output and then starts it.            }
begin
  Result := Setup;
  if (not Result) then Exit;

  Result := StartIt;
  if (not Result) then Exit;
end;

function TAudioIn.ProcessBuffer(B: lpstr; N: Integer): Boolean;
{ This is called whenver move buffer data is needed.                    }
begin
  { Do not call the read buffer routine if we want to stop }
  if (not ContinueProcessing) then
  begin
    Result := FALSE;
    Exit;
  end;

  { N can change, but we dont' care }
  if Assigned(FOnBufferFilled) then
  begin
    Result := FOnBufferFilled(B, N);
  end
  else
    Result := TRUE;

  { On a filled buffer, increment it }
  if (Result) then FilledBuffers := FilledBuffers + 1;
end;

procedure TAudioIn.StopAtOnce;
{ Write the buffer to the wave device and toggel buffer index.          }
begin
  { if the device isn't open, just return }
  if (not WaveDeviceOpen) then Exit;

  Active := False;
  ContinueProcessing := FALSE;

  { stop playing }
  waveInReset(WaveHandle);

  { close the device and unprepare the headers }
  CloseWaveDevice;
end;

procedure TAudioIn.StopGracefully;
{ Write the buffer to the wave device and toggel buffer index.          }
begin
  { if the device isn't open, just return }
  if (not WaveDeviceOpen) then Exit;
  ContinueProcessing := FALSE;
end;

procedure TCallBackWinIn.BufferFinished(var Msg: TMessage);
{ This is called when each buffer is filled.                            }
begin
  with AudioComponent do
  begin
    ProcessedBuffers := ProcessedBuffers + 1;
    QueuedBuffers := QueuedBuffers - 1;
    Active := (QueuedBuffers > 0);
    if (ProcessBuffer(WaveBuffer[BufIndex], FBufferSize)) then QueueBuffer;

    if (not Active) then
    begin
      ContinueProcessing := FALSE;
      CloseWaveDevice;
    end;

  end;
end;

procedure TCallBackWinIn.WaveOpenIn(var Msg: TMessage);
{ This is called at the termination of each buffer.                     }
begin
  if Assigned(AudioComponent.FonOpen) then AudioComponent.FonOpen(Self);
end;

procedure TCallBackWinIn.WaveCloseIn(var Msg: TMessage);
{ This is called at the termination of each buffer.                     }
begin
  if Assigned(AudioComponent.FonClose) then AudioComponent.FonClose(Self);
end;

end.

