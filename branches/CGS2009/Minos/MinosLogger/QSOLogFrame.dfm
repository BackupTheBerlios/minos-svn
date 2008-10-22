inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    inherited ModeComboBoxGJV: TComboBox [3]
    end
    inherited RSTTXEdit: TLabeledEdit [4]
    end
    inherited GJVOKButton: TButton [5]
    end
    inherited GJVForceButton: TButton [6]
    end
    inherited GJVCancelButton: TButton [7]
    end
    inherited DateEdit: TLabeledEdit [8]
    end
    inherited NonScoreCheckBox: TCheckBox [9]
    end
    inherited SerTXEdit: TLabeledEdit [10]
    end
    inherited RSTRXEdit: TLabeledEdit [11]
    end
    inherited SerRXEdit: TLabeledEdit [12]
    end
    inherited QTHEdit: TLabeledEdit [13]
    end
    inherited TimeEdit: TLabeledEdit [14]
      Left = 144
      EditLabel.ExplicitLeft = 119
      EditLabel.ExplicitTop = 7
      EditLabel.ExplicitWidth = 22
      ExplicitLeft = 144
    end
    inherited CallsignEdit: TLabeledEdit [15]
    end
    inherited DeletedCheckBox: TCheckBox [16]
      TabOrder = 18
    end
    inherited CommentsEdit: TLabeledEdit [17]
    end
    inherited LocEdit: TLabeledEdit [18]
    end
    object MatchXferButton: TBitBtn
      Left = 321
      Top = 116
      Width = 67
      Height = 25
      Hint = 'Get fields from current matching QSO'
      Caption = 'Match &Xfer'
      DoubleBuffered = True
      Enabled = False
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 15
      TabStop = False
    end
    object BandMapPanel: TPanel
      Left = 394
      Top = 111
      Width = 177
      Height = 34
      TabOrder = 17
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
  end
end
