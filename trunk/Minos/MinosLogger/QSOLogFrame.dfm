inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    inherited ModeLabel: TLabel
      Left = 266
      ExplicitLeft = 266
    end
    object DateLabel: TLabel [5]
      Left = 156
      Top = 45
      Width = 57
      Height = 14
      AutoSize = False
      Caption = '00/00/00'
    end
    object TimeLabel: TLabel [6]
      Left = 217
      Top = 45
      Width = 39
      Height = 14
      AutoSize = False
      Caption = '00:00'
    end
    object Label1: TLabel [7]
      Left = 3
      Top = 45
      Width = 139
      Height = 13
      Caption = 'Time taken at QSO end:'
    end
    inherited GJVCancelButton: TButton [8]
      TabOrder = 18
    end
    inherited GJVForceButton: TButton [9]
      TabOrder = 12
    end
    object MatchXferButton: TBitBtn [10]
      Left = 321
      Top = 112
      Width = 67
      Height = 30
      Hint = 'Get fields from current matching QSO'
      Caption = 'Match &Xfer F12'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 11
      TabStop = False
      WordWrap = True
    end
    inherited ModeComboBoxGJV: TComboBox [11]
      Left = 299
      ExplicitLeft = 299
    end
    inherited GJVOKButton: TButton [12]
      TabOrder = 14
    end
    object BandMapPanel: TPanel [13]
      Left = 394
      Top = 111
      Width = 177
      Height = 34
      TabOrder = 13
      object BandMapButton: TButton
        Left = 14
        Top = 3
        Width = 67
        Height = 25
        Caption = 'Band &Map'
        TabOrder = 0
        TabStop = False
        OnClick = BandMapButtonClick
      end
      object AutoBandmapTune: TCheckBox
        Left = 87
        Top = 0
        Width = 49
        Height = 17
        TabStop = False
        Caption = 'Tune'
        TabOrder = 1
      end
      object AutoBandmapTime: TCheckBox
        Left = 87
        Top = 15
        Width = 46
        Height = 17
        TabStop = False
        Caption = 'Time'
        TabOrder = 2
      end
    end
    inherited CommentsEdit: TLabeledEdit [14]
      TabOrder = 10
    end
    inherited SerTXEdit: TLabeledEdit [15]
      TabOrder = 5
    end
    inherited QTHEdit: TLabeledEdit
      TabOrder = 9
    end
    inherited CallsignEdit: TLabeledEdit [17]
      TabOrder = 3
    end
    inherited DeletedCheckBox: TCheckBox [18]
      TabOrder = 16
    end
    inherited SerRXEdit: TLabeledEdit [19]
      TabOrder = 7
    end
    inherited RSTRXEdit: TLabeledEdit [20]
      TabOrder = 6
    end
    inherited NonScoreCheckBox: TCheckBox [21]
      Left = 404
      ExplicitLeft = 404
    end
    inherited RSTTXEdit: TLabeledEdit [22]
      TabOrder = 4
    end
    inherited LocEdit: TLabeledEdit [23]
      TabOrder = 8
    end
    inherited ModeButton: TButton
      Left = 359
      ExplicitLeft = 359
    end
    inherited FirstUnfilledButton: TButton
      TabOrder = 20
      OnClick = FirstUnfilledButtonClick
    end
    inherited CatchupButton: TButton
      TabOrder = 15
      OnClick = CatchupButtonClick
    end
    inherited SecondOpComboBox: TComboBox
      TabOrder = 19
      OnExit = SecondOpComboBoxExit
    end
    inherited MainOpComboBox: TComboBox
      OnExit = MainOpComboBoxExit
    end
  end
end
