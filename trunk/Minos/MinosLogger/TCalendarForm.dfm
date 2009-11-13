object CalendarForm: TCalendarForm
  Left = 0
  Top = 0
  Caption = 'Select Contest from VHF Calendar'
  ClientHeight = 550
  ClientWidth = 748
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object CalendarGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 748
    Height = 509
    Align = alClient
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
    TabOrder = 0
    OnDblClick = CalendarGridDblClick
    ExplicitWidth = 750
  end
  object Panel1: TPanel
    Left = 0
    Top = 509
    Width = 748
    Height = 41
    Align = alBottom
    TabOrder = 1
    ExplicitWidth = 750
    DesignSize = (
      748
      41)
    object CalendarVersionLabel: TLabel
      Left = 266
      Top = 14
      Width = 103
      Height = 13
      Caption = 'CalendarVersionLabel'
    end
    object CloseButton: TButton
      Left = 651
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = 'Close'
      ModalResult = 2
      TabOrder = 0
      OnClick = CloseButtonClick
      ExplicitLeft = 653
    end
    object SelectButton: TButton
      Left = 14
      Top = 6
      Width = 83
      Height = 25
      Caption = 'Select Contest'
      Default = True
      ModalResult = 1
      TabOrder = 1
      OnClick = SelectButtonClick
    end
    object GetCalendarButton: TButton
      Left = 121
      Top = 6
      Width = 139
      Height = 25
      Caption = 'Download latest Calendar'
      TabOrder = 2
      OnClick = GetCalendarButtonClick
    end
    object YearSpinEdit: TEdit
      Left = 472
      Top = 10
      Width = 121
      Height = 21
      TabOrder = 3
      Text = 'YearSpinEdit'
    end
    object YearDownButton: TButton
      Left = 432
      Top = 8
      Width = 26
      Height = 25
      Caption = '<<'
      TabOrder = 4
      OnClick = YearDownButtonClick
    end
    object YearUpButton: TButton
      Left = 605
      Top = 8
      Width = 31
      Height = 25
      Caption = '>>'
      TabOrder = 5
      OnClick = YearUpButtonClick
    end
  end
  object IdHTTP1: TIdHTTP
    MaxLineAction = maException
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.ContentRangeEnd = 0
    Request.ContentRangeStart = 0
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    HTTPOptions = [hoForceEncodeParams]
    Left = 40
    Top = 464
  end
end
