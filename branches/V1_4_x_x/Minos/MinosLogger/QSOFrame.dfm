object GJVEditFrame: TGJVEditFrame
  Left = 0
  Top = 0
  Width = 577
  Height = 153
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  ParentFont = False
  TabOrder = 0
  TabStop = True
  object G0GJVPanel: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 153
    Align = alClient
    BevelKind = bkFlat
    BevelOuter = bvNone
    TabOrder = 0
    object ModeLabel: TLabel
      Left = 245
      Top = 45
      Width = 30
      Height = 13
      Caption = 'Mode'
    end
    object BrgSt: TLabel
      Left = 526
      Top = 20
      Width = 46
      Height = 13
      Caption = '(999XT)'
    end
    object DistSt: TLabel
      Left = 482
      Top = 20
      Width = 45
      Height = 13
      Caption = '(99999)'
    end
    object SecondOpLabel: TLabel
      Left = 378
      Top = 6
      Width = 28
      Height = 13
      Caption = 'Op2:'
    end
    object OperatorLabel: TLabel
      Left = 269
      Top = 6
      Width = 28
      Height = 13
      Caption = 'Op1:'
    end
    object TimeEdit: TLabeledEdit
      Left = 139
      Top = 41
      Width = 46
      Height = 21
      Hint = 
        'Date/Time is read only - double click to modify, or use right mo' +
        'use button to set to "now"'
      TabStop = False
      AutoSelect = False
      EditLabel.Width = 28
      EditLabel.Height = 13
      EditLabel.Caption = 'Time'
      LabelPosition = lpLeft
      MaxLength = 5
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 6
      OnChange = TimeEditChange
      OnDblClick = TimeEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = TimeEditKeyPress
    end
    object CallsignEdit: TLabeledEdit
      Left = 4
      Top = 83
      Width = 94
      Height = 21
      AutoSelect = False
      CharCase = ecUpperCase
      EditLabel.Width = 72
      EditLabel.Height = 13
      EditLabel.Caption = 'Callsign (F1)'
      TabOrder = 7
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
      EditLabel.Width = 56
      EditLabel.Height = 13
      EditLabel.Caption = 'RS(T)(F2)'
      TabOrder = 8
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object SerTXEdit: TLabeledEdit
      Left = 170
      Top = 83
      Width = 54
      Height = 21
      Hint = 'Serial TX is read only - double click if you need to modify it'
      TabStop = False
      AutoSelect = False
      EditLabel.Width = 52
      EditLabel.Height = 13
      EditLabel.Caption = 'Serial TX'
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 9
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
      EditLabel.Width = 56
      EditLabel.Height = 13
      EditLabel.Caption = 'RS(T)(F3)'
      TabOrder = 10
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
      EditLabel.Width = 53
      EditLabel.Height = 13
      EditLabel.Caption = 'Serial RX'
      TabOrder = 11
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
      EditLabel.Width = 46
      EditLabel.Height = 13
      EditLabel.Caption = 'Loc (F5)'
      TabOrder = 12
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
      EditLabel.Width = 55
      EditLabel.Height = 13
      EditLabel.Caption = 'Exchange'
      TabOrder = 13
      OnChange = GJVEditChange
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object CommentsEdit: TLabeledEdit
      Left = 4
      Top = 120
      Width = 121
      Height = 21
      AutoSelect = False
      EditLabel.Width = 62
      EditLabel.Height = 13
      EditLabel.Caption = 'Comments'
      TabOrder = 14
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object ModeComboBoxGJV: TComboBox
      Left = 278
      Top = 41
      Width = 52
      Height = 21
      ItemHeight = 13
      TabOrder = 1
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
      TabOrder = 15
      TabStop = False
    end
    object GJVForceButton: TButton
      Left = 196
      Top = 116
      Width = 50
      Height = 25
      Hint = 'Force logging the QSO, even when incomplete'
      Caption = '&Force...'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 16
      TabStop = False
    end
    object GJVCancelButton: TButton
      Left = 256
      Top = 116
      Width = 55
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 17
      TabStop = False
    end
    object DateEdit: TLabeledEdit
      Left = 34
      Top = 41
      Width = 64
      Height = 21
      Hint = 
        'Date/Time is read only - double click to modify, or use right mo' +
        'use button to set to "now"'
      TabStop = False
      AutoSelect = False
      EditLabel.Width = 27
      EditLabel.Height = 13
      EditLabel.Caption = 'Date'
      LabelPosition = lpLeft
      MaxLength = 10
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 5
      OnChange = DateEditChange
      OnDblClick = DateEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object NonScoreCheckBox: TCheckBox
      Left = 401
      Top = 43
      Width = 94
      Height = 17
      TabStop = False
      Caption = 'Non Scoring'
      TabOrder = 3
    end
    object DeletedCheckBox: TCheckBox
      Left = 504
      Top = 43
      Width = 65
      Height = 17
      TabStop = False
      Caption = 'Deleted'
      TabOrder = 4
    end
    object TimeNowButton: TButton
      Left = 200
      Top = 39
      Width = 33
      Height = 25
      Caption = 'Now'
      TabOrder = 0
      TabStop = False
      OnClick = TimeNowButtonClick
    end
    object ModeButton: TButton
      Left = 341
      Top = 39
      Width = 33
      Height = 25
      Caption = 'A1A'
      TabOrder = 2
      TabStop = False
      OnClick = ModeButtonClick
    end
    object FirstUnfilledButton: TButton
      Left = 145
      Top = 1
      Width = 113
      Height = 25
      Caption = 'First Unfilled QSO'
      TabOrder = 18
      TabStop = False
    end
    object CatchupButton: TButton
      Left = 6
      Top = 1
      Width = 129
      Height = 25
      Caption = 'Catch-up (Post Entry)'
      TabOrder = 19
      TabStop = False
    end
    object SecondOpComboBox: TComboBox
      Left = 407
      Top = 3
      Width = 64
      Height = 21
      CharCase = ecUpperCase
      ItemHeight = 0
      TabOrder = 21
      TabStop = False
    end
    object MainOpComboBox: TComboBox
      Left = 298
      Top = 3
      Width = 65
      Height = 21
      CharCase = ecUpperCase
      ItemHeight = 0
      TabOrder = 20
      TabStop = False
    end
  end
end
