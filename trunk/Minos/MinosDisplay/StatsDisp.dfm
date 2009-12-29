object StatsDispFrame: TStatsDispFrame
  Left = 0
  Top = 0
  Width = 250
  Height = 286
  TabOrder = 0
  TabStop = True
  object SLabel1: TLabel
    Left = 0
    Top = 0
    Width = 250
    Height = 240
    Align = alClient
    Alignment = taCenter
    Layout = tlCenter
    WordWrap = True
    ExplicitWidth = 3
    ExplicitHeight = 13
  end
  object Panel1: TPanel
    Left = 0
    Top = 240
    Width = 250
    Height = 46
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      250
      46)
    object P1Edit: TLabeledEdit
      Left = 8
      Top = 15
      Width = 50
      Height = 21
      Anchors = [akLeft, akBottom]
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'P 1 (mins)'
      TabOrder = 0
      OnExit = P1EditExit
      OnKeyPress = P2EditKeyPress
    end
    object P2Edit: TLabeledEdit
      Left = 79
      Top = 14
      Width = 49
      Height = 21
      Anchors = [akLeft, akBottom]
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'P 2 (mins)'
      TabOrder = 1
      OnExit = P2EditExit
      OnKeyPress = P2EditKeyPress
    end
  end
  object RecheckTimer: TTimer
    Interval = 10000
    OnTimer = RecheckTimerTimer
    Left = 8
    Top = 8
  end
end
