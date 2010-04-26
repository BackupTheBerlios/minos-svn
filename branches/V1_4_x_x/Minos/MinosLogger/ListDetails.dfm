object ContactListDetails: TContactListDetails
  Left = 0
  Top = 0
  Caption = 'Contact List Details'
  ClientHeight = 189
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object CloseButton: TButton
    Left = 48
    Top = 136
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = CloseButtonClick
  end
  object Button1: TButton
    Left = 296
    Top = 136
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object ListNameEdit: TLabeledEdit
    Left = 96
    Top = 24
    Width = 305
    Height = 21
    EditLabel.Width = 46
    EditLabel.Height = 13
    EditLabel.Caption = 'List Name'
    LabelPosition = lpLeft
    TabOrder = 2
  end
end
