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
      inherited GJVCancelButton: TButton [5]
        TabOrder = 20
        OnClick = GJVQSOEditFrame1GJVCancelButtonClick
      end
      inherited TimeEdit: TLabeledEdit [6]
        EditLabel.Width = 28
        EditLabel.ExplicitLeft = 108
        EditLabel.ExplicitTop = 45
        EditLabel.ExplicitWidth = 28
        TabOrder = 22
      end
      inherited PriorButton: TButton [7]
      end
      inherited NextButton: TButton [8]
        TabOrder = 19
      end
      inherited MainOpComboBox: TComboBox [9]
        TabOrder = 14
      end
      inherited CallsignEdit: TLabeledEdit [10]
        EditLabel.Width = 72
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 72
      end
      inherited RSTTXEdit: TLabeledEdit [11]
        EditLabel.Width = 56
        EditLabel.ExplicitLeft = 108
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 56
      end
      inherited SerTXEdit: TLabeledEdit [12]
        EditLabel.Width = 52
        EditLabel.ExplicitLeft = 170
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 52
      end
      inherited RSTRXEdit: TLabeledEdit [13]
        EditLabel.Width = 56
        EditLabel.ExplicitLeft = 254
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 56
      end
      inherited SerRXEdit: TLabeledEdit [14]
        EditLabel.Width = 53
        EditLabel.ExplicitLeft = 337
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 53
      end
      inherited LocEdit: TLabeledEdit [15]
        EditLabel.Width = 46
        EditLabel.ExplicitLeft = 420
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 46
      end
      inherited QTHEdit: TLabeledEdit [16]
        EditLabel.Width = 55
        EditLabel.ExplicitLeft = 504
        EditLabel.ExplicitTop = 67
        EditLabel.ExplicitWidth = 55
      end
      inherited CommentsEdit: TLabeledEdit [17]
        EditLabel.Width = 62
        EditLabel.ExplicitLeft = 4
        EditLabel.ExplicitTop = 104
        EditLabel.ExplicitWidth = 62
      end
      inherited ModeComboBoxGJV: TComboBox [18]
      end
      inherited GJVForceButton: TButton [19]
        TabOrder = 23
        OnClick = GJVQSOEditFrame1GJVForceButtonClick
      end
      inherited GJVOKButton: TButton [20]
        TabOrder = 21
        OnClick = GJVQSOEditFrame1GJVOKButtonClick
      end
      inherited DeletedCheckBox: TCheckBox [21]
      end
      inherited NonScoreCheckBox: TCheckBox [22]
      end
      inherited InsertAfterButton: TButton [23]
        TabOrder = 16
      end
      inherited InsertBeforeButton: TButton
        TabOrder = 15
      end
      inherited SecondOpComboBox: TComboBox [25]
        TabOrder = 18
      end
      inherited ModeButton: TButton [26]
      end
      inherited FirstUnfilledButton: TButton [27]
        TabOrder = 12
      end
      inherited CatchupButton: TButton [28]
        TabOrder = 13
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
