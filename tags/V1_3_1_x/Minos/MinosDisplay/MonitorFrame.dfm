object MonitoringFrame: TMonitoringFrame
  Left = 0
  Top = 0
  Width = 589
  Height = 307
  TabOrder = 0
  TabStop = True
  object MultSplitter: TSplitter
    Left = 280
    Top = 0
    Width = 9
    Height = 288
    Align = alRight
    MinSize = 1
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
    Width = 280
    Height = 288
    Align = alClient
    TabOrder = 1
    TabStop = True
    ExplicitWidth = 286
    ExplicitHeight = 288
    inherited QSOTree: TVirtualStringTree
      Width = 280
      Height = 288
      OnBeforeItemErase = LogMonitorFrameQSOTreeBeforeItemErase
      OnColumnResize = LogMonitorFrameQSOTreeColumnResize
      OnDblClick = LogMonitorFrameQSOTreeDblClick
      OnGetText = LogMonitorFrameQSOTreeGetText
      OnPaintText = LogMonitorFrameQSOTreePaintText
      OnGetNodeDataSize = LogMonitorFrameQSOTreeGetNodeDataSize
      ExplicitWidth = 286
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
    ExplicitWidth = 300
    ExplicitHeight = 288
    inherited FilterSplitter: TSplitter
      Top = 178
      Width = 300
      ExplicitTop = 178
      ExplicitWidth = 300
    end
    inherited MultPages: TPageControl
      Width = 300
      Height = 178
      ActivePage = MultDispFrame.StatsTab
      ExplicitWidth = 300
      ExplicitHeight = 178
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
      end
      inherited LocatorsTab: TTabSheet
        ExplicitLeft = 4
        ExplicitTop = 4
        ExplicitWidth = 248
        ExplicitHeight = 182
      end
      inherited StatsTab: TTabSheet
        ExplicitWidth = 252
        ExplicitHeight = 170
        inherited StatsDispFrame: TStatsDispFrame
          Width = 252
          Height = 170
          ExplicitWidth = 252
          ExplicitHeight = 170
          inherited SLabel1: TLabel
            Width = 252
            Height = 124
          end
          inherited Panel1: TPanel
            Top = 124
            Width = 252
            ExplicitTop = 124
            ExplicitWidth = 252
          end
        end
      end
    end
    inherited FilterPanel: TPanel
      Top = 188
      Width = 300
      ExplicitTop = 188
      ExplicitWidth = 300
    end
  end
  object InitTimer: TTimer
    Interval = 100
    OnTimer = InitTimerTimer
    Left = 240
    Top = 32
  end
end
