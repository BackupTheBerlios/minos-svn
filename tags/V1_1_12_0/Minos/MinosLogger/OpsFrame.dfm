object OperatorFrame: TOperatorFrame
  Left = 0
  Top = 0
  Width = 320
  Height = 240
  TabOrder = 0
  TabStop = True
  object Label1: TLabel
    Left = 18
    Top = 24
    Width = 69
    Height = 13
    Caption = 'Main Operator'
  end
  object Label2: TLabel
    Left = 18
    Top = 90
    Width = 82
    Height = 13
    Caption = 'Second Operator'
  end
  object MainOpComboBox: TComboBox
    Left = 30
    Top = 43
    Width = 145
    Height = 21
    ItemHeight = 0
    TabOrder = 0
    Text = 'MainOpComboBox'
    OnChange = MainOpComboBoxChange
    OnExit = OpComboBoxExit
    OnKeyPress = MainOpComboBoxKeyPress
  end
  object SecondOpComboBox: TComboBox
    Left = 30
    Top = 109
    Width = 145
    Height = 21
    ItemHeight = 0
    TabOrder = 1
    Text = 'SecondOpComboBox'
    OnChange = SecondOpComboBoxChange
    OnExit = OpComboBoxExit
    OnKeyPress = MainOpComboBoxKeyPress
  end
  object OpChangeTimer: TTimer
    Interval = 50
    OnTimer = OpChangeTimerTimer
    Left = 128
    Top = 72
  end
end
