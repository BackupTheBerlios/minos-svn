object QRigSyncMain: TQRigSyncMain
  Left = 0
  Top = 0
  Caption = 'QS1R <-> Main Rig sync '
  ClientHeight = 124
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
    Top = 29
    Width = 46
    Height = 13
    Caption = 'Rig1Label'
  end
  object QF1Label: TLabel
    Left = 152
    Top = 29
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
  object QS1RLabel: TLabel
    Left = 47
    Top = 52
    Width = 27
    Height = 13
    Caption = 'QS1R'
  end
  object QS1RFLabel: TLabel
    Left = 152
    Top = 52
    Width = 58
    Height = 13
    Caption = 'QS1RFLabel'
  end
  object CloseButton: TButton
    Left = 496
    Top = 4
    Width = 116
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object RigSelectButton: TButton
    Left = 266
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
    Width = 116
    Height = 25
    Caption = 'Transfer 1 -> QS1R'
    TabOrder = 2
    OnClick = Transfer12ButtonClick
  end
  object ConnectQS1RButton: TButton
    Left = 368
    Top = 4
    Width = 116
    Height = 25
    Caption = 'Connect QS1R'
    TabOrder = 3
    OnClick = ConnectQS1RButtonClick
  end
  object Transfer21: TButton
    Left = 496
    Top = 35
    Width = 116
    Height = 25
    Caption = 'Transfer QS1R -> 1'
    TabOrder = 4
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
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    OnWrite = ClientSocket1Write
    Left = 304
    Top = 48
  end
  object ApplicationEvents1: TApplicationEvents
    OnException = ApplicationEvents1Exception
    Left = 328
    Top = 48
  end
  object Timer2: TTimer
    OnTimer = Timer2Timer
    Left = 272
    Top = 40
  end
end
