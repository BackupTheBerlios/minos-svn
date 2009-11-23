object MonitorMain: TMonitorMain
  Left = 547
  Top = 125
  Caption = 'MonitorMain'
  ClientHeight = 400
  ClientWidth = 600
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object JvNetscapeSplitter1: TJvNetscapeSplitter
    Left = 153
    Top = 0
    Height = 361
    Align = alLeft
    MinSize = 1
    Maximized = False
    Minimized = False
    ButtonCursor = crDefault
    ButtonColor = clAqua
    ExplicitLeft = 248
    ExplicitTop = 96
    ExplicitHeight = 100
  end
  object Panel1: TPanel
    Left = 0
    Top = 361
    Width = 600
    Height = 39
    Align = alBottom
    TabOrder = 0
    DesignSize = (
      600
      39)
    object ScoreLabel: TLabel
      Left = 169
      Top = 16
      Width = 431
      Height = 13
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = 'ScoreLabel'
    end
    object CloseButton: TButton
      Left = 8
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = CloseButtonClick
    end
    object FocusCurrentButton: TButton
      Left = 88
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Current'
      TabOrder = 1
      OnClick = FocusCurrentButtonClick
    end
  end
  object MonitorTree: TVirtualStringTree
    Left = 0
    Top = 0
    Width = 153
    Height = 361
    Align = alLeft
    AutoExpandDelay = 100
    Header.AutoSizeIndex = -1
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'MS Sans Serif'
    Header.Font.Style = []
    Header.Options = [hoColumnResize, hoDrag]
    Header.ParentFont = True
    TabOrder = 1
    TabStop = False
    TreeOptions.AutoOptions = [toAutoExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
    OnDblClick = MonitorTreeDblClick
    OnGetText = MonitorTreeGetText
    OnGetNodeDataSize = MonitorTreeGetNodeDataSize
    OnInitChildren = MonitorTreeInitChildren
    OnInitNode = MonitorTreeInitNode
    OnMouseDown = MonitorTreeMouseDown
    Columns = <
      item
        Margin = 0
        Position = 0
        Spacing = 0
        Width = 180
      end>
  end
  object ContestPageControl: TPageControl
    Left = 163
    Top = 0
    Width = 437
    Height = 361
    Align = alClient
    PopupMenu = PopupMenu1
    TabOrder = 2
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 16
    Top = 16
  end
  object StartupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = StartupTimerTimer
    Left = 16
    Top = 88
  end
  object ConnectTimer: TTimer
    Enabled = False
    OnTimer = ConnectTimerTimer
    Left = 16
    Top = 51
  end
  object MonitorTimer: TTimer
    Interval = 100
    OnTimer = MonitorTimerTimer
    Left = 16
    Top = 176
  end
  object PopupMenu1: TPopupMenu
    Left = 216
    Top = 56
    object Close1: TMenuItem
      Caption = 'Close'
      OnClick = Close1Click
    end
    object Cancel1: TMenuItem
      Caption = 'Cancel'
      OnClick = Cancel1Click
    end
  end
end
