unit DrawBox;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs;

type
  TDrawBox = class(TCustomControl)
  private
    { Private declarations }
    FRedraw: TNotifyEvent;
    FBackGroundColor: tColor;
    procedure setBackgroundColor(val: tColor);
    procedure Resize;


  protected
    { Protected declarations }
    myWidth: Integer;
    myHeight: Integer;
    theBitmap: TBitmap;
    procedure PAINT; override;
    procedure WMEraseBkgnd(var M: TWMEraseBkgnd); message WM_ERASEBKGND;
    procedure WMSize(var Msg: TWMSize); message WM_SIZE;
    procedure SetBounds(ALeft, ATop, AWidth, AHeight: Integer); override;
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Redraw; virtual;
    procedure ChildResize; virtual;
    procedure ClearBmp;

  published
    { Published declarations }
    property BackGroundColor: tColor read FBackGroundColor write setBackgroundColor default clSilver;
  end;

implementation

constructor TDrawBox.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  theBitmap := TBitmap.Create;
  Width := 50;
  Height := 50;
  BackGroundColor := clSilver;
  Resize;
  ClearBmp;
  myWidth := Width;
  myHeight := Height;
end;

destructor TDrawBox.Destroy;
begin
  theBitmap.Free;
  inherited Destroy;
end;

procedure TDrawBox.SetBounds(ALeft, ATop, AWidth, AHeight: Integer);
begin
  Inherited;
  Resize;
end;

procedure TDrawBox.Resize;
begin
  if not (theBitmap = nil) then
    if not ((myWidth = Width) and (myHeight = Height)) then
    begin
      theBitmap.Width := Width;
      theBitmap.Height := Height;
      myWidth := Width;
      myHeight := Height;
      ChildResize;
      Redraw;
      Invalidate;
    end;
end;

procedure TDrawBox.ChildResize;
begin
end;

procedure TDrawBox.WMSize(var Msg: TWMSize);
begin
  inherited;
  resize;
end;

procedure TDrawBox.ClearBmp;
var NewRect: TRect;
begin
  theBitmap.Canvas.BRUSH.COLOR := FBackGroundColor;
  theBitmap.Canvas.BRUSH.Style := bsSolid;

  NewRect := Rect(0, 0, myWidth, myHeight);
  theBitmap.Canvas.FillRect(NewRect);
end;

procedure TDrawBox.PAINT;
begin
  if not ((myWidth = Width) and (myHeight = Height)) then
    Resize
  else
  begin
    Canvas.CopyMode := cmSrcCopy;
    Canvas.Draw(0, 0, theBitmap);
  end;
end;

procedure TDrawBox.Redraw;
begin
  if not ((theBitmap = nil) or (Parent = nil)) then
  begin
    ClearBmp;
    Canvas.MOVETO(0, 0);
    Canvas.LineTo(Width, Height);
    Canvas.MOVETO(0, Height);
    Canvas.LineTo(Width, 0);
  end;
end;

procedure TDrawBox.setBackgroundColor(val: tColor);
begin
  FBackGroundColor := val;
  Redraw;
  invalidate;
end;

procedure TDrawBox.WMEraseBkgnd(var M: TWMEraseBkgnd);
begin
  M.Result := LRESULT(False);
end;
{
procedure register;
begin
  RegisterComponents('Samples',[TDrawBox]);
end;
}
end.

