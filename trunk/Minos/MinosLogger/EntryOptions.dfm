object EntryOptionsForm: TEntryOptionsForm
  Left = 86
  Top = 169
  Caption = 'Produce Contest Entry/Export File'
  ClientHeight = 547
  ClientWidth = 579
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
  object OptionsScrollBox: TScrollBox
    Left = 0
    Top = 0
    Width = 410
    Height = 547
    Align = alClient
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 410
    Top = 0
    Width = 169
    Height = 547
    Align = alRight
    Caption = 'Panel1'
    TabOrder = 1
    DesignSize = (
      169
      547)
    object CloseButton: TButton
      Left = 94
      Top = 380
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'OK'
      Default = True
      TabOrder = 0
      OnClick = CloseButtonClick
    end
    object CancelButton: TButton
      Left = 94
      Top = 452
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelButtonClick
    end
    object EntryGroup: TRadioGroup
      Left = 25
      Top = 32
      Width = 144
      Height = 313
      Anchors = [akTop, akRight]
      Caption = 'File Format'
      ItemIndex = 0
      Items.Strings = (
        'Reg1Test(entry)'
        'ADIF(export)'
        'G0GJV(export)'
        'Minos(export)'
        'KML(GoogleEarth)'
        'Printable(export)')
      TabOrder = 2
    end
  end
  object GridHintTimer: TTimer
    Enabled = False
    Left = 448
    Top = 424
  end
  object SaveDialog: TSaveDialog
    Filter = 'ADIF|*.adi|RSGB Log|*.log|Summary|*.txt'
    InitialDir = '.'
    Options = [ofOverwritePrompt, ofNoChangeDir, ofNoReadOnlyReturn, ofEnableSizing]
    Left = 440
    Top = 376
  end
end
