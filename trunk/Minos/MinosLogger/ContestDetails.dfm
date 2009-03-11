object ContestEntryDetails: TContestEntryDetails
  Left = 437
  Top = 174
  Caption = 'Details of Contest Entry'
  ClientHeight = 515
  ClientWidth = 695
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
  object BandLabel: TLabel
    Left = 335
    Top = 106
    Width = 29
    Height = 13
    Caption = 'Band'
  end
  object SectionLabel: TLabel
    Left = 519
    Top = 106
    Width = 42
    Height = 13
    Caption = 'Section'
  end
  object Label1: TLabel
    Left = 8
    Top = 195
    Width = 59
    Height = 13
    Caption = 'Start Date'
  end
  object Label2: TLabel
    Left = 216
    Top = 195
    Width = 52
    Height = 13
    Caption = 'End Date'
  end
  object StartTimeLabel: TLabel
    Left = 117
    Top = 195
    Width = 60
    Height = 13
    Caption = 'Start Time'
  end
  object EndTimeLabel: TLabel
    Left = 328
    Top = 195
    Width = 53
    Height = 13
    Caption = 'End Time'
  end
  object OKButton: TButton
    Left = 56
    Top = 478
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = OKButtonClick
  end
  object CancelButton: TButton
    Left = 544
    Top = 478
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    OnClick = CancelButtonClick
  end
  object EntDetailButton: TButton
    Left = 261
    Top = 478
    Width = 153
    Height = 25
    Caption = 'Entry Details'
    TabOrder = 1
    OnClick = EntDetailButtonClick
  end
  object ContestNameEdit: TLabeledEdit
    Left = 15
    Top = 119
    Width = 265
    Height = 21
    EditLabel.Width = 81
    EditLabel.Height = 13
    EditLabel.Caption = 'Contest Name'
    TabOrder = 4
  end
  object BandComboBox: TComboBox
    Left = 335
    Top = 119
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 5
  end
  object CallsignEdit: TLabeledEdit
    Left = 15
    Top = 162
    Width = 121
    Height = 21
    CharCase = ecUpperCase
    EditLabel.Width = 45
    EditLabel.Height = 13
    EditLabel.Caption = 'Callsign'
    TabOrder = 7
  end
  object LocatorEdit: TLabeledEdit
    Left = 159
    Top = 162
    Width = 121
    Height = 21
    CharCase = ecUpperCase
    EditLabel.Width = 42
    EditLabel.Height = 13
    EditLabel.Caption = 'Locator'
    TabOrder = 8
  end
  object ExchangeEdit: TLabeledEdit
    Left = 300
    Top = 162
    Width = 233
    Height = 21
    EditLabel.Width = 222
    EditLabel.Height = 13
    EditLabel.Caption = 'Exchange as sent (Postcode, QTH, etc)'
    TabOrder = 9
  end
  object ScoreOptions: TRadioGroup
    Left = 10
    Top = 264
    Width = 225
    Height = 82
    Caption = 'Scoring'
    ItemIndex = 0
    Items.Strings = (
      'Commenced Kilometer'
      'Point per QSO')
    TabOrder = 15
  end
  object MultGroupBox: TGroupBox
    Left = 434
    Top = 249
    Width = 249
    Height = 97
    Caption = 'Multipliers and Exchange'
    TabOrder = 17
    object DXCCMult: TCheckBox
      Left = 16
      Top = 19
      Width = 177
      Height = 17
      Caption = 'DXCC Multiplier'
      TabOrder = 0
    end
    object LocatorMult: TCheckBox
      Left = 16
      Top = 40
      Width = 193
      Height = 17
      Caption = 'Locator Multiplier'
      TabOrder = 1
    end
    object ExchangeComboBox: TComboBox
      Left = 16
      Top = 63
      Width = 230
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 2
      Text = 'No Exchange Required'
      Items.Strings = (
        'No Exchange Required'
        'PostCode Multipliers'
        'Other Exchange Multiplier'
        'Exchange Required (no multiplier)')
    end
  end
  object OptionsGroupBox: TGroupBox
    Left = 10
    Top = 357
    Width = 225
    Height = 105
    Caption = 'Options'
    TabOrder = 18
    object ReadOnlyOption: TCheckBox
      Left = 6
      Top = 25
      Width = 97
      Height = 17
      Caption = 'Read Only'
      TabOrder = 0
    end
    object PostEventOption: TCheckBox
      Left = 6
      Top = 48
      Width = 209
      Height = 17
      Caption = 'Post Event Entry (not real time)'
      TabOrder = 1
    end
    object MultiBandCheckBox4: TCheckBox
      Left = 8
      Top = 74
      Width = 201
      Height = 17
      Caption = 'Multi-Band (see Band combo)'
      TabOrder = 2
    end
  end
  object FieldsGroupBox: TGroupBox
    Left = 434
    Top = 357
    Width = 249
    Height = 105
    Caption = 'Optional Fields'
    TabOrder = 19
    Visible = False
    object RSTField: TCheckBox
      Left = 8
      Top = 19
      Width = 97
      Height = 17
      Caption = 'RST'
      Checked = True
      State = cbChecked
      TabOrder = 0
      Visible = False
    end
    object SerialField: TCheckBox
      Left = 8
      Top = 39
      Width = 97
      Height = 17
      Caption = 'Serial'
      Checked = True
      State = cbChecked
      TabOrder = 1
      Visible = False
    end
    object LocatorField: TCheckBox
      Left = 8
      Top = 60
      Width = 97
      Height = 17
      Caption = 'Locator'
      Checked = True
      State = cbChecked
      TabOrder = 2
      Visible = False
    end
    object QTHField: TCheckBox
      Left = 8
      Top = 81
      Width = 97
      Height = 17
      Caption = 'QTH'
      Checked = True
      State = cbChecked
      TabOrder = 3
      Visible = False
    end
  end
  object BundleBox: TScrollBox
    Left = 8
    Top = 8
    Width = 681
    Height = 92
    TabOrder = 3
    inline QTHBundleFrame: TSettingBundleFrame
      Left = 344
      Top = 32
      Width = 333
      Height = 23
      TabOrder = 4
      TabStop = True
      ExplicitLeft = 344
      ExplicitTop = 32
      inherited BundleNameLabel: TLabel
        Width = 75
        ExplicitWidth = 75
      end
      inherited BundleSection: THistoryComboBox
        OnChange = BundleFrameBundleSectionChange
      end
      inherited BundleEdit: TButton
        OnClick = BundleFrameBundleEditClick
      end
    end
    inline StationBundleFrame: TSettingBundleFrame
      Left = 6
      Top = 32
      Width = 333
      Height = 23
      TabOrder = 3
      TabStop = True
      ExplicitLeft = 6
      ExplicitTop = 32
      inherited BundleNameLabel: TLabel
        Width = 75
        ExplicitWidth = 75
      end
      inherited BundleSection: THistoryComboBox
        OnChange = BundleFrameBundleSectionChange
      end
      inherited BundleEdit: TButton
        Left = 264
        OnClick = BundleFrameBundleEditClick
        ExplicitLeft = 264
      end
    end
    inline EntryBundleFrame: TSettingBundleFrame
      Left = 344
      Top = 4
      Width = 333
      Height = 23
      TabOrder = 2
      TabStop = True
      ExplicitLeft = 344
      ExplicitTop = 4
      inherited BundleNameLabel: TLabel
        Width = 75
        ExplicitWidth = 75
      end
      inherited BundleSection: THistoryComboBox
        OnChange = BundleFrameBundleSectionChange
      end
      inherited BundleEdit: TButton
        OnClick = BundleFrameBundleEditClick
      end
    end
    object BSHelpButton: TButton
      Left = 237
      Top = 59
      Width = 202
      Height = 25
      Caption = 'How to use these settings?'
      TabOrder = 5
      OnClick = BSHelpButtonClick
    end
    object VHFCalendarButton: TButton
      Left = 239
      Top = 5
      Width = 89
      Height = 18
      Caption = 'VHF Calendar'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = VHFCalendarButtonClick
    end
    object ContestNameSelected: TEdit
      Left = 8
      Top = 5
      Width = 225
      Height = 21
      TabStop = False
      ReadOnly = True
      TabOrder = 0
    end
  end
  object SectionComboBox: TComboBox
    Left = 519
    Top = 119
    Width = 150
    Height = 21
    Style = csDropDownList
    CharCase = ecUpperCase
    ItemHeight = 13
    TabOrder = 6
  end
  object PowerEdit: TLabeledEdit
    Left = 562
    Top = 162
    Width = 121
    Height = 21
    EditLabel.Width = 35
    EditLabel.Height = 13
    EditLabel.Caption = 'Power'
    TabOrder = 10
  end
  object StartDateEdit: TJvDatePickerEdit
    Left = 8
    Top = 214
    Width = 97
    Height = 21
    AllowNoDate = True
    Checked = True
    EnableValidation = False
    TabOrder = 11
    OnKeyPress = DateEditKeyPress
  end
  object EndDateEdit: TJvDatePickerEdit
    Left = 216
    Top = 214
    Width = 97
    Height = 21
    AllowNoDate = True
    Checked = True
    EnableValidation = False
    TabOrder = 13
    OnKeyPress = DateEditKeyPress
  end
  object StartTimeCombo: TComboBox
    Left = 116
    Top = 214
    Width = 74
    Height = 21
    ItemHeight = 13
    TabOrder = 12
  end
  object EndTimeCombo: TComboBox
    Left = 328
    Top = 214
    Width = 74
    Height = 21
    ItemHeight = 13
    TabOrder = 14
  end
  object LocatorGroupBox: TGroupBox
    Left = 241
    Top = 264
    Width = 187
    Height = 82
    Caption = 'Locator Length'
    TabOrder = 16
    object AllowLoc8CB: TCheckBox
      Left = 10
      Top = 52
      Width = 167
      Height = 17
      Caption = 'Allow 8 char Locators'
      TabOrder = 1
    end
    object AllowLoc4CB: TCheckBox
      Left = 10
      Top = 24
      Width = 167
      Height = 17
      Caption = 'Allow 4 char Locators'
      TabOrder = 0
    end
  end
end
