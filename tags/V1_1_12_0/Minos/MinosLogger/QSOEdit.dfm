object QSOEditDlg: TQSOEditDlg
  Left = 0
  Top = 0
  Caption = 'QSOEditDlg'
  ClientHeight = 515
  ClientWidth = 566
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
    Width = 566
    Height = 248
    Align = alClient
    Header.AutoSizeIndex = 0
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
    ExplicitHeight = 296
    Columns = <>
  end
  inline GJVQSOEditFrame: TGJVQSOEditFrame
    Left = 0
    Top = 248
    Width = 566
    Height = 267
    Align = alBottom
    TabOrder = 1
    TabStop = True
    ExplicitTop = 248
    ExplicitWidth = 566
    inherited G0GJVPanel: TPanel
      Width = 566
      ExplicitWidth = 566
      ExplicitHeight = 219
      inherited GJVOKButton: TButton
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
      end
      inherited GJVForceButton: TButton
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
      end
      inherited GJVCancelButton: TButton
        Left = 249
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
        ExplicitLeft = 249
      end
      inherited InsertBeforeButton: TButton
        Visible = False
      end
      inherited InsertAfterButton: TButton
        Visible = False
        OnClick = nil
      end
      inherited PriorButton: TButton
        Left = 472
        ExplicitLeft = 472
      end
      inherited NextButton: TButton
        OnClick = GJVQSOEditFrameNextButtonClick
      end
    end
  end
  object ShowFocusTimer: TTimer
    OnTimer = ShowFocusTimerTimer
    Left = 40
    Top = 24
  end
end
