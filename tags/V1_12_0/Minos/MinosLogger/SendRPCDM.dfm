object SendDM: TSendDM
  OldCreateOrder = False
  Height = 150
  Width = 215
  object ConnectTimer: TTimer
    OnTimer = ConnectTimerTimer
    Left = 24
    Top = 8
  end
  object LogTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 136
    Top = 16
  end
end
