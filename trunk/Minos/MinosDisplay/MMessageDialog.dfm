object MessageDialog: TMessageDialog
  Left = 287
  Top = 188
  Caption = 'Minos Logger'
  ClientHeight = 211
  ClientWidth = 404
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -21
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 25
  object MessageLabel: TLabel
    Left = 0
    Top = 0
    Width = 404
    Height = 137
    Align = alTop
    Alignment = taCenter
    AutoSize = False
    Caption = 'Error/Message label'
    Layout = tlCenter
    WordWrap = True
  end
  object Button1: TButton
    Left = 52
    Top = 152
    Width = 93
    Height = 41
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object Button2: TButton
    Left = 260
    Top = 152
    Width = 93
    Height = 41
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
end
