object MonitoringFrame: TMonitoringFrame
  Left = 0
  Top = 0
  Width = 589
  Height = 307
  TabOrder = 0
  TabStop = True
  object MonitorStatusBar: TStatusBar
    Left = 0
    Top = 288
    Width = 589
    Height = 19
    Panels = <>
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 589
    Height = 288
    Align = alClient
    Caption = 'Panel1'
    TabOrder = 1
    object MultSplitter: TSplitter
      Left = 327
      Top = 1
      Width = 9
      Height = 286
      Align = alRight
      MinSize = 1
      OnMoved = MultSplitterMoved
      ExplicitLeft = 319
      ExplicitTop = 25
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
    inline LogMonitorFrame: TLogMonitorFrame
      Left = 1
      Top = 1
      Width = 326
      Height = 286
      Align = alClient
      TabOrder = 1
      TabStop = True
      ExplicitLeft = 1
      ExplicitTop = 1
      ExplicitWidth = 326
      ExplicitHeight = 286
      inherited QSOTree: TVirtualStringTree
        Width = 326
        Height = 286
        OnBeforeItemErase = LogMonitorFrameQSOTreeBeforeItemErase
        OnDblClick = LogMonitorFrameQSOTreeDblClick
        OnGetText = LogMonitorFrameQSOTreeGetText
        OnPaintText = LogMonitorFrameQSOTreePaintText
        OnGetNodeDataSize = LogMonitorFrameQSOTreeGetNodeDataSize
        ExplicitLeft = 152
        ExplicitTop = 99
        ExplicitWidth = 326
        ExplicitHeight = 286
      end
    end
    object MultPanel: TPanel
      Left = 336
      Top = 1
      Width = 252
      Height = 286
      Align = alRight
      TabOrder = 0
      inline MultDispFrame: TMultDispFrame
        Left = 1
        Top = 1
        Width = 250
        Height = 284
        Align = alClient
        TabOrder = 0
        TabStop = True
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitWidth = 250
        ExplicitHeight = 284
        inherited FilterSplitter: TSplitter
          Top = 174
          Width = 250
          ExplicitTop = 178
          ExplicitWidth = 300
        end
        inherited MultPages: TPageControl
          Width = 250
          Height = 174
          ActivePage = MultDispFrame.StatsTab
          ExplicitWidth = 250
          ExplicitHeight = 174
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
            ExplicitWidth = 202
            ExplicitHeight = 166
            inherited StatsDispFrame: TStatsDispFrame
              Width = 202
              Height = 166
              ExplicitWidth = 202
              ExplicitHeight = 166
              inherited SLabel1: TLabel
                Width = 202
                Height = 120
              end
              inherited Panel1: TPanel
                Top = 120
                Width = 202
                ExplicitTop = 120
                ExplicitWidth = 202
              end
            end
          end
        end
        inherited FilterPanel: TPanel
          Top = 184
          Width = 250
          ExplicitTop = 184
          ExplicitWidth = 250
        end
      end
    end
  end
  object InitTimer: TTimer
    Interval = 100
    OnTimer = InitTimerTimer
    Left = 240
    Top = 32
  end
end
