object MinosShowMessage: TMinosShowMessage
  Left = 221
  Top = 332
  Caption = 'Minos Message Display'
  ClientHeight = 289
  ClientWidth = 551
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
  object Label1: TLabel
    Left = 0
    Top = 0
    Width = 551
    Height = 241
    Align = alTop
    Alignment = taCenter
    Anchors = [akLeft, akTop, akRight, akBottom]
    AutoSize = False
    Caption = 'Label1'
    Layout = tlCenter
    WordWrap = True
  end
  object Button1: TButton
    Left = 238
    Top = 256
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = Button1Click
  end
end
