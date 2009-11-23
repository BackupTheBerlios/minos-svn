object RigCtlMain: TRigCtlMain
  Left = 467
  Top = 235
  Caption = 'Minos Rig Control Program'
  ClientHeight = 292
  ClientWidth = 391
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 32
    Width = 37
    Height = 13
    Caption = 'Label1'
  end
  object Label2: TLabel
    Left = 32
    Top = 59
    Width = 37
    Height = 13
    Caption = 'Label2'
  end
  object Label3: TLabel
    Left = 16
    Top = 88
    Width = 47
    Height = 13
    Caption = 'Rig type'
  end
  object Label4: TLabel
    Left = 16
    Top = 140
    Width = 23
    Height = 13
    Caption = 'Port'
  end
  object CloseButton: TButton
    Left = 280
    Top = 253
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object RigBase: TLabeledEdit
    Left = 16
    Top = 210
    Width = 121
    Height = 21
    EditLabel.Width = 81
    EditLabel.Height = 13
    EditLabel.Caption = 'Rig Frequency'
    TabOrder = 1
    Text = '28000000'
  end
  object TransvertBase: TLabeledEdit
    Left = 16
    Top = 253
    Width = 121
    Height = 21
    EditLabel.Width = 132
    EditLabel.Height = 13
    EditLabel.Caption = 'Transverted Frequency'
    TabOrder = 2
    Text = '432000000'
  end
  object ChangeButton: TButton
    Left = 163
    Top = 253
    Width = 75
    Height = 25
    Caption = 'Change'
    TabOrder = 3
    OnClick = ChangeButtonClick
  end
  object RigCombo: TComboBox
    Left = 16
    Top = 106
    Width = 345
    Height = 21
    ItemHeight = 0
    TabOrder = 4
  end
  object RigSelectButton: TButton
    Left = 280
    Top = 137
    Width = 75
    Height = 25
    Caption = 'Select'
    TabOrder = 5
    OnClick = RigSelectButtonClick
  end
  object PortCombo: TComboBox
    Left = 56
    Top = 138
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 6
    Text = 'COM1:'
    Items.Strings = (
      'COM1:'
      'COM2:')
  end
  object ModeRadioGroup: TRadioGroup
    Left = 168
    Top = 168
    Width = 185
    Height = 73
    Caption = 'Mode'
    Enabled = False
    Items.Strings = (
      'Unknown'
      'A1A'
      'J3E')
    TabOrder = 7
  end
  object Timer1: TTimer
    Interval = 2000
    OnTimer = Timer1Timer
    Left = 48
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 16
  end
end
