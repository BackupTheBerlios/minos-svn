object SettingBundleFrame: TSettingBundleFrame
  Left = 0
  Top = 0
  Width = 333
  Height = 23
  TabOrder = 0
  TabStop = True
  object BundleNameLabel: TLabel
    Left = 1
    Top = 4
    Width = 61
    Height = 13
    Caption = 'Bundle name'
  end
  object BundleSection: THistoryComboBox
    Left = 89
    Top = 0
    Width = 169
    Height = 21
    Style = csDropDownList
    ItemHeight = 0
    TabOrder = 0
    OnChange = BundleSectionChange
  end
  object BundleEdit: TButton
    Left = 273
    Top = 0
    Width = 49
    Height = 20
    Caption = 'Edit'
    TabOrder = 1
    OnClick = BundleEditClick
  end
end
