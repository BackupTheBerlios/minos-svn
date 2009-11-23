inherited GJVQSOLogFrame: TGJVQSOLogFrame
  Width = 576
  ExplicitWidth = 576
  inherited G0GJVPanel: TPanel
    Width = 576
    ExplicitWidth = 576
    ExplicitHeight = 153
    inherited LocEdit: TLabeledEdit [3]
    end
    inherited CommentsEdit: TLabeledEdit [4]
    end
    inherited DeletedCheckBox: TCheckBox [5]
      TabOrder = 18
    end
    inherited CallsignEdit: TLabeledEdit [6]
    end
    inherited TimeEdit: TLabeledEdit [7]
      Left = 144
      EditLabel.ExplicitLeft = 119
      EditLabel.ExplicitTop = 7
      EditLabel.ExplicitWidth = 22
      ExplicitLeft = 144
    end
    inherited QTHEdit: TLabeledEdit [8]
    end
    inherited SerRXEdit: TLabeledEdit [9]
    end
    inherited RSTRXEdit: TLabeledEdit [10]
    end
    inherited SerTXEdit: TLabeledEdit [11]
    end
    inherited NonScoreCheckBox: TCheckBox [12]
    end
    inherited ModeComboBoxGJV: TComboBox [13]
    end
    inherited RSTTXEdit: TLabeledEdit [14]
    end
    inherited GJVOKButton: TButton [15]
    end
    inherited GJVForceButton: TButton [16]
    end
    inherited GJVCancelButton: TButton [17]
    end
    inherited DateEdit: TLabeledEdit [18]
    end
    object MatchXferButton: TBitBtn
      Left = 321
      Top = 116
      Width = 67
      Height = 25
      Hint = 'Get fields from current matching QSO'
      Caption = 'Match &Xfer'
      Enabled = False
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
