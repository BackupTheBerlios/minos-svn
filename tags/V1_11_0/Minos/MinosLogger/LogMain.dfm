object LogContainer: TLogContainer
  Left = 327
  Top = 345
  Caption = 'Minos Contest Logger'
  ClientHeight = 636
  ClientWidth = 742
  Color = clBtnFace
  UseDockManager = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyUp = FormKeyUp
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ContestPageControl: TPageControl
    Left = 0
    Top = 0
    Width = 742
    Height = 617
    Align = alClient
    DockSite = True
    MultiLine = True
    ParentShowHint = False
    PopupMenu = TabPopup
    ShowHint = True
    TabOrder = 0
    TabStop = False
    OnChange = ContestPageControlChange
    OnMouseDown = ContestPageControlMouseDown
    OnMouseMove = ContestPageControlMouseMove
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 617
    Width = 742
    Height = 19
    Panels = <
      item
        Width = 450
      end
      item
        Width = 100
      end
      item
        Alignment = taRightJustify
        Width = 50
      end>
    ParentFont = True
    UseSystemFont = False
    OnDblClick = StatusBar1DblClick
  end
  object ActionManager: TActionManager
    ActionBars = <
      item
        Items.HideUnused = False
        Items = <
          item
            Items.HideUnused = False
            Items = <
              item
                Action = FileOpen1
                ImageIndex = 7
              end
              item
                Action = ContestDetailsAction
                Caption = '&Details...'
              end
              item
                Caption = '&Reopen...'
              end
              item
                Action = MakeEntryAction
                Caption = '&Produce Entry/Export File...'
              end
              item
                Action = FileCloseAction1
                Caption = '&Close'
              end
              item
                Action = FileExit1
                ImageIndex = 43
              end>
            Caption = 'F&ile'
          end
          item
            Items.HideUnused = False
            Items = <
              item
                Action = EditCut1
                ImageIndex = 0
                ShortCut = 16472
              end
              item
                Action = EditCopy1
                ImageIndex = 1
                ShortCut = 16451
              end
              item
                Action = EditPaste1
                ImageIndex = 2
                ShortCut = 16470
              end
              item
                Action = EditSelectAll1
                ShortCut = 16449
              end
              item
                Action = EditUndo1
                ImageIndex = 3
                ShortCut = 16474
              end
              item
                Action = EditDelete1
                ImageIndex = 5
                ShortCut = 16430
              end>
            Caption = '&Edit'
          end
          item
            Items.HideUnused = False
            Items = <
              item
                Visible = False
                Action = SearchFind1
                ImageIndex = 34
                ShortCut = 16454
              end
              item
                Action = GoToSerialAction
                Caption = '&Go To Serial...'
              end
              item
                Visible = False
                Action = SearchFindNext1
                ImageIndex = 33
                ShortCut = 114
              end
              item
                Visible = False
                Action = SearchReplace1
                ImageIndex = 32
              end
              item
                Visible = False
                Action = SearchFindFirst1
              end>
            Caption = '&Search'
          end
          item
            Items.HideUnused = False
            Items = <
              item
                Action = LocCalcAction
                Caption = '&Locator Calculator'
                ShortCut = 49219
              end
              item
                Action = LoggerAction
                Caption = 'L&ogger'
                ShortCut = 49228
              end
              item
                Action = AnalyseMinosLogAction
                Caption = '&Analyse Minos Log'
              end
              item
                Action = CustomizeActionBars1
              end>
            Caption = '&Tools'
          end
          item
            Items.HideUnused = False
            Items = <
              item
                Action = HelpAboutAction
                Caption = '&About...'
              end>
            Caption = '&Help'
          end>
      end>
    Left = 64
    Top = 32
    StyleName = 'XP Style'
    object EditCut1: TEditCut
      Category = 'Edit'
      Caption = 'Cu&t'
      Enabled = False
      Hint = 'Cut|Cuts the selection and puts it on the Clipboard'
      ImageIndex = 0
      ShortCut = 16472
    end
    object EditCopy1: TEditCopy
      Category = 'Edit'
      Caption = '&Copy'
      Enabled = False
      Hint = 'Copy|Copies the selection and puts it on the Clipboard'
      ImageIndex = 1
      ShortCut = 16451
    end
    object EditPaste1: TEditPaste
      Category = 'Edit'
      Caption = '&Paste'
      Enabled = False
      Hint = 'Paste|Inserts Clipboard contents'
      ImageIndex = 2
      ShortCut = 16470
    end
    object EditSelectAll1: TEditSelectAll
      Category = 'Edit'
      Caption = 'Select &All'
      Enabled = False
      Hint = 'Select All|Selects the entire document'
      ShortCut = 16449
    end
    object EditUndo1: TEditUndo
      Category = 'Edit'
      Caption = '&Undo'
      Enabled = False
      Hint = 'Undo|Reverts the last action'
      ImageIndex = 3
      ShortCut = 16474
    end
    object EditDelete1: TEditDelete
      Category = 'Edit'
      Caption = '&Delete'
      Hint = 'Delete|Erases the selection'
      ImageIndex = 5
      ShortCut = 16430
    end
    object SearchFind1: TSearchFind
      Category = 'Search'
      Caption = '&Find...'
      Enabled = False
      Hint = 'Find|Finds the specified text'
      ImageIndex = 34
      ShortCut = 16454
      Visible = False
    end
    object SearchFindNext1: TSearchFindNext
      Category = 'Search'
      Caption = 'Find &Next'
      Enabled = False
      Hint = 'Find Next|Repeats the last find'
      ImageIndex = 33
      ShortCut = 114
      Visible = False
    end
    object SearchReplace1: TSearchReplace
      Category = 'Search'
      Caption = '&Replace'
      Enabled = False
      Hint = 'Replace|Replaces specific text with different text'
      ImageIndex = 32
      Visible = False
    end
    object SearchFindFirst1: TSearchFindFirst
      Category = 'Search'
      Caption = 'F&ind First'
      Enabled = False
      Hint = 'Find First|Finds the first occurance of specified text'
      Visible = False
    end
    object CustomizeActionBars1: TCustomizeActionBars
      Category = 'Tools'
      Caption = '&Customize'
      CustomizeDlg.StayOnTop = False
    end
    object HelpAboutAction: TAction
      Category = 'Help'
      Caption = 'About...'
      Enabled = False
      OnExecute = HelpAboutActionExecute
    end
    object FileOpen1: TFileOpen
      Category = 'File'
      Caption = '&Open Contest...'
      Dialog.Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
      Hint = 'Open|Opens an existing contest'
      ImageIndex = 7
      BeforeExecute = FileOpen1BeforeExecute
      OnAccept = FileOpen1Accept
      OnCancel = FileOpen1Cancel
    end
    object ListOpen1: TFileOpen
      Category = 'File'
      Caption = 'Open &Archive List...'
      Dialog.Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
      Hint = 'Open|Opens an existing contact list'
      ImageIndex = 7
      BeforeExecute = ListOpen1BeforeExecute
      OnAccept = ListOpen1Accept
      OnCancel = ListOpen1Cancel
    end
    object ContestDetailsAction: TAction
      Category = 'File'
      Caption = 'Details...'
      Enabled = False
      Hint = 'Details|Show contest details'
      OnExecute = ContestDetailsActionExecute
    end
    object FileCloseAction1: TAction
      Category = 'File'
      Caption = 'Close'
      Enabled = False
      Hint = 'Close|Close this contest/list'
      OnExecute = FileCloseAction1Execute
    end
    object CloseAllAction: TAction
      Category = 'File'
      Caption = 'Close All'
      Enabled = False
      Hint = 'Close all contests'
      OnExecute = CloseAllActionExecute
    end
    object CloseAllButAction: TAction
      Category = 'File'
      Caption = 'Close all but this contest'
      Enabled = False
      Hint = 'Close all other contests'
      OnExecute = CloseAllButActionExecute
    end
    object FileExit1: TFileExit
      Category = 'File'
      Caption = 'E&xit'
      Hint = 'Exit|Quits the application'
      ImageIndex = 43
    end
    object LocCalcAction: TAction
      Category = 'Tools'
      Caption = 'Locator Calculator'
      Enabled = False
      ShortCut = 49219
      OnExecute = LocCalcActionExecute
    end
    object LoggerAction: TAction
      Category = 'Tools'
      Caption = 'Logger'
      Enabled = False
      ShortCut = 49228
      OnExecute = LoggerActionExecute
    end
    object GoToSerialAction: TAction
      Category = 'Search'
      Caption = '&Go To Contact Serial...'
      Enabled = False
      OnExecute = GoToSerialActionExecute
    end
    object MakeEntryAction: TAction
      Category = 'File'
      Caption = 'Produce Entry/Export File...'
      Enabled = False
      OnExecute = MakeEntryActionExecute
    end
    object AnalyseMinosLogAction: TAction
      Category = 'Tools'
      Caption = 'Analyse Minos Log'
      OnExecute = AnalyseMinosLogActionExecute
    end
    object NextUnfilledAction: TAction
      Category = 'Search'
      Caption = 'Goto First Unfilled Contact'
      OnExecute = NextUnfilledActionExecute
    end
    object FileNewAction: TAction
      Category = 'File'
      Caption = 'New...'
      OnExecute = FileNewActionExecute
    end
    object SetTimeNowAction: TAction
      Caption = 'Set Time To Now'
      OnExecute = SetTimeNowActionExecute
    end
    object NextContactDetailsOnLeftAction: TAction
      Caption = 'Next Contact Details On Left'
      OnExecute = NextContactDetailsOnLeftActionExecute
    end
    object ManageListsAction: TAction
      Category = 'File'
      Caption = '&Manage Archive Lists...'
      OnExecute = ManageListsActionExecute
    end
    object ShiftTabLeftAction: TAction
      Category = 'File'
      Caption = 'Shift Tab Left'
      OnExecute = ShiftLeftAction
    end
    object ShiftTabRightAction: TAction
      Category = 'File'
      Caption = 'Shift Tab Right'
      OnExecute = ShiftRightAction
    end
  end
  object TimeDisplayTimer: TTimer
    OnTimer = TimeDisplayTimerTimer
    Left = 120
    Top = 32
  end
  object StartupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = StartupTimerTimer
    Left = 168
    Top = 32
  end
  object SyncTimer: TTimer
    Interval = 100
    OnTimer = SyncTimerTimer
    Left = 168
    Top = 80
  end
  object OpenDialog1: TOpenDialog
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 24
    Top = 136
  end
  object TabPopup: TPopupMenu
    Left = 64
    Top = 72
    object CloseContest1: TMenuItem
      Action = FileCloseAction1
    end
    object CloseAllContest: TMenuItem
      Action = CloseAllAction
    end
    object Closeallbutthiscontest1: TMenuItem
      Action = CloseAllButAction
    end
    object Details1: TMenuItem
      Action = ContestDetailsAction
    end
    object GoToSerial1: TMenuItem
      Action = GoToSerialAction
    end
    object GotoNextUnfilled2: TMenuItem
      Action = NextUnfilledAction
    end
    object AnalyseMinosLog2: TMenuItem
      Action = AnalyseMinosLogAction
    end
    object AnalyseMinosLog3: TMenuItem
      Action = NextContactDetailsOnLeftAction
    end
    object ProduceEntryExportFile1: TMenuItem
      Action = MakeEntryAction
    end
    object SetTimeToNow1: TMenuItem
      Action = SetTimeNowAction
    end
    object ShiftLeft1: TMenuItem
      Action = ShiftTabLeftAction
    end
    object ShiftRight1: TMenuItem
      Action = ShiftTabRightAction
    end
    object Cancel1: TMenuItem
      Caption = 'Cancel'
    end
  end
  object MainMenu1: TMainMenu
    Left = 16
    Top = 32
    object File1: TMenuItem
      Caption = 'File'
      OnClick = File1Click
      object Open1: TMenuItem
        Action = FileOpen1
      end
      object OpenList1: TMenuItem
        Action = ListOpen1
      end
      object ManageLists1: TMenuItem
        Action = ManageListsAction
      end
      object Deta1: TMenuItem
        Action = ContestDetailsAction
      end
      object New1: TMenuItem
        Action = FileNewAction
      end
      object ReopenMenu: TMenuItem
        Caption = 'Reopen'
      end
      object ProduceEntryExportFile2: TMenuItem
        Action = MakeEntryAction
      end
      object Close1: TMenuItem
        Action = FileCloseAction1
      end
      object Exit1: TMenuItem
        Action = FileExit1
      end
    end
    object Edit1: TMenuItem
      Caption = 'Edit'
      object Cut1: TMenuItem
        Action = EditCut1
      end
      object Copy1: TMenuItem
        Action = EditCopy1
      end
      object Paste1: TMenuItem
        Action = EditPaste1
      end
      object SelectAll1: TMenuItem
        Action = EditSelectAll1
      end
      object Undo1: TMenuItem
        Action = EditUndo1
      end
      object Delete1: TMenuItem
        Action = EditDelete1
      end
    end
    object Search1: TMenuItem
      Caption = 'Search'
      object Find1: TMenuItem
        Action = SearchFind1
      end
      object GoToContactSerial1: TMenuItem
        Action = GoToSerialAction
      end
      object GotonextUnfilled1: TMenuItem
        Action = NextUnfilledAction
      end
      object FindNext1: TMenuItem
        Action = SearchFindNext1
      end
      object Replace1: TMenuItem
        Action = SearchReplace1
      end
      object FindFirst1: TMenuItem
        Action = SearchFindFirst1
      end
    end
    object ools1: TMenuItem
      Caption = 'Tools'
      object LocatorCalculator1: TMenuItem
        Action = LocCalcAction
      end
      object Logger1: TMenuItem
        Action = LoggerAction
      end
      object AnalyseMinosLog1: TMenuItem
        Action = AnalyseMinosLogAction
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Action = HelpAboutAction
      end
    end
  end
  object ContestMRU: TadpMRU
    MaxItems = 10
    RegistryPath = '\software\G0GJV\Minos\Logger\ContestMRU'
    ParentMenuItem = ReopenMenu
    OnClick = ContestMRUClick
    Left = 19
    Top = 77
  end
  object GridHintTimer: TTimer
    OnTimer = GridHintTimerTimer
    Left = 120
    Top = 80
  end
  object ApplicationEvents1: TApplicationEvents
    OnDeactivate = ApplicationEvents1Deactivate
    Left = 88
    Top = 136
  end
end
