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
  Font.Name = 'Verdana'
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
    Header.Font.Name = 'Verdana'
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
    Columns = <>
  end
  inline GJVQSOEditFrame: TGJVQSOEditFrame
    Left = 0
    Top = 256
    Width = 580
    Height = 190
    Align = alBottom
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    TabStop = True
    ExplicitTop = 256
    ExplicitWidth = 580
    ExplicitHeight = 190
    inherited G0GJVPanel: TPanel
      Width = 580
      Height = 190
      ExplicitWidth = 580
      ExplicitHeight = 190
      inherited RSTTXEdit: TLabeledEdit [5]
      end
      inherited CallsignEdit: TLabeledEdit [6]
      end
      inherited TimeEdit: TLabeledEdit [7]
        TabOrder = 22
      end
      inherited GJVCancelButton: TButton [8]
        TabOrder = 20
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
      end
      inherited GJVForceButton: TButton [9]
        TabOrder = 23
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
      end
      inherited GJVOKButton: TButton [10]
        TabOrder = 21
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
      end
      inherited DeletedCheckBox: TCheckBox [11]
      end
      inherited NonScoreCheckBox: TCheckBox [12]
      end
      inherited DateEdit: TLabeledEdit [13]
      end
      inherited InsertAfterButton: TButton [14]
        TabOrder = 16
      end
      inherited NextButton: TButton [15]
        TabOrder = 19
      end
      inherited PriorButton: TButton [16]
      end
      inherited SerTXEdit: TLabeledEdit [17]
      end
      inherited ModeComboBoxGJV: TComboBox [18]
      end
      inherited CommentsEdit: TLabeledEdit [19]
      end
      inherited QTHEdit: TLabeledEdit [20]
      end
      inherited RSTRXEdit: TLabeledEdit [21]
      end
      inherited SerRXEdit: TLabeledEdit [22]
      end
      inherited MainOpComboBox: TComboBox
        TabOrder = 14
      end
      inherited SecondOpComboBox: TComboBox [24]
        TabOrder = 18
      end
      inherited CatchupButton: TButton [25]
        TabOrder = 13
      end
      inherited FirstUnfilledButton: TButton [26]
        TabOrder = 12
      end
      inherited ModeButton: TButton [27]
      end
      inherited LocEdit: TLabeledEdit [28]
      end
      inherited InsertBeforeButton: TButton [29]
        TabOrder = 15
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
