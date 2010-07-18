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
      inherited TimeNowButton: TButton [5]
      end
      inherited ModeButton: TButton [6]
      end
      inherited FirstUnfilledButton: TButton [7]
        TabOrder = 15
      end
      inherited CatchupButton: TButton [8]
      end
      inherited SecondOpComboBox: TComboBox [9]
        TabOrder = 16
      end
      inherited MainOpComboBox: TComboBox [10]
        TabOrder = 17
      end
      inherited SerRXEdit: TLabeledEdit [12]
      end
      inherited RSTRXEdit: TLabeledEdit [13]
      end
      inherited SerTXEdit: TLabeledEdit [14]
      end
      inherited RSTTXEdit: TLabeledEdit [15]
      end
      inherited CallsignEdit: TLabeledEdit [16]
      end
      inherited TimeEdit: TLabeledEdit [17]
      end
      inherited GJVCancelButton: TButton [18]
        TabOrder = 24
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
      end
      inherited GJVForceButton: TButton [19]
        TabOrder = 23
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
      end
      inherited GJVOKButton: TButton [20]
        TabOrder = 22
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
      end
      inherited DeletedCheckBox: TCheckBox [21]
      end
      inherited NonScoreCheckBox: TCheckBox [22]
      end
      inherited DateEdit: TLabeledEdit [23]
      end
      inherited InsertAfterButton: TButton [24]
        TabOrder = 19
      end
      inherited NextButton: TButton [25]
        TabOrder = 21
      end
      inherited PriorButton: TButton [26]
        TabOrder = 20
      end
      inherited InsertBeforeButton: TButton
        TabOrder = 18
      end
      inherited ModeComboBoxGJV: TComboBox [28]
      end
      inherited CommentsEdit: TLabeledEdit [29]
      end
      inherited QTHEdit: TLabeledEdit [30]
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
