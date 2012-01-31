object AboutBox: TAboutBox
  Left = 243
  Top = 108
  BorderStyle = bsDialog
  Caption = 'About'
  ClientHeight = 344
  ClientWidth = 489
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 489
    Height = 303
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      489
      303)
    object ProgramIcon: TImage
      Left = 8
      Top = 8
      Width = 65
      Height = 57
      Picture.Data = {
        055449636F6E0000010001002020100001000400E80200001600000028000000
        2000000040000000010004000000000080020000000000000000000000000000
        0000000000000000000080000080000000808000800000008000800080800000
        C0C0C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000
        FFFFFF0000000000000000000000000000007700000000000000000000000000
        0007007000000000000000000000000000070070000000000000000000000000
        0000770000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000FFF00000000000000000000000000000000F000
        000000000000000000000000FFFFF00000000000000000F00000000FFF00FFFF
        F000000000000FF000000FFFFFFFFFFFFFF0000000000FF00000FFFFFFFFFF00
        0FFF00000000FFFF000FFF00FFFFF0000FFF00000000FFFF00FFF0000FFFF000
        0FFFF0000000FFFF00FFF0000FFFF0000FFFF000000FFFFF00FFF0000FFFF000
        0FFFF000000FFFFF00FFFF000FFFFF0FFFFFF000000FFFFFF0FFFFF0FFFFFFFF
        FFFFF000000FFFFFF00FFFFFFFFFFFFFFFFFF0000000FFFFFF0FFFFFFFFFFFFF
        FFFFF0000000FFFFFF00FFFFFFFFFFFFFF00F00000000FFFFFF00FFFFFFFFFFF
        F0000000000000FFFFFF000FFFFFFFFF0000000000000000FFFFF0000FFFFFFF
        000000000000000000FF000000FFF00000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000FFFE3FF3FFFC1FE9FFF01869FFF03833FFE0383FFCE0381FF840380F
        F040380FF0403807E0400807E0000007E0000007C0000007C0000007C0000007
        C0000007C0000007C0000007C0000007C0000007C0000007C0000007C0000007
        E0000007E0000007F0000007F8000003FC000003FFC00787FFF03FCFFFF87FFF
        FFFCFFFF}
      Stretch = True
      IsControl = True
    end
    object ProductName: TLabel
      Left = 103
      Top = 21
      Width = 273
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'Product Name'
      IsControl = True
    end
    object Version: TLabel
      Left = 103
      Top = 40
      Width = 273
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'Version'
      IsControl = True
    end
    object Copyright: TLabel
      Left = 15
      Top = 85
      Width = 449
      Height = 13
      Alignment = taCenter
      AutoSize = False
      Caption = 'Copyright'
      IsControl = True
    end
    object Comments: TLabel
      Left = 15
      Top = 104
      Width = 449
      Height = 151
      Alignment = taCenter
      Anchors = [akLeft, akTop, akBottom]
      AutoSize = False
      Caption = 'Comments'
      Layout = tlCenter
      WordWrap = True
      IsControl = True
      ExplicitHeight = 141
    end
    object WebLabel: TLabel
      Left = 163
      Top = 260
      Width = 106
      Height = 13
      Cursor = crHandPoint
      Anchors = [akLeft, akTop, akBottom]
      Caption = 'http://minos.berlios.de'
      OnClick = WebLabelClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 303
    Width = 489
    Height = 41
    Align = alBottom
    TabOrder = 1
    object OKButton: TButton
      Left = 212
      Top = 4
      Width = 65
      Height = 33
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
      IsControl = True
    end
  end
end
