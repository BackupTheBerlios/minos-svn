object MinosChatForm: TMinosChatForm
  Left = 0
  Top = 0
  Caption = 'Minos Logging Inter Station Chat'
  ClientHeight = 200
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter_Image: TImage
    Left = 60
    Top = 100
    Width = 4
    Height = 4
    AutoSize = True
    Picture.Data = {
      07544269746D617066000000424D660000000000000036000000280000000400
      000004000000010018000000000030000000C40E0000C40E0000000000000000
      0000FF00FFFF00FFFFFFFFFFFFFFFF00FF000000C0C0C0FFFFFFFF00FF000000
      000000FF00FFFF00FFFF00FFFF00FFFF00FF}
    Transparent = True
    Visible = False
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 426
    Height = 174
    Align = alClient
    Caption = 'Panel2'
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 186
      Top = 1
      Width = 10
      Height = 172
      OnMoved = Splitter1Moved
      ExplicitLeft = 123
      ExplicitHeight = 258
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 185
      Height = 172
      Align = alLeft
      TabOrder = 0
      object StationList: TListBox
        Left = 1
        Top = 1
        Width = 183
        Height = 170
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object ChatMemo: TMemo
      Left = 196
      Top = 1
      Width = 229
      Height = 172
      Align = alClient
      Lines.Strings = (
        '')
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 1
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 174
    Width = 426
    Height = 26
    Align = alBottom
    Caption = 'Panel1'
    TabOrder = 1
    DesignSize = (
      426
      26)
    object ChatEdit: TLabeledEdit
      Left = 72
      Top = 1
      Width = 289
      Height = 21
      Anchors = [akLeft, akTop, akRight, akBottom]
      EditLabel.Width = 58
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
