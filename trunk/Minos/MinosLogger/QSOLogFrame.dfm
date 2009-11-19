inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    inherited SerTXEdit: TLabeledEdit [3]
      TabOrder = 7
    end
    inherited NonScoreCheckBox: TCheckBox [4]
    end
    inherited ModeComboBoxGJV: TComboBox [5]
    end
    inherited RSTTXEdit: TLabeledEdit [6]
      TabOrder = 6
    end
    inherited GJVOKButton: TButton [7]
      TabOrder = 16
    end
    inherited GJVForceButton: TButton [8]
      TabOrder = 14
    end
    inherited GJVCancelButton: TButton [9]
      TabOrder = 19
    end
    inherited DateEdit: TLabeledEdit [10]
      TabOrder = 4
    end
    object MatchXferButton: TBitBtn [11]
      Left = 321
      Top = 116
      Width = 67
      Height = 25
      Hint = 'Get fields from current matching QSO'
      Caption = 'Match &Xfer'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 13
      TabStop = False
    end
    object BandMapPanel: TPanel [12]
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
    inherited TimeEdit: TLabeledEdit [13]
      TabOrder = 17
    end
    inherited CallsignEdit: TLabeledEdit [14]
      TabOrder = 5
    end
    inherited DeletedCheckBox: TCheckBox [15]
      TabOrder = 18
    end
    inherited CommentsEdit: TLabeledEdit [16]
      TabOrder = 12
    end
    inherited LocEdit: TLabeledEdit [17]
      TabOrder = 10
    end
    inherited SerRXEdit: TLabeledEdit [18]
      TabOrder = 9
    end
    inherited RSTRXEdit: TLabeledEdit [19]
      TabOrder = 8
    end
    inherited QTHEdit: TLabeledEdit [20]
      TabOrder = 11
    end
    inherited FirstUnfilledButton: TButton
      TabOrder = 20
      OnClick = FirstUnfilledButtonClick
    end
    inherited BackfillButton: TButton
      TabOrder = 21
      OnClick = BackfillButtonClick
    end
  end
end
