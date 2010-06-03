object SettingBundleFrame: TSettingBundleFrame
  Left = 0
  Top = 0
  Width = 333
  Height = 35
  TabOrder = 0
  TabStop = True
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 333
    Height = 35
    Align = alClient
    BevelOuter = bvLowered
    TabOrder = 0
    object BundleNameLabel: TLabel
      Left = 4
      Top = 12
      Width = 82
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Bundle name'
    end
    object BundleSection: THistoryComboBox
      Left = 92
      Top = 8
      Width = 169
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = BundleSectionChange
    end
    object BundleEdit: TButton
      Left = 276
      Top = 8
      Width = 49
      Height = 20
      Caption = 'Edit'
      TabOrder = 1
      OnClick = BundleEditClick
    end
  end
end
