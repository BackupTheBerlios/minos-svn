object QRigSyncMain: TQRigSyncMain
  Left = 0
  Top = 0
  Caption = 'QS1R <-> Main Rig sync '
  ClientHeight = 214
  ClientWidth = 418
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 80
    Top = 40
    Width = 31
    Height = 13
    Caption = 'Label1'
  end
  object CloseButton: TButton
    Left = 312
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object MuteButton: TButton
    Left = 172
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Mute'
    TabOrder = 1
    OnClick = MuteButtonClick
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 8
    Top = 8
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 8
    Top = 64
  end
  object StartUpTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = StartUpTimerTimer
    Left = 16
    Top = 120
  end
end
