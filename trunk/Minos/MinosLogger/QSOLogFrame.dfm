inherited GJVQSOLogFrame: TGJVQSOLogFrame
  inherited G0GJVPanel: TPanel
    object DateLabel: TStaticText [5]
      Left = 6
      Top = 42
      Width = 82
      Height = 14
      Alignment = taRightJustify
      AutoSize = False
      Caption = '00/00/00'
      TabOrder = 21
    end
    object TimeLabel: TStaticText [6]
      Left = 108
      Top = 42
      Width = 48
      Height = 14
      AutoSize = False
      Caption = '00:00'
      TabOrder = 22
    end
    object BandMapPanel: TPanel [7]
      Left = 424
      Top = 111
      Width = 177
      Height = 34
      TabOrder = 19
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
    object MatchXferButton: TBitBtn [8]
      Left = 335
      Top = 109
      Width = 74
      Height = 39
      Hint = 'Get fields from current matching QSO'
      Caption = 'Match &Xfer F12'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 20
      TabStop = False
      WordWrap = True
    end
  end
end
