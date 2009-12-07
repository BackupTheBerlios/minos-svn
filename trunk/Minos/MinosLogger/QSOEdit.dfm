object QSOEditDlg: TQSOEditDlg
  Left = 0
  Top = 0
  Caption = 'QSOEditDlg'
  ClientHeight = 446
  ClientWidth = 580
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyUp = FormKeyUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object QSOHistoryTree: TVirtualStringTree
    Left = 0
    Top = 0
    Width = 580
    Height = 256
    Align = alClient
    Header.AutoSizeIndex = 0
    Header.DefaultHeight = 17
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.MainColumn = -1
    Header.Options = [hoColumnResize, hoDrag]
    TabOrder = 0
    TabStop = False
    OnBeforeItemErase = QSOHistoryTreeBeforeItemErase
    OnGetText = QSOHistoryTreeGetText
    OnGetNodeDataSize = QSOHistoryTreeGetNodeDataSize
    OnInitChildren = QSOHistoryTreeInitChildren
    OnInitNode = QSOHistoryTreeInitNode
    ExplicitWidth = 566
    ExplicitHeight = 248
    Columns = <>
  end
  inline GJVQSOEditFrame: TGJVQSOEditFrame
    Left = 0
    Top = 256
    Width = 580
    Height = 190
    Align = alBottom
    TabOrder = 1
    TabStop = True
    ExplicitTop = 256
    ExplicitWidth = 580
    ExplicitHeight = 190
    inherited G0GJVPanel: TPanel
      Width = 580
      Height = 190
      ExplicitWidth = 566
      ExplicitHeight = 267
      inherited GJVOKButton: TButton
        Left = 159
        Top = 149
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
        ExplicitLeft = 159
        ExplicitTop = 149
      end
      inherited GJVForceButton: TButton
        Left = 213
        Top = 149
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
        ExplicitLeft = 213
        ExplicitTop = 149
      end
      inherited GJVCancelButton: TButton
        Left = 269
        Top = 149
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
        ExplicitLeft = 269
        ExplicitTop = 149
      end
      inherited InsertBeforeButton: TButton
        Left = 389
        Top = 118
        Visible = False
        ExplicitLeft = 389
        ExplicitTop = 118
      end
      inherited InsertAfterButton: TButton
        Left = 389
        Top = 149
        Visible = False
        OnClick = nil
        ExplicitLeft = 389
        ExplicitTop = 149
      end
      inherited PriorButton: TButton
        Left = 492
        Top = 113
        ExplicitLeft = 492
        ExplicitTop = 113
      end
      inherited NextButton: TButton
        Left = 492
        Top = 149
        OnClick = GJVQSOEditFrameNextButtonClick
        ExplicitLeft = 492
        ExplicitTop = 149
      end
    end
  end
  object InitialiseTimer: TTimer
    Enabled = False
    Interval = 10
    OnTimer = InitialiseTimerTimer
    Left = 40
    Top = 72
  end
end
