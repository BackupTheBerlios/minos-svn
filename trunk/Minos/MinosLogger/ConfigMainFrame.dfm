object TConfigFrame: TTConfigFrame
  Left = 0
  Top = 0
  Width = 354
  Height = 272
  TabOrder = 0
  TabStop = True
  object Panel1: TPanel
    Left = 0
    Top = 157
    Width = 354
    Height = 115
    Align = alBottom
    TabOrder = 1
    object HideCheckBox: TCheckBox
      Left = 30
      Top = 15
      Width = 107
      Height = 17
      Caption = 'Hide Server Apps'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = HideCheckBoxClick
    end
    object StationIdEdit: TLabeledEdit
      Left = 111
      Top = 38
      Width = 148
      Height = 21
      EditLabel.Width = 77
      EditLabel.Height = 13
      EditLabel.Caption = 'My circle of Hell '
      LabelPosition = lpLeft
      TabOrder = 1
    end
    object SetButton: TButton
      Left = 286
      Top = 38
      Width = 43
      Height = 25
      Caption = 'Set'
      TabOrder = 2
      OnClick = SetButtonClick
    end
    object ClearAllButton: TButton
      Left = 30
      Top = 69
      Width = 75
      Height = 25
      Caption = 'Clear All'
      TabOrder = 3
      OnClick = ClearAllButtonClick
    end
    object StartButton: TButton
      Left = 152
      Top = 6
      Width = 72
      Height = 25
      Caption = 'Start'
      TabOrder = 4
      Visible = False
      OnClick = StartButtonClick
    end
    object StopButton: TButton
      Left = 265
      Top = 6
      Width = 72
      Height = 25
      Caption = 'Stop'
      TabOrder = 5
      Visible = False
      OnClick = StopButtonClick
    end
    object CancelButton: TButton
      Left = 248
      Top = 69
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 6
    end
    object ModifyButton: TButton
      Left = 140
      Top = 69
      Width = 75
      Height = 25
      Caption = 'Modify'
      TabOrder = 7
      OnClick = ModifyButtonClick
    end
  end
  object ElementScrollBox: TScrollBox
    Left = 0
    Top = 0
    Width = 354
    Height = 157
    Align = alClient
    TabOrder = 0
  end
end
