inherited GJVQSOEditFrame: TGJVQSOEditFrame
  Width = 574
  Height = 267
  ExplicitWidth = 574
  ExplicitHeight = 267
  inherited G0GJVPanel: TPanel
    Width = 574
    Height = 267
    ExplicitWidth = 574
    ExplicitHeight = 267
    object Label1: TLabel [3]
      Left = 14
      Top = 164
      Width = 69
      Height = 13
      Caption = 'Main Operator'
    end
    object Label2: TLabel [4]
      Left = 288
      Top = 164
      Width = 82
      Height = 13
      Caption = 'Second Operator'
    end
    inherited TimeEdit: TLabeledEdit
      Hint = 'Date/Time is read only - double click to modify'
    end
    inherited GJVOKButton: TButton
      Left = 139
      Top = 225
      ExplicitLeft = 139
      ExplicitTop = 225
    end
    inherited GJVForceButton: TButton
      Left = 193
      Top = 225
      ExplicitLeft = 193
      ExplicitTop = 225
    end
    inherited GJVCancelButton: TButton
      Left = 253
      Top = 225
      Width = 91
      Caption = 'Return to Log'
      ExplicitLeft = 253
      ExplicitTop = 225
      ExplicitWidth = 91
    end
    inherited DateEdit: TLabeledEdit
      Hint = 'Date/Time is read only - double click to modify'
    end
    inherited BandChoiceRadioGroup: TRadioGroup
      TabOrder = 23
    end
    object InsertBeforeButton: TButton
      Left = 8
      Top = 194
      Width = 75
      Height = 25
      Caption = 'Insert Before'
      TabOrder = 16
      OnClick = InsertBeforeButtonClick
    end
    object InsertAfterButton: TButton
      Left = 8
      Top = 225
      Width = 75
      Height = 25
      Caption = 'Insert After'
      TabOrder = 17
      OnClick = InsertAfterButtonClick
    end
    object FirstUnfilledButton: TButton
      Left = 371
      Top = 225
      Width = 75
      Height = 25
      Caption = 'First Unfilled'
      TabOrder = 18
      OnClick = FirstUnfilledButtonClick
    end
    object PriorButton: TButton
      Left = 470
      Top = 194
      Width = 75
      Height = 25
      Caption = 'Prior'
      TabOrder = 19
      OnClick = PriorButtonClick
    end
    object NextButton: TButton
      Left = 472
      Top = 225
      Width = 75
      Height = 25
      Caption = 'Next'
      TabOrder = 20
      OnClick = NextButtonClick
    end
    object MainOpComboBox: TComboBox
      Left = 109
      Top = 158
      Width = 145
      Height = 21
      ItemHeight = 13
      TabOrder = 21
      Text = 'MainOpComboBox'
      OnChange = MainOpComboBoxChange
      OnKeyPress = MainOpComboBoxKeyPress
    end
    object SecondOpComboBox: TComboBox
      Left = 406
      Top = 160
      Width = 145
      Height = 21
      ItemHeight = 13
      TabOrder = 22
      Text = 'SecondOpComboBox'
      OnChange = SecondOpComboBoxChange
      OnKeyPress = MainOpComboBoxKeyPress
    end
  end
  object ROFieldTimer: TTimer
    Interval = 100
    OnTimer = ROFieldTimerTimer
    Left = 120
    Top = 192
  end
end
