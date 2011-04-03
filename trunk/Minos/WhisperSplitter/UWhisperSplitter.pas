unit UWhisperSplitter;
{
  Official Notice:

  VCL based TSplitter extension.
  Version 1.0.4, November, 2008
  Original Author: Eric Himmer, Whisper Computer Solutions, Inc.

  Modified by: <enter your info here if you modify the unit>

  Provides extensions to the standard TSplitter control.  Extensions include:
  - Gradient color (based on the Color property of the TSplitter)
  - Highlight color (also gradient)
  - Single click minimize/restore
  - Image drawing inside the splitter
  - Auto-restore
  - Programatical minimize/restore methods

  Development Environments tested on:
  - CodeGear RAD Studio 2007, i.e. Delphi 2007 and CBuilder 2007
  - Delphi 7
  - Delphi 5

  Properties:
    HightlightColor
    - Specify a color to use when the mouse cursor is over the splitter
      (produced as a gradient as well).  Default is clWhite gradient.
      example: fSplitterLeft.HighlightColor := clSkyBlue;
    Bitmap
    - Optionally specify a bitmap to use inside the splitter. The demo
      application provided with this unit has an example bitmap image of an
      inset dot that you can also use freely.  The bitmap will be centered
      within the splitter.  The default is nil, in which case no image will be
      shown.
    BitmapInterations
    - Number of times to repeat the Bitmap image within the splitter.  The
      default is 10.  If used with the inset dot bitmap included with the demo
      application, it will produce a nice string of inset dots to indicate a
      grip type handle.
    AutoRestore
    - Set to true if you want to have the splitter/panel automatically restore
      itself from a minimized state when the mouse cursor enters the splitter.
      Default is false.
    Minimized
    - Read only property which indicates if the splitter/panel is in the
      minimized state.

  Methods:
    Restore
    - Use to restore the splitter/panel to its last non-minimized size.
    Minimize
    - Use to manually minimize the splitter/panel.
    Create
    - Create an instance of the TSplitter extension functionality.

  Operational Events:
    Click
    - Minimizes or restores the splitter/panel (toggles the state).
    MouseMove
    - If the mouse is not over the splitter, the TSplitter.Color property is
      used (drawn in gradient form).
    - If the mouse is over the splitter, the HighlightColor property of the
      corresponding TWhisperSplitter instance is used (drawn in gradient form).
    - If AutoRestore property is set to true and the splitter/panel is
      minimized, the splitter/panel will automatically restore itself to the
      size it was befoire being minimized.

  License:
    You are authorized to use this extension freely in any commercial or
    non-commercial application, whether sold or distributed free.  However,
    this unit cannot be used to create a splitter component that would
    be sold (if used in a splitter component, that component must also be
    distributed as freeware, unless provided as part of a component set).
    All I ask is that you keep this complete "Official Notice" at the head of
    the unit and note whether you've modified it from the original.
    Please, if you would be so kind as to forward any bug fixes, suggestions
    or enhancement to:
      Whisper Computer Solutions, Inc.
      www.WhisperSolutions.com
      support@whispersolutions.com

  Usage (see the included demo application as a reference):
    - Include this unit in your application.
    - Add a standard TPanel that is justified left, right, top or bottom.
    - Add standard TSplitter to your form and set the properties as desired
      to be used to size the TPanel you added in the previous step.
    - Add a private TWhisperSplitter object variable and name it whatever you
      want.  example: fSplitterLeft: TWhisperSplitter;
    - In your FormCreate event, create an instance of the object variable.
      example: fSplitterLeft := TWhisperSplitter.Create(SplitterLeft, Panel1);
      where SplitterLeft is the name of the TSplitter, and Panel1 is the name
      of the TPanel you added that is being sized by the TSplitter.
    - Set any optional properties (see the section on Properties above)
    - In your FormDestroy event, free the TWhisperSplitter object.
      examples: "SplitterLeft.Free" or "FreeAndNil(SplitterLeft);"


  Thank you... Hope it serves you well!
}

{$IFDEF VER185}
  {$DEFINE D2007}
{$ENDIF}
{$IFDEF VER180}
  {$DEFINE D2006}
{$ENDIF}
{$IFDEF VER150}
  {$DEFINE DELPHI7}
  {$DEFINE PRE_D2006}
{$ENDIF}
{$IFDEF VER140}
  {$DEFINE DELPHI6}
  {$DEFINE PRE_D2006}
{$ENDIF}
{$IFDEF VER130}
  {$DEFINE DELPHI5}
  {$DEFINE PRE_D2006}
{$ENDIF}

interface
{$TYPEINFO ON}
uses
{$IFNDEF PRE_D2006}
  Windows, Classes, ExtCtrls, Graphics, Controls;
{$ELSE}
  Windows, Classes, ExtCtrls, Graphics, Controls, Messages, Contnrs, SysUtils;
{$ENDIF}

{$IFDEF PRE_D2006}
type
  myCOLOR16 = Word;
  myPTriVertex = ^myTTriVertex;
  _myTRIVERTEX = packed record
    x: Longint;
    y: Longint;
    Red: myCOLOR16;
    Green: myCOLOR16;
    Blue: myCOLOR16;
    Alpha: myCOLOR16;
  end;
  myTTriVertex = _myTRIVERTEX;
  myTRIVERTEX = _myTRIVERTEX;
{$ENDIF}

type
  TExtSplitter = class(TSplitter)
  published
    property OnClick;
{$IFNDEF PRE_D2006}
    property OnMouseEnter;
    property OnMouseLeave;
{$ENDIF}
    property OnMoved;
    property OnPaint;
  end;

//{$TYPEINFO ON}
type
  TWhisperSplitter = class(TObject)
  private
    fSplitter: TSplitter;
    fPanel: TPanel;
    fAutoRestore: Boolean;
    fBitmap: TBitmap;
    fBitmapInterations: Integer;
    fColor: TColor;
    fHighlightColor: TColor;
    fMinimized: Boolean;
    fRestoreSize: Integer;
    fPanelSize: Integer;
{$IFDEF PRE_D2006}
    FOldWindowProc: TWndMethod;
{$ENDIF}
    FOnSplitterMoved: TNotifyEvent;
    procedure SplitterClick(Sender: TObject);
    procedure SplitterMouseEnter(Sender: TObject);
    procedure SplitterMouseLeave(Sender: TObject);
    procedure SplitterMoved(Sender: TObject);
    procedure SplitterPaint(Sender: TObject);
{$IFDEF PRE_D2006}
    procedure WindowProc(var Message: TMessage);
{$ENDIF}
  published
    property AutoRestore: Boolean read fAutoRestore write fAutoRestore;
    property Bitmap: TBitmap read fBitmap write fBitmap;
    property BitmapInterations: Integer read fBitmapInterations write fBitmapInterations;
    property HighlightColor: TColor read fHighlightColor write fHighlightColor;
    property Minimized: Boolean read fMinimized;

    property OnSplitterMoved: TNotifyEvent read FOnSplitterMoved write FOnSplitterMoved;
  public
    procedure Restore;
    procedure Minimize;
    constructor Create(pSplitter: TSplitter; pPanel: TPanel);
{$IFDEF PRE_D2006}
    destructor Destroy; override;
{$ENDIF}
  end;


implementation

constructor TWhisperSplitter.Create(pSplitter: TSplitter; pPanel: TPanel);
begin
  inherited Create;
  fSplitter := pSplitter;
  fPanel := pPanel;
  fAutoRestore := false;
  fBitmap := nil;
  fBitmapInterations := 10;
  fColor := fSplitter.Color;
  fHighlightColor := clWhite;
  fMinimized := false;
  TExtSplitter(fSplitter).OnClick := SplitterClick;
{$IFNDEF PRE_D2006}
  TExtSplitter(fSplitter).OnMouseEnter := SplitterMouseEnter;
  TExtSplitter(fSplitter).OnMouseLeave := SplitterMouseLeave;
{$ENDIF}
  OnSplitterMoved := TExtSplitter(fSplitter).OnMoved;
  TExtSplitter(fSplitter).OnMoved := SplitterMoved;
  TExtSplitter(fSplitter).OnPaint := SplitterPaint;
  if fSplitter.Align in [alTop, alBottom] then
    fPanelSize := fPanel.Height
  else
    fPanelSize := fPanel.Width;
  fRestoreSize := fPanelSize;
{$IFDEF PRE_D2006}
  FOldWindowProc := pSplitter.WindowProc;
  pSplitter.WindowProc := Self.WindowProc;
{$ENDIF}
end;

{$IFDEF PRE_D2006}
destructor TWhisperSplitter.Destroy;
begin
  if fSplitter <> nil then
  try
    fSplitter.WindowProc := FOldWindowProc;
  except
  end;
  inherited;
end;
{$ENDIF}

{$IFDEF PRE_D2006}
procedure TWhisperSplitter.WindowProc(var Message: TMessage);
begin
  case Message.Msg of
    CM_MOUSEENTER:  SplitterMouseEnter(Self);
    CM_MOUSELEAVE:  SplitterMouseLeave(Self);
  else
    FOldWindowProc(Message);
  end;
end;
{$ENDIF}

procedure TWhisperSplitter.Restore;
begin
  if Self <> nil then
  begin
    if (fSplitter.Align in [alTop, alBottom]) and
       (fPanel.Height <= fSplitter.MinSize) then
    begin
      fPanelSize := fRestoreSize;
      fMinimized := false;
      SplitterMoved(nil);
    end
    else if (fSplitter.Align in [alLeft, alRight]) and
            (fPanel.Width <= fSplitter.MinSize) then
    begin
      fPanelSize := fRestoreSize;
      fMinimized := false;
      SplitterMoved(nil);
    end;
  end;
end;

procedure TWhisperSplitter.Minimize;
begin
  if Self <> nil then
  begin
    if (fSplitter.Align in [alTop, alBottom]) and
       (fPanel.Height > fSplitter.MinSize) then
    begin
      fRestoreSize := fPanel.Height;
      fPanelSize := fSplitter.MinSize;
      fMinimized := true;
      SplitterMoved(nil);
    end
    else if (fSplitter.Align in [alLeft, alRight]) and
            (fPanel.Width > fSplitter.MinSize) then
    begin
      fRestoreSize := fPanel.Width;
      fPanelSize := fSplitter.MinSize;
      fMinimized := true;
      SplitterMoved(nil);
    end;
  end;
end;

procedure TWhisperSplitter.SplitterMouseEnter(Sender: TObject);
begin
  fSplitter.Color := fHighlightColor;
  if fAutoRestore then
    Self.Restore;
end;

procedure TWhisperSplitter.SplitterMouseLeave(Sender: TObject);
begin
  fSplitter.Color := fColor;
end;

procedure TWhisperSplitter.SplitterMoved(Sender: TObject);
var
  vLeft, vTop: Integer;
begin
  if fSplitter.Align in [alTop, alBottom] then
  begin
    if (fPanel.Height <> fRestoreSize) and
       (fPanel.Height <> fSplitter.MinSize) then
    begin
      fPanelSize := fPanel.Height;
    end;
    if fSplitter.Align = alTop then
      fPanel.Height := fPanelSize
    else
    begin
      vTop := fPanel.Top + (fPanel.Height - fPanelSize);
      fPanel.SetBounds(fPanel.Left, vTop, fPanel.Width, fPanelSize);
    end;
    fMinimized := ( fPanel.Height <= fSplitter.MinSize );
  end else if fSplitter.Align in [alLeft, alRight] then
  begin
    if (fPanel.Width <> fRestoreSize) and
       (fPanel.Width <> fSplitter.MinSize) then
    begin
      fPanelSize := fPanel.Width;
    end;
    if fSplitter.Align = alLeft then
      fPanel.Width := fPanelSize
    else
    begin
      vLeft := fPanel.Left + (fPanel.Width - fPanelSize);
      fPanel.SetBounds(vLeft, fPanel.Top, fPanelSize, fPanel.Height);
    end;
    fMinimized := ( fPanel.Width <= fSplitter.MinSize );
  end;
  fPanel.Invalidate;
  OnSplitterMoved(Sender);
end;

procedure TWhisperSplitter.SplitterClick(Sender:TObject);
begin
  if fSplitter.Align in [alTop, alBottom] then
  begin
    if fPanel.Height > fSplitter.MinSize then
    begin
      fRestoreSize := fPanel.Height;
      fPanelSize := fSplitter.MinSize;
      fMinimized := true;
    end else
    begin
      fPanelSize := fRestoreSize;
      fMinimized := false;
    end;
  end else if fSplitter.Align in [alLeft, alRight] then
  begin
    if fPanel.Width > fSplitter.MinSize then
    begin
      fRestoreSize := fPanel.Width;
      fPanelSize := fSplitter.MinSize;
      fMinimized := true;
    end else
    begin
      fPanelSize := fRestoreSize;
      fMinimized := false;
    end;
  end;
end;

procedure TWhisperSplitter.SplitterPaint(Sender: TObject);
var
  vButtonRect: TRect;
  i: Integer;
{$IFDEF PRE_D2006}
  vVertexArray: array [0..1] of myTTriVertex;
  vVertices: array [0..1] of TTriVertex ABSOLUTE vVertexArray;
{$ELSE}
  vVertexArray: array [0..1] of TTriVertex;
{$ENDIF}
  vGradientRect: TGradientRect;

{$IFDEF PRE_D2006}
  procedure SetVertexColor(var pVertex: myTTriVertex; pColor: TColor);
{$ELSE}
  procedure SetVertexColor(var pVertex: TTriVertex; pColor: TColor);
{$ENDIF}
  var
    vRGB: LongInt;
  begin
    vRGB := ColorToRGB(pColor);
    pVertex.Red := (vRGB and $FF) shl 8;
    pVertex.Green := (vRGB and $FF00);
    pVertex.Blue := (vRGB and $FF0000) shr 8;
    pVertex.Alpha := 0;
  end;

begin
  // Setup the gradient
  vVertexArray[0].x := fSplitter.ClientRect.Left;
  vVertexArray[0].y := fSplitter.ClientRect.Top;
  SetVertexColor(vVertexArray[0], fSplitter.Color);
  vVertexArray[1].x := fSplitter.ClientRect.Right;
  vVertexArray[1].y := fSplitter.ClientRect.Bottom;
  vVertexArray[1].Red := vVertexArray[0].Red div 2;
  vVertexArray[1].Green := vVertexArray[0].Green div 2;
  vVertexArray[1].Blue := vVertexArray[0].Blue div 2;
  vVertexArray[1].Alpha := 0;
  vGradientRect.UpperLeft := 0;
  vGradientRect.LowerRight := 1;
  if fSplitter.Align in [alTop, alBottom] then
  begin
    // Draw a horizontal gradient splitter bar
{$IFDEF PRE_D2006}
    GradientFill(fSplitter.Canvas.Handle, vVertices[0], 2, @vGradientRect, 1, GRADIENT_FILL_RECT_V);
{$ELSE}
    GradientFill(fSplitter.Canvas.Handle, @vVertexArray, 2, @vGradientRect, 1, GRADIENT_FILL_RECT_V);
{$ENDIF}
    if fBitmap <> nil then
    begin
      // Draw the splitter image horizontally
      vButtonRect.Top := (fSplitter.ClientRect.Bottom - fBitmap.Height) div 2;
      vButtonRect.Left := (fSplitter.ClientRect.Right - (fBitmapInterations * fBitmap.Width)) div 2;
      vButtonRect.Bottom := vButtonRect.Top + fBitmap.Height;
      vButtonRect.Right := vButtonRect.Left + (fBitmapInterations * fBitmap.Width);
      for i := 0 to fBitmapInterations-1 do
        fSplitter.Canvas.Draw(vButtonRect.Left + (i * fBitmap.Height), vButtonRect.Top, fBitmap);
    end;
  end else if fSplitter.Align in [alLeft, alRight] then
  begin
    // Draw a vertical gradient splitter bar
{$IFDEF PRE_D2006}
    GradientFill(fSplitter.Canvas.Handle, vVertices[0], 2, @vGradientRect, 1, GRADIENT_FILL_RECT_H);
{$ELSE}
    GradientFill(fSplitter.Canvas.Handle, @vVertexArray, 2, @vGradientRect, 1, GRADIENT_FILL_RECT_H);
{$ENDIF}
    if fBitmap <> nil then
    begin
      // Draw the splitter image vertically
      vButtonRect.Top := (fSplitter.ClientRect.Bottom - (fBitmapInterations * fBitmap.Height)) div 2;
      vButtonRect.Left := (fSplitter.ClientRect.Right - fBitmap.Width) div 2;
      vButtonRect.Bottom := vButtonRect.Top + (fBitmapInterations * fBitmap.Height);
      vButtonRect.Right := vButtonRect.Left + fBitmap.Width;
      for i := 0 to fBitmapInterations-1 do
        fSplitter.Canvas.Draw(vButtonRect.Left, vButtonRect.Top + (i * fBitmap.Height), fBitmap);
    end;
  end;
 end;

end.
