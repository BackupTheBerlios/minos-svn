inherited GJVQSOEditFrame: TGJVQSOEditFrame
  Width = 574
  Height = 193
  ExplicitWidth = 574
  ExplicitHeight = 193
  inherited G0GJVPanel: TPanel
    Width = 574
    Height = 193
    ExplicitWidth = 574
    ExplicitHeight = 193
    inherited TimeEdit: TLabeledEdit
      Hint = 'Date/Time is read only - double click to modify'
    end
    inherited GJVOKButton: TButton
      Left = 139
      Top = 149
      ExplicitLeft = 139
      ExplicitTop = 149
    end
    inherited GJVForceButton: TButton
      Left = 193
      Top = 149
      TabOrder = 21
      ExplicitLeft = 193
      ExplicitTop = 149
    end
    inherited GJVCancelButton: TButton
      Left = 253
      Top = 149
      Width = 91
      Caption = 'Return to Log'
      TabOrder = 22
      ExplicitLeft = 253
      ExplicitTop = 149
      ExplicitWidth = 91
    end
    inherited DateEdit: TLabeledEdit
      Hint = 'Date/Time is read only - double click to modify'
    end
    inherited FirstUnfilledButton: TButton
      OnClick = FirstUnfilledButtonClick
    end
    inherited CatchupButton: TButton
      TabOrder = 25
    end
    inherited SecondOpComboBox: TComboBox
      TabOrder = 23
      OnExit = SecondOpComboBoxExit
    end
    inherited MainOpComboBox: TComboBox
      TabOrder = 24
      OnExit = MainOpComboBoxExit
    end
    object InsertBeforeButton: TButton
      Left = 368
      Top = 118
      Width = 90
      Height = 25
      Caption = 'Insert Before'
      TabOrder = 16
      OnClick = InsertBeforeButtonClick
    end
    object InsertAfterButton: TButton
      Left = 368
      Top = 149
      Width = 90
      Height = 25
      Caption = 'Insert After'
      TabOrder = 17
      OnClick = InsertAfterButtonClick
    end
    object PriorButton: TButton
      Left = 472
      Top = 118
      Width = 75
      Height = 25
      Caption = 'Prior'
      TabOrder = 19
      TabStop = False
      OnClick = PriorButtonClick
    end
    object NextButton: TButton
      Left = 472
      Top = 149
      Width = 75
      Height = 25
      Caption = 'Next'
      TabOrder = 20
      TabStop = False
      OnClick = NextButtonClick
    end
  end
  object ROFieldTimer: TTimer
    Interval = 100
    OnTimer = ROFieldTimerTimer
    Left = 160
    Top = 112
  end
end
