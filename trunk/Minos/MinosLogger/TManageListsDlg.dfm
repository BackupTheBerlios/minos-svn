object ManageListsDlg: TManageListsDlg
  Left = 0
  Top = 0
  Caption = 'Manage Archive Lists'
  ClientHeight = 263
  ClientWidth = 434
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
  object ListsListBox: TListBox
    Left = 16
    Top = 16
    Width = 393
    Height = 185
    ItemHeight = 13
    TabOrder = 0
  end
  object CloseListButton: TButton
    Left = 32
    Top = 224
    Width = 121
    Height = 25
    Caption = 'Close Selected List'
    TabOrder = 1
    OnClick = CloseListButtonClick
  end
  object ExitButton: TButton
    Left = 320
    Top = 224
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Exit'
    ModalResult = 2
    TabOrder = 2
  end
  object EditListButton: TButton
    Left = 179
    Top = 224
    Width = 122
    Height = 25
    Caption = 'Edit Selected List'
    TabOrder = 3
    OnClick = EditListButtonClick
  end
end
