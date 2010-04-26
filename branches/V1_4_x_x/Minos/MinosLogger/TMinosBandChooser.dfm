object MinosBandChooser: TMinosBandChooser
  Left = 0
  Top = 0
  Caption = 'Please choose a valid band descriptor'
  ClientHeight = 250
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 23
  object ScreedLabel: TLabel
    Left = 26
    Top = 24
    Width = 374
    Height = 73
    Alignment = taCenter
    AutoSize = False
    Caption = 'Bad Band'
    Layout = tlCenter
    WordWrap = True
  end
  object BandCombo: TComboBox
    Left = 141
    Top = 122
    Width = 145
    Height = 31
    Style = csDropDownList
    ItemHeight = 23
    TabOrder = 0
  end
  object OKButton: TButton
    Left = 176
    Top = 184
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 1
  end
end
