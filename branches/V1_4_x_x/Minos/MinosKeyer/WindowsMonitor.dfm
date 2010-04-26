object WindowsMonitorForm: TWindowsMonitorForm
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Windows Port'
  ClientHeight = 37
  ClientWidth = 222
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  object PTTInCheckBox: TCheckBox
    Left = 16
    Top = 8
    Width = 75
    Height = 17
    Caption = 'PTT'
    TabOrder = 0
    OnClick = PTTInCheckBoxClick
  end
  object L1CheckBox: TCheckBox
    Left = 87
    Top = 8
    Width = 67
    Height = 17
    Caption = 'L1'
    TabOrder = 1
    OnClick = PTTInCheckBoxClick
  end
  object L2CheckBox: TCheckBox
    Left = 150
    Top = 8
    Width = 57
    Height = 17
    Caption = 'L2'
    TabOrder = 2
    OnClick = PTTInCheckBoxClick
  end
  object StartupTimer: TTimer
    OnTimer = StartupTimerTimer
    Left = 8
    Top = 8
  end
end
