inherited GJVQSOEditFrame: TGJVQSOEditFrame
  Width = 629
  Height = 193
  ExplicitWidth = 629
  ExplicitHeight = 193
  inherited G0GJVPanel: TPanel
    Width = 629
    Height = 193
    ExplicitWidth = 629
    ExplicitHeight = 193
    inherited RSTTXEdit: TLabeledEdit [5]
      TabOrder = 7
    end
    inherited SerTXEdit: TLabeledEdit [6]
      TabOrder = 8
    end
    inherited RSTRXEdit: TLabeledEdit [7]
      TabOrder = 9
    end
    inherited SecondOpComboBox: TComboBox [8]
      TabOrder = 24
      OnExit = SecondOpComboBoxExit
    end
    inherited CatchupButton: TButton [9]
      TabOrder = 21
    end
    inherited FirstUnfilledButton: TButton [10]
      TabOrder = 22
      OnClick = FirstUnfilledButtonClick
    end
    inherited CallsignEdit: TLabeledEdit [11]
      TabOrder = 6
    end
    inherited DeletedCheckBox: TCheckBox [12]
      TabOrder = 5
    end
    inherited NonScoreCheckBox: TCheckBox [13]
      TabOrder = 4
    end
    inherited GJVCancelButton: TButton [14]
      Width = 91
      Caption = 'Return to Log'
      TabOrder = 16
      ExplicitWidth = 91
    end
    inherited GJVForceButton: TButton
      TabOrder = 15
    end
    inherited MainOpComboBox: TComboBox [16]
      TabOrder = 23
      OnExit = MainOpComboBoxExit
    end
    inherited SerRXEdit: TLabeledEdit [17]
      TabOrder = 10
    end
    inherited LocEdit: TLabeledEdit [18]
      TabOrder = 11
    end
    inherited QTHEdit: TLabeledEdit [19]
      TabOrder = 12
    end
    inherited CommentsEdit: TLabeledEdit [20]
      TabOrder = 13
    end
    inherited ModeComboBoxGJV: TComboBox [21]
    end
    inherited GJVOKButton: TButton [22]
      TabOrder = 14
    end
    inherited ModeButton: TButton [23]
      TabOrder = 3
    end
    object InsertBeforeButton: TButton
      Left = 24
      Top = 153
      Width = 90
      Height = 25
      Caption = 'Insert Before'
      TabOrder = 17
      TabStop = False
      OnClick = InsertBeforeButtonClick
    end
    object InsertAfterButton: TButton
      Left = 139
      Top = 153
      Width = 90
      Height = 25
      Caption = 'Insert After'
      TabOrder = 18
      TabStop = False
      OnClick = InsertAfterButtonClick
    end
    object PriorButton: TButton
      Left = 392
      Top = 153
      Width = 75
      Height = 25
      Caption = 'Prior'
      TabOrder = 20
      TabStop = False
      OnClick = PriorButtonClick
    end
    object NextButton: TButton
      Left = 491
      Top = 153
      Width = 75
      Height = 25
      Caption = 'Next'
      TabOrder = 19
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
      TabOrder = 1
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
      TabOrder = 2
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
