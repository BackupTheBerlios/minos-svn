object MonitoringFrame: TMonitoringFrame
  Left = 0
  Top = 0
  Width = 589
  Height = 307
  TabOrder = 0
  TabStop = True
  object MultSplitter: TJvNetscapeSplitter
    Left = 279
    Top = 0
    Height = 288
    Align = alRight
    MinSize = 1
    Maximized = False
    Minimized = False
    ButtonCursor = crDefault
    ExplicitLeft = 384
    ExplicitTop = 104
    ExplicitHeight = 100
  end
  object MonitorStatusBar: TStatusBar
    Left = 0
    Top = 288
    Width = 589
    Height = 19
    Panels = <>
  end
  inline LogMonitorFrame: TLogMonitorFrame
    Left = 0
    Top = 0
    Width = 279
    Height = 288
    Align = alClient
    TabOrder = 1
    TabStop = True
    ExplicitWidth = 279
    ExplicitHeight = 288
    inherited QSOTree: TVirtualStringTree
      Width = 279
      Height = 288
      OnBeforeItemErase = LogMonitorFrameQSOTreeBeforeItemErase
      OnColumnResize = LogMonitorFrameQSOTreeColumnResize
      OnDblClick = LogMonitorFrameQSOTreeDblClick
      OnGetText = LogMonitorFrameQSOTreeGetText
      OnPaintText = LogMonitorFrameQSOTreePaintText
      OnGetNodeDataSize = LogMonitorFrameQSOTreeGetNodeDataSize
      ExplicitWidth = 279
      ExplicitHeight = 288
    end
  end
  inline MultDispFrame: TMultDispFrame
    Left = 289
    Top = 0
    Width = 300
    Height = 288
    Align = alRight
    TabOrder = 2
    TabStop = True
    ExplicitLeft = 289
    ExplicitHeight = 288
    inherited FilterSplitter: TJvNetscapeSplitter
      Top = 178
      ExplicitTop = 178
    end
    inherited MultPages: TPageControl
      Height = 178
      ActivePage = MultDispFrame.StatsTab
      ExplicitHeight = 178
      inherited CountriesTab: TTabSheet
        ExplicitLeft = 4
        ExplicitTop = 4
        ExplicitWidth = 252
        ExplicitHeight = 182
      end
      inherited DistrictTab: TTabSheet
        ExplicitLeft = 4
        ExplicitTop = 4
        ExplicitWidth = 252
        ExplicitHeight = 182
      end
      inherited LocatorsTab: TTabSheet
        ExplicitLeft = 4
        ExplicitTop = 4
        ExplicitWidth = 252
        ExplicitHeight = 182
      end
      inherited StatsTab: TTabSheet
        ExplicitHeight = 170
        inherited StatsDispFrame: TStatsDispFrame
          Height = 170
          ExplicitHeight = 170
          inherited SLabel1: TLabel
            Height = 124
          end
          inherited Panel1: TPanel
            Top = 124
            ExplicitTop = 124
          end
        end
      end
    end
    inherited FilterPanel: TPanel
      Top = 188
      ExplicitTop = 188
    end
  end
  object InitTimer: TTimer
    Interval = 100
    OnTimer = InitTimerTimer
    Left = 240
    Top = 32
  end
end
