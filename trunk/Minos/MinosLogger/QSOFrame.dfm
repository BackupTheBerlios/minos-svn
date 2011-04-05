object GJVEditFrame: TGJVEditFrame
  Left = 0
  Top = 0
  Width = 620
  Height = 156
  TabOrder = 0
  TabStop = True
  object G0GJVPanel: TPanel
    Left = 0
    Top = 0
    Width = 620
    Height = 156
    Align = alClient
    BevelKind = bkFlat
    BevelOuter = bvNone
    TabOrder = 0
    object ModeLabel: TLabel
      Left = 194
      Top = 42
      Width = 26
      Height = 13
      Caption = 'Mode'
    end
    object BrgSt: TLabel
      Left = 506
      Top = 121
      Width = 60
      Height = 22
      AutoSize = False
      Caption = '(999XT)'
    end
    object DistSt: TLabel
      Left = 432
      Top = 121
      Width = 56
      Height = 22
      AutoSize = False
      Caption = '(99999)'
    end
    object SecondOpLabel: TLabel
      Left = 467
      Top = 6
      Width = 24
      Height = 13
      Caption = 'Op2:'
    end
    object OperatorLabel: TLabel
      Left = 329
      Top = 6
      Width = 24
      Height = 13
      Caption = 'Op1:'
    end
    object CallsignEdit: TLabeledEdit
      Left = 4
      Top = 83
      Width = 94
      Height = 21
      AutoSelect = False
      CharCase = ecUpperCase
      EditLabel.Width = 59
      EditLabel.Height = 13
      EditLabel.Caption = 'Callsign (F1)'
      TabOrder = 4
      OnChange = GJVEditChange
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object RSTTXEdit: TLabeledEdit
      Left = 108
      Top = 83
      Width = 36
      Height = 21
      AutoSelect = False
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'RS(T)(F2)'
      TabOrder = 5
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object SerTXEdit: TLabeledEdit
      Left = 184
      Top = 83
      Width = 54
      Height = 21
      Hint = 'Serial TX is read only - double click if you need to modify it'
      TabStop = False
      AutoSelect = False
      EditLabel.Width = 41
      EditLabel.Height = 13
      EditLabel.Caption = 'Serial TX'
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 6
      OnChange = SerTXEditChange
      OnDblClick = SerTXEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object RSTRXEdit: TLabeledEdit
      Left = 254
      Top = 83
      Width = 54
      Height = 21
      AutoSelect = False
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'RS(T)(F3)'
      TabOrder = 7
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object SerRXEdit: TLabeledEdit
      Left = 337
      Top = 83
      Width = 54
      Height = 21
      AutoSelect = False
      EditLabel.Width = 42
      EditLabel.Height = 13
      EditLabel.Caption = 'Serial RX'
      TabOrder = 8
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object LocEdit: TLabeledEdit
      Left = 420
      Top = 83
      Width = 65
      Height = 21
      AutoSelect = False
      CharCase = ecUpperCase
      EditLabel.Width = 39
      EditLabel.Height = 13
      EditLabel.Caption = 'Loc (F5)'
      TabOrder = 9
      OnChange = LocEditChange
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object QTHEdit: TLabeledEdit
      Left = 504
      Top = 83
      Width = 62
      Height = 21
      AutoSelect = False
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'Exchange'
      TabOrder = 10
      OnChange = GJVEditChange
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object CommentsEdit: TLabeledEdit
      Left = 4
      Top = 126
      Width = 121
      Height = 21
      AutoSelect = False
      EditLabel.Width = 50
      EditLabel.Height = 13
      EditLabel.Caption = 'Comments'
      TabOrder = 11
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object ModeComboBoxGJV: TComboBox
      Left = 248
      Top = 37
      Width = 57
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      TabStop = False
      Text = 'J3E'
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      Items.Strings = (
        'J3E'
        'A1A'
        'F3E')
    end
    object GJVOKButton: TButton
      Left = 142
      Top = 116
      Width = 41
      Height = 25
      Hint = 'Log the QSO, or move to next unfilled field'
      Caption = 'Log'
      Default = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 12
      TabStop = False
    end
    object GJVForceButton: TButton
      Left = 196
      Top = 116
      Width = 58
      Height = 25
      Hint = 'Force logging the QSO, even when incomplete'
      Caption = '&Force...'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 13
      TabStop = False
    end
    object GJVCancelButton: TButton
      Left = 272
      Top = 116
      Width = 55
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 14
      TabStop = False
    end
    object NonScoreCheckBox: TCheckBox
      Left = 371
      Top = 41
      Width = 117
      Height = 18
      TabStop = False
      Caption = 'Non Scoring'
      TabOrder = 2
    end
    object DeletedCheckBox: TCheckBox
      Left = 504
      Top = 42
      Width = 80
      Height = 16
      TabStop = False
      Caption = 'Deleted'
      TabOrder = 3
    end
    object ModeButton: TButton
      Left = 318
      Top = 35
      Width = 36
      Height = 25
      Caption = 'A1A'
      TabOrder = 1
      TabStop = False
      OnClick = ModeButtonClick
    end
    object FirstUnfilledButton: TButton
      Left = 185
      Top = 1
      Width = 132
      Height = 25
      Caption = 'First Unfilled QSO'
      TabOrder = 15
      TabStop = False
    end
    object CatchupButton: TButton
      Left = 6
      Top = 1
      Width = 162
      Height = 25
      Caption = 'Catch-up (Post Entry)'
      TabOrder = 16
      TabStop = False
    end
    object SecondOpComboBox: TComboBox
      Left = 506
      Top = 3
      Width = 84
      Height = 21
      CharCase = ecUpperCase
      ItemHeight = 13
      TabOrder = 18
      TabStop = False
    end
    object MainOpComboBox: TComboBox
      Left = 368
      Top = 3
      Width = 84
      Height = 21
      CharCase = ecUpperCase
      ItemHeight = 13
      TabOrder = 17
      TabStop = False
    end
  end
end
