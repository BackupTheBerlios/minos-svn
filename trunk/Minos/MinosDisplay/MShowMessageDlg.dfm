object MinosShowMessage: TMinosShowMessage
  Left = 221
  Top = 332
  Caption = 'Minos Message Display'
  ClientHeight = 289
  ClientWidth = 604
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
    Width = 604
    Height = 241
    Align = alTop
    Alignment = taCenter
    Anchors = [akLeft, akTop, akRight, akBottom]
    AutoSize = False
    Caption = 'Label1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Verdana'
    Font.Style = []
    ParentFont = False
    Layout = tlCenter
    WordWrap = True
    ExplicitWidth = 551
  end
  object Button1: TButton
    Left = 264
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
