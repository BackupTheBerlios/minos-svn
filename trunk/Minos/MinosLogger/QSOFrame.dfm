object GJVEditFrame: TGJVEditFrame
  Left = 0
  Top = 0
  Width = 556
  Height = 153
  TabOrder = 0
  TabStop = True
  object G0GJVPanel: TPanel
    Left = 0
    Top = 0
    Width = 556
    Height = 153
    Align = alClient
    BevelKind = bkFlat
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitHeight = 112
    object ModeLabel: TLabel
      Left = 196
      Top = 45
      Width = 26
      Height = 13
      Caption = 'Mode'
    end
    object BrgSt: TLabel
      Left = 513
      Top = 46
      Width = 12
      Height = 13
      Caption = 'xx'
    end
    object DistSt: TLabel
      Left = 465
      Top = 46
      Width = 12
      Height = 13
      Caption = 'xx'
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
      TabOrder = 2
      OnChange = GJVEditChange
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object RSTTXEdit: TLabeledEdit
      Left = 109
      Top = 83
      Width = 36
      Height = 21
      AutoSelect = False
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'RS(T)(F2)'
      TabOrder = 3
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object SerTXEdit: TLabeledEdit
      Left = 163
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
      TabOrder = 4
      OnChange = SerTXEditChange
      OnDblClick = SerTXEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object RSTRXEdit: TLabeledEdit
      Left = 239
      Top = 83
      Width = 54
      Height = 21
      AutoSelect = False
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'RS(T)(F3)'
      TabOrder = 5
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object SerRXEdit: TLabeledEdit
      Left = 316
      Top = 83
      Width = 54
      Height = 21
      AutoSelect = False
      EditLabel.Width = 42
      EditLabel.Height = 13
      EditLabel.Caption = 'Serial RX'
      TabOrder = 6
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object LocEdit: TLabeledEdit
      Left = 395
      Top = 83
      Width = 65
      Height = 21
      AutoSelect = False
      CharCase = ecUpperCase
      EditLabel.Width = 39
      EditLabel.Height = 13
      EditLabel.Caption = 'Loc (F5)'
      TabOrder = 7
      OnChange = LocEditChange
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object QTHEdit: TLabeledEdit
      Left = 483
      Top = 83
      Width = 62
      Height = 21
      AutoSelect = False
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = 'Exchange'
      TabOrder = 8
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
      EditLabel.Width = 50
      EditLabel.Height = 13
      EditLabel.Caption = 'Comments'
      TabOrder = 9
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object ModeComboBoxGJV: TComboBox
      Left = 237
      Top = 41
      Width = 52
      Height = 21
      ItemHeight = 13
      TabOrder = 10
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
      TabOrder = 11
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
      TabOrder = 12
      TabStop = False
    end
    object GJVCancelButton: TButton
      Left = 256
      Top = 116
      Width = 55
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 13
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
      EditLabel.Width = 23
      EditLabel.Height = 13
      EditLabel.Caption = 'Date'
      LabelPosition = lpLeft
      MaxLength = 10
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 0
      OnChange = DateEditChange
      OnDblClick = DateEditDblClick
      OnEnter = EditControlEnter
      OnExit = EditControlExit
      OnKeyDown = EditKeyDown
      OnKeyPress = GJVEditKeyPress
    end
    object NonScoreCheckBox: TCheckBox
      Left = 297
      Top = 44
      Width = 88
      Height = 17
      TabStop = False
      Caption = 'Non Scoring'
      TabOrder = 14
    end
    object DeletedCheckBox: TCheckBox
      Left = 392
      Top = 44
      Width = 65
      Height = 17
      TabStop = False
      Caption = 'Deleted'
      TabOrder = 15
    end
    object BandChoiceRadioGroup: TRadioGroup
      Left = 4
      Top = 0
      Width = 541
      Height = 35
      Caption = 'Band'
      TabOrder = 16
    end
  end
end
