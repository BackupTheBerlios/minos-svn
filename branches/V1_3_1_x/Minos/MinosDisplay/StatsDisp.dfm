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
    object PeriodSetButton: TButton
      Left = 115
      Top = 22
      Width = 36
      Height = 22
      Anchors = [akLeft, akBottom]
      Caption = 'Set'
      TabOrder = 0
      OnClick = PeriodSetButtonClick
    end
    object P1Edit: TLabeledEdit
      Left = 2
      Top = 23
      Width = 50
      Height = 21
      Anchors = [akLeft, akBottom]
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'P 1 (mins)'
      TabOrder = 1
      OnKeyPress = P2EditKeyPress
    end
    object P2Edit: TLabeledEdit
      Left = 61
      Top = 23
      Width = 49
      Height = 21
      Anchors = [akLeft, akBottom]
      EditLabel.Width = 44
      EditLabel.Height = 13
      EditLabel.Caption = 'P2 (mins)'
      TabOrder = 2
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
