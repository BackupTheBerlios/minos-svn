object QSOEditDlg: TQSOEditDlg
  Left = 0
  Top = 0
  Caption = 'QSOEditDlg'
  ClientHeight = 446
  ClientWidth = 631
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnKeyUp = FormKeyUp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object QSOHistoryTree: TVirtualStringTree
    Left = 0
    Top = 0
    Width = 631
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
    Width = 631
    Height = 190
    Align = alBottom
    TabOrder = 1
    TabStop = True
    ExplicitTop = 256
    ExplicitWidth = 631
    ExplicitHeight = 190
    inherited G0GJVPanel: TPanel
      Width = 631
      Height = 190
      ExplicitWidth = 631
      ExplicitHeight = 190
      inherited PriorButton: TButton [5]
      end
      inherited TimeEdit: TLabeledEdit [6]
        EditLabel.ExplicitLeft = 114
        EditLabel.ExplicitTop = 45
        EditLabel.ExplicitWidth = 22
        TabOrder = 22
      end
      inherited GJVCancelButton: TButton [7]
        TabOrder = 20
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
      end
      inherited InsertBeforeButton: TButton [8]
        TabOrder = 15
      end
      inherited InsertAfterButton: TButton [9]
        TabOrder = 16
      end
      inherited NonScoreCheckBox: TCheckBox [10]
      end
      inherited DeletedCheckBox: TCheckBox [11]
      end
      inherited GJVOKButton: TButton [12]
        TabOrder = 21
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
      end
      inherited GJVForceButton: TButton [13]
        TabOrder = 23
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
      end
      inherited ModeComboBoxGJV: TComboBox [14]
      end
      inherited CommentsEdit: TLabeledEdit [15]
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 110
        EditLabel.ExplicitWidth = 50
      end
      inherited QTHEdit: TLabeledEdit [16]
        EditLabel.ExplicitLeft = 504
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 47
      end
      inherited LocEdit: TLabeledEdit [17]
        EditLabel.ExplicitLeft = 420
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 39
      end
      inherited MainOpComboBox: TComboBox [18]
        TabOrder = 14
      end
      inherited NextButton: TButton [19]
        TabOrder = 19
      end
      inherited SerRXEdit: TLabeledEdit [20]
        EditLabel.ExplicitLeft = 337
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 42
      end
      inherited CallsignEdit: TLabeledEdit [21]
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 59
      end
      inherited RSTTXEdit: TLabeledEdit [22]
        EditLabel.ExplicitLeft = 108
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 47
      end
      inherited SerTXEdit: TLabeledEdit [23]
        EditLabel.ExplicitLeft = 184
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 41
      end
      inherited RSTRXEdit: TLabeledEdit [24]
        EditLabel.ExplicitLeft = 254
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 47
      end
      inherited CatchupButton: TButton [25]
        TabOrder = 13
      end
      inherited FirstUnfilledButton: TButton [26]
        TabOrder = 12
      end
      inherited ModeButton: TButton [27]
      end
      inherited SecondOpComboBox: TComboBox [28]
        TabOrder = 18
      end
      inherited DateEdit: TLabeledEdit
        EditLabel.ExplicitLeft = 8
        EditLabel.ExplicitTop = 45
        EditLabel.ExplicitWidth = 23
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
