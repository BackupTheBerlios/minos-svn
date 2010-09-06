object QRigSyncMain: TQRigSyncMain
  Left = 0
  Top = 0
  Caption = 'QS1R <-> Main Rig sync '
  ClientHeight = 87
  ClientWidth = 633
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
    Left = 527
    Top = 9
    Width = 40
    Height = 13
    Caption = 'RigLabel'
  end
  object QFLabel: TLabel
    Left = 48
    Top = 53
    Width = 39
    Height = 13
    Caption = 'QFLabel'
  end
  object Label1: TLabel
    Left = 48
    Top = 9
    Width = 212
    Height = 13
    Caption = 'Press Select to change rigs and parameters.'
  end
  object CloseButton: TButton
    Left = 512
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object MuteButton: TButton
    Left = 372
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Mute'
    TabOrder = 1
    OnClick = MuteButtonClick
  end
  object RigSelectButton: TButton
    Left = 368
    Top = 4
    Width = 75
    Height = 25
    Caption = 'Select'
    TabOrder = 2
    OnClick = RigSelectButtonClick
  end
  object TransferButton: TButton
    Left = 232
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Transfer'
    TabOrder = 3
    OnClick = TransferButtonClick
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Top = 2
  end
  object OmniRig: TOmniRigX
    AutoConnect = False
    ConnectKind = ckRunningOrNew
    OnRigTypeChange = OmniRigRigTypeChange
    OnStatusChange = OmniRigStatusChange
    OnParamsChange = OmniRigParamsChange
    Top = 42
  end
end
