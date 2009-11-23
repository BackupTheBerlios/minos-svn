object MinosHelpForm: TMinosHelpForm
  Left = 0
  Top = 0
  ClientHeight = 395
  ClientWidth = 629
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 354
    Width = 629
    Height = 41
    Align = alBottom
    TabOrder = 0
    ExplicitLeft = 152
    ExplicitTop = 232
    ExplicitWidth = 185
    DesignSize = (
      629
      41)
    object CloseButton: TButton
      Left = 533
      Top = 9
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Close'
      TabOrder = 0
      OnClick = CloseButtonClick
    end
  end
  object HelpMemo: TMemo
    Left = 0
    Top = 0
    Width = 629
    Height = 354
    Align = alClient
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 1
    ExplicitWidth = 426
    ExplicitHeight = 245
  end
end
