object LocCalcForm: TLocCalcForm
  Left = 13
  Top = 20
  Caption = 'Locator Calculator'
  ClientHeight = 353
  ClientWidth = 288
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 8
    Width = 281
    Height = 121
    Caption = 'Station 1'
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 26
      Width = 68
      Height = 13
      Caption = 'Maidenhead'
    end
    object Label2: TLabel
      Left = 8
      Top = 60
      Width = 49
      Height = 13
      Caption = 'Lat/Long'
    end
    object Label3: TLabel
      Left = 8
      Top = 94
      Width = 25
      Height = 13
      Caption = 'NGR'
    end
    object S1Loc: TEdit
      Left = 80
      Top = 22
      Width = 193
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 1
      OnExit = S1LocExit
    end
    object S1LatLong: TEdit
      Tag = 1
      Left = 80
      Top = 56
      Width = 193
      Height = 21
      TabStop = False
      TabOrder = 0
      OnExit = S1LatLongExit
    end
    object S1NGR: TEdit
      Tag = 2
      Left = 80
      Top = 90
      Width = 193
      Height = 21
      TabStop = False
      CharCase = ecUpperCase
      TabOrder = 2
      OnExit = S1NGRExit
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 136
    Width = 281
    Height = 121
    Caption = 'Station 2'
    TabOrder = 1
    object Label4: TLabel
      Left = 8
      Top = 25
      Width = 68
      Height = 13
      Caption = 'Maidenhead'
    end
    object Label5: TLabel
      Left = 8
      Top = 59
      Width = 49
      Height = 13
      Caption = 'Lat/Long'
    end
    object Label6: TLabel
      Left = 8
      Top = 93
      Width = 25
      Height = 13
      Caption = 'NGR'
    end
    object S2Loc: TEdit
      Tag = 10
      Left = 80
      Top = 21
      Width = 193
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 0
      OnExit = S2LocExit
    end
    object S2LatLong: TEdit
      Tag = 11
      Left = 80
      Top = 55
      Width = 193
      Height = 21
      TabStop = False
      TabOrder = 2
      OnExit = S2LatLongExit
    end
    object S2NGR: TEdit
      Tag = 12
      Left = 80
      Top = 89
      Width = 193
      Height = 21
      TabStop = False
      CharCase = ecUpperCase
      TabOrder = 1
      OnExit = S2NGRExit
    end
  end
  object CalcButton: TButton
    Left = 18
    Top = 262
    Width = 75
    Height = 29
    Caption = 'Calc'
    TabOrder = 2
    OnClick = CalcButtonClick
  end
  object Distance: TEdit
    Left = 104
    Top = 266
    Width = 177
    Height = 21
    TabStop = False
    ReadOnly = True
    TabOrder = 3
  end
  object ExitButton: TButton
    Left = 16
    Top = 312
    Width = 75
    Height = 25
    Caption = 'Exit'
    Default = True
    TabOrder = 4
    OnClick = ExitButtonClick
  end
  object CancelButton: TButton
    Left = 200
    Top = 312
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 5
    OnClick = CancelButtonClick
  end
end
