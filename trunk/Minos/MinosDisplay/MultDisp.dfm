object MultDispFrame: TMultDispFrame
  Left = 0
  Top = 0
  Width = 296
  Height = 300
  TabOrder = 0
  TabStop = True
  object FilterSplitter: TSplitter
    Left = 0
    Top = 190
    Width = 296
    Height = 10
    Cursor = crVSplit
    Hint = 'Click here to show/hide the multiplier filtering controls'
    Align = alBottom
    Color = clBtnFace
    MinSize = 1
    ParentColor = False
    OnMoved = FilterSplitterMoved
    ExplicitTop = 0
    ExplicitWidth = 259
  end
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
  object MultPages: TPageControl
    Left = 0
    Top = 0
    Width = 296
    Height = 190
    ActivePage = DistrictTab
    Align = alClient
    MultiLine = True
    TabOrder = 0
    TabPosition = tpRight
    object CountriesTab: TTabSheet
      Caption = 'Countries'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object CountryMultTree: TVirtualStringTree
        Left = 0
        Top = 0
        Width = 248
        Height = 182
        Align = alClient
        Header.AutoSizeIndex = 0
        Header.DefaultHeight = 17
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'Verdana'
        Header.Font.Style = []
        Header.MainColumn = -1
        Header.Options = [hoColumnResize, hoDblClickResize, hoDrag]
        TabOrder = 0
        TabStop = False
        TreeOptions.AutoOptions = [toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
        TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toThemeAware, toUseBlendedImages]
        TreeOptions.SelectionOptions = [toCenterScrollIntoView]
        OnGetText = CountryMultTreeGetText
        OnGetNodeDataSize = CountryMultTreeGetNodeDataSize
        OnInitChildren = CountryMultTreeInitChildren
        OnInitNode = CountryMultTreeInitNode
        Columns = <>
      end
    end
    object DistrictTab: TTabSheet
      Caption = 'Districts'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object DistrictMultTree: TVirtualStringTree
        Left = 0
        Top = 0
        Width = 248
        Height = 182
        Align = alClient
        Header.AutoSizeIndex = 0
        Header.DefaultHeight = 17
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'Verdana'
        Header.Font.Style = []
        Header.MainColumn = -1
        Header.Options = [hoColumnResize, hoDblClickResize, hoDrag]
        TabOrder = 0
        TreeOptions.AutoOptions = [toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
        TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
        TreeOptions.PaintOptions = [toHideFocusRect, toShowButtons, toShowDropmark, toThemeAware, toUseBlendedImages]
        TreeOptions.SelectionOptions = [toCenterScrollIntoView]
        OnGetText = DistrictMultTreeGetText
        OnGetNodeDataSize = DistrictMultTreeGetNodeDataSize
        OnInitChildren = DistrictMultTreeInitChildren
        OnInitNode = DistrictMultTreeInitNode
        Columns = <>
      end
    end
    object LocatorsTab: TTabSheet
      Caption = 'Locators'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LocatorMultTree: TVirtualStringTree
        Left = 0
        Top = 0
        Width = 248
        Height = 182
        Align = alClient
        Header.AutoSizeIndex = 0
        Header.DefaultHeight = 17
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'Verdana'
        Header.Font.Style = []
        Header.MainColumn = -1
        Header.Options = [hoColumnResize, hoDrag]
        TabOrder = 0
        TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes, toDisableAutoscrollOnFocus]
        TreeOptions.SelectionOptions = [toCenterScrollIntoView]
        OnGetText = LocatorMultTreeGetText
        OnGetNodeDataSize = LocatorMultTreeGetNodeDataSize
        OnInitChildren = LocatorMultTreeInitChildren
        OnInitNode = LocatorMultTreeInitNode
        Columns = <>
      end
    end
    object StatsTab: TTabSheet
      Caption = 'Stats'
      ImageIndex = 3
      inline StatsDispFrame: TStatsDispFrame
        Left = 0
        Top = 0
        Width = 248
        Height = 182
        Align = alClient
        TabOrder = 0
        TabStop = True
        ExplicitWidth = 248
        ExplicitHeight = 182
        inherited SLabel1: TLabel
          Width = 248
          Height = 136
        end
        inherited Panel1: TPanel
          Top = 136
          Width = 248
          ExplicitTop = 136
          ExplicitWidth = 248
          DesignSize = (
            248
            46)
        end
      end
    end
    object ErrTab: TTabSheet
      Caption = 'Errors'
      ImageIndex = 4
      object ErrList: TListBox
        Left = 0
        Top = 0
        Width = 248
        Height = 182
        Align = alClient
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object CompSheet: TTabSheet
      Caption = 'Comparison'
      ImageIndex = 5
      inline CompFrame: TCompFrame
        Left = -72
        Top = -58
        Width = 320
        Height = 240
        TabOrder = 0
        ExplicitLeft = -72
        ExplicitTop = -58
      end
    end
  end
  object FilterPanel: TPanel
    Left = 0
    Top = 200
    Width = 296
    Height = 100
    Align = alBottom
    TabOrder = 1
    object UnworkedCB: TCheckBox
      Left = 128
      Top = 5
      Width = 99
      Height = 17
      Caption = 'Unworked only'
      TabOrder = 0
      OnClick = FilterClick
    end
    object ContEU: TCheckBox
      Left = 16
      Top = 28
      Width = 97
      Height = 17
      Caption = 'Europe'
      TabOrder = 1
      OnClick = FilterClick
    end
    object ContAS: TCheckBox
      Left = 16
      Top = 46
      Width = 97
      Height = 17
      Caption = 'Asia'
      TabOrder = 2
      OnClick = FilterClick
    end
    object ContAF: TCheckBox
      Left = 16
      Top = 67
      Width = 97
      Height = 17
      Caption = 'Africa'
      TabOrder = 3
      OnClick = FilterClick
    end
    object ContOC: TCheckBox
      Left = 128
      Top = 28
      Width = 92
      Height = 17
      Caption = 'Oceania'
      TabOrder = 4
      OnClick = FilterClick
    end
    object ContSA: TCheckBox
      Left = 128
      Top = 46
      Width = 92
      Height = 17
      Caption = 'S America'
      TabOrder = 5
      OnClick = FilterClick
    end
    object ContNA: TCheckBox
      Left = 128
      Top = 67
      Width = 92
      Height = 17
      Caption = 'N America'
      TabOrder = 6
      OnClick = FilterClick
    end
    object WorkedCB: TCheckBox
      Left = 16
      Top = 5
      Width = 97
      Height = 17
      Caption = 'Worked only'
      TabOrder = 7
      OnClick = FilterClick
    end
  end
  object FilterTimer: TTimer
    OnTimer = FilterTimerTimer
    Left = 8
    Top = 8
  end
end
