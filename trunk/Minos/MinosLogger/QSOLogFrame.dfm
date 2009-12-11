inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    inherited BrgSt: TLabel
      Top = 13
      ExplicitTop = 13
    end
    inherited DistSt: TLabel
      Top = 13
      ExplicitTop = 13
    end
    inherited DateEdit: TLabeledEdit [5]
      TabOrder = 4
    end
    inherited SerRXEdit: TLabeledEdit [6]
      TabOrder = 9
    end
    inherited DeletedCheckBox: TCheckBox [7]
      TabOrder = 18
    end
    inherited TimeEdit: TLabeledEdit [8]
      TabOrder = 17
    end
    inherited CallsignEdit: TLabeledEdit [9]
      TabOrder = 5
    end
    inherited QTHEdit: TLabeledEdit [10]
      TabOrder = 11
    end
    inherited GJVCancelButton: TButton [11]
      TabOrder = 19
    end
    inherited GJVForceButton: TButton [12]
      TabOrder = 14
    end
    inherited RSTTXEdit: TLabeledEdit [13]
      TabOrder = 6
    end
    inherited NonScoreCheckBox: TCheckBox [14]
    end
    inherited RSTRXEdit: TLabeledEdit [15]
      TabOrder = 8
    end
    inherited LocEdit: TLabeledEdit [16]
      TabOrder = 10
    end
    object MatchXferButton: TBitBtn [17]
      Left = 321
      Top = 112
      Width = 67
      Height = 30
      Hint = 'Get fields from current matching QSO'
      Caption = 'Match &Xfer F12'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 13
      TabStop = False
      WordWrap = True
    end
    inherited ModeComboBoxGJV: TComboBox [18]
    end
    inherited GJVOKButton: TButton [19]
      TabOrder = 16
    end
    object BandMapPanel: TPanel [20]
      Left = 394
      Top = 111
      Width = 177
      Height = 34
      TabOrder = 15
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
    inherited CommentsEdit: TLabeledEdit [21]
      TabOrder = 12
    end
    inherited SerTXEdit: TLabeledEdit [22]
      TabOrder = 7
    end
    inherited FirstUnfilledButton: TButton
      TabOrder = 20
      OnClick = FirstUnfilledButtonClick
    end
    inherited CatchupButton: TButton
      TabOrder = 21
      OnClick = CatchupButtonClick
    end
    inherited SecondOpComboBox: TComboBox
      TabOrder = 22
      OnExit = SecondOpComboBoxExit
      OnKeyPress = nil
    end
    inherited MainOpComboBox: TComboBox
      TabOrder = 23
      OnExit = MainOpComboBoxExit
      OnKeyPress = nil
    end
  end
end
