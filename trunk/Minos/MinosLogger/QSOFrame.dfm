object GJVEditFrame: TGJVEditFrame
  Left = 0
  Top = 0
  Width = 577
  Height = 153
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
      Width = 26
      Height = 13
      Caption = 'Mode'
    end
    object BrgSt: TLabel
      Left = 470
      Top = 11
      Width = 12
      Height = 13
      Caption = 'xx'
    end
    object DistSt: TLabel
      Left = 422
      Top = 11
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
      EditLabel.Width = 47
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
      EditLabel.Width = 41
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
      EditLabel.Width = 47
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
      EditLabel.Width = 42
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
      EditLabel.Width = 39
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
      EditLabel.Width = 47
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
      EditLabel.Width = 50
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
      TabOrder = 3
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
      Left = 401
      Top = 43
      Width = 94
      Height = 17
      TabStop = False
      Caption = 'Non Scoring'
      TabOrder = 5
    end
    object DeletedCheckBox: TCheckBox
      Left = 504
      Top = 43
      Width = 65
      Height = 17
      TabStop = False
      Caption = 'Deleted'
      TabOrder = 6
    end
    object TimeNowButton: TButton
      Left = 200
      Top = 39
      Width = 33
      Height = 25
      Caption = 'Now'
      TabOrder = 2
      OnClick = TimeNowButtonClick
    end
    object ModeButton: TButton
      Left = 341
      Top = 39
      Width = 33
      Height = 25
      Caption = 'A1A'
      TabOrder = 4
      OnClick = ModeButtonClick
    end
  end
end
