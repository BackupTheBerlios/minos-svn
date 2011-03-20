object ContestEntryDetails: TContestEntryDetails
  Left = 437
  Top = 174
  Caption = 'Details of Contest Entry'
  ClientHeight = 565
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
    Top = 154
    Width = 29
    Height = 13
    Caption = 'Band'
  end
  object SectionLabel: TLabel
    Left = 519
    Top = 154
    Width = 42
    Height = 13
    Caption = 'Section'
  end
  object StartTimeLabel: TLabel
    Left = 127
    Top = 245
    Width = 60
    Height = 13
    Caption = 'Start Time'
  end
  object EndTimeLabel: TLabel
    Left = 340
    Top = 245
    Width = 53
    Height = 13
    Caption = 'End Time'
  end
  object Label1: TLabel
    Left = 492
    Top = 254
    Width = 81
    Height = 13
    Caption = 'Main Operator'
  end
  object Label2: TLabel
    Left = 492
    Top = 318
    Width = 97
    Height = 13
    Caption = 'Second Operator'
  end
  object OKButton: TButton
    Left = 56
    Top = 526
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = OKButtonClick
  end
  object CancelButton: TButton
    Left = 544
    Top = 526
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
    Top = 526
    Width = 153
    Height = 25
    Caption = 'Entry Details'
    TabOrder = 1
    OnClick = EntDetailButtonClick
  end
  object ContestNameEdit: TLabeledEdit
    Left = 15
    Top = 167
    Width = 265
    Height = 21
    EditLabel.Width = 81
    EditLabel.Height = 13
    EditLabel.Caption = 'Contest Name'
    TabOrder = 4
  end
  object BandComboBox: TComboBox
    Left = 335
    Top = 167
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 5
  end
  object CallsignEdit: TLabeledEdit
    Left = 15
    Top = 210
    Width = 121
    Height = 21
    CharCase = ecUpperCase
    EditLabel.Width = 45
    EditLabel.Height = 13
    EditLabel.Caption = 'Callsign'
    TabOrder = 7
    OnExit = CallsignEditExit
  end
  object LocatorEdit: TLabeledEdit
    Left = 159
    Top = 210
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
    Top = 210
    Width = 233
    Height = 21
    EditLabel.Width = 222
    EditLabel.Height = 13
    EditLabel.Caption = 'Exchange as sent (Postcode, QTH, etc)'
    TabOrder = 9
  end
  object ScoreOptions: TRadioGroup
    Left = 10
    Top = 312
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
    Top = 384
    Width = 249
    Height = 126
    Caption = 'Multipliers and Exchange'
    TabOrder = 17
    object DXCCMult: TCheckBox
      Left = 16
      Top = 28
      Width = 177
      Height = 17
      Caption = 'DXCC Multiplier'
      TabOrder = 0
    end
    object LocatorMult: TCheckBox
      Left = 16
      Top = 49
      Width = 193
      Height = 17
      Caption = 'Locator Multiplier'
      TabOrder = 1
    end
    object ExchangeComboBox: TComboBox
      Left = 16
      Top = 96
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
    object GLocMult: TCheckBox
      Left = 16
      Top = 70
      Width = 97
      Height = 17
      Caption = 'G Locs only'
      TabOrder = 3
    end
  end
  object OptionsGroupBox: TGroupBox
    Left = 10
    Top = 405
    Width = 225
    Height = 57
    Caption = 'Options'
    TabOrder = 18
    object ProtectedOption: TCheckBox
      Left = 6
      Top = 25
      Width = 97
      Height = 17
      Caption = 'Protected'
      TabOrder = 0
      OnClick = ProtectedOptionClick
    end
  end
  object FieldsGroupBox: TGroupBox
    Left = 241
    Top = 405
    Width = 187
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
    Height = 129
    TabOrder = 3
    inline QTHBundleFrame: TSettingBundleFrame
      Left = 344
      Top = 53
      Width = 333
      Height = 40
      TabOrder = 2
      TabStop = True
      ExplicitLeft = 344
      ExplicitTop = 53
      ExplicitHeight = 40
      inherited Panel1: TPanel
        Height = 40
        ExplicitHeight = 40
        inherited BundleSection: THistoryComboBox
          OnChange = BundleFrameBundleSectionChange
        end
        inherited BundleEdit: TButton
          OnClick = BundleFrameBundleEditClick
        end
      end
    end
    inline StationBundleFrame: TSettingBundleFrame
      Left = 6
      Top = 53
      Width = 333
      Height = 40
      TabOrder = 1
      TabStop = True
      ExplicitLeft = 6
      ExplicitTop = 53
      ExplicitHeight = 40
      inherited Panel1: TPanel
        Height = 40
        ExplicitHeight = 40
        inherited BundleSection: THistoryComboBox
          OnChange = BundleFrameBundleSectionChange
        end
        inherited BundleEdit: TButton
          OnClick = BundleFrameBundleEditClick
        end
      end
    end
    inline EntryBundleFrame: TSettingBundleFrame
      Left = 344
      Top = 4
      Width = 333
      Height = 40
      TabOrder = 0
      TabStop = True
      ExplicitLeft = 344
      ExplicitTop = 4
      ExplicitHeight = 40
      inherited Panel1: TPanel
        Height = 40
        ExplicitHeight = 40
        inherited BundleSection: THistoryComboBox
          OnChange = BundleFrameBundleSectionChange
        end
        inherited BundleEdit: TButton
          OnClick = BundleFrameBundleEditClick
        end
      end
    end
    object BSHelpButton: TButton
      Left = 237
      Top = 98
      Width = 202
      Height = 25
      Caption = 'How to use these settings?'
      TabOrder = 3
      OnClick = BSHelpButtonClick
    end
    object Panel1: TPanel
      Left = 8
      Top = 6
      Width = 331
      Height = 40
      BevelOuter = bvLowered
      TabOrder = 4
      object VHFCalendarButton: TButton
        Left = 235
        Top = 9
        Width = 89
        Height = 18
        Caption = 'VHF Calendar'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Verdana'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = VHFCalendarButtonClick
      end
      object ContestNameSelected: TEdit
        Left = 4
        Top = 9
        Width = 225
        Height = 21
        TabStop = False
        ReadOnly = True
        TabOrder = 1
      end
    end
  end
  object SectionComboBox: TComboBox
    Left = 519
    Top = 167
    Width = 150
    Height = 21
    Style = csDropDownList
    CharCase = ecUpperCase
    ItemHeight = 13
    TabOrder = 6
  end
  object PowerEdit: TLabeledEdit
    Left = 562
    Top = 210
    Width = 121
    Height = 21
    EditLabel.Width = 35
    EditLabel.Height = 13
    EditLabel.Caption = 'Power'
    TabOrder = 10
  end
  object StartTimeCombo: TComboBox
    Left = 124
    Top = 262
    Width = 74
    Height = 21
    ItemHeight = 13
    TabOrder = 11
  end
  object EndTimeCombo: TComboBox
    Left = 337
    Top = 262
    Width = 74
    Height = 21
    ItemHeight = 13
    TabOrder = 12
  end
  object LocatorGroupBox: TGroupBox
    Left = 241
    Top = 312
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
  object StartDateEdit: TLabeledEdit
    Left = 15
    Top = 262
    Width = 72
    Height = 21
    EditLabel.Width = 59
    EditLabel.Height = 13
    EditLabel.Caption = 'Start Date'
    TabOrder = 20
    OnKeyPress = DateEditKeyPress
  end
  object EndDateEdit: TLabeledEdit
    Left = 229
    Top = 262
    Width = 72
    Height = 21
    EditLabel.Width = 52
    EditLabel.Height = 13
    EditLabel.Caption = 'End Date'
    TabOrder = 21
    OnKeyPress = DateEditKeyPress
  end
  object StartDateButton: TButton
    Left = 95
    Top = 262
    Width = 21
    Height = 20
    Caption = '...'
    TabOrder = 22
    OnClick = StartDateButtonClick
  end
  object EndDateButton: TButton
    Left = 308
    Top = 262
    Width = 21
    Height = 20
    Caption = '...'
    TabOrder = 23
    OnClick = EndDateButtonClick
  end
  object MainOpComboBox: TComboBox
    Left = 492
    Top = 278
    Width = 145
    Height = 21
    CharCase = ecUpperCase
    ItemHeight = 13
    TabOrder = 13
  end
  object SecondOpComboBox: TComboBox
    Left = 492
    Top = 342
    Width = 145
    Height = 21
    CharCase = ecUpperCase
    ItemHeight = 13
    TabOrder = 14
  end
  object AntOffsetEdit: TLabeledEdit
    Left = 130
    Top = 472
    Width = 41
    Height = 21
    EditLabel.Width = 85
    EditLabel.Height = 13
    EditLabel.Caption = 'Antenna Offset'
    LabelPosition = lpLeft
    LabelSpacing = 10
    TabOrder = 24
  end
end
