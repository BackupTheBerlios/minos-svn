object EntryOptionsForm: TEntryOptionsForm
  Left = 86
  Top = 169
  Caption = 'Produce Contest Entry/Export File'
  ClientHeight = 547
  ClientWidth = 567
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  DesignSize = (
    567
    547)
  PixelsPerInch = 96
  TextHeight = 13
  object EntryGroup: TRadioGroup
    Left = 440
    Top = 32
    Width = 123
    Height = 273
    Anchors = [akTop, akRight]
    Caption = 'File Format'
    ItemIndex = 0
    Items.Strings = (
      'Reg1Test(entry)'
      'ADIF(export)'
      'G0GJV(export)'
      'Minos(export)'
      'KML(GoogleEarth)')
    TabOrder = 0
  end
  object CloseButton: TButton
    Left = 486
    Top = 380
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = CloseButtonClick
  end
  object DetailGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 433
    Height = 547
    Align = alLeft
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 2
    DefaultColWidth = 200
    FixedRows = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goEditing, goAlwaysShowEditor, goThumbTracking]
    TabOrder = 2
    OnSelectCell = DetailGridSelectCell
    ColWidths = (
      200
      258)
    RowHeights = (
      24
      24
      24
      24
      27)
  end
  object CancelButton: TButton
    Left = 486
    Top = 452
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = CancelButtonClick
  end
  object SaveDialog: TSaveDialog
    Filter = 'ADIF|*.adi|RSGB Log|*.log|Summary|*.txt'
    InitialDir = '.'
    Options = [ofOverwritePrompt, ofNoChangeDir, ofNoReadOnlyReturn, ofEnableSizing]
    Left = 440
    Top = 376
  end
end
