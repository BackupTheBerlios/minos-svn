inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    inherited LocEdit: TLabeledEdit [3]
      TabOrder = 10
    end
    inherited CommentsEdit: TLabeledEdit [4]
      TabOrder = 12
    end
    inherited DeletedCheckBox: TCheckBox [5]
      TabOrder = 18
    end
    inherited TimeEdit: TLabeledEdit [6]
      TabOrder = 17
    end
    inherited CallsignEdit: TLabeledEdit [7]
      TabOrder = 5
    end
    inherited SerRXEdit: TLabeledEdit
      TabOrder = 9
    end
    inherited QTHEdit: TLabeledEdit [9]
      TabOrder = 11
    end
    inherited NonScoreCheckBox: TCheckBox [10]
    end
    inherited RSTRXEdit: TLabeledEdit [11]
      TabOrder = 8
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
    inherited SerTXEdit: TLabeledEdit [13]
      TabOrder = 7
    end
    object MatchXferButton: TBitBtn [14]
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
    inherited ModeComboBoxGJV: TComboBox [15]
    end
    inherited RSTTXEdit: TLabeledEdit [16]
      TabOrder = 6
    end
    inherited GJVOKButton: TButton [17]
      TabOrder = 16
    end
    inherited GJVForceButton: TButton [18]
      TabOrder = 14
    end
    inherited GJVCancelButton: TButton [19]
      TabOrder = 19
    end
    inherited DateEdit: TLabeledEdit [20]
      TabOrder = 4
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
