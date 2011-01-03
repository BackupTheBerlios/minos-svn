object QRigSyncMain: TQRigSyncMain
  Left = 0
  Top = 0
  Caption = 'QS1R <-> Main Rig sync '
  ClientHeight = 107
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
  object Rig1Label: TLabel
    Left = 47
    Top = 40
    Width = 46
    Height = 13
    Caption = 'Rig1Label'
  end
  object QF1Label: TLabel
    Left = 152
    Top = 40
    Width = 45
    Height = 13
    Caption = 'QF1Label'
  end
  object Label1: TLabel
    Left = 48
    Top = 9
    Width = 212
    Height = 13
    Caption = 'Press Select to change rigs and parameters.'
  end
  object Rig2Label: TLabel
    Left = 48
    Top = 79
    Width = 46
    Height = 13
    Caption = 'Rig2Label'
  end
  object QF2Label: TLabel
    Left = 152
    Top = 77
    Width = 45
    Height = 13
    Caption = 'QF2Label'
  end
  object CloseButton: TButton
    Left = 550
    Top = 74
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object RigSelectButton: TButton
    Left = 368
    Top = 4
    Width = 75
    Height = 25
    Caption = 'Select'
    TabOrder = 1
    OnClick = RigSelectButtonClick
  end
  object Transfer12Button: TButton
    Left = 368
    Top = 35
    Width = 113
    Height = 25
    Caption = 'Transfer 1 -> 2'
    TabOrder = 2
    OnClick = Transfer12ButtonClick
  end
  object Transfer21Button: TButton
    Left = 368
    Top = 74
    Width = 113
    Height = 25
    Caption = 'Transfer 2 -> 1'
    TabOrder = 3
    OnClick = Transfer21ButtonClick
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
