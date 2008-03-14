object MinosChatForm: TMinosChatForm
  Left = 0
  Top = 0
  Caption = 'Minos Logging Inter Station Chat'
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
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 426
    Height = 260
    Align = alClient
    Caption = 'Panel2'
    TabOrder = 0
    object JvNetscapeSplitter1: TJvNetscapeSplitter
      Left = 123
      Top = 1
      Height = 258
      Align = alLeft
      MinSize = 1
      Maximized = False
      Minimized = False
      ButtonCursor = crDefault
      ExplicitLeft = 117
      ExplicitTop = -3
      ExplicitHeight = 134
    end
    object StationList: TListBox
      Left = 1
      Top = 1
      Width = 122
      Height = 258
      Align = alLeft
      ItemHeight = 13
      TabOrder = 0
    end
    object ChatMemo: TMemo
      Left = 133
      Top = 1
      Width = 292
      Height = 258
      Align = alClient
      Lines.Strings = (
        '')
      ReadOnly = True
      TabOrder = 1
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 260
    Width = 426
    Height = 26
    Align = alBottom
    Caption = 'Panel1'
    TabOrder = 1
    DesignSize = (
      426
      26)
    object ChatEdit: TLabeledEdit
      Left = 53
      Top = 1
      Width = 308
      Height = 21
      Anchors = [akLeft, akTop, akRight, akBottom]
      EditLabel.Width = 49
      EditLabel.Height = 13
      EditLabel.Caption = 'Type here'
      LabelPosition = lpLeft
      TabOrder = 0
    end
    object SendButton: TButton
      Left = 367
      Top = 2
      Width = 53
      Height = 20
      Anchors = [akRight, akBottom]
      Caption = 'Send'
      Default = True
      TabOrder = 1
      OnClick = SendButtonClick
    end
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 16
    Top = 32
  end
  object ConnectTimer: TTimer
    OnTimer = ConnectTimerTimer
    Left = 16
    Top = 64
  end
  object StartUpTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = StartUpTimerTimer
    Left = 16
    Top = 96
  end
end
