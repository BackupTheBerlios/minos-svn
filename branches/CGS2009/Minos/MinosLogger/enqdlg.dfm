object EnquireDlg: TEnquireDlg
  Left = 249
  Top = 210
  HelpContext = 400
  Caption = 'Enquire value'
  ClientHeight = 219
  ClientWidth = 427
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object PromptLabel: TLabel
    Left = 40
    Top = 8
    Width = 337
    Height = 113
    Alignment = taCenter
    AutoSize = False
    Caption = 'PromptLabel'
    Layout = tlCenter
    WordWrap = True
  end
  object EnqValue: TEdit
    Left = 40
    Top = 136
    Width = 345
    Height = 24
    TabOrder = 0
    OnKeyPress = EnqValueKeyPress
  end
  object Button1: TButton
    Left = 104
    Top = 184
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object Button2: TButton
    Left = 240
    Top = 184
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
