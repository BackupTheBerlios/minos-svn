object ClockDlg: TClockDlg
  Left = 361
  Top = 594
  Caption = 'Minos Logger Clock Correction'
  ClientHeight = 231
  ClientWidth = 389
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 24
    Width = 89
    Height = 13
    Caption = 'Computer Time'
  end
  object Label2: TLabel
    Left = 32
    Top = 56
    Width = 89
    Height = 13
    Caption = 'Corrected Time'
  end
  object RawTime: TLabel
    Left = 160
    Top = 24
    Width = 52
    Height = 13
    Caption = 'RawTime'
  end
  object CorrectedTime: TLabel
    Left = 160
    Top = 56
    Width = 85
    Height = 13
    Caption = 'CorrectedTime'
  end
  object Label5: TLabel
    Left = 30
    Top = 113
    Width = 66
    Height = 13
    Caption = 'Set date To'
  end
  object Label6: TLabel
    Left = 14
    Top = 153
    Width = 88
    Height = 13
    Caption = 'Modify Time By'
  end
  object OKButton: TButton
    Left = 16
    Top = 192
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OKButtonClick
  end
  object ClearButton: TButton
    Left = 106
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Clear'
    TabOrder = 1
    OnClick = ClearButtonClick
  end
  object ApplyButton: TButton
    Left = 197
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Apply'
    TabOrder = 2
    OnClick = ApplyButtonClick
  end
  object DayEdit: TLabeledEdit
    Left = 111
    Top = 111
    Width = 41
    Height = 21
    EditLabel.Width = 23
    EditLabel.Height = 13
    EditLabel.Caption = 'Day'
    TabOrder = 3
    Text = '1'
    OnChange = ClockEditChange
  end
  object MonthEdit: TLabeledEdit
    Left = 206
    Top = 111
    Width = 41
    Height = 21
    EditLabel.Width = 34
    EditLabel.Height = 13
    EditLabel.Caption = 'Month'
    TabOrder = 4
    Text = '1'
    OnChange = ClockEditChange
  end
  object YearEdit: TLabeledEdit
    Left = 296
    Top = 111
    Width = 41
    Height = 21
    EditLabel.Width = 26
    EditLabel.Height = 13
    EditLabel.Caption = 'Year'
    TabOrder = 5
    Text = '2000'
    OnChange = ClockEditChange
  end
  object HoursEdit: TLabeledEdit
    Left = 111
    Top = 151
    Width = 41
    Height = 21
    EditLabel.Width = 33
    EditLabel.Height = 13
    EditLabel.Caption = 'Hours'
    TabOrder = 6
    Text = '0'
    OnChange = ClockEditChange
  end
  object MinutesEdit: TLabeledEdit
    Left = 206
    Top = 151
    Width = 41
    Height = 21
    EditLabel.Width = 43
    EditLabel.Height = 13
    EditLabel.Caption = 'Minutes'
    TabOrder = 7
    Text = '0'
    OnChange = ClockEditChange
  end
  object SecondsEdit: TLabeledEdit
    Left = 298
    Top = 151
    Width = 41
    Height = 21
    EditLabel.Width = 48
    EditLabel.Height = 13
    EditLabel.Caption = 'Seconds'
    TabOrder = 8
    Text = '0'
    OnChange = ClockEditChange
  end
  object UpDown1: TUpDown
    Left = 152
    Top = 111
    Width = 16
    Height = 21
    Associate = DayEdit
    Min = 1
    Max = 31
    Position = 1
    TabOrder = 9
    Thousands = False
  end
  object UpDown2: TUpDown
    Left = 247
    Top = 111
    Width = 16
    Height = 21
    Associate = MonthEdit
    Min = 1
    Max = 12
    Position = 1
    TabOrder = 10
    Thousands = False
  end
  object UpDown3: TUpDown
    Left = 337
    Top = 111
    Width = 16
    Height = 21
    Associate = YearEdit
    Min = 2000
    Max = 2020
    Position = 2000
    TabOrder = 11
    Thousands = False
  end
  object UpDown4: TUpDown
    Left = 152
    Top = 151
    Width = 16
    Height = 21
    Associate = HoursEdit
    Min = -100
    TabOrder = 12
    Thousands = False
  end
  object UpDown5: TUpDown
    Left = 247
    Top = 151
    Width = 16
    Height = 21
    Associate = MinutesEdit
    Min = -100
    TabOrder = 13
    Thousands = False
  end
  object UpDown6: TUpDown
    Left = 339
    Top = 151
    Width = 16
    Height = 21
    Associate = SecondsEdit
    Min = -100
    TabOrder = 15
    Thousands = False
  end
  object CancelButton: TButton
    Left = 288
    Top = 191
    Width = 75
    Height = 25
    Caption = 'Cancel'
    Default = True
    TabOrder = 14
    OnClick = CancelButtonClick
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
  end
end
