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
    Header.DefaultHeight = 17
    Header.Options = [hoColumnResize, hoDblClickResize, hoDrag, hoVisible]
    Header.ParentFont = True
    TabOrder = 0
    TabStop = False
    TreeOptions.AutoOptions = [toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
    TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
    TreeOptions.PaintOptions = [toHideFocusRect, toShowButtons, toShowDropmark, toShowHorzGridLines, toThemeAware, toUseBlendedImages]
    TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect]
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
