object LogMonitorFrame: TLogMonitorFrame
  Left = 0
  Top = 0
  Width = 320
  Height = 240
  TabOrder = 0
  TabStop = True
  object QSOTree: TVirtualStringTree
    Left = 0
    Top = 0
    Width = 320
    Height = 240
    Align = alClient
    Header.AutoSizeIndex = -1
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.Options = [hoColumnResize, hoDrag, hoVisible]
    Header.ParentFont = True
    TabOrder = 0
    TabStop = False
    TreeOptions.AutoOptions = [toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
    TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
    TreeOptions.PaintOptions = [toHideFocusRect, toShowButtons, toShowDropmark, toShowHorzGridLines, toThemeAware, toUseBlendedImages]
    TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect]
    OnBeforeCellPaint = QSOTreeBeforeCellPaint
    OnBeforeItemErase = QSOTreeBeforeItemErase
    OnColumnResize = QSOTreeColumnResize
    OnDblClick = QSOTreeDblClick
    OnGetText = QSOTreeGetText
    OnPaintText = QSOTreePaintText
    OnGetNodeDataSize = QSOTreeGetNodeDataSize
    Columns = <
      item
        Margin = 0
        Position = 0
        Spacing = 0
        Width = 505
      end>
  end
end
