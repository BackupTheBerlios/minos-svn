inherited GJVQSOEditFrame: TGJVQSOEditFrame
  Width = 629
  Height = 193
  ExplicitWidth = 629
  ExplicitHeight = 193
  inherited G0GJVPanel: TPanel
    Width = 629
    Height = 193
    ExplicitWidth = 574
    ExplicitHeight = 193
    inherited GJVForceButton: TButton
      TabOrder = 20
    end
    inherited GJVCancelButton: TButton
      Width = 91
      Caption = 'Return to Log'
      TabOrder = 22
      ExplicitWidth = 91
    end
    inherited FirstUnfilledButton: TButton
      TabOrder = 13
      OnClick = FirstUnfilledButtonClick
    end
    inherited SecondOpComboBox: TComboBox
      TabOrder = 21
      OnExit = SecondOpComboBoxExit
    end
    inherited MainOpComboBox: TComboBox
      TabOrder = 19
      OnExit = MainOpComboBoxExit
    end
    object InsertBeforeButton: TButton
      Left = 368
      Top = 118
      Width = 90
      Height = 25
      Caption = 'Insert Before'
      TabOrder = 14
      OnClick = InsertBeforeButtonClick
    end
    object InsertAfterButton: TButton
      Left = 368
      Top = 149
      Width = 90
      Height = 25
      Caption = 'Insert After'
      TabOrder = 15
      OnClick = InsertAfterButtonClick
    end
    object PriorButton: TButton
      Left = 472
      Top = 118
      Width = 75
      Height = 25
      Caption = 'Prior'
      TabOrder = 17
      TabStop = False
      OnClick = PriorButtonClick
    end
    object NextButton: TButton
      Left = 472
      Top = 149
      Width = 75
      Height = 25
      Caption = 'Next'
      TabOrder = 18
      TabStop = False
      OnClick = NextButtonClick
    end
    object TimeEdit: TLabeledEdit
      Left = 139
      Top = 41
      Width = 46
      Height = 21
      Hint = 'Date/Time is read only - double click to modify'
      TabStop = False
      AutoSelect = False
      EditLabel.Width = 22
      EditLabel.Height = 13
      EditLabel.Caption = 'Time'
      LabelPosition = lpLeft
      MaxLength = 5
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 23
      OnChange = TimeEditChange
      OnDblClick = TimeEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = TimeEditKeyPress
    end
    object DateEdit: TLabeledEdit
      Left = 34
      Top = 41
      Width = 64
      Height = 21
      Hint = 'Date/Time is read only - double click to modify'
      TabStop = False
      AutoSelect = False
      EditLabel.Width = 23
      EditLabel.Height = 13
      EditLabel.Caption = 'Date'
      LabelPosition = lpLeft
      MaxLength = 10
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 24
      OnChange = DateEditChange
      OnDblClick = DateEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
  end
  object ROFieldTimer: TTimer
    Interval = 100
    OnTimer = ROFieldTimerTimer
    Left = 160
    Top = 112
  end
end
