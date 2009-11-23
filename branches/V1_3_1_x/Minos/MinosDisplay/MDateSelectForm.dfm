object DateSelectForm: TDateSelectForm
  Left = 0
  Top = 0
  Caption = 'DateSelectForm'
  ClientHeight = 241
  ClientWidth = 567
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DateLabel: TLabel
    Left = 100
    Top = 200
    Width = 98
    Height = 19
    Caption = '11/22/1933'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object MonthDownButton: TButton
    Left = 8
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Month <'
    TabOrder = 0
    OnClick = MonthDownButtonClick
  end
  object YearDownButton: TButton
    Left = 8
    Top = 136
    Width = 75
    Height = 25
    Caption = 'Year <<'
    TabOrder = 1
    OnClick = YearDownButtonClick
  end
  object MonthUpButton: TButton
    Left = 448
    Top = 56
    Width = 75
    Height = 25
    Caption = '> Month'
    TabOrder = 2
    OnClick = MonthUpButtonClick
  end
  object YearUpButton: TButton
    Left = 448
    Top = 136
    Width = 75
    Height = 25
    Caption = '>> Year'
    TabOrder = 3
    OnClick = YearUpButtonClick
  end
  object SelectButton: TButton
    Left = 248
    Top = 195
    Width = 75
    Height = 25
    Caption = 'Select'
    Default = True
    TabOrder = 4
    OnClick = SelectButtonClick
  end
  object CancelButton: TButton
    Left = 424
    Top = 195
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 1
    TabOrder = 5
    OnClick = CancelButtonClick
  end
  object CalendarGrid: TStringGrid
    Left = 100
    Top = 29
    Width = 325
    Height = 151
    ColCount = 7
    DefaultColWidth = 45
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 7
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected]
    ScrollBars = ssNone
    TabOrder = 6
    OnClick = CalendarGridClick
    OnDrawCell = CalendarGridDrawCell
  end
end
