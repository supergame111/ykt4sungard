object fmPrintMode: TfmPrintMode
  Left = 275
  Top = 201
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = '��ӡģʽѡ��'
  ClientHeight = 274
  ClientWidth = 425
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object UIPanel1: TUIPanel
    Left = 0
    Top = 0
    Width = 425
    Height = 274
    Align = alClient
    BevelOuter = bvNone
    Color = 16184303
    Font.Charset = GB2312_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = '����'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    StyleItemName = '�Ի���'
    object lbPrinterInfo: TLabel
      Left = 90
      Top = 10
      Width = 6
      Height = 12
    end
    object Label3: TLabel
      Left = 16
      Top = 10
      Width = 60
      Height = 12
      Caption = '��ǰ��ӡ��'
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 30
      Width = 410
      Height = 195
      Caption = '�����ʽ'
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = '����'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object rbToPrinter: TRzRadioButton
        Left = 27
        Top = 26
        Width = 110
        Height = 17
        Caption = '�������ӡ��[&M]'
        Checked = True
        FrameColor = 8409372
        Font.Charset = GB2312_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = '����'
        Font.Style = []
        HighlightColor = 2203937
        HotTrack = True
        HotTrackColor = 3983359
        HotTrackColorType = htctActual
        ParentFont = False
        TabOrder = 2
        TabStop = True
        OnClick = PrintModeChanged
      end
      object rbToFile: TRzRadioButton
        Left = 27
        Top = 115
        Width = 122
        Height = 17
        Caption = '������ı��ļ�[&T]'
        FrameColor = 8409372
        Font.Charset = GB2312_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = '����'
        Font.Style = []
        HighlightColor = 2203937
        HotTrack = True
        HotTrackColor = 3983359
        HotTrackColorType = htctActual
        ParentFont = False
        TabOrder = 3
        OnClick = PrintModeChanged
      end
      object edFileName: TEdit
        Left = 32
        Top = 162
        Width = 249
        Height = 20
        TabOrder = 7
        OnExit = edFileNameExit
      end
      object btnSave: TBitBtn
        Left = 288
        Top = 158
        Width = 75
        Height = 25
        Hint = '���ļ��Ի���ѡ���ļ�'
        Caption = '&Eѡ���ļ�'
        Font.Charset = GB2312_CHARSET
        Font.Color = clWindowText
        Font.Height = -14
        Font.Name = '����'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        OnClick = btnSaveClick
      end
      object GroupBox2: TGroupBox
        Left = 150
        Top = 17
        Width = 246
        Height = 91
        TabOrder = 1
        object Label1: TLabel
          Left = 8
          Top = 23
          Width = 12
          Height = 12
          Caption = '��'
        end
        object Label2: TLabel
          Left = 10
          Top = 53
          Width = 12
          Height = 12
          Caption = '��'
        end
        object lbStartPage: TLabel
          Left = 93
          Top = 23
          Width = 138
          Height = 12
          Caption = 'ҳ����0��ʾ���涨���ޣ�'
        end
        object lbEndPage: TLabel
          Left = 93
          Top = 53
          Width = 138
          Height = 12
          Caption = 'ҳ����0��ʾ���涨���ޣ�'
        end
        object edStart: TSpinEdit
          Left = 34
          Top = 20
          Width = 57
          Height = 21
          MaxValue = 1879048191
          MinValue = 0
          TabOrder = 0
          Value = 0
        end
        object edEnd: TSpinEdit
          Left = 34
          Top = 50
          Width = 57
          Height = 21
          MaxValue = 1879048191
          MinValue = 0
          TabOrder = 1
          Value = 0
        end
      end
      object ckPageRange: TRzCheckBox
        Left = 166
        Top = 15
        Width = 139
        Height = 17
        Caption = 'ָ����ӡ/Ԥ��ҳ��Χ'
        FrameColor = 8409372
        HighlightColor = 2203937
        HotTrack = True
        HotTrackColor = 3983359
        HotTrackColorType = htctActual
        State = cbUnchecked
        TabOrder = 0
        OnClick = ckPageRangeClick
      end
      object rbToExcelFile: TRzRadioButton
        Left = 27
        Top = 139
        Width = 129
        Height = 17
        Caption = '�����Excel�ļ�[&E]'
        FrameColor = 8409372
        Font.Charset = GB2312_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = '����'
        Font.Style = []
        HighlightColor = 2203937
        HotTrack = True
        HotTrackColor = 3983359
        HotTrackColorType = htctActual
        ParentFont = False
        TabOrder = 4
        OnClick = PrintModeChanged
      end
      object rbToHTMLFile: TRzRadioButton
        Left = 187
        Top = 139
        Width = 129
        Height = 17
        Caption = '�����HTML�ļ�[&H]'
        FrameColor = 8409372
        Font.Charset = GB2312_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = '����'
        Font.Style = []
        HighlightColor = 2203937
        HotTrack = True
        HotTrackColor = 3983359
        HotTrackColorType = htctActual
        ParentFont = False
        TabOrder = 5
        OnClick = PrintModeChanged
      end
    end
    object btnPrint: TBitBtn
      Left = 29
      Top = 237
      Width = 79
      Height = 25
      Hint = '��ӡ'
      Caption = '��ӡ(&P)'
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = '����'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnPrintClick
      Glyph.Data = {
        36040000424D3604000000000000360000002800000010000000100000000100
        2000000000000004000000000000000000000000000000000000FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00B5B5B500FFFFFF00FFFFFF00FFFF
        FF009C9C9C009C9C9C00A5A5A500FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00B5B5B500B5B5B500DEDEDE00ADADAD00525252006B6B
        6B00ADADAD00D6D6D600E7DEDE009C9C9C00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00B5B5B500B5B5B500FFFFFF00FFFFFF00E7DEDE00B5B5B5005A5A63003131
        3100313131004A4A4A00848484009C9C9C00A5A5A500FFFFFF00A5A5A500B5B5
        B500F7F7F700FFFFFF00F7F7F700D6D6D600B5B5B500A5A5A500ADADAD009C9C
        9C007B7B7B005252520031313900313131008C8C8C00FFFFFF00B5ADAD00EFEF
        EF00EFEFEF00CECECE00BDB5B500C6C6C600D6CECE00B5B5B500ADADAD00ADA5
        A500ADADAD00ADADAD00A5A5A500848484009C9C9C00FFFFFF00ADA5AD00BDBD
        BD00B5B5B500C6C6C600D6D6D600DEDEDE00F7F7F700F7F7F700E7E7E700D6D6
        D600BDBDBD00ADADAD00ADA5A500ADADAD00A5A5A500FFFFFF00A5A5A500C6BD
        C600D6D6D600DEDEDE00D6D6D600EFEFEF00DEDEDE00B5BDB500CECECE00D6DE
        DE00DEDEDE00DEDEDE00D6D6D600C6C6C600ADADAD00FFFFFF00FFFFFF00B5AD
        AD00DEDEDE00DEDEDE00E7E7E700D6D6D600C6C6C600BDDEBD00CED6CE00D6BD
        B500BDB5B500BDBDBD00C6C6C600D6CECE00BDBDBD00FFFFFF00FFFFFF00FFFF
        FF00B5ADAD00CECECE00C6BDBD00C6C6C600EFEFEF00FFF7F700F7F7F700F7EF
        E700E7E7E700DEDEDE00C6C6C600ADA5A500FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00B5ADAD00E7E7E700D6D6D600B5B5B500CECECE00DEDEE700DEE7
        E700DEDEDE00D6D6D600C6BDBD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFEFEF00FFE7DE00EFD6CE00EFD6CE00EFDED600E7DE
        DE00DEDEDE00ADADAD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00DEB5B500FFE7DE00FFDECE00FFD6C600FFCEB500FFC6
        AD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00DEB5B500FFE7DE00FFDECE00FFD6C600FFCEB500FFBD
        AD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00DEB5B500FFE7DE00FFDECE00FFD6C600FFCEB500FFC6
        AD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00DEB5B500FFE7DE00FFE7DE00FFDECE00FFD6C600FFCEB500FFBD
        AD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00DEB5B500DEB5B500DEB5B500DEB5B500F7BDB500F7BDB500FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00}
    end
    object btnPreview: TBitBtn
      Left = 121
      Top = 237
      Width = 79
      Height = 25
      Hint = 'Ԥ����ӡЧ��'
      Caption = 'Ԥ��(&V)'
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = '����'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = btnPreviewClick
      Glyph.Data = {
        36040000424D3604000000000000360000002800000010000000100000000100
        2000000000000004000000000000000000000000000000000000FFFFFF00FFFF
        FF00B5848400B5848400B5848400B5848400B5848400B5848400B5848400B584
        8400B5848400B5848400B5848400B5848400B5848400FFFFFF00FFFFFF00FFFF
        FF00636B7B00FFEFD600F7E7C600F7DEBD00F7DEB500F7D6AD00F7D6A500F7CE
        9C00F7CE9400F7CE9C00F7CE9C00F7D69C00B5848400FFFFFF00FFFFFF005A9C
        C600318CD6007B849C00F7E7CE00F7DEC600F7DEBD00F7D6B500F7D6AD00F7D6
        A500EFCE9C00EFCE9400EFCE9400F7D69C00B5848400FFFFFF00FFFFFF00FFFF
        FF004AB5FF00298CE70084849C00F7E7CE00F7DEC600F7DEBD00F7D6B500F7D6
        AD00F7D6A500EFCE9C00EFCE9400F7D69C00B5848400FFFFFF00FFFFFF00FFFF
        FF00B58473004AB5FF00218CDE007B849C00F7E7CE00F7DEC600F7DEBD00F7D6
        B500F7D6AD00F7D6A500EFCE9C00F7D69C00B5848400FFFFFF00FFFFFF00FFFF
        FF00BD8C8400FFFFF7004ABDFF005A94BD00A5847B00BD948C00AD7B7B00BD94
        8C00D6B59C00F7D6AD00F7D6A500F7D69C00B5848400FFFFFF00FFFFFF00FFFF
        FF00BD8C8400FFFFFF00FFF7EF00BDA59C00C6A59C00E7CEBD00F7DEC600E7C6
        AD00C69C9400D6B59C00F7D6AD00F7D6A500B5848400FFFFFF00FFFFFF00FFFF
        FF00CE9C8400FFFFFF00FFFFF700C69C9400E7CEC600FFEFDE00FFE7D600FFFF
        F700E7C6AD00BD948C00F7DEB500F7DEAD00B5848400FFFFFF00FFFFFF00FFFF
        FF00CE9C8400FFFFFF00FFFFFF00AD7B7B00FFEFE700FFEFE700FFEFDE00FFFF
        F700F7DEC600AD7B7B00F7DEBD00FFDEB500B5848400FFFFFF00FFFFFF00FFFF
        FF00DEAD8400FFFFFF00FFFFFF00C69C9C00E7D6D600FFF7EF00FFEFE700FFFF
        DE00E7CEBD00BD948C00F7E7C600F7DEB500B5848400FFFFFF00FFFFFF00FFFF
        FF00DEAD8400FFFFFF00FFFFFF00DECECE00CEADAD00E7D6D600FFEFE700E7CE
        C600C6A59C00C6A59400E7DEC600C6BDAD00B5848400FFFFFF00FFFFFF00FFFF
        FF00E7B58C00FFFFFF00FFFFFF00FFFFFF00DECECE00C69C9C00AD7B7B00C69C
        9400D6BDB500BD847B00BD847B00BD847B00B5848400FFFFFF00FFFFFF00FFFF
        FF00E7B58C00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00E7CECE00BD847B00EFB57300EFA54A00C6846B00FFFFFF00FFFFFF00FFFF
        FF00EFBD9400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00E7D6D600BD847B00FFC67300CE947300FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00EFBD9400FFF7F700FFF7F700FFF7F700FFF7F700FFF7F700FFF7F700FFF7
        F700E7D6CE00BD847B00CE9C8400FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00EFBD9400DEAD8400DEAD8400DEAD8400DEAD8400DEAD8400DEAD8400DEAD
        8400DEAD8400BD847B00FFFFFF00FFFFFF00FFFFFF00FFFFFF00}
    end
    object btnSetPrinter: TBitBtn
      Left = 214
      Top = 237
      Width = 99
      Height = 25
      Hint = '���ô�ӡ��'
      Caption = '���ô�ӡ��'
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = '����'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = btnSetPrinterClick
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000120B0000120B00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCED4D9D1CACDCCCBCFEBEEEDFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2B382D2B382DFFFFFF2B382D2B
        382D114475062E54E4E4E8F3F2F4EDECEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        2B382D98FDD22B382DFFFFFF2B382D1BF7672B382D216BAC796481183E6A0B2F
        55B9BDC8FAFAFBFFFFFFFFFFFFFFFFFF2B382D98FDD22B382D2B382D2B382D1B
        F7672B382D2E94EC2771BC1B95E01587C2464B69E9EAEDFFFFFFFFFFFFFFFFFF
        2B382D98FDD255FAB055FAB041FDA11BF7672B382D5EDEFD23C1FF2780C43853
        79E1E2E7F9F9FAFFFFFFFFFFFFFFFFFFFFFFFF2B382D98FDD255FAB01BF7672B
        382D305873679AB060E1F81B71A9292D6F37375F404A67FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF2B382D55FAB02B382D4E476BFFFFFF838F9E66ADBA36C5F817A1
        D725739F37426EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2B382D55FAB02B382D30
        61A09C969B4D799C2CA4DC2EA7D0298EC7217DAC213F79FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFF2B382D55FAB02B382D36B2ED0D5EA72E9FDC33C1FF2290CD3731
        5CB1C1CBADC1CCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2B382D41FDA12B382D8C
        DFFF56DCFF4FC7FF89E9FC5FD5FF3293CA8E96A7FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF2B382D84FEBC55FAB01BF7672B382DB9FDFF62B2E1225C849FCAD52B74
        9A607E96EDF0F3FFFFFFFFFFFFFFFFFF2B382D98FDD241FDA155FAB041FDA11B
        F7672B382D6F7E9AB9C2CE7391A65E7F97FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        2B382D98FDD22B382D2B382D2B382D1BF7672B382D345B79C3CED8F2F5F7EEF2
        F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2B382D98FDD22B382DFFFFFF2B382D1B
        F7672B382DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFF2B382D2B382DFFFFFF2B382D2B382DFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    end
    object ImageButton4: TBitBtn
      Left = 326
      Top = 237
      Width = 79
      Height = 25
      Hint = '�رնԻ���'
      Cancel = True
      Caption = '�ر�(&X)'
      Font.Charset = GB2312_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = '����'
      Font.Style = []
      ModalResult = 2
      ParentFont = False
      TabOrder = 4
      Glyph.Data = {
        36040000424D3604000000000000360000002800000010000000100000000100
        2000000000000004000000000000000000000000000000000000FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B00007B7B
        000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B00007B7B
        00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFFFF00000000000000
        0000000000000000000000000000FFFFFF00FFFFFF007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B00000000000000000000FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B7B007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B7B007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF000000FF007B7B7B007B7B7B007B7B7B007B7B00007B7B
        7B007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF0000007B0000007B000000FF000000FF007B7B7B007B7B7B007B7B00007B7B
        7B007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF000000FF000000FF000000FF000000FF000000FF007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF000000FF000000FF000000FF000000FF007B7B7B007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF000000FF007B7B7B007B7B7B007B7B7B007B7B00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B7B007B7B7B00FFFF00007B7B
        00007B7B00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B7B007B7B7B007B7B7B007B7B
        7B00FFFF00007B7B00007B7B00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B7B007B7B7B007B7B7B007B7B
        7B007B7B7B007B7B7B00FFFF00007B7B000000000000FFFFFF00FFFFFF00FFFF
        FF00FFFFFF00FFFFFF00FFFFFF007B7B7B007B7B7B007B7B7B007B7B7B007B7B
        7B007B7B7B007B7B7B007B7B7B007B7B7B0000000000FFFFFF00}
    end
  end
  object PrinterSetupDialog: TPrinterSetupDialog
    Left = 254
    Top = 1
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'txt'
    Filter = '�ı��ļ�(*.txt)|*.txt|�����ļ�(*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'ѡ��������ı��ļ�'
    Left = 318
    Top = 1
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'Excel'
    Filter = 'Excel�ļ�(*.xls)|*.xls|�����ļ�(*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'ѡ�������Excel�ļ�'
    Left = 286
    Top = 1
  end
  object SaveDialog2: TSaveDialog
    DefaultExt = 'htm'
    Filter = 'HTML�ļ�(*.html)|*.html,*.htm|�����ļ�(*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'ѡ�������HTML�ļ�'
    Left = 350
    Top = 1
  end
end