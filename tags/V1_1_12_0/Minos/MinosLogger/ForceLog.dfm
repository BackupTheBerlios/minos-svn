object ForceLogDlg: TForceLogDlg
  Left = 434
  Top = 230
  Caption = 'Force log the current contact'
  ClientHeight = 346
  ClientWidth = 602
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 13
    Width = 145
    Height = 13
    Caption = 'Current Errors in Contact'
  end
  object Label2: TLabel
    Left = 67
    Top = 277
    Width = 468
    Height = 13
    Caption = 
      'Press "Force" to accept the QSO with all its errors, or "cancel"' +
      ' to return to logging'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object ErrList: TListBox
    Left = 24
    Top = 45
    Width = 225
    Height = 121
    ItemHeight = 13
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 299
    Top = 13
    Width = 289
    Height = 244
    Caption = 'Contact is:'
    TabOrder = 1
    object CheckBox1: TCheckBox
      Left = 16
      Top = 22
      Width = 265
      Height = 17
      Caption = 'Unfilled - enter later'
      TabOrder = 0
    end
    object CheckBox2: TCheckBox
      Left = 16
      Top = 46
      Width = 265
      Height = 17
      Caption = 'Backpacker (i.e. Valid Duplicate)'
      TabOrder = 1
    end
    object CheckBox3: TCheckBox
      Left = 16
      Top = 70
      Width = 265
      Height = 17
      Caption = 'Insert manual score Now'
      TabOrder = 2
    end
    object CheckBox4: TCheckBox
      Left = 16
      Top = 94
      Width = 265
      Height = 17
      Caption = 'Non Scoring'
      TabOrder = 3
    end
    object CheckBox5: TCheckBox
      Left = 16
      Top = 118
      Width = 265
      Height = 17
      Caption = 'Don'#39't Print/Export Entry'
      TabOrder = 4
    end
    object CheckBox6: TCheckBox
      Left = 16
      Top = 142
      Width = 265
      Height = 17
      Caption = 'Force Country for Multiplier'
      TabOrder = 5
    end
    object CheckBox7: TCheckBox
      Left = 16
      Top = 166
      Width = 265
      Height = 17
      Caption = 'No District Available'
      TabOrder = 6
    end
    object CheckBox8: TCheckBox
      Left = 16
      Top = 190
      Width = 265
      Height = 17
      Caption = 'Cross band (half score)'
      TabOrder = 7
    end
  end
  object ScoreIl: TLabeledEdit
    Left = 128
    Top = 183
    Width = 121
    Height = 21
    EditLabel.Width = 123
    EditLabel.Height = 13
    EditLabel.Caption = 'Manual distance (km)'
    LabelPosition = lpLeft
    TabOrder = 2
  end
  object CtryMultIl: TLabeledEdit
    Left = 128
    Top = 223
    Width = 121
    Height = 21
    EditLabel.Width = 121
    EditLabel.Height = 13
    EditLabel.Caption = 'Country for Multiplier'
    LabelPosition = lpLeft
    TabOrder = 3
  end
  object OKButton: TButton
    Left = 48
    Top = 306
    Width = 75
    Height = 25
    Caption = '&Force'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = OKButtonClick
  end
  object LocCalcButton: TButton
    Left = 236
    Top = 306
    Width = 129
    Height = 25
    Caption = '&Locator Calculator'
    TabOrder = 5
    OnClick = LocCalcButtonClick
  end
  object CancelButton: TButton
    Left = 480
    Top = 306
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 6
    OnClick = CancelButtonClick
  end
end
