object frmWhisperSplitterDemo: TfrmWhisperSplitterDemo
  Left = 367
  Top = 188
  Caption = 'Whisper'#39's Splitter Extension Demo'
  ClientHeight = 399
  ClientWidth = 620
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object SplitterLeft: TSplitter
    Left = 185
    Top = 90
    Width = 9
    Height = 201
    MinSize = 1
    ExplicitTop = 0
    ExplicitHeight = 399
  end
  object SplitterRight: TSplitter
    Left = 451
    Top = 90
    Width = 9
    Height = 201
    Align = alRight
    MinSize = 1
    ExplicitLeft = 530
    ExplicitTop = 0
    ExplicitHeight = 399
  end
  object SplitterTop: TSplitter
    Left = 0
    Top = 81
    Width = 620
    Height = 9
    Cursor = crVSplit
    Align = alTop
    MinSize = 1
    ExplicitTop = 41
  end
  object SplitterBottom: TSplitter
    Left = 0
    Top = 291
    Width = 620
    Height = 9
    Cursor = crVSplit
    Align = alBottom
    MinSize = 1
    ExplicitTop = 355
  end
  object Image_DotInset: TImage
    Left = 260
    Top = 165
    Width = 4
    Height = 4
    AutoSize = True
    Picture.Data = {
      07544269746D617066000000424D660000000000000036000000280000000400
      000004000000010018000000000030000000C40E0000C40E0000000000000000
      0000FF00FFFF00FFFFFFFFFFFFFFFF00FF000000C0C0C0FFFFFFFF00FF000000
      000000FF00FFFF00FFFF00FFFF00FFFF00FF}
    Transparent = True
    Visible = False
  end
  object pnlLeft: TPanel
    Left = 0
    Top = 90
    Width = 185
    Height = 201
    Align = alLeft
    Caption = 'Left'
    TabOrder = 0
    ExplicitLeft = 295
    ExplicitTop = 155
    ExplicitHeight = 41
  end
  object pnlCenter: TPanel
    Left = 194
    Top = 90
    Width = 257
    Height = 201
    Align = alClient
    Caption = 'Center'
    TabOrder = 1
    ExplicitLeft = 245
    ExplicitTop = 170
    ExplicitWidth = 185
    ExplicitHeight = 41
    DesignSize = (
      257
      201)
    object btnRestoreAll: TButton
      Left = 91
      Top = 40
      Width = 75
      Height = 25
      Anchors = []
      Caption = 'Restore All'
      TabOrder = 0
      OnClick = btnRestoreAllClick
    end
    object btnMinimizeAll: TButton
      Left = 91
      Top = 135
      Width = 75
      Height = 25
      Anchors = []
      Caption = 'Minimize All'
      TabOrder = 1
      OnClick = btnMinimizeAllClick
    end
  end
  object pnlRight: TPanel
    Left = 460
    Top = 90
    Width = 160
    Height = 201
    Align = alRight
    Caption = 'Right'
    TabOrder = 2
  end
  object pnlTop: TPanel
    Left = 0
    Top = 0
    Width = 620
    Height = 81
    Align = alTop
    Caption = 'Top'
    TabOrder = 3
  end
  object pnlBottom: TPanel
    Left = 0
    Top = 300
    Width = 620
    Height = 99
    Align = alBottom
    Caption = 'Bottom'
    TabOrder = 4
  end
end
