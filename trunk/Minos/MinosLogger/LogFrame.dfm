object SingleLogFrame: TSingleLogFrame
  Left = 0
  Top = 0
  Width = 750
  Height = 550
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  ParentFont = False
  TabOrder = 0
  TabStop = True
  OnMouseMove = FrameMouseMove
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
    Width = 750
    Height = 550
    Align = alClient
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    object LogAreaSplitter: TSplitter
      Left = 0
      Top = 431
      Width = 750
      Height = 8
      Cursor = crVSplit
      Align = alBottom
      Color = clBtnFace
      MinSize = 1
      ParentColor = False
      OnMoved = LogAreaSplitterMoved
      ExplicitTop = 273
    end
    object TopPanel: TPanel
      Left = 0
      Top = 0
      Width = 750
      Height = 431
      Align = alClient
      Caption = 'TopPanel'
      ParentColor = True
      TabOrder = 0
      object MultSplitter: TSplitter
        Left = 430
        Top = 1
        Width = 9
        Height = 275
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
        Left = 439
        Top = 1
        Width = 310
        Height = 275
        Align = alRight
        BevelOuter = bvNone
        Caption = 'MultPanel'
        TabOrder = 0
        inline MultDispFrame: TMultDispFrame
          Left = 0
          Top = 0
          Width = 310
          Height = 275
          Align = alClient
          TabOrder = 0
          TabStop = True
          ExplicitWidth = 310
          ExplicitHeight = 275
          inherited FilterSplitter: TSplitter
            Top = 165
            Width = 310
            ExplicitTop = 161
            ExplicitWidth = 310
          end
          inherited MultPages: TPageControl
            Width = 310
            Height = 165
            ActivePage = MultDispFrame.StatsTab
            TabStop = False
            ExplicitWidth = 310
            ExplicitHeight = 165
            inherited CountriesTab: TTabSheet
              ExplicitLeft = 4
              ExplicitTop = 4
              ExplicitWidth = 248
              ExplicitHeight = 182
            end
            inherited DistrictTab: TTabSheet
              ExplicitLeft = 4
              ExplicitTop = 4
              ExplicitWidth = 248
              ExplicitHeight = 182
              inherited DistrictMultTree: TVirtualStringTree
                TabStop = False
              end
            end
            inherited LocatorsTab: TTabSheet
              ExplicitLeft = 4
              ExplicitTop = 4
              ExplicitWidth = 248
              ExplicitHeight = 182
              inherited LocatorMultTree: TVirtualStringTree
                TabStop = False
              end
            end
            inherited StatsTab: TTabSheet
              ExplicitWidth = 262
              ExplicitHeight = 157
              inherited StatsDispFrame: TStatsDispFrame
                Width = 262
                Height = 157
                TabStop = False
                ExplicitWidth = 262
                ExplicitHeight = 157
                inherited SLabel1: TLabel
                  Width = 262
                  Height = 111
                  ExplicitWidth = 4
                end
                inherited Panel1: TPanel
                  Top = 111
                  Width = 262
                  ExplicitTop = 111
                  ExplicitWidth = 262
                  inherited PeriodSetButton: TButton
                    TabStop = False
                  end
                  inherited P1Edit: TLabeledEdit
                    TabStop = False
                    EditLabel.Width = 59
                    EditLabel.ExplicitLeft = 2
                    EditLabel.ExplicitTop = 7
                    EditLabel.ExplicitWidth = 59
                  end
                  inherited P2Edit: TLabeledEdit
                    TabStop = False
                    EditLabel.Width = 55
                    EditLabel.ExplicitLeft = 61
                    EditLabel.ExplicitTop = 7
                    EditLabel.ExplicitWidth = 55
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
          end
          inherited FilterPanel: TPanel
            Top = 175
            Width = 310
            ExplicitTop = 175
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
        Width = 429
        Height = 275
        Align = alClient
        TabOrder = 1
        TabStop = True
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitWidth = 429
        ExplicitHeight = 275
        inherited QSOTree: TVirtualStringTree
          Width = 429
          Height = 275
          OnDblClick = LogMonitorQSOTreeDblClick
          OnEnter = LogMonitorQSOTreeEnter
          OnKeyDown = LogMonitorQSOTreeKeyDown
          OnMouseDown = LogMonitorQSOTreeMouseDown
          ExplicitWidth = 429
          ExplicitHeight = 275
        end
      end
      object LogDetailPanel: TPanel
        Left = 1
        Top = 276
        Width = 748
        Height = 154
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        object NextContactDetailsSplitter: TSplitter
          Left = 584
          Top = 0
          Width = 9
          Height = 154
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
          Left = 593
          Top = 0
          Width = 155
          Height = 154
          Align = alRight
          BevelOuter = bvLowered
          BevelWidth = 3
          TabOrder = 0
          object NextContactDetailsLabel: TLabel
            Left = 3
            Top = 38
            Width = 149
            Height = 113
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
              ItemHeight = 23
              TabOrder = 0
              TabStop = False
            end
          end
        end
        inline GJVQSOLogFrame: TGJVQSOLogFrame
          Left = 0
          Top = 0
          Width = 584
          Height = 154
          Align = alClient
          PopupMenu = EntryChoiceMenu
          TabOrder = 1
          TabStop = True
          ExplicitWidth = 584
          ExplicitHeight = 154
          inherited G0GJVPanel: TPanel
            Width = 584
            Height = 154
            ExplicitWidth = 584
            ExplicitHeight = 154
            inherited GJVOKButton: TButton
              OnClick = GJVQSOLogFrame1GJVOKButtonClick
            end
            inherited GJVForceButton: TButton
              OnClick = GJVQSOLogFrame1GJVForceButtonClick
            end
            inherited GJVCancelButton: TButton
              OnClick = GJVQSOLogFrame1GJVCancelButtonClick
            end
            inherited TimeNowButton: TButton
              OnClick = SetTimeNowClick
            end
            inherited MatchXferButton: TBitBtn
              OnClick = GJVQSOLogFrame1MatchXferButtonClick
            end
            inherited BandMapPanel: TPanel
              inherited AutoBandmapTune: TCheckBox
                OnClick = AutoBandmapTuneClick
              end
              inherited AutoBandmapTime: TCheckBox
                OnClick = AutoBandmapTimeClick
              end
            end
          end
        end
      end
    end
    object MatchPanel: TPanel
      Left = 0
      Top = 439
      Width = 750
      Height = 111
      Align = alBottom
      Caption = 'MatchPanel'
      TabOrder = 1
      object ArchiveMatchSplitter: TSplitter
        Left = 576
        Top = 1
        Width = 9
        Height = 109
        Hint = 'Click here to show/hide archive matches'
        Align = alRight
        Color = clBtnFace
        MinSize = 1
        ParentColor = False
        OnMoved = ArchiveMatchSplitterMoved
        ExplicitLeft = 552
      end
      object ArchiveMatchPanel: TPanel
        Left = 585
        Top = 1
        Width = 164
        Height = 109
        Align = alRight
        TabOrder = 0
        object ArchiveMatchTree: TVirtualStringTree
          Left = 1
          Top = 1
          Width = 162
          Height = 107
          Hint = 'Matching archive entries'
          Align = alClient
          BevelKind = bkFlat
          Header.AutoSizeIndex = 0
          Header.DefaultHeight = 17
          Header.Font.Charset = DEFAULT_CHARSET
          Header.Font.Color = clWindowText
          Header.Font.Height = -11
          Header.Font.Name = 'Verdana'
          Header.Font.Style = []
          Header.MainColumn = -1
          Header.Options = [hoColumnResize, hoDrag]
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
          OnKeyDown = ArchiveMatchTreeKeyDown
          OnMouseDown = ArchiveMatchTreeMouseDown
          Columns = <>
        end
      end
      object ContestMatchPanel: TPanel
        Left = 1
        Top = 1
        Width = 575
        Height = 109
        Align = alClient
        Caption = 'ContestMatchPanel'
        TabOrder = 1
        object MatchSplitter: TSplitter
          Left = 395
          Top = 1
          Width = 9
          Height = 107
          Hint = 'Click here to show/hide other contest matches'
          Align = alRight
          Color = clBtnFace
          MinSize = 1
          ParentColor = False
          OnMoved = MatchSplitterMoved
          ExplicitLeft = 152
        end
        object ThisMatchTree: TVirtualStringTree
          Left = 1
          Top = 1
          Width = 394
          Height = 107
          Hint = 'Matching QSOs in current contest'
          Align = alClient
          BevelKind = bkFlat
          Header.AutoSizeIndex = 0
          Header.DefaultHeight = 17
          Header.Font.Charset = DEFAULT_CHARSET
          Header.Font.Color = clWindowText
          Header.Font.Height = -11
          Header.Font.Name = 'Verdana'
          Header.Font.Style = []
          Header.MainColumn = -1
          Header.Options = [hoColumnResize, hoDrag]
          ParentShowHint = False
          PopupMenu = EntryChoiceMenu
          ShowHint = True
          TabOrder = 0
          TabStop = False
          TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoSpanColumns, toAutoTristateTracking, toAutoDeleteMovedNodes]
          TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
          TreeOptions.SelectionOptions = [toFullRowSelect]
          OnDblClick = ThisMatchTreeDblClick
          OnEnter = ThisMatchTreeEnter
          OnGetText = ThisMatchTreeGetText
          OnPaintText = ThisMatchTreePaintText
          OnGetNodeDataSize = ThisMatchTreeGetNodeDataSize
          OnKeyDown = ThisMatchTreeKeyDown
          OnMouseDown = ThisMatchTreeMouseDown
          Columns = <>
        end
        object OtherMatchPanel: TPanel
          Left = 404
          Top = 1
          Width = 170
          Height = 107
          Align = alRight
          TabOrder = 1
          object OtherMatchTree: TVirtualStringTree
            Left = 1
            Top = 1
            Width = 168
            Height = 105
            Hint = 'Matching QSOs in any other loaded contests'
            Align = alClient
            BevelKind = bkFlat
            Header.AutoSizeIndex = 0
            Header.DefaultHeight = 17
            Header.Font.Charset = DEFAULT_CHARSET
            Header.Font.Color = clWindowText
            Header.Font.Height = -11
            Header.Font.Name = 'Verdana'
            Header.Font.Style = []
            Header.MainColumn = -1
            Header.Options = [hoColumnResize, hoDrag]
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
            Columns = <>
          end
        end
      end
    end
  end
  object EntryChoiceMenu: TPopupMenu
    Left = 328
    Top = 222
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
    object SetTimeNow: TMenuItem
      Caption = 'Set Time to Now'
      OnClick = SetTimeNowClick
    end
    object N3: TMenuItem
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
end
