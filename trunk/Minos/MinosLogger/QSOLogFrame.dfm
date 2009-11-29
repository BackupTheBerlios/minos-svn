inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    inherited GJVForceButton: TButton [3]
      TabOrder = 14
    end
    inherited NonScoreCheckBox: TCheckBox [4]
    end
    inherited QTHEdit: TLabeledEdit [5]
      TabOrder = 11
    end
    inherited CallsignEdit: TLabeledEdit [6]
      TabOrder = 5
    end
    inherited TimeEdit: TLabeledEdit [7]
      TabOrder = 17
    end
    inherited DeletedCheckBox: TCheckBox [8]
      TabOrder = 18
    end
    inherited SerRXEdit: TLabeledEdit [9]
      TabOrder = 9
    end
    inherited LocEdit: TLabeledEdit [10]
      TabOrder = 10
    end
    inherited RSTRXEdit: TLabeledEdit [11]
      TabOrder = 8
    end
    inherited RSTTXEdit: TLabeledEdit [12]
      TabOrder = 6
    end
    inherited CommentsEdit: TLabeledEdit [13]
      TabOrder = 12
    end
    inherited DateEdit: TLabeledEdit [14]
      TabOrder = 4
    end
    inherited GJVCancelButton: TButton
      TabOrder = 19
    end
    object BandMapPanel: TPanel [16]
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
    inherited GJVOKButton: TButton [17]
      TabOrder = 16
    end
    inherited ModeComboBoxGJV: TComboBox [18]
    end
    object MatchXferButton: TBitBtn [19]
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
    inherited SerTXEdit: TLabeledEdit [20]
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
  end
end
