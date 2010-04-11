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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RigLabel: TLabel
    Left = 207
    Top = 47
    Width = 40
    Height = 13
    Caption = 'RigLabel'
  end
  object QFLabel: TLabel
    Left = 72
    Top = 144
    Width = 39
    Height = 13
    Caption = 'QFLabel'
  end
  object Label1: TLabel
    Left = 48
    Top = 8
    Width = 212
    Height = 13
    Caption = 'Press Select to change rigs and parameters.'
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
  object RigSelectButton: TButton
    Left = 48
    Top = 42
    Width = 75
    Height = 25
    Caption = 'Select'
    TabOrder = 2
    OnClick = RigSelectButtonClick
  end
  object TransferButton: TButton
    Left = 32
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Transfer'
    TabOrder = 3
    OnClick = TransferButtonClick
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 8
    Top = 8
  end
  object OmniRig: TOmniRigX
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    OnRigTypeChange = OmniRigRigTypeChange
    OnStatusChange = OmniRigStatusChange
    OnParamsChange = OmniRigParamsChange
    Left = 360
    Top = 16
  end
end
