object SingleLogFrame: TSingleLogFrame
  Left = 0
  Top = 0
  Width = 790
  Height = 470
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  ParentFont = False
  TabOrder = 0
  TabStop = True
  object Splitter_Image: TImage
    Left = 60
    Top = 100
    Width = 4
    Height = 4
    AutoSize = True
    Picture.Data = {
      07544269746D617066000000424D660000000000000036000000280000000400
      000004000000010018000000000030000000C40E0000C40E0000000000000000
      0000FF00FFFF00FFFFFFFFFFFFFFFF00FF000000C0C0C0FFFFFFFF00FF000000
      000000FF00FFFF00FFFF00FFFF00FFFF00FF}
    Transparent = True
    Visible = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 790
    Height = 470
    Align = alClient
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    object LogAreaSplitter: TSplitter
      Left = 0
      Top = 373
      Width = 790
      Height = 4
      Cursor = crVSplit
      Align = alBottom
      Color = clBtnFace
      MinSize = 1
      ParentColor = False
      OnMoved = LogAreaSplitterMoved
      ExplicitTop = 369
    end
    object TopPanel: TPanel
      Left = 0
      Top = 0
      Width = 790
      Height = 373
      Align = alClient
      Caption = 'TopPanel'
      ParentColor = True
      TabOrder = 0
      object MultSplitter: TSplitter
        Left = 470
        Top = 1
        Width = 9
        Height = 216
        Hint = 'Click here to show/hide stats and multipliers'
        Align = alRight
        Color = clBtnFace
        MinSize = 1
        ParentColor = False
        OnMoved = MultSplitterMoved
        ExplicitLeft = 429
        ExplicitHeight = 300
      end
      object MultPanel: TPanel
        Left = 479
        Top = 1
        Width = 310
        Height = 216
        Align = alRight
        BevelOuter = bvNone
        Caption = 'MultPanel'
        TabOrder = 0
        inline MultDispFrame: TMultDispFrame
          Left = 0
          Top = 0
          Width = 310
          Height = 216
          Align = alClient
          TabOrder = 0
          TabStop = True
          ExplicitWidth = 310
          ExplicitHeight = 216
          inherited FilterSplitter: TSplitter
            Top = 106
            Width = 310
            ExplicitTop = 161
            ExplicitWidth = 310
          end
          inherited MultPages: TPageControl
            Width = 310
            Height = 106
            TabStop = False
            ExplicitWidth = 310
            ExplicitHeight = 106
            inherited DistrictTab: TTabSheet
              inherited DistrictMultTree: TVirtualStringTree
                TabStop = False
              end
            end
            inherited LocatorsTab: TTabSheet
              ExplicitWidth = 182
              ExplicitHeight = 77
              inherited LocatorMultTree: TVirtualStringTree
                Width = 202
                Height = 98
                TabStop = False
                ExplicitWidth = 182
                ExplicitHeight = 77
              end
            end
            inherited StatsTab: TTabSheet
              inherited StatsDispFrame: TStatsDispFrame
                TabStop = False
                inherited Panel1: TPanel
                  inherited P1Edit: TLabeledEdit
                    TabStop = False
                    EditLabel.ExplicitLeft = 8
                    EditLabel.ExplicitTop = -1
                    EditLabel.ExplicitWidth = 47
                  end
                  inherited P2Edit: TLabeledEdit
                    TabStop = False
                    EditLabel.ExplicitLeft = 79
                    EditLabel.ExplicitTop = -2
                    EditLabel.ExplicitWidth = 47
                  end
                end
                inherited RecheckTimer: TTimer
                  Top = 40
                end
              end
            end
            inherited ErrTab: TTabSheet
              inherited ErrList: TListBox
                TabStop = False
              end
            end
            inherited CompSheet: TTabSheet
              inherited CompFrame: TCompFrame
                Left = 0
                Top = 0
                Width = 202
                Height = 98
                Align = alClient
                ExplicitLeft = 0
                ExplicitTop = 0
                ExplicitWidth = 248
                ExplicitHeight = 182
                inherited CompGrid: TVirtualStringTree
                  Width = 202
                  Height = 98
                  ExplicitWidth = 248
                  ExplicitHeight = 182
                end
              end
            end
          end
          inherited FilterPanel: TPanel
            Top = 116
            Width = 310
            ExplicitTop = 95
            ExplicitWidth = 310
            inherited UnworkedCB: TCheckBox
              Width = 121
              TabStop = False
              ExplicitWidth = 121
            end
            inherited ContEU: TCheckBox
              TabStop = False
            end
            inherited ContAS: TCheckBox
              TabStop = False
            end
            inherited ContAF: TCheckBox
              TabStop = False
            end
            inherited ContOC: TCheckBox
              TabStop = False
            end
            inherited ContSA: TCheckBox
              TabStop = False
            end
            inherited ContNA: TCheckBox
              TabStop = False
            end
            inherited WorkedCB: TCheckBox
              TabStop = False
            end
          end
          inherited FilterTimer: TTimer
            Left = 12
          end
        end
      end
      inline LogMonitor: TLogMonitorFrame
        Left = 1
        Top = 1
        Width = 469
        Height = 216
        Align = alClient
        TabOrder = 1
        TabStop = True
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitWidth = 469
        ExplicitHeight = 216
        inherited QSOTree: TVirtualStringTree
          Width = 469
          Height = 216
          HintMode = hmHint
          ParentShowHint = False
          ShowHint = True
          OnDblClick = LogMonitorQSOTreeDblClick
          OnEnter = LogMonitorQSOTreeEnter
          OnGetHint = LogMonitorQSOTreeGetHint
          OnKeyDown = LogMonitorQSOTreeKeyDown
          OnMouseDown = LogMonitorQSOTreeMouseDown
          ExplicitLeft = 3
          ExplicitTop = -6
          ExplicitWidth = 469
          ExplicitHeight = 213
        end
      end
      object LogDetailPanel: TPanel
        Left = 1
        Top = 217
        Width = 788
        Height = 155
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        object NextContactDetailsSplitter: TSplitter
          Left = 624
          Top = 0
          Width = 9
          Height = 155
          Hint = 'Click here to show/hide Next Contact Details'
          Align = alRight
          Color = clBtnFace
          MinSize = 1
          ParentColor = False
          OnMoved = NextContactDetailsSplitterMoved
          ExplicitLeft = 583
          ExplicitHeight = 112
        end
        object NextContactDetailsPanel: TPanel
          Left = 633
          Top = 0
          Width = 155
          Height = 155
          Align = alRight
          BevelOuter = bvLowered
          BevelWidth = 3
          TabOrder = 0
          ExplicitHeight = 154
          object NextContactDetailsLabel: TLabel
            Left = 3
            Top = 38
            Width = 149
            Height = 114
            Hint = 
              'Next contact details (use Right Mouse Button if you want to move' +
              ' it)'
            Align = alClient
            Alignment = taCenter
            AutoSize = False
            Caption = 'Minos'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -21
            Font.Name = 'Verdana'
            Font.Style = [fsBold]
            ParentFont = False
            ParentShowHint = False
            PopupMenu = EntryChoiceMenu
            ShowHint = True
            Layout = tlCenter
            WordWrap = True
            ExplicitLeft = 6
            ExplicitTop = 43
            ExplicitWidth = 146
            ExplicitHeight = 110
          end
          object BandPanel: TPanel
            Left = 3
            Top = 3
            Width = 149
            Height = 35
            Align = alTop
            BevelOuter = bvNone
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -19
            Font.Name = 'Verdana'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object BandCombo: TComboBox
              Left = 0
              Top = 0
              Width = 149
              Height = 31
              Align = alClient
              Style = csDropDownList
              ItemHeight = 0
              TabOrder = 0
              TabStop = False
            end
          end
        end
        inline GJVQSOLogFrame: TGJVQSOLogFrame
          Left = 0
          Top = 0
          Width = 624
          Height = 155
          Align = alClient
          PopupMenu = EntryChoiceMenu
          TabOrder = 1
          TabStop = True
          ExplicitWidth = 624
          ExplicitHeight = 155
          inherited G0GJVPanel: TPanel
            Width = 624
            Height = 155
            ExplicitWidth = 624
            ExplicitHeight = 155
            inherited BandMapPanel: TPanel
              inherited AutoBandmapTune: TCheckBox
                OnClick = AutoBandmapTuneClick
              end
              inherited AutoBandmapTime: TCheckBox
                OnClick = AutoBandmapTimeClick
              end
            end
            inherited MatchXferButton: TBitBtn
              OnClick = GJVQSOLogFrame1MatchXferButtonClick
            end
            inherited CallsignEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 4
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 59
            end
            inherited RSTTXEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 108
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 47
            end
            inherited SerTXEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 184
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 41
            end
            inherited RSTRXEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 254
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 47
            end
            inherited SerRXEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 337
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 42
            end
            inherited LocEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 420
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 39
            end
            inherited QTHEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 504
              EditLabel.ExplicitTop = 67
              EditLabel.ExplicitWidth = 47
            end
            inherited CommentsEdit: TLabeledEdit
              EditLabel.ExplicitLeft = 4
              EditLabel.ExplicitTop = 110
              EditLabel.ExplicitWidth = 50
            end
            inherited GJVOKButton: TButton
              OnClick = GJVQSOLogFrame1GJVOKButtonClick
            end
            inherited GJVForceButton: TButton
              OnClick = GJVQSOLogFrame1GJVForceButtonClick
            end
            inherited GJVCancelButton: TButton
              OnClick = GJVQSOLogFrame1GJVCancelButtonClick
            end
          end
        end
      end
    end
    object MatchPanel: TPanel
      Left = 0
      Top = 377
      Width = 790
      Height = 93
      Align = alBottom
      Caption = 'MatchPanel'
      TabOrder = 1
      object ArchiveMatchSplitter: TSplitter
        Left = 616
        Top = 1
        Width = 9
        Height = 91
        Hint = 'Click here to show/hide archive matches'
        Align = alRight
        Color = clBtnFace
        MinSize = 1
        ParentColor = False
        OnMoved = ArchiveMatchSplitterMoved
        ExplicitLeft = 552
        ExplicitHeight = 109
      end
      object ArchiveMatchPanel: TPanel
        Left = 625
        Top = 1
        Width = 164
        Height = 91
        Align = alRight
        TabOrder = 0
        ExplicitHeight = 109
        object ArchiveMatchTree: TVirtualStringTree
          Left = 1
          Top = 1
          Width = 162
          Height = 89
          Hint = 'Matching archive entries'
          Align = alClient
          BevelKind = bkFlat
          Header.AutoSizeIndex = 0
          Header.DefaultHeight = 17
          Header.MainColumn = -1
          Header.Options = [hoColumnResize, hoDrag]
          Header.ParentFont = True
          ParentShowHint = False
          PopupMenu = EntryChoiceMenu
          ShowHint = True
          TabOrder = 0
          TabStop = False
          TreeOptions.SelectionOptions = [toFullRowSelect]
          OnDblClick = ArchiveMatchTreeDblClick
          OnEnter = ArchiveMatchTreeEnter
          OnGetText = ArchiveMatchTreeGetText
          OnGetNodeDataSize = ArchiveMatchTreeGetNodeDataSize
          OnMouseDown = ArchiveMatchTreeMouseDown
          ExplicitHeight = 107
          Columns = <>
        end
      end
      object ContestMatchPanel: TPanel
        Left = 1
        Top = 1
        Width = 615
        Height = 91
        Align = alClient
        Caption = 'ContestMatchPanel'
        TabOrder = 1
        ExplicitHeight = 109
        object MatchSplitter: TSplitter
          Left = 435
          Top = 1
          Width = 9
          Height = 89
          Hint = 'Click here to show/hide other contest matches'
          Align = alRight
          Color = clBtnFace
          MinSize = 1
          ParentColor = False
          OnMoved = MatchSplitterMoved
          ExplicitLeft = 152
          ExplicitHeight = 107
        end
        object ThisMatchTree: TVirtualStringTree
          Left = 1
          Top = 1
          Width = 434
          Height = 89
          Hint = 'Matching QSOs in current contest'
          Align = alClient
          BevelKind = bkFlat
          Header.AutoSizeIndex = 0
          Header.DefaultHeight = 17
          Header.MainColumn = -1
          Header.Options = [hoColumnResize, hoDrag]
          Header.ParentFont = True
          ParentShowHint = False
          PopupMenu = EntryChoiceMenu
          ShowHint = True
          TabOrder = 0
          TabStop = False
          TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoSpanColumns, toAutoTristateTracking, toAutoDeleteMovedNodes]
          TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
          TreeOptions.SelectionOptions = [toFullRowSelect]
          OnBeforeItemErase = ThisMatchTreeBeforeItemErase
          OnDblClick = ThisMatchTreeDblClick
          OnEnter = ThisMatchTreeEnter
          OnGetText = ThisMatchTreeGetText
          OnPaintText = ThisMatchTreePaintText
          OnGetNodeDataSize = ThisMatchTreeGetNodeDataSize
          OnKeyDown = ThisMatchTreeKeyDown
          OnMouseDown = ThisMatchTreeMouseDown
          ExplicitHeight = 107
          Columns = <>
        end
        object OtherMatchPanel: TPanel
          Left = 444
          Top = 1
          Width = 170
          Height = 89
          Align = alRight
          TabOrder = 1
          ExplicitHeight = 107
          object OtherMatchTree: TVirtualStringTree
            Left = 1
            Top = 1
            Width = 168
            Height = 87
            Hint = 'Matching QSOs in any other loaded contests'
            Align = alClient
            BevelKind = bkFlat
            Header.AutoSizeIndex = 0
            Header.DefaultHeight = 17
            Header.MainColumn = -1
            Header.Options = [hoColumnResize, hoDrag]
            Header.ParentFont = True
            ParentShowHint = False
            PopupMenu = EntryChoiceMenu
            ShowHint = True
            TabOrder = 0
            TabStop = False
            TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoSpanColumns, toAutoTristateTracking, toAutoDeleteMovedNodes]
            TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
            TreeOptions.SelectionOptions = [toFullRowSelect]
            OnDblClick = OtherMatchTreeDblClick
            OnEnter = OtherMatchTreeEnter
            OnGetText = OtherMatchTreeGetText
            OnGetNodeDataSize = OtherMatchTreeGetNodeDataSize
            OnKeyDown = OtherMatchTreeKeyDown
            OnMouseDown = OtherMatchTreeMouseDown
            ExplicitHeight = 105
            Columns = <>
          end
        end
      end
    end
  end
  object EntryChoiceMenu: TPopupMenu
    OnPopup = EntryChoiceMenuPopup
    Left = 328
    Top = 222
    object MenuEditContact: TMenuItem
      Action = LogContainer.EditContactAction
    end
    object MenuEditSeparator: TMenuItem
      Caption = '-'
      Visible = False
    end
    object Close1: TMenuItem
      Action = LogContainer.FileCloseAction1
    end
    object Details1: TMenuItem
      Action = LogContainer.ContestDetailsAction
    end
    object ProduceEntryExportFile1: TMenuItem
      Action = LogContainer.MakeEntryAction
    end
    object Details2: TMenuItem
      Action = LogContainer.AnalyseMinosLogAction
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object Go1: TMenuItem
      Action = LogContainer.GoToSerialAction
    end
    object GotoNextUnfilled1: TMenuItem
      Action = LogContainer.NextUnfilledAction
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object NextContactDetailsOnLeft: TMenuItem
      Caption = 'Next Contact Details On Left'
      OnClick = NextContactDetailsOnLeftClick
    end
    object Multiplecontesttablines1: TMenuItem
      Action = LogContainer.ScrollingContestTabsAction
    end
    object ShowOperators1: TMenuItem
      Action = LogContainer.ShowOperatorsAction
    end
    object Shifttableft1: TMenuItem
      Action = LogContainer.ShiftTabLeftAction
    end
    object Shifttabright1: TMenuItem
      Action = LogContainer.ShiftTabRightAction
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Cancel1: TMenuItem
      Caption = 'Cancel'
    end
  end
  object NextContactDetailsTimer: TTimer
    OnTimer = NextContactDetailsTimerTimer
    Left = 704
    Top = 312
  end
  object PublishTimer: TTimer
    OnTimer = PublishTimerTimer
    Left = 40
    Top = 40
  end
  object OnShowTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = OnShowTimerTimer
    Left = 8
    Top = 40
  end
  object TimerUpdateQSOTimer: TTimer
    OnTimer = TimerUpdateQSOTimerTimer
    Left = 16
    Top = 184
  end
end
