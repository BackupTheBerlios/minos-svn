inherited GJVQSOEditFrame: TGJVQSOEditFrame
  Width = 574
  Height = 220
  ExplicitWidth = 574
  ExplicitHeight = 220
  inherited G0GJVPanel: TPanel
    Width = 574
    Height = 220
    ExplicitWidth = 574
    ExplicitHeight = 220
    inherited TimeEdit: TLabeledEdit
      Hint = 'Date/Time is read only - double click to modify'
    end
    inherited GJVOKButton: TButton
      Left = 139
      Top = 179
      ExplicitLeft = 139
      ExplicitTop = 179
    end
    inherited GJVForceButton: TButton
      Left = 193
      Top = 179
      TabOrder = 21
      ExplicitLeft = 193
      ExplicitTop = 179
    end
    inherited GJVCancelButton: TButton
      Left = 253
      Top = 179
      Width = 91
      Caption = 'Return to Log'
      TabOrder = 22
      ExplicitLeft = 253
      ExplicitTop = 179
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
    end
    inherited MainOpComboBox: TComboBox
      TabOrder = 24
    end
    object InsertBeforeButton: TButton
      Left = 8
      Top = 148
      Width = 75
      Height = 25
      Caption = 'Insert Before'
      TabOrder = 16
      OnClick = InsertBeforeButtonClick
    end
    object InsertAfterButton: TButton
      Left = 8
      Top = 179
      Width = 75
      Height = 25
      Caption = 'Insert After'
      TabOrder = 17
      OnClick = InsertAfterButtonClick
    end
    object PriorButton: TButton
      Left = 470
      Top = 148
      Width = 75
      Height = 25
      Caption = 'Prior'
      TabOrder = 19
      TabStop = False
      OnClick = PriorButtonClick
    end
    object NextButton: TButton
      Left = 472
      Top = 179
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
