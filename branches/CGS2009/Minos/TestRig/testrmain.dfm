object TestRigMain: TTestRigMain
  Left = 467
  Top = 235
  Caption = 'Minos Rig Emulator'
  ClientHeight = 358
  ClientWidth = 394
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
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
    Top = 64
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
    Top = 152
    Width = 23
    Height = 13
    Caption = 'Port'
  end
  object CloseButton: TButton
    Left = 304
    Top = 301
    Width = 75
    Height = 25
    Caption = 'Close'
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object FreqSlider: TxTrackBar
    Left = 16
    Top = 189
    Width = 353
    Height = 45
    Max = 5000
    Frequency = 100
    Position = 1000
    TabOrder = 1
    OnChange = FreqSliderChange
  end
  object RigCombo: TComboBox
    Left = 16
    Top = 112
    Width = 345
    Height = 21
    ItemHeight = 0
    TabOrder = 2
  end
  object RigSelectButton: TButton
    Left = 280
    Top = 152
    Width = 75
    Height = 25
    Caption = 'Select'
    TabOrder = 3
    OnClick = RigSelectButtonClick
  end
  object PortCombo: TComboBox
    Left = 56
    Top = 150
    Width = 145
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 4
    Text = 'COM1:'
    Items.Strings = (
      'COM1:'
      'COM2:')
  end
  object ModeRadioGroup: TRadioGroup
    Left = 24
    Top = 248
    Width = 185
    Height = 105
    Caption = 'Mode'
    Items.Strings = (
      'Unknown'
      'A1A'
      'J3E')
    TabOrder = 5
    OnClick = ModeRadioGroupClick
  end
  object Timer1: TTimer
    Interval = 250
    OnTimer = Timer1Timer
    Left = 355
    Top = 8
  end
  object StartupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = StartupTimerTimer
    Left = 16
    Top = 16
  end
end
