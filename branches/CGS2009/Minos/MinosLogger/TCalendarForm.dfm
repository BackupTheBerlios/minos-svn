object CalendarForm: TCalendarForm
  Left = 0
  Top = 0
  Caption = 'Select Contest from VHF Calendar'
  ClientHeight = 550
  ClientWidth = 750
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
    Width = 750
    Height = 509
    Align = alClient
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
    TabOrder = 0
    OnDblClick = CalendarGridDblClick
  end
  object Panel1: TPanel
    Left = 0
    Top = 509
    Width = 750
    Height = 41
    Align = alBottom
    TabOrder = 1
    DesignSize = (
      750
      41)
    object CalendarVersionLabel: TLabel
      Left = 266
      Top = 14
      Width = 103
      Height = 13
      Caption = 'CalendarVersionLabel'
    end
    object Button1: TButton
      Left = 653
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = 'Close'
      ModalResult = 2
      TabOrder = 0
      OnClick = Button1Click
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
    object YearSpinEdit: TJvSpinEdit
      Left = 502
      Top = 11
      Width = 121
      Height = 21
      EditorEnabled = False
      MaxValue = 2020.000000000000000000
      MinValue = 2000.000000000000000000
      Value = 2000.000000000000000000
      OnBottomClick = YearSpinEditBottomClick
      OnTopClick = YearSpinEditTopClick
      Anchors = [akTop, akRight]
      TabOrder = 3
    end
  end
  object IdHTTP1: TIdHTTP
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    HTTPOptions = [hoForceEncodeParams]
    Left = 40
    Top = 464
  end
end
