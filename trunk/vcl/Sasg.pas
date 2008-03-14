// $Id$
{ ****************************************************************** }
{                                                                    }
{   Delphi component TSortAStringGrid                                }
{                                                                    }
{   Sort A StringGrid                                                }
{                                                                    }
{   Freeware 1997 by J.A. Godfried                                   }
{                                                                    }
{ ****************************************************************** }

{$ObjExportAll On}

unit SASG;

interface

uses WinTypes, WinProcs, Messages, SysUtils, Classes, Controls,
     Forms, Graphics, Grids, Dialogs;

type
   ToSortType  = (Character, Numeric, Date);
   SortDirType = (Ascending, Descending);
   TSortAStringGrid = class(TComponent)
      private
         FGridToSort : TStringGrid;
         FStartIndex : Integer;
         FEndIndex : Integer;
         FSortIndex : Integer;
         FSortType : ToSortType;
         FCaseSensitiv : Boolean;
         FSortDir : SortDirType;
         FShowMsg : Boolean;

         procedure AutoInitialize;
         procedure QuickSortGrid(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
         procedure BubbleSortGrid(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
         procedure qsortGrid(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
         procedure qsortGridNumeric(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
         procedure qsortGridDate(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);

      protected
         procedure Notification(AComponent : TComponent; Operation : TOperation); override;

      public
         ErrorCode : Integer;
         ErrorText : String;

         constructor Create(AOwner: TComponent); override;
         function Execute : Boolean;

      published
         property GridToSort : TStringGrid read FGridToSort write FGridToSort;
         property StartIndex : Integer read FStartIndex write FStartIndex default 0;
         property EndIndex : Integer read FEndIndex write FEndIndex default 0;
         property SortIndex : Integer read FSortIndex write FSortIndex default 0;
         property SortType : ToSortType read FSortType write FSortType default Character;
         property CaseSensitiv : Boolean read FCaseSensitiv write FCaseSensitiv default False;
         property SortDirection : SortDirType read FSortDir write FSortDir default Ascending;
         property ShowMessageOnError : Boolean read FShowMsg write FShowMsg default False;
  end;

procedure Register;

implementation

const
   MinErrCode = -4;
   MaxErrCode = 7;
   ErrorTextConst : Array [MinErrCode..MaxErrCode] of string = (
{-4}   'Case Sensitivity ignored for Date Sort.'
{-3}  ,'Case Sensitivity ignored for Numeical Sort.'
{-2}  ,'Column/Row contains non-date values for date sort. Sorted as Character.'
{-1}  ,'Column/Row contains non-numerical values for numeric sort. Sorted as Character.'
{ 0}  ,'Ok'
{ 1}  ,'No StringGrid given'
{ 2}  ,'StartIndex is greater or equal to EndIndex'
{ 3}  ,'StartIndex is less then 0'
{ 4}  ,'StartIndex is greater then number of rows/columns in StringGrid'
{ 5}  ,'EndIndex is greater then number of rows/columns in StringGrid'
{ 6}  ,'Sort Index is less then 0'
{ 7}  ,'Sort Index is greater then number of rows/columns in StringGrid'
   );

procedure Register;
begin
   RegisterComponents('MJGAddons', [TSortAStringGrid]);
end;

procedure TSortAStringGrid.AutoInitialize;
begin
   FGridToSort := nil;
   FStartIndex := 0;
   FEndIndex := 0;
   FSortIndex := 0;
   FSortType := Character;
   FCaseSensitiv := False;
   FSortDir := Ascending;
   FShowMsg := False;
end;

procedure TSortAStringGrid.Notification(AComponent : TComponent; Operation : TOperation);
begin
   inherited Notification(AComponent, Operation);
   if Operation <> opRemove then
      Exit;
   if AComponent = FGridToSort then
      FGridToSort := nil;
end;

constructor TSortAStringGrid.Create(AOwner: TComponent);
begin
   inherited Create(AOwner);
   AutoInitialize;
end;

function TSortAStringGrid.Execute : Boolean;
Var
   CheckForNum, NumCheck, NumErr : Integer;
   StrToChk : String;

begin
   ErrorCode := 0;
   If FGridToSort = NIL THEN
      ErrorCode := 1
   Else If FStartIndex >= FEndIndex THEN
      ErrorCode := 2
   Else If FStartIndex < 0 THEN
      ErrorCode := 3
   Else If FStartIndex > (FGridToSort.RowCount-1) THEN
      ErrorCode := 4
   Else If FEndIndex > (FGridToSort.RowCount-1) THEN
      ErrorCode := 5
   Else If FSortIndex < 0 THEN
      ErrorCode := 6
   Else If SortIndex > (FGridToSort.ColCount-1) THEN
      ErrorCode := 7
   Else If SortType = Numeric THEN
      For CheckForNum := FStartIndex TO FEndIndex DO
      Begin
         StrToChk := FGridToSort.Cols[FSortIndex].Strings[CheckForNum];
         if Length(Trim(StrToChk)) = 0 then
         begin
            NumCheck := 0; // treat blank as OK
            NumErr := 0
         end
         else
            Val(FGridToSort.Cols[FSortIndex].Strings[CheckForNum],NumCheck,NumErr);

         If NumErr <> 0 Then
         Begin
            ErrorCode := -1;
            SortType := Character
         end
         else
            If CaseSensitiv Then
               ErrorCode := -3;
      end
   Else If SortType = Date THEN
   begin
      If CaseSensitiv Then
         ErrorCode := -4;
      For CheckForNum := FStartIndex TO FEndIndex DO
      Begin
         StrToChk := FGridToSort.Cols[FSortIndex].Strings[CheckForNum];
         Try
            StrToDate(StrToChk)
         Except On EConvertError do
            begin
               ErrorCode := -2;
               SortType := Character
            end
         end
      end
   end;

   ErrorText := ErrorTextConst[ErrorCode];
   Result := True;
   If ErrorCode <= 0 Then
   begin
      QuickSortGrid(FGridToSort, FStartIndex, FEndIndex, FSortIndex);
      If (ErrorCode < 0) And FShowMsg Then
         MessageDlg(ErrorText,mtWarning,[mbOK],0)
   end
   else
   begin
      Result := False;
      If FShowMsg Then
         MessageDlg(ErrorText,mtError,[mbOK],0)
   end
end;

procedure TSortAStringGrid.QuickSortGrid(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
var
   j : Word;
   i : Word;
   sortGrid, revGrid, BufferGrid : TStringGrid;

Function UpString(Instring : String) : String;
var
   tel : byte;
   outstring : string;
begin
   OutString := InString;
   FOR tel := 1 TO length(Instring) DO
      OutString[tel] := upcase(OutString[tel]);
   UpString := OutString;
end;

begin
   sortGrid := TStringGrid.Create(Nil);
   sortGrid.RowCount := sGrid.RowCount;
   sortGrid.ColCount := 2;
   for j := StartIdx to EndIdx do
   begin
      sortGrid.Cells[0, j] := IntToStr(j);
      sortGrid.Cells[1, j] := sGrid.Cells[SortIdx, j]
   end;

   If SortType = Character Then
   begin
      If Not(CaseSensitiv) Then
         For j := StartIdx to EndIdx do
            SortGrid.Cells[1, j] := UpString(SortGrid.Cells[1, j]);
      qsortGrid(sortGrid, StartIdx, EndIdx, 1)
   end
   else if SortType = Numeric Then
      qsortGridNumeric(sortGrid, StartIdx, EndIdx, 1)
   else if SortType = Date Then
      qsortGridDate(sortGrid, StartIdx, EndIdx, 1);



   If SortDirection = Descending THEN
   begin
      revGrid := TStringGrid.Create(Nil);
      try
         revGrid.RowCount := sGrid.RowCount;
         revGrid.ColCount := 2;

         FOR j := EndIdx DOWNTO StartIdx DO
         begin
            revGrid.rows[EndIdx-j+StartIdx] := sortGrid.rows[j]
         end;

         FOR j := EndIdx DOWNTO StartIdx DO
         begin
            sortGrid.rows[j] := revGrid.rows[j]
         end;
      finally
         revGrid.Free;
      end;
   end;

   // this nicked from sortgrid.pas

   //Now rearrange the rows of the grid in sorted order.
   //This is a fast but space inefficient way to do it.
   //First, create a buffer grid and size it correctly.
   BufferGrid:=TStringGrid.Create(sGrid);
   try
     BufferGrid.ColCount:=sGrid.ColCount;
     BufferGrid.RowCount:=sGrid.RowCount;
     //Copy the rows to the buffer grid in sorted order.
     for i:=StartIdx to EndIdx do
     begin
          BufferGrid.Rows[i].Assign(sGrid.Rows[StrToInt(sortGrid.Cells[0,i])]);
     end;
     //Now put the rows back into the original grid.
     for i:=StartIdx to EndIdx do
     begin
          sGrid.Rows[i].Assign(BufferGrid.Rows[i]);
     end;
   finally
         BufferGrid.Free;
         SortGrid.Free;
   end;
end;

procedure TSortAStringGrid.BubbleSortGrid(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
Var
   Idx : Word;
   Changed : Boolean;
   tempRow : TStringList;
   fields, i : Word;

begin
   tempRow :=TStringList.Create;
   fields := sGrid.ColCount;
   repeat
      Changed := False;
      for Idx := StartIdx to EndIdx-1 do
      begin
         if sGrid.Cells[SortIdx, Idx] > sGrid.Cells[SortIdx, Idx+1] then
         begin
            tempRow.Clear;
            for i := 0 to fields - 1 do
               tempRow.Add(sGrid.cells[i, Idx+1]);
            sGrid.rows[Idx+1] := sGrid.rows[Idx];
            for i := 0 to fields - 1 do
               sGrid.cells[i, Idx] := tempRow.Strings[i];
            Changed := True;
         end;
      end;
   until Changed = False;
   tempRow.Free;
end;

function sfloat(s :String):Extended;
Var
   temp : Extended;
begin
      try
         temp := StrToFloat(s);
      Except On EConvertError do
         temp := 0
      end;

      sfloat := temp;
end;

procedure TSortAStringGrid.qsortGridNumeric(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
Var
   x, y : Word;
   temp: Extended;
   tempRow : TStringList;
   ind : Word;
   fields, i : Word;
begin
   tempRow :=TStringList.Create;
   fields := sGrid.ColCount;
   if StartIdx < EndIdx then
   begin
      x:= StartIdx;
      y:= EndIdx;
      ind := (StartIdx+EndIdx) div 2;
      temp := sfloat(sGrid.cells[SortIdx, ind]);
      while x <= y do
      begin
         while sfloat(sGrid.cells[SortIdx, x]) < temp do
            Inc(x);
         while sfloat(sGrid.cells[SortIdx, y]) > temp do
            Dec(y);
         if x <= y then
         begin
            tempRow.Clear;
            for i := 0 to fields - 1 do
               tempRow.Add(sGrid.cells[i, x]);
            sGrid.rows[x] := sGrid.rows[y];
            for i := 0 to fields - 1 do
               sGrid.cells[i, y] := tempRow.Strings[i];
            Inc(x);
            Dec(y);
         end;
      end;
      tempRow.Free;
      qsortGridNumeric(sGrid, StartIdx, y, SortIdx);
      qsortGridNumeric(sGrid, x, EndIdx, SortIdx);
   end;
end;

procedure TSortAStringGrid.qsortGridDate(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
Var
   x, y : Word;
   temp: TDateTime;
   tempRow : TStringList;
   ind : Word;
   fields, i : Word;
begin
   tempRow :=TStringList.Create;
   fields := sGrid.ColCount;
   if StartIdx < EndIdx then
   begin
      x:= StartIdx;
      y:= EndIdx;
      ind := (StartIdx+EndIdx) div 2;
      temp := StrToDate(sGrid.cells[SortIdx, ind]);
      while x <= y do
      begin
         while StrToDate(sGrid.cells[SortIdx, x]) < temp do
               Inc(x);
         while StrToDate(sGrid.cells[SortIdx, y]) > temp do
               Dec(y);
         if x <= y then
         begin
            tempRow.Clear;
            for i := 0 to fields - 1 do
               tempRow.Add(sGrid.cells[i, x]);
            sGrid.rows[x] := sGrid.rows[y];
            for i := 0 to fields - 1 do
               sGrid.cells[i, y] := tempRow.Strings[i];
            Inc(x);
            Dec(y);
         end;
      end;
      tempRow.Free;
      qsortGridDate(sGrid, StartIdx, y, SortIdx);
      qsortGridDate(sGrid, x, EndIdx, SortIdx);
   end;
end;

procedure TSortAStringGrid.qsortGrid(sGrid : TStringGrid; StartIdx, EndIdx, SortIdx : Integer);
Var
   x, y : Word;
   temp: String;
   tempRow : TStringList;
   ind : Word;
   fields, i : Word;

begin
   if (EndIdx-StartIdx) < 5 then
      BubbleSortGrid(sGrid, StartIdx, EndIdx, SortIdx)
   else
   begin
      tempRow :=TStringList.Create;
      fields := sGrid.ColCount;
      if StartIdx < EndIdx then
      begin
         x:= StartIdx;
         y:= EndIdx;
         ind := (StartIdx+EndIdx) div 2;
         temp := sGrid.cells[SortIdx, ind];
         while x <= y do
         begin
            while sGrid.cells[SortIdx, x] < temp do
               Inc(x);
            while sGrid.cells[SortIdx, y] > temp do
               Dec(y);
            if x <= y then
            begin
               tempRow.Clear;
               for i := 0 to fields - 1 do
                  tempRow.Add(sGrid.cells[i, x]);
               sGrid.rows[x] := sGrid.rows[y];
               for i := 0 to fields - 1 do
                  sGrid.cells[i, y] := tempRow.Strings[i];
               Inc(x);
               Dec(y);
            end;
         end;
         tempRow.Free;
         qsortGrid(sGrid, StartIdx, y, SortIdx);
         qsortGrid(sGrid, x, EndIdx, SortIdx);
      end;
   end;
end;

end.
