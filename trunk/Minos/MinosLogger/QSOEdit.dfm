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
  Font.Name = 'Verdana'
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
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
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
      inherited ModeLabel: TLabel
        Width = 30
        ExplicitWidth = 30
      end
      inherited SecondOpLabel: TLabel
        Width = 28
        ExplicitWidth = 28
      end
      inherited OperatorLabel: TLabel
        Width = 28
        ExplicitWidth = 28
      end
      inherited SecondOpComboBox: TComboBox [5]
        TabOrder = 18
      end
      inherited ModeButton: TButton [6]
      end
      inherited FirstUnfilledButton: TButton [7]
        TabOrder = 12
      end
      inherited CatchupButton: TButton [8]
        TabOrder = 13
      end
      inherited RSTRXEdit: TLabeledEdit [9]
        EditLabel.Width = 56
        EditLabel.ExplicitLeft = 254
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 56
      end
      inherited SerTXEdit: TLabeledEdit [10]
        EditLabel.Width = 52
        EditLabel.ExplicitLeft = 170
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 52
      end
      inherited RSTTXEdit: TLabeledEdit [11]
        EditLabel.Width = 56
        EditLabel.ExplicitLeft = 108
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 56
      end
      inherited CallsignEdit: TLabeledEdit [12]
        EditLabel.Width = 72
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 72
      end
      inherited SerRXEdit: TLabeledEdit [13]
        EditLabel.Width = 53
        EditLabel.ExplicitLeft = 337
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 53
      end
      inherited NextButton: TButton [14]
        TabOrder = 19
      end
      inherited PriorButton: TButton [15]
      end
      inherited TimeEdit: TLabeledEdit [16]
        EditLabel.Width = 28
        EditLabel.ExplicitLeft = 108
        EditLabel.ExplicitTop = 45
        EditLabel.ExplicitWidth = 28
        TabOrder = 22
      end
      inherited GJVCancelButton: TButton [17]
        TabOrder = 20
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
      end
      inherited InsertBeforeButton: TButton [18]
        TabOrder = 15
      end
      inherited InsertAfterButton: TButton [19]
        TabOrder = 16
      end
      inherited NonScoreCheckBox: TCheckBox [20]
      end
      inherited DeletedCheckBox: TCheckBox [21]
      end
      inherited GJVOKButton: TButton [22]
        TabOrder = 21
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
      end
      inherited GJVForceButton: TButton [23]
        TabOrder = 23
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
      end
      inherited ModeComboBoxGJV: TComboBox [24]
      end
      inherited CommentsEdit: TLabeledEdit [25]
        EditLabel.Width = 62
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 104
        EditLabel.ExplicitWidth = 62
      end
      inherited QTHEdit: TLabeledEdit [26]
        EditLabel.Width = 55
        EditLabel.ExplicitLeft = 504
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 55
      end
      inherited LocEdit: TLabeledEdit [27]
        EditLabel.Width = 46
        EditLabel.ExplicitLeft = 420
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 46
      end
      inherited MainOpComboBox: TComboBox [28]
        TabOrder = 14
      end
      inherited DateEdit: TLabeledEdit
        EditLabel.Width = 27
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 45
        EditLabel.ExplicitWidth = 27
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
