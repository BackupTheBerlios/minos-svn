unit FBVUMeter;
{$ObjExportAll On}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  DrawBox;

type
  TFBVUMeter = class(TDrawBox)
  private
    { Private declarations }
    FLeft: single;
    FRight: single;

    Peak: array[0..1] of integer;
    PeakCount: array[0..1] of integer;

    FLeftIndex: integer;
    FRightIndex: integer;

    FMax: integer;
    FMin: integer;
    FHighLevel: integer;
    FMedLevel: integer;
    FPenWIdth : integer;

    FStereo: boolean;
    x1: array[0..1] of integer;
    x2: array[0..1] of integer;
    Current: array[0..1] of integer;

    // 3 colors, bright and dimmed
    FHigh, FDHigh, FMed, FDMed, FLow, FDLow: TColor;

    // to make it easy, an array of the values and colors for all lines
    levels: array[0..100] of single;
    lvlColor: array[0..100] of TColor;
    lvlDColor: array[0..100] of TColor;

    numberLevels: integer;

    procedure setLevels;
    procedure drawLine(line: integer; dimmed: boolean; channel: integer);
    procedure correctChannel(val: Single; channel: integer);
  protected
    { Protected declarations }
    procedure ReDraw; override;
    procedure ChildResize; override;

    procedure setMax(max: integer);
    procedure setMin(min: integer);

    procedure setHighLevel(level: integer);
    procedure setMedLevel(level: Integer);

    procedure setHighColor(col: TColor);
    procedure setMedColor(col: TColor);
    procedure setLowColor(col: TColor);
    procedure setColors;
    procedure setStereo(val: boolean);
    procedure setPenWIdth(val: integer);
  public
    { Public declarations }
    constructor Create(aOwner: TComponent); override;
    procedure SetValue(left, right: single);
  published
    { Published declarations }
    property levelMax: integer read FMax write setMax default 0;
    property levelMin: integer read FMin write setMin default -80;

    property levelHigh: integer read FHighLevel write setHighLevel default -6;
    property levelMed: integer read FMedLevel write setMedLevel default -12;

    property colorHigh: TColor read FHigh write setHighColor default clRed;
    property colorMed: TColor read FMed write setMedColor default clYellow;
    property colorLow: TColor read FLow write setLowColor default clLime;

    property stereo: boolean read FStereo write setStereo default true;
    property penWidth: integer read FPenwidth write setPenWidth Default 2;
  end;

procedure Register;

implementation

uses math;

function SetShadow(color: TColor): TColor;
type
  ColorConvert = record
    case byte of
      0: (z: TColor);
      1: (a, b, c, d: byte);
  end;
begin
  ColorConvert(result).a := ColorConvert(color).a div 2;
  ColorConvert(result).b := ColorConvert(color).b div 2;
  ColorConvert(result).c := ColorConvert(color).c div 2;
  ColorConvert(result).d := ColorConvert(color).d div 2;
end;

procedure TFBVUMeter.setMax(max: integer);
begin
  FMax := max;
  if (FMax < FMin) then
    FMax := FMin + 1;
  setLevels;
end;

procedure TFBVUMeter.setMin(min: integer);
begin
  Fmin := min;
  if FMin > FMax then
    FMin := FMax - 1;
  setLevels;
end;

procedure TFBVUMeter.setHighLevel(level: integer);
begin
  FHighLevel := level;
  if FHighLevel > FMax then
    FHighLevel := FMax;
  if FHighLevel < FMin then
    FHighLevel := FMin;
  setLevels;
end;

procedure TFBVUMeter.setMedLevel(level: Integer);
begin
  FMedLevel := level;
  if FMedLevel > FMax then
    FMedLevel := FMax;
  if FMedLevel < FMin then
    FMedLevel := FMin;
  setLevels;
end;

procedure TFBVUMeter.setLevels;
var
  i: integer;
//  x, y: extended;
  range: integer;
  levelMed: integer;
  levelHigh: integer;
  Xmiddle : integer;
begin
  numberLevels := ((height - 2) div 3) - 1;

  if numberLevels = 0 then
    numberLevels := 1;
  if numberLevels > 100 then
    numberLevels := 100;

  range := FMax - FMin;
  if range < 1 then range := 1;

  levelMed := numberLevels - round((FMax - FMedLevel) * numberLevels / range);
  levelHigh := numberLevels - round((FMax - FHIghLevel) * numberLevels / range);

  for i := numberLevels downto 0 do
  begin
    levels[i] := power(10, -1.0 * (numberLevels - i) / numberLevels * (range / 20.0));
    if i > levelHigh then
    begin
      lvlColor[i] := FHigh;
      lvlDColor[i] := FDHigh;
    end
    else if i > levelMed then
    begin
      lvlColor[i] := FMed;
      lvlDColor[i] := FDMed;
    end
    else
    begin
      lvlColor[i] := FLow;
      lvlDColor[i] := FDLow;
    end;
  end;

  if FStereo then
    Xmiddle := Width div 2
  else
    XMiddle := Width;
  x1[0] := 1;
  x2[0] := XMiddle - 2;
  x1[1] := XMiddle + 1;
  x2[1] := Width - 2 ;

  Redraw;
end;

procedure TFBVUMeter.setHighColor(col: TColor);
begin
  FHigh := col;
  setColors;
end;

procedure TFBVUMeter.setMedColor(col: TColor);
begin
  FMed := col;
  setColors;
end;

procedure TFBVUMeter.setLowColor(col: TColor);
begin
  FLow := col;
  setColors;
end;

procedure TFBVUMeter.setColors;
begin
  FDHigh := SetShadow(FHigh);
  FDMed := SetShadow(FMed);
  FDLow := SetShadow(FLow);
  Redraw;
end;

procedure TFBVUMeter.setStereo(val: boolean);
begin
  if not (val = FStereo) then
  begin
    FStereo := val;
    setLevels;
  end;
end;

procedure TFBVUMeter.setPenWidth(val: integer);
begin
  if not (val = FPenWidth) then
  begin
    FPenWidth := val;
    ReDraw;
  end;
end;

constructor TFBVUMeter.Create(aOwner: TComponent);
begin
  inherited Create(aOwner);
  Height := 152;
  Width := 32;
  FLeft := 0.0;
  FRight := 0.0;
  FLeftIndex := 0;
  FRightIndex := 0;
  FPenWidth := 2;

  FHigh := clRed;
  FMed := clYellow;
  FLow := clLime; //$0000FF00; //clGreen;

  setColors;

  FMax := 0;
  FMin := -50;
  FHighLevel := -6;
  FMedLevel := -12;

  setLevels;

  FStereo := false;
  setStereo(true); // force calculation

  Redraw;
end;

procedure TFBVUMeter.ChildResize;
begin
  setLevels;
end;

procedure TFBVUMeter.drawLine(line: integer; dimmed: boolean; channel: integer);
begin
  with theBitmap.canvas do
  begin
    pen.Width := Penwidth;
    if dimmed then
      pen.Color := lvlDColor[line]
    else
      pen.Color := lvlColor[line];
    MoveTo(x1[channel], Height - (line * 3) - 2);
    LineTo(x2[channel], Height - (line * 3) - 2);
  end;
end;

procedure TFBVUMeter.Redraw;
var
  i: integer;
begin
  if not ((theBitmap = nil) or (Parent = nil)) then
  begin
    ClearBmp;
    for i := 0 to numberLevels do
    begin
      drawLine(i, true, 0);
      if FStereo then
        drawLine(i, true, 1);
    end;
  end;
  Current[0] := 0;
  Current[1] := 0;
  Peak[0] := 0;
  Peak[1] := 0;


{  with theBitmap.canvas do
  begin
    curColor := FLow;
    pen.Color := curCOlor;
    pen.Width := 2;
    for c := 0 to 1 do
    begin
    dimmed := false;
    if c = 0 then
    begin
    val    := FLeft;
    x1     := 3;
    x2     := width div 2 - 2;
    end
    else
    begin
    val    := FRight;
    x1     := x2 + 3;
    x2     := width - 3;
    end;
    Pen.Color := FLow;
    curColor := FLow;
    for i := 0 to numberLevels do
    begin
      if not dimmed then
        if val < levels[i] then
          dimmed := true;
      if dimmed then
      begin
        if not (curColor = lvlDColor[i]) then
        begin
          curColor := lvlDColor[i];
          Pen.Color := curColor;
        end;
      end
      else
      begin
        if not (curColor = lvlColor[i]) then
        begin
          curColor := lvlColor[i];
          Pen.Color := curColor;
        end;
      end;
      MoveTo(x1,Height-(i*3)-2);
      LineTo(x2,Height-(i*3)-2);
    end;
    end;
  end;
  END;}
end;

procedure TFBVUMeter.correctChannel(val: Single; channel: integer);
var
  i: integer;
begin
  i := Current[channel];
  // try up
  if val > levels[i] then
  begin
    while ((i < numberLevels) and (val > levels[i])) do
    begin
      drawLine(i, false, channel);
      inc(i);
    end;
  end
  else
  begin
    while ((i > 0) and (val <= levels[i])) do
    begin
      dec(i);
      drawLine(i, true, channel);
    end;

  end;
  Current[channel] := i;

   if i >= Peak[channel] then
   begin
      Peak[channel] := i;
      PeakCount[channel] := 20;
   end
   else
   begin
     dec(PeakCount[channel]);

    if PeakCount[channel] <= 0 then
    begin
      drawLine(Peak[channel], true, channel);
      PeakCount[channel] := 0;
      Peak[channel] := i;
    end
    else
      drawLine(Peak[channel], false, channel);
 end;

end;

procedure TFBVUMeter.SetValue(left, right: single);
//var
//  i: integer;
begin
  FLeft := left;
  FRight := right;
  correctChannel(Left, 0);
  if FStereo then
    correctChannel(right, 1);
  Paint;
end;

procedure Register;
begin
  RegisterComponents('Snd', [TFBVUMeter]);
end;

end.

