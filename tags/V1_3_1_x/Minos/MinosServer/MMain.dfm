object MinosMainForm: TMinosMainForm
  Left = 320
  Top = 139
  Caption = 'Minos Messaging Server'
  ClientHeight = 137
  ClientWidth = 239
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SPanel1: TPanel
    Left = 0
    Top = 0
    Width = 239
    Height = 137
    Align = alClient
    BevelWidth = 0
    TabOrder = 0
    object ClientLabel: TLabel
      Left = 168
      Top = 48
      Width = 6
      Height = 13
      Caption = '0'
    end
    object ServerLabel: TLabel
      Left = 16
      Top = 48
      Width = 6
      Height = 13
      Caption = '0'
    end
    object PubLabel: TLabel
      Left = 16
      Top = 88
      Width = 6
      Height = 13
      Caption = '0'
    end
    object SubLabel: TLabel
      Left = 168
      Top = 88
      Width = 6
      Height = 13
      Caption = '0'
    end
    object Label1: TLabel
      Left = 17
      Top = 29
      Width = 36
      Height = 13
      Caption = 'Servers'
    end
    object Label2: TLabel
      Left = 168
      Top = 29
      Width = 31
      Height = 13
      Caption = 'Clients'
    end
    object Label3: TLabel
      Left = 17
      Top = 69
      Width = 46
      Height = 13
      Caption = 'Published'
    end
    object Label4: TLabel
      Left = 168
      Top = 69
      Width = 53
      Height = 13
      Caption = 'Subscribed'
    end
    object CloseButton: TButton
      Left = 72
      Top = 40
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = CloseButtonClick
    end
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
  end
  object ApplicationEvents1: TApplicationEvents
    OnException = ApplicationEvents1Exception
    Left = 54
    Top = 1
  end
  object MainMenu1: TMainMenu
    Left = 440
    Top = 8
    object File1: TMenuItem
      Caption = '&File'
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
        OnClick = Exit1Click
      end
    end
  end
  object ScanTimer: TTimer
    Interval = 20000
    OnTimer = ScanTimerTimer
    Left = 96
  end
end
