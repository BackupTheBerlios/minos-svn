object ClientMain: TClientMain
  Left = 547
  Top = 125
  Caption = 'ClientMain'
  ClientHeight = 400
  ClientWidth = 500
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SPanel1: TPanel
    Left = 0
    Top = 0
    Width = 500
    Height = 400
    Align = alClient
    BevelWidth = 0
    Caption = 'SPanel1'
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 306
      Width = 23
      Height = 13
      Caption = 'From'
    end
    object Label4: TLabel
      Left = 8
      Top = 336
      Width = 13
      Height = 13
      Caption = 'To'
    end
    object Label5: TLabel
      Left = 184
      Top = 336
      Width = 38
      Height = 13
      Caption = 'To Host'
    end
    object Label8: TLabel
      Left = 16
      Top = 369
      Width = 43
      Height = 13
      Caption = 'Message'
    end
    object LogMemo: TMemo
      Left = 0
      Top = 0
      Width = 500
      Height = 289
      Align = alTop
      Lines.Strings = (
        'LogMemo')
      ScrollBars = ssVertical
      TabOrder = 0
    end
    object CloseButton: TButton
      Left = 400
      Top = 307
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 1
      OnClick = CloseButtonClick
    end
    object FromUser: TEdit
      Left = 47
      Top = 302
      Width = 121
      Height = 21
      TabOrder = 2
      Text = 'mjg'
    end
    object ToUser: TEdit
      Left = 48
      Top = 332
      Width = 121
      Height = 21
      TabOrder = 3
      Text = 'G0GJV'
    end
    object ToHost: TEdit
      Left = 240
      Top = 334
      Width = 121
      Height = 21
      TabOrder = 4
      Text = 'localhost'
    end
    object MessageEdit: TEdit
      Left = 80
      Top = 365
      Width = 169
      Height = 21
      TabOrder = 5
      Text = 'MessageEdit'
    end
    object SendButton: TButton
      Left = 272
      Top = 363
      Width = 75
      Height = 25
      Caption = 'Send'
      TabOrder = 6
      OnClick = SendButtonClick
    end
    object SubscribeButton: TButton
      Left = 400
      Top = 367
      Width = 75
      Height = 25
      Caption = 'Subscribe'
      TabOrder = 7
      OnClick = SubscribeButtonClick
    end
  end
  object LogTimer: TTimer
    Interval = 100
    OnTimer = LogTimerTimer
    Left = 16
    Top = 16
  end
  object StartupTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = StartupTimerTimer
    Left = 16
    Top = 64
  end
end
