object MinosControlForm: TMinosControlForm
  Left = 0
  Top = 0
  Caption = 'Minos Logging Line Control'
  ClientHeight = 286
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PTTInCheckBox: TCheckBox
    Left = 96
    Top = 32
    Width = 97
    Height = 17
    Caption = 'PTT Input'
    Enabled = False
    TabOrder = 0
  end
  object L1CheckBox: TCheckBox
    Left = 96
    Top = 72
    Width = 97
    Height = 17
    Caption = 'L1'
    Enabled = False
    TabOrder = 1
  end
  object L2CheckBox: TCheckBox
    Left = 96
    Top = 112
    Width = 97
    Height = 17
    Caption = 'L2'
    Enabled = False
    TabOrder = 2
  end
  object PTTOutCheckBox: TCheckBox
    Left = 96
    Top = 176
    Width = 97
    Height = 17
    Caption = 'PTT Out'
    Enabled = False
    TabOrder = 3
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 16
    Top = 32
  end
  object ConnectTimer: TTimer
    Enabled = False
    OnTimer = ConnectTimerTimer
    Left = 16
    Top = 64
  end
end
