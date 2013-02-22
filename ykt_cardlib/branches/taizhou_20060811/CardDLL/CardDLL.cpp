#include "stdafx.h"
#include "CardDLL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <time.h>

int InitInstance();
int ifReadyOK();
//int isConnectMF = -1;    //�ж϶�д���Ƿ�ɹ���ʼ��
static int	ErrCode;
static char ErrMsg[256];

// ������SMARTCOM411DXQ.dll
unsigned char SMT_UCWORKINGKEY[256]="";
int nAutoFlag = 0;

//////////////////////////////////////////////////////////////////////////
// ��ʼ����
// ��ʼ��������
#define SMARD_CARD_SECT_NO 32
// ��Ȩ��������
#define AUTH_CARD_SECT_NO 32
// ��Կ����
#define CARD_KEY_LEN 6
// ��ʼ����ʱд��Ŀ���λ
static BYTE CARD_NEW_CTRL_BIT[] = {0x7F,0x07,0x88,0x00};
// ����ʱ�Ŀ���λ
static BYTE CARD_INIT_CTRL_BIT[] = {0xFF,0x07,0x80,0x69};
// PHILIP������
static BYTE PHILIPS_CARD_TYPE[] = {0x18,0};
// ��Կ���ڵĿ��
#define PASSWD_BLOCK_NUM 3
// ��Ȩ��Կ
static BYTE CARD_MAIN_KEY[9] = "";
// �жϴ����Ƿ��Ѿ���
static BOOL com_is_open = FALSE;
// 0����KEYA, 1����KEYB
#define CARD_KEY_A 0
#define CARD_KEY_B 1
// ��ʼ���Ľ�ֵ
static BYTE CARD_INIT_KEY[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
// �ж��Ƿ��Ѷ���Ȩ��
static BOOL has_read_main_key = FALSE;
// ��Ȩ��־
static short AUTH_CARD_FLAG = -1;
static unsigned char s_water_common_card[2] = {0xF9,0x9E};
static unsigned char s_water_clear_card[2] = {0xF9,0x9C};
static unsigned char s_water_clear_card2[2] = {0xF9,0x9A};
static unsigned char s_water_new_card[2] = {0xF9,0x9D};
static unsigned char s_water_auth_card[2] = {0xF9,0x9F};

// ������Ȩ����Կʱ������
static BYTE AUTH_CARD_SEED[8] = "";
//////////////////////////////////////////////////////////////////////////

// ������COMM.H�����ݽṹ

// ������COMM.H�����ݽṹ
char bufSmartCom[2056] = "";
char bufDesdll[2056] = "";
char pathBuf[512] = "";
char iniFilePathBuf[512] = "";

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
 	GetIniFile(iniFilePathBuf);
	GetPrivateProfileString("LOADLIBRARY", "DLL1", "", bufSmartCom, sizeof(bufSmartCom), iniFilePathBuf);
	GetPrivateProfileString("LOADLIBRARY", "DLL2", "", bufDesdll, sizeof(bufDesdll), iniFilePathBuf);
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ��ȡָ��Ŀ¼���ļ�
void GetIniFile(char *FilePathBuf)
{
	char pathBuf[512] = "";
	int len = GetModuleFileName(NULL, pathBuf, sizeof(pathBuf));
	if (len <= 0)
	{
		MessageBox(NULL, "Get Path Err!", "", NULL);
		return ;
	}
	char *p = pathBuf + len;
	while (p >= pathBuf)
	{
		if (*p == '\\')
		{
			break;
		}
		p--;
	}
	if (p > pathBuf)
	{
		*p = '\0';
	}

	if (!strnicmp(p - 4, "\\dll", 4))
	{
		p -= 4;
		*p = '\0';
	}
/*	FilePathBuf = strncat("\\", pathBuf, 1);*/
	strncat(pathBuf, "\\CardDll.ini", 13);
	strcpy(FilePathBuf, pathBuf);
}
//////////////////////////////////////////////////////////////////////////

HINSTANCE hDLL = NULL;
typedef INT (CALLBACK* LPSMT_ConnectMF280)(int, int);
typedef INT (CALLBACK* LPSMT_ControlBuzzer)(void);
typedef INT (CALLBACK* LPSMT_CloseMF280)(void);	
// typedef INT (CALLBACK* LPSMT_CreateGreyCard)(BOOL bPersonalFlag,BOOL bStateFlag, BOOL bBankInfo,BOOL bPacketFlag[8]);
// typedef INT (CALLBACK* LPSMT_WriteCardStateInfo)(unsigned char ucShowCardNo[5],int nDealCardNo[1],unsigned char ucDeadLineDate[3],unsigned char ucCardRightType[1]);
// typedef INT (CALLBACK* LPSMT_PacketAddMomey)(int ,int );
// typedef INT (CALLBACK* LPSMT_PacketSetMoney)(int ,int );
// typedef INT (CALLBACK* LPSMT_AutoDll)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_JudgeAutoFlag)();
typedef INT (CALLBACK* LPSMT_RequestCardExist)(unsigned char *,unsigned char *);
// typedef INT (CALLBACK* LPSMT_ReadCardStateInfo)(unsigned char ucShowCardNo[5],int nDealCardNo[1],unsigned char ucDeadLineDate[3],unsigned char ucCardRightType[1]);
// typedef INT (CALLBACK* LPSMT_WritePersonalInfo)(unsigned char ucName[8],unsigned char ucDutyNo[4],unsigned char ucCertificateNo[20],unsigned char ucDepartmentNo[10],unsigned char ucIdentifyNo[4],unsigned char ucSexNo[1], unsigned char ucCardNo[20]);
// typedef INT (CALLBACK* LPSMT_ReadPersonalInfo)(unsigned char ucName[8],unsigned char ucDutyNo[4],unsigned char ucCertificateNo [20],unsigned char ucDepartmentNo[10],unsigned char ucIdentifyNo[4],unsigned char ucSexNo[1] ,unsigned char ucCardNo[20]);
// typedef INT (CALLBACK* LPSMT_ChangeName)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ChangeDutyNo)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ChangeCerificateNo)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ChangeDepartmentNo)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ChangeIdentifyNo)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ChangeSexNo)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ReadPacketInfo)(int,LOOPPURSEINFO *LoopPurseInfo);
// typedef INT (CALLBACK* LPSMT_ChangeDeadLineDate)(unsigned char *);
// typedef INT (CALLBACK* LPSMT_ChangeCardRightType)(unsigned char ucCardRightType[1]);
// typedef INT (CALLBACK* LPSMT_WritePersonalPassword)(unsigned char PersonalPsd[3]);
// typedef INT (CALLBACK* LPSMT_ReadPersonalPassword)(unsigned char PersonalPsd[3]);
// typedef INT (CALLBACK* LPSMT_ClearCard)();
typedef INT (CALLBACK* LPSMT_SleepCard)();
typedef void (CALLBACK* LPSetLastErrMsg)(int err_code,char* format, ...);
typedef void (CALLBACK* LPGetMFLastErr)();
typedef INT	(CALLBACK* LPMF_ReadSeriesNo)(unsigned char *Buffer);
typedef INT (CALLBACK* LPSMT_ExeCommand)(unsigned char *Buffer,int Len);
typedef INT (CALLBACK* LPSMT_Login_With_UserKey)(int Sector, unsigned char *Buffer, int PaDSelect);
typedef INT (CALLBACK* LPSMT_WriteBlock)(int Sector,int BlockNo,unsigned char *BlockBuf);
typedef INT (CALLBACK* LPSMT_ReadBlock)(int Sector,int BlockNo, unsigned char *BlockBuf);
typedef void (CALLBACK* LPSMT_CopyWorkingKey)(unsigned char *SMT_UCWORKINGKEY);
// typedef INT (CALLBACK* LPSMT_WriteLibraryNo)(unsigned char ucLibraryNo[13]);
// typedef INT (CALLBACK* LPSMT_ReadLibraryNo)(unsigned char ucLibraryNo[13]);
// typedef INT (CALLBACK* LPSMT_ChangeCardNo)(unsigned char ucCardNo[20]);
// typedef INT (CALLBACK* LPSMT_ChangeShowCardNo)(unsigned char ucShowCardNo[5]);

//typedef INT (CALLBACK* LPSMT_SMT_ReadLibraryNo)(unsigned char ucLibraryNo[13]);
LPSMT_ConnectMF280				SMT_ConnectMF280=NULL;
LPSMT_ControlBuzzer				SMT_ControlBuzzer=NULL;
LPSMT_CloseMF280				SMT_CloseMF280=NULL;
// LPSMT_CreateGreyCard			SMT_CreateGreyCard=NULL;
// LPSMT_WriteCardStateInfo		SMT_WriteCardStateInfo=NULL;
//LPSMT_AutoDll					SMT_AutoDll=NULL;
//LPSMT_JudgeAutoFlag				SMT_JudgeAutoFlag=NULL;
LPSMT_RequestCardExist			SMT_RequestCardExist=NULL;
//LPSMT_ReadCardStateInfo			SMT_ReadCardStateInfo=NULL;
//LPSMT_WritePersonalInfo			SMT_WritePersonalInfo=NULL;
//LPSMT_ReadPersonalInfo			SMT_ReadPersonalInfo=NULL;
//LPSMT_WriteLibraryNo			SMT_WriteLibraryNo=NULL;
//LPSMT_ReadLibraryNo				SMT_ReadLibraryNo=NULL;
//LPSMT_ChangeName				SMT_ChangeName=NULL;
//LPSMT_ChangeDutyNo				SMT_ChangeDutyNo=NULL;
//LPSMT_ChangeCerificateNo		SMT_ChangeCerificateNo=NULL;
//LPSMT_ChangeDepartmentNo		SMT_ChangeDepartmentNo=NULL;
//LPSMT_ChangeIdentifyNo			SMT_ChangeIdentifyNo=NULL;
//LPSMT_ChangeSexNo				SMT_ChangeSexNo=NULL;
//LPSMT_ChangeCardNo				SMT_ChangeCardNo=NULL;
//LPSMT_ChangeDeadLineDate		SMT_ChangeDeadLineDate=NULL;
//LPSMT_ChangeCardRightType		SMT_ChangeCardRightType=NULL;
//LPSMT_WritePersonalPassword		SMT_WritePersonalPassword=NULL;
//LPSMT_ReadPersonalPassword		SMT_ReadPersonalPassword=NULL;

//Ǯ������
//LPSMT_PacketSetMoney     SMT_PacketSetMoney=NULL;
//LPSMT_PacketAddMomey     SMT_PacketAddMomey=NULL;
//LPSMT_ReadPacketInfo	 SMT_ReadPacketInfo=NULL;
//LPSMT_ClearCard		 SMT_ClearCard=NULL;
LPSMT_SleepCard		 SMT_SleepCard=NULL;
// LPSMT_ChangeShowCardNo	SMT_ChangeShowCardNo=NULL;

// ��½��
LPSMT_Login_With_UserKey SMT_Login_With_UserKey = NULL;

// ��ȡ���к�
LPMF_ReadSeriesNo MF_ReadSeriesNo = NULL;
// ��, д�����ݿ�
LPSMT_ReadBlock SMT_ReadBlock = NULL; 
LPSMT_WriteBlock SMT_WriteBlock = NULL;

// ���ó�����Ϣ
LPSetLastErrMsg SetLastErrMsg = NULL;
LPGetMFLastErr GetMFLastErr = NULL;

// ����ȫ���ַ���
LPSMT_CopyWorkingKey SMT_CopyWorkingKey = NULL;


// ��Կ���㺯��(������Desdll.dll)
HINSTANCE hDesDLL = NULL;
typedef long (CALLBACK * lpgen_user_card_keyA )(unsigned char *key,unsigned char *text,unsigned char *keyA);;
typedef long (CALLBACK * lpgen_user_card_keyB )(unsigned char *key,unsigned char *text,unsigned char *keyA);;
typedef INT (CALLBACK *lpencrypt_work_key)(unsigned char* text,unsigned char* mtext);
typedef long (CALLBACK * lpgen_auth_card_keyA)(BYTE seed[8],BYTE key[6]);
typedef long (CALLBACK * lpgen_auth_card_keyB)(BYTE seed[8],BYTE key[6]);
typedef long (CALLBACK* lpdecrypt_work_key)(BYTE src_key[8],BYTE des_key[8]);
typedef long (CALLBACK* lpget_check_char)(BYTE buf[16]);

lpgen_user_card_keyA	gen_user_card_keyA = NULL;
lpgen_user_card_keyB	gen_user_card_keyB = NULL;
lpencrypt_work_key      encrypt_work_key = NULL; 
lpgen_auth_card_keyA	gen_auth_card_keyA = NULL;
lpgen_auth_card_keyB	gen_auth_card_keyB = NULL;
lpdecrypt_work_key		decrypt_work_key   = NULL;
lpget_check_char		get_check_char	   = NULL;

// ����KEYA
static void SMT_Cal_KeyA(unsigned char *key, unsigned char *ucSerialNo, unsigned char *ucKeyA)
{
// 	memcpy(ucRawData,ucSerialNo,4);
// 	for(int i = 0;i < 4; i++)
// 		ucRawData[i+4] = ~ucRawData[i];
	gen_user_card_keyA(key, ucSerialNo, ucKeyA);
// 	des_code(SMT_UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_KeyA(ucDesData,ucKeyA);
}

// ����KEYB
static void SMT_Cal_KeyB(unsigned char *key, unsigned char *ucSerialNo, unsigned char *ucKeyB)
{
// 	memcpy(ucRawData + 4, ucSerialNo, 4);
// 	for(int i = 0;i < 4; i++)
// 		ucRawData[i] = ~ucRawData[i + 4];
// 	TriDes(SMT_UCWORKINGKEY, ucRawData, ucDesData, '0');
// 	Cal_Key(ucDesData, ucKeyB);
	gen_user_card_keyB(key, ucSerialNo, ucKeyB);
}

//ʹ�ø�����Կ��ϵ��½��Ƭ
static int Login_Card(int sector)
{
	unsigned char ucSerialNo[5], ucKeyA[6];
	int ret;
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
		return -1;//��Ƭ���ڸ�Ӧ��
	//1 --- ����KEYA
// 	memcpy(ucRawData,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i+4] = ~ucRawData[i];
// 	des_code(SMT_UCWORKINGKEY,ucRawData,ucDesData,'0');
//  	Cal_KeyA(ucDesData,ucKeyA);
//	SMT_Cal_KeyA(ucRawData, ucSerialNo, ucDesData, ucKeyA);
//	Cal_Key(ucDesData, ucKeyA);
	SMT_Cal_KeyA(SMT_UCWORKINGKEY, ucSerialNo, ucKeyA);
	ret = SMT_Login_With_UserKey(sector,ucKeyA,0);                        // ���λ�ó�������
	if(ret != 0)
	{
		return -2;//��Ƭ��½ʧ��
	}
	return 0;
}


//FreeLibrary(hDLL);
void SetErrMsg(int ecode,char *errmsg)
{
	ErrCode=ecode;;
	strcpy(ErrMsg,errmsg);
}

int __stdcall GetLastErrMsg(int ecode[1],char *errmsg)
{
	ecode[0]=ErrCode;
	strcpy(errmsg,ErrMsg);
	return 0;
}

int __stdcall dec2bcd(unsigned char *sDecStr,unsigned char *sBcdStr,int bcd_len)
{
	int i;
	unsigned char lch,hch;

	for(i=0;i<bcd_len;i++)
	{
		lch=sDecStr[2*i]-'0';
		hch=sDecStr[2*i+1]-'0';
		lch=lch<<4;
		hch=hch&0x0F;
		lch=lch&0xF0;
		sBcdStr[i]=hch|lch;
	}
	return 0;
}

int __stdcall bcd2dec(unsigned char *sBcdStr,int bcd_len,unsigned char *sDecStr)
{	
	int i;
	unsigned char lch,hch;

	for(i=0;i<bcd_len;i++)
	{
		hch=(sBcdStr[i]&0x0F);
		lch=(sBcdStr[i]&0xF0);
		lch=lch>>4;
		sDecStr[2*i]=lch+'0';
		sDecStr[2*i+1]=hch+'0';
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ������㺯��
void dec2hex(unsigned char *sDec,int dec_len,char *sHex)
{
	int i=0;
    int k=0;
	for(i=0;i<dec_len;i++)
	{
        k=sDec[i];
        sprintf(&sHex[2*i],"%02X",k);
	}
}

void hex2dec(char *sHex,unsigned char * sDec,int dec_len)
{
	int i=0;
	unsigned long ul;
	char sHexTmp[3];

	memset(sHexTmp,0,sizeof(sHexTmp));
	for(i=0;i<dec_len;i++)
	{
		memcpy(sHexTmp,&sHex[2*i],2);
		ul=strtoul(sHexTmp,NULL,16);
		sDec[i]=(unsigned char)ul;
	}
}

// �ı����������unsigned char* �� char*
int  bcd2decA(unsigned char *sBcdStr, int bcd_len, char *sDecStr)
{	
	int i;
	unsigned char lch,hch;

	for(i=0;i<bcd_len;i++)
	{
		hch=(sBcdStr[i]&0x0F);
		lch=(sBcdStr[i]&0xF0);
		lch=lch>>4;
		sDecStr[2*i]=lch+'0';
		sDecStr[2*i+1]=hch+'0';
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////

int __stdcall ConnectMF(int com,int baud)
{
	int ret = 0;
	if(hDLL == NULL && hDesDLL == NULL)
	{
		ret=Load_Dll();
		if(ret)
			return -1;
	}
	if (com < 0)
	{
		com = 1;       //Ĭ�ϴ���1
	}
	if (baud < 1)
	{
		baud = 19200;  //Ĭ��19200
	}
//	SMT_CloseMF280();
	ret = SMT_ConnectMF280(com,baud);
	if(ret!= 0)
	{
		char  sEmsg[256]="";
		sprintf(sEmsg,"���ӵ�COM%dʧ��",com);
		SetErrMsg(ret,sEmsg);
		return ret;
	}
	SMT_ControlBuzzer();
	return 0;
}

int __stdcall CloseMF(void)
{
	int ret = 0;

	ret = SMT_CloseMF280();
	if(ret)
	{
		SetErrMsg(ret,"�رմ���ʧ��");
		return ret;
	}
//	isConnectMF=-1;
	return 0;
}

int __stdcall HaltCard(void)
{
	int ret = 0;
	ret = SMT_SleepCard();
	if (ret)
	{
		SetErrMsg(ret, "���߿�ʧ��");
		return ret;
	}
	return 0;
}

//���п�
int __stdcall PublishCard(TPublishCard *pc)
{
	int ret = 0; 
	unsigned char ucBcdShowCardNo[5];
	unsigned char ucDeadLineDate[3];
	unsigned char ucCardRightType[1];
	BOOL bPersonalFlag=TRUE;
	BOOL bStateFlag=TRUE;
	BOOL bBankInfo=TRUE;
	BOOL bPacketFlag[8]={TRUE,TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
	int iCardNo[1];
	memset(ucBcdShowCardNo,0,sizeof(ucBcdShowCardNo));
//	if (isConnectMF) return isConnectMF;

	ret = ifReadyOK();
	if (ret !=0)
		return ret;
	/*
	if(pc->isFormat=='Y'||pc->isFormat=='y')
	{
		ret = SMT_CreateGreyCard(bPersonalFlag,bStateFlag,bBankInfo,bPacketFlag);
		if(ret != 0)
		{
			sprintf(szMsg,"��ʼ����ʧ��,ret %d",ret);
			MessageBox(NULL,szMsg,"����",0);		
			return ret;
		}
	}
	*/
	ret=SMT_ClearCard();
	if(ret)
	{
		SetErrMsg(ret,"��ʽ����ʧ��");
		return ret;
	}
	ret = strlen((char*)pc->ShowCardNo);
	if( 10==ret )	
	{
		//SetErrMsg(ret,"��ʾ���ų��Ȳ���10λ");
		//return -4;		     //��ʾ����λ������
		//ת��ΪBCD��
		dec2bcd(pc->ShowCardNo,ucBcdShowCardNo,5);
	}
	//ת��Ϊ����
	char sYear[3];
	char sMon[3];
	char sDay[3];
	
	memset(sYear,0,sizeof(sYear));
	memset(sMon,0,sizeof(sMon));
	memset(sDay,0,sizeof(sDay));

	memcpy(sYear,pc->DeadLineDate+2,2);
	memcpy(sMon,pc->DeadLineDate+4,2);
	memcpy(sDay,pc->DeadLineDate+6,2);
	
	ucDeadLineDate[0]=atoi(sYear);
	ucDeadLineDate[1]=atoi(sMon);
	ucDeadLineDate[2]=atoi(sDay);
	iCardNo[0]=atoi((char*)pc->CardNo);
	ucCardRightType[0]=pc->CardRightType;
	ret = SMT_WriteCardStateInfo(ucBcdShowCardNo,iCardNo,ucDeadLineDate,ucCardRightType);
	if(ret)
	{
		SetErrMsg(ret,"д��������Ϣʧ��");
		return ret;           //д������Ϣ����
	}
	
	//��ע,��ȱ��д���Ż�ѧ�ŵĽӿ�
	//д�ֿ���������Ϣ
	ret = SMT_WritePersonalInfo(pc->ucName,
								pc->ucDutyNo,
								pc->ucCertificateNo,
								pc->ucDepartmentNo,
								pc->ucIdentifyNo,
								pc->ucSexNo,
								pc->ucCardNo);
	if(ret)
	{
		SetErrMsg(ret,"д������Ϣʧ��");
		return ret;           //
	}
	ret=SMT_WriteLibraryNo(pc->ucLibraryNo);
	if(ret)
	{
		SetErrMsg(ret,"дͼ��֤��ʧ��");
		return ret;
	}
	ret=WriteCardPwd(pc->ucPwd);
	if(ret)
	{
		SetErrMsg(ret,"д������ʧ��");
		return ret;
	}
	//Ԥ����
	unsigned short int PacketNo = 1;
	int Money;
	Money = pc->Money;
	ret = SMT_PacketSetMoney(PacketNo,Money);
	if(ret)
	{
		SetErrMsg(ret,"����Ǯ�����ʧ��");
		return ret;		
	}
	SMT_ControlBuzzer();
	return 0;
}

int ifReadyOK()
{
//	if (isConnectMF) return isConnectMF;

	int ret = 0;

	//����д���Ƿ���Ȩ
	ret = SMT_JudgeAutoFlag();
	if(ret!=1)
	{
		SetErrMsg(ret,"������δ��Ȩ");
		return ret;
	}
	//����û����Ƿ��ڸ�Ӧ��
	char ucSerialNo[9];
	char ucType[3];

	ret = RequestCard(ucSerialNo,ucType);
	if(ret)
	{
		return ret;
	}

	return 0;
}

int __stdcall ReaderAuthor(unsigned char *ucAuthorInfo)
{
	int ret = 0;
	unsigned char ucAuthor[9];
//	if (isConnectMF) return isConnectMF;
	memset(ucAuthor,0,sizeof(ucAuthor));
	memcpy(ucAuthor,ucAuthorInfo,8);
	//����д����Ȩ
	ret = SMT_AutoDll(ucAuthor);
	if(ret)
	{
		SetErrMsg(ret,"��������Ȩʧ��");
		return ret;
	}
    // ����ȫ�ֱ���workingkey
	SMT_ControlBuzzer();
	return 0;
}

int __stdcall RequestCard(char *ucSerialNo,char *ucType)
{

	int ret = 0;
	unsigned char SerialNo[4],Type[1];
	memset(SerialNo,0,4);
	memset(Type,0,1);

//	if (isConnectMF) return isConnectMF;
	//����û����Ƿ��ڸ�Ӧ��
	ret = SMT_RequestCardExist(SerialNo,Type);
	if(ret)
	{
		SetErrMsg(ret,"�����ڶ�������Ӧ��");
		return ret;
	}
	sprintf(ucSerialNo,"%02X%02X%02X%02X",
		SerialNo[0],
		SerialNo[1],
		SerialNo[2],
		SerialNo[3]);
	sprintf(ucType,"%02X",Type[0]);
	return 0;
}

// ��������: ��ȡ����״̬��Ϣ, 
// ��������: ����ͬ��
int __stdcall SMT_ReadCardStateInfo(unsigned char ucShowCardNo[5],
								   int nDealCardNo[1],
								   unsigned char ucDeadLineDate[3],
								   unsigned char ucCardRightType[1])
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(1,0,ucBuffer);
	if(ret != 0)
		return -6; //����ʧ��
	nDealCardNo[0] = ucBuffer[0]*256*256+ucBuffer[1]*256+ucBuffer[2];
	ucCardRightType[0] = ucBuffer[3];
	memcpy(ucDeadLineDate,ucBuffer+4,3);
	memcpy(ucShowCardNo,ucBuffer+7,5);
	return 0;
}

int __stdcall ReadCardInfo(TPublishCard *pc)
{
	int ret = 0; 
	unsigned char ucBcdShowCardNo[5];
	unsigned char ucDeadLineDate[3];		//�û���ʹ�ý�ֹ����
	unsigned char ucCardRightType[1];		//Ȩ��λ
	int iCardNo[1];

//	if (isConnectMF) return isConnectMF;

	ret = ifReadyOK();
	if (ret !=0)
		return ret;


/*	memset(pc,0,sizeof(TPublishCard));*/  // ������������

	ret = SMT_ReadCardStateInfo(ucBcdShowCardNo,iCardNo,ucDeadLineDate,ucCardRightType);
	if (ret !=0)
	{
		SetErrMsg(ret,"����������Ϣʧ��");
		return ret;    
	}
	bcd2dec(ucBcdShowCardNo,5,pc->ShowCardNo);
	//ת��hex���ڸ�ʽ��ʮ����
	sprintf((char*)pc->DeadLineDate,"20%02d%02d%02d",ucDeadLineDate[0],ucDeadLineDate[1],ucDeadLineDate[2]);
	sprintf((char*)pc->CardNo,"%d",iCardNo[0]);
	pc->CardRightType=ucCardRightType[0];
	ret = SMT_ReadPersonalInfo(pc->ucName,pc->ucDutyNo,pc->ucCertificateNo,pc->ucDepartmentNo,pc->ucIdentifyNo,pc->ucSexNo,pc->ucCardNo);
	if (ret !=0)
	{
		SetErrMsg(ret,"����������Ϣʧ��");
		return ret;     
	}
	ret=SMT_ReadLibraryNo(pc->ucLibraryNo);
	if(ret)
	{
		SetErrMsg(ret,"дͼ��֤��ʧ��");
		return ret;
	}
	return 0;
}

//���¿���Ϣ
int __stdcall UpdateCardInfo (TPublishCard *CardInfo)
{
	int ret = 0; 
	unsigned char ucDeadLineDate[3];
	unsigned char ucCardRightType[1];
	unsigned char ucBcdShowCardNo[5];
	char sYear[3];
	char sMon[3];
	char sDay[3];
	
	ret = ifReadyOK();
	if (ret !=0)
		return ret;

	//��ע,��ȱ��д���Ż�ѧ�ŵĽӿ�
	//ת��Ϊ����
	memset(sYear,0,sizeof(sYear));
	memset(sMon,0,sizeof(sMon));
	memset(sDay,0,sizeof(sDay));
	memset(ucBcdShowCardNo,0,sizeof(ucBcdShowCardNo));

	memcpy(sYear,CardInfo->DeadLineDate+2,2);
	memcpy(sMon,CardInfo->DeadLineDate+4,2);
	memcpy(sDay,CardInfo->DeadLineDate+6,2);
	
	ucDeadLineDate[0]=atoi(sYear);
	ucDeadLineDate[1]=atoi(sMon);
	ucDeadLineDate[2]=atoi(sDay);

	ret = SMT_ChangeDeadLineDate(ucDeadLineDate);					//�޸�ʹ�ý�ֹ���ڣ�8���ֽ�����(������)��ʽΪ��"yy-mm-dd"��
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸Ľ�ֹ����ʱ����");
		return ret;
	}
	ucCardRightType[0]=CardInfo->CardRightType;
	ret = SMT_ChangeCardRightType(ucCardRightType);					//�޸�ʹ��Ȩ�����ͣ�1--254��	
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸Ŀ�Ȩ������ʱ����");
		return ret;
	}
	ret = SMT_ChangeName(CardInfo->ucName);							//�޸�����
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸�����ʱ����");
		return ret;	
	}
	ret = SMT_ChangeDutyNo(CardInfo->ucDutyNo);						//�޸�ְ�����
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸�ְ�����ʱ����");
		return ret;
	}
	ret = SMT_ChangeCerificateNo(CardInfo->ucCertificateNo);		//�޸�֤������
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸�֤������ʱ����");
		return ret;	
	}
	ret = SMT_ChangeDepartmentNo(CardInfo->ucDepartmentNo);			//�޸Ĳ��ű���
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸Ĳ��Ŵ���ʱ����");
		return ret;
	}
	ret = SMT_ChangeIdentifyNo(CardInfo->ucIdentifyNo);				//�޸����ݴ���
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸����ݴ���ʱ����");
		return ret;
	}
	ret = SMT_ChangeSexNo(CardInfo->ucSexNo);						//�޸��Ա����
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸��Ա�ʱ����");
		return ret;
	}
	ret = SMT_ChangeCardNo(CardInfo->ucCardNo);					//
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸�ѧ����ʱ����");
		return ret;
	}
	ret = SMT_WriteLibraryNo(CardInfo->ucLibraryNo);	//
	if(ret != 0)
	{
		SetErrMsg(ret,"�޸�ͼ��֤��ʱ����");
		return ret;
	}
	ret = strlen((char*)CardInfo->ShowCardNo);
	if( 10==ret )	
	{
		//ת��ΪBCD��
		dec2bcd(CardInfo->ShowCardNo,ucBcdShowCardNo,5);
		ret = SMT_ChangeShowCardNo(ucBcdShowCardNo);	//
		if(ret != 0)
		{
			SetErrMsg(ret,"�޸���ʾ����ʱ����");
			return ret;
		}
	}
	return 0;
}

int __stdcall ReadPacketInfo( LPLOOPPURSEINFO LoopPurseInfo )
{
	int ret = 0; 

//	if (isConnectMF) return isConnectMF;

	ret = ifReadyOK();
	if (ret !=0)
		return ret;

	int nPacketNo = 1;
	
	memset(LoopPurseInfo,0,sizeof(LOOPPURSEINFO));  //��ǰ�����ڴ淢�������ԭ����û��#pragma pack(1)

	ret = SMT_ReadPacketInfo(nPacketNo,LoopPurseInfo);
	if (ret !=0)
	{
		SetErrMsg(ret,"������Ϣʧ��");
		return ret;
	}
	return ret;
}

int __stdcall ReadCardNO(int nCardNo[1])
{
	int ret;
	unsigned char ucBuffer[16];	
	ret = Login_Card(1);
	if(ret != 0)	
		return ret;
	ret = SMT_ReadBlock(1,0,ucBuffer);
	if(ret != 0)	
		return ret;
	nCardNo[0] = ucBuffer[0]*256*256+ucBuffer[1]*256+ucBuffer[2];
	return 0;
}

int __stdcall WriteCardPwd(unsigned char pwd[6])
{
	int ret=0;
	unsigned char ucPwd[3];
	dec2bcd(pwd,ucPwd,3);
	ret=SMT_WritePersonalPassword(ucPwd);
	if(ret)
	{
		SetErrMsg(ret,"д������ʧ��");
		return ret;
	}
	return 0;
}

int __stdcall ReadCardPwd(unsigned char pwd[6])
{
	int ret=0;
	unsigned char ucPwd[3];
	ret=SMT_ReadPersonalPassword(ucPwd);
	if(ret)
	{
		SetErrMsg(ret,"��������ʧ��");
		return ret;
	}
	bcd2dec(ucPwd,3,pwd);
	return 0;
}

int __stdcall Load_Dll()
{
	if(hDLL)
		return 0;
//    hDLL = LoadLibrary("SmartCom411DXQ.dll");
	hDLL = LoadLibrary(bufSmartCom);
	if(!hDLL)
	{
		SetErrMsg(-1,"װ��SmartCom411DXQ.dll��̬��ʧ��");
		return -1;
	}

	SMT_ConnectMF280		 = (LPSMT_ConnectMF280)GetProcAddress(hDLL,"SMT_ConnectMF280");
	SMT_ControlBuzzer        = (LPSMT_ControlBuzzer)GetProcAddress(hDLL,"SMT_ControlBuzzer");
	SMT_CloseMF280           = (LPSMT_CloseMF280)GetProcAddress(hDLL,"SMT_CloseMF280");
//	SMT_CreateGreyCard       = (LPSMT_CreateGreyCard)GetProcAddress(hDLL,"SMT_CreateGreyCard");
//	SMT_WriteCardStateInfo   = (LPSMT_WriteCardStateInfo)GetProcAddress(hDLL,"SMT_WriteCardStateInfo");
//	SMT_PacketAddMomey       = (LPSMT_PacketAddMomey)GetProcAddress(hDLL,"SMT_PacketAddMomey");
//	SMT_PacketSetMoney       = (LPSMT_PacketSetMoney)GetProcAddress(hDLL,"SMT_PacketSetMoney");
// 	SMT_AutoDll				 = (LPSMT_AutoDll)GetProcAddress(hDLL,"SMT_AutoDll");
// 	SMT_JudgeAutoFlag  	     = (LPSMT_JudgeAutoFlag)GetProcAddress(hDLL,"SMT_JudgeAutoFlag");
    SMT_RequestCardExist  	 = (LPSMT_RequestCardExist)GetProcAddress(hDLL,"SMT_RequestCardExist");
//	SMT_ReadCardStateInfo  	 = (LPSMT_ReadCardStateInfo)GetProcAddress(hDLL,"SMT_ReadCardStateInfo");
//  SMT_WritePersonalInfo  	 = (LPSMT_WritePersonalInfo)GetProcAddress(hDLL,"SMT_WritePersonalInfo");
//	SMT_ReadPersonalInfo     = (LPSMT_ReadPersonalInfo)GetProcAddress(hDLL,"SMT_ReadPersonalInfo");
//	SMT_ChangeName           = (LPSMT_ChangeName)GetProcAddress(hDLL,"SMT_ChangeName");
//	SMT_ChangeDutyNo         = (LPSMT_ChangeDutyNo)GetProcAddress(hDLL,"SMT_ChangeDutyNo");
//	SMT_ChangeCerificateNo   = (LPSMT_ChangeCerificateNo)GetProcAddress(hDLL,"SMT_ChangeCerificateNo");
//	SMT_ChangeDepartmentNo   = (LPSMT_ChangeDepartmentNo)GetProcAddress(hDLL,"SMT_ChangeDepartmentNo");
//	SMT_ChangeIdentifyNo     = (LPSMT_ChangeIdentifyNo)GetProcAddress(hDLL,"SMT_ChangeIdentifyNo");
//	SMT_ChangeSexNo          = (LPSMT_ChangeSexNo)GetProcAddress(hDLL,"SMT_ChangeSexNo");
//  SMT_ReadPacketInfo       = (LPSMT_ReadPacketInfo)GetProcAddress(hDLL,"SMT_ReadPacketInfo");
//	SMT_ChangeDeadLineDate   = (LPSMT_ChangeDeadLineDate)GetProcAddress(hDLL,"SMT_ChangeDeadLineDate");
//	SMT_ChangeCardRightType  = (LPSMT_ChangeCardRightType)GetProcAddress(hDLL,"SMT_ChangeCardRightType");
//	SMT_WritePersonalPassword   = (LPSMT_ChangeDeadLineDate)GetProcAddress(hDLL,"SMT_WritePersonalPassword");
//	SMT_ReadPersonalPassword  = (LPSMT_ChangeCardRightType)GetProcAddress(hDLL,"SMT_ReadPersonalPassword");
	SMT_SleepCard            = (LPSMT_SleepCard)GetProcAddress(hDLL,"SMT_SleepCard");
	MF_ReadSeriesNo          = (LPMF_ReadSeriesNo)GetProcAddress(hDLL, "MF_ReadSeriesNo");
	SMT_Login_With_UserKey   = (LPSMT_Login_With_UserKey)GetProcAddress(hDLL, "SMT_Login_With_UserKey");
	SMT_WriteBlock           = (LPSMT_WriteBlock)GetProcAddress(hDLL, "SMT_WriteBlock");
	SMT_ReadBlock            = (LPSMT_ReadBlock)GetProcAddress(hDLL, "SMT_ReadBlock");
    SetLastErrMsg            = (LPSetLastErrMsg)GetProcAddress(hDLL, "SetLastErrMsg");
	GetMFLastErr             = (LPGetMFLastErr)GetProcAddress(hDLL, "GetMFLastErr");
//	SMT_ClearCard			 = (LPSMT_ClearCard)GetProcAddress(hDLL,"SMT_ClearCard");
//	SMT_WriteLibraryNo		 = (LPSMT_WriteLibraryNo)GetProcAddress(hDLL,"SMT_WriteLibraryNo");
//	SMT_ReadLibraryNo		 = (LPSMT_ReadLibraryNo)GetProcAddress(hDLL,"SMT_ReadLibraryNo");
//	SMT_ChangeCardNo		 = (LPSMT_ChangeCardNo)GetProcAddress(hDLL,"SMT_ChangeCardNo");
//	SMT_ChangeShowCardNo	 = (LPSMT_ChangeShowCardNo)GetProcAddress(hDLL,"SMT_ChangeShowCardNo");
	if ((!SMT_ConnectMF280) ||
		(!SMT_ControlBuzzer) ||
		(!SMT_CloseMF280) ||
//		(!SMT_CreateGreyCard) ||
	//	(!SMT_WriteCardStateInfo) ||
        (!SMT_WriteBlock)  ||
		(!SMT_ReadBlock)   ||
		(!MF_ReadSeriesNo) ||
//		(!SMT_PacketAddMomey) ||
//		(!SMT_PacketSetMoney) ||
//		(!SMT_AutoDll) ||
//		(!SMT_JudgeAutoFlag) ||
		(!SMT_RequestCardExist) ||
//		(!SMT_ReadCardStateInfo) ||
//		(!SMT_WritePersonalInfo) ||
//		(!SMT_ReadPersonalInfo) ||
//		(!SMT_ChangeName) ||
//		(!SMT_ChangeDutyNo) ||
//		(!SMT_ChangeCerificateNo) ||
//		(!SMT_ChangeDepartmentNo) ||
//		(!SMT_ChangeIdentifyNo) ||
//		(!SMT_ChangeSexNo) ||
//		(!SMT_ReadPacketInfo) ||
//		(!SMT_ChangeDeadLineDate) ||
//		(!SMT_ChangeCardRightType) ||
//		(!SMT_WritePersonalPassword)||
//		(!SMT_ReadPersonalPassword)||
		(!SMT_Login_With_UserKey) ||
		(!SMT_SleepCard)||
		(!SetLastErrMsg)||
		(!GetMFLastErr) 
//		(!SMT_ClearCard)
//		(!SMT_WriteLibraryNo)||
//		(!SMT_ReadLibraryNo)
//		(!SMT_ChangeCardNo)
//		(!SMT_ChangeShowCardNo)
		)
	{
		FreeLibrary(hDLL);
		hDLL = NULL;
		return -2;
	}
//////////////////////////////////////////////////////////////////////////
// ����dessdll.dll��	
	if (hDesDLL)
	{
		return 0;
	}
//	hDesDLL = LoadLibrary("desdll.dll");
	hDesDLL = LoadLibrary(bufDesdll);
	if(!hDesDLL)
	{
		SetErrMsg(-1,"װ��desdll.dll��̬��ʧ��");
		return -1;
	}
	gen_user_card_keyA = (lpgen_user_card_keyA)GetProcAddress(hDesDLL,"gen_user_card_keyA");
	gen_user_card_keyB = (lpgen_user_card_keyB)GetProcAddress(hDesDLL,"gen_user_card_keyB");
	encrypt_work_key   = (lpencrypt_work_key)GetProcAddress(hDesDLL, "encrypt_work_key");
	gen_auth_card_keyA = (lpgen_auth_card_keyA)GetProcAddress(hDesDLL,"gen_auth_card_keyA");
	gen_auth_card_keyB = (lpgen_auth_card_keyB)GetProcAddress(hDesDLL,"gen_auth_card_keyB");
	decrypt_work_key = (lpdecrypt_work_key)GetProcAddress(hDesDLL,"decrypt_work_key");
	get_check_char = (lpget_check_char)GetProcAddress(hDesDLL,"get_check_char");
	if (
		(!gen_user_card_keyA) || 
		(!gen_user_card_keyB) ||
		(!encrypt_work_key)   ||
		(!gen_auth_card_keyA) ||
		(!gen_auth_card_keyB) ||
		(!decrypt_work_key)	  ||	
		(!get_check_char)     
		)
	{
		FreeLibrary(hDesDLL);
		hDesDLL = NULL;
		return -2;
	}
	return 0;
}
/************************************************************************/
/*           �����ӵĺ����� �������ܺ���, ��Դ��SMARTCOM411DXQ          */
/************************************************************************/

// ascWorkKey����������Կ
// ��������: ������֤ 
// �����´�½���������豸, ���Ų���
int  _stdcall Load_WorkKey(char *ascWorkKey)
{
	//����������Կ
	int ret=0;
	int i=0;
	unsigned char ucCryptWorkKey[17];
//  memset(ucWorkKey,0,sizeof(ucWorkKey));
	memset(SMT_UCWORKINGKEY, 0, sizeof(SMT_UCWORKINGKEY));
	hex2dec(ascWorkKey, ucCryptWorkKey, 8);
//	en_key(ucCryptWorkKey,ucWorkKey);
//  ��һ��dessdll.dll������㷨.......................
// 	int j = 0;
// 	char temp[3] = "";
// 	for(i = 0;i < 8;++i)
// 	{
// 		strncpy(temp, (const char *)ascWorkKey[i*2], 2);
// 		ucCryptWorkKey[i] = strtoul(temp, NULL, 16);
// 	}
	encrypt_work_key(ucCryptWorkKey, SMT_UCWORKINGKEY);
	nAutoFlag = 1;										// ���������ӵĶ���
	return 0;
}

// ��������: �жϱ�־
// ��������: ��
int __stdcall SMT_JudgeAutoFlag()
//����ֵ��     0��δ��Ȩ     1����Ȩ
{
	return nAutoFlag;
}

// ��������: ������֤
// ��������: uckWorkingKey������Կ 
int __stdcall SMT_AutoDll(unsigned char *ucWorkingKey)
{
	int i;
	memcpy(SMT_UCWORKINGKEY, ucWorkingKey, 8);
	for(i=0;i<8;i++)
		SMT_UCWORKINGKEY[i+8] = ~ucWorkingKey[i];
	has_read_main_key = TRUE;
	nAutoFlag = 1;
	return 0;
}

// ��������: ��ȡ���к�
// ��������: ���к�����
int __stdcall ReadSerial(unsigned char *serial)
{
	return MF_ReadSeriesNo(serial);
}

// ��������: �����㿨
// ��������: bPersonalFlag(������Ϣ��־), bStateFlag(״̬��־), bBankInfo(�洢��Ϣ), bPacketFlag(����־)
int __stdcall SMT_CreateGreyCard(BOOL bPersonalFlag,BOOL bStateFlag, BOOL bBankInfo,BOOL bPacketFlag[8])
{
	unsigned char ucSerialNo[5];
	unsigned char ucKeyA[6],ucKeyB[6];
	unsigned char ucRawData[256],ucTmpRawData[16],ucDesData[256];
	
	int i,j,startSector=4;
	int ret;
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret <= 0)
		return -1;//��Ƭ���ڸ�Ӧ��
	//1��ʹ�ó��������½��Ƭ  ǰ32������
	memset(ucKeyA,0xFF,6);
	memset(ucKeyB,0xFF,6);
	memset(ucRawData,0x00,16);
	for(i=0;i<32;i++)
	{
		ret = SMT_Login_With_UserKey(i,ucKeyA,0);
		if(ret != 0)
			return -2; //�ǳ�����
//		ret = SMT_Login_With_UserKey(i,ucKeyB,1);
//		if(ret != 0)
//			return -3; //�ǳ�����
		for(j=0;j<3;j++)
		{
			if(i==0&&j==0)
				continue;
			ret=SMT_WriteBlock(i,j,ucRawData);
			if(ret)
				return -4;
		}
	}
	//2������KEYA��KEYB����Ǯ������
	//1 --- ����KEYA
	//////////////////////////////////////////////////////
	// void SMT_Cal_KeyA(unsigned char *ucRawData, unsigned char *ucSerialNo, unsigned char *ucDesData, unsigned char *ucKeyA) 
//	SMT_Cal_KeyA(ucRawData, ucSerialNo, ucDesData, ucKeyA);
	SMT_Cal_KeyA(SMT_UCWORKINGKEY, ucSerialNo, ucKeyA);
// 	memcpy(ucRawData,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i+4] = ~ucRawData[i];
// 	des_code(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_KeyA(ucDesData,ucKeyA);
	//////////////////////////////////////////////////////
//	Cal_Key(ucDesData, ucKeyA);
	//����KEYB
	//////////////////////////////////////////////////////
	// void SMT_Cal_KeyB(unsigned char *ucRawData, unsigned char *ucSerialNo, unsigned char *ucDesData, unsigned char *ucKeyB)
	SMT_Cal_KeyB(SMT_UCWORKINGKEY, ucSerialNo, ucKeyB);
// 	memcpy(ucRawData+4,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i] = ~ucRawData[i+4];
// 	TriDes(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_Key(ucDesData, ucKeyB);
	///////////////////////////////////////////////////////
	memcpy(ucRawData,ucKeyA,6);
	ucRawData[6] = 0x7F;
	ucRawData[7] = 0x07;
	ucRawData[8] = 0x88;
	ucRawData[9] = 0x00;
	memcpy(ucRawData+10,ucKeyB,6);
	memset(ucDesData,0xFF,6);
	for(i=0;i<32;i++)
	{
//		ret = SMT_Login_With_UserKey(i,ucDesData,1);
		ret = SMT_Login_With_UserKey(i,ucDesData,0);
		if(ret != 0)
			return -5; //�ǳ�����
		memcpy(ucTmpRawData,ucRawData,16);
		ret = SMT_WriteBlock(i,3,ucTmpRawData);
		if(ret != 0)
			return -6; //��Ƭ����ʧ��
		ret = SMT_Login_With_UserKey(i,ucKeyA,0);
		if(ret)
			return -11;
	}
	//����Ǯ���� ��0��
	j = 0;
	for(i=0;i<8;i++)
	{
		if(bPacketFlag[i] == TRUE)
			ucRawData[j] = 0x00;
		else
			ucRawData[j] = 0xFF;
		j = j + 1;
		if(bPacketFlag[i] == TRUE)
		{
			ucRawData[j] = startSector;
			startSector+=1;
		}
		else
			ucRawData[j] = 0xFF;
		j=j+1;
	}
	//1 ʹ���û������½
	ret = SMT_Login_With_UserKey(3,ucKeyA,0);
	if(ret != 0)
		return -7;//ʹ���û�����KEYA���ܵ�½
	ret = SMT_WriteBlock(3,0,ucRawData);
	if(ret != 0)
		return -8;//�޷�д��
	memset(ucRawData,0xFF,16);
	for(i=0;i<8;i++)
	{
		if(bPacketFlag[i] == TRUE)
		{
			ucRawData[i] = startSector;
			startSector += 1;
		}
		else
			ucRawData[i] = 0xFF;
	}
	ret = SMT_WriteBlock(3,1,ucRawData);
	if(ret != 0)
		return -9;//�޷�д��
	memset(ucRawData,0xFF,16);  //20050305����
	ret = SMT_WriteBlock(3,2,ucRawData);
	if(ret != 0)
		return -10;//�޷�д��
	return 0;
}

// ��������: д�뿨��״̬��Ϣ 
// ��������: ucShowCardNo(�û���ʾ����), nDealCardNo(�û����׿���), 
// ucDeadLineDate(���Ľ�ֹ����), ucCardRightType(�û���ʹ������Ȩ��)
int __stdcall SMT_WriteCardStateInfo(unsigned char ucShowCardNo[5], 
						   int nDealCardNo[1], 
						   unsigned char ucDeadLineDate[3], 
						   unsigned char ucCardRightType[1])
{
	unsigned char ucSerialNo[5],ucRawData[256],ucKeyA[6];// ucDesData[256]
	int ret;
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
		return -1;//��Ƭ���ڸ�Ӧ��
	//1 --- ����KEYA
// 	memcpy(ucRawData,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i+4] = ~ucRawData[i];
// 	des_code(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_KeyA(ucDesData,ucKeyA);
//	Cal_Key(ucDesData, ucKeyA);
//	SMT_Cal_KeyA(ucRawData, ucSerialNo, ucDesData, ucKeyA);
	SMT_Cal_KeyA(SMT_UCWORKINGKEY, ucSerialNo, ucKeyA);
	ret = SMT_Login_With_UserKey(1, ucKeyA, 0);
	if(ret != 0)
		return -2;//��Ƭ��½ʧ��
	//��0��������֯
	if(nDealCardNo[0] > 0x0FFFFF)
		return -3;//���׿��Ź���
	memset(ucRawData,0xFF,16);
	ucRawData[0] = nDealCardNo[0] / 256 / 256;
	ucRawData[1] = nDealCardNo[0] / 256;
	ucRawData[2] = nDealCardNo[0] % 256;
	ucRawData[3] = ucCardRightType[0];
	memcpy(ucRawData + 4, ucDeadLineDate, 3);
	memcpy(ucRawData + 7, ucShowCardNo, 5);
	ret = SMT_WriteBlock(1, 0, ucRawData);
	if(ret != 0)
		return -5; //д��ʧ��
	return 0;				
}

// ��������: �޸��û�����
// ��������: �û����� 
int __stdcall SMT_ChangeShowCardNo(unsigned char ucShowCardNo[5])
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(1,0,ucBuffer);
	if(ret != 0)
		return -6; //����ʧ��
	memcpy(ucBuffer+7,ucShowCardNo,5);
	ret = SMT_WriteBlock(1,0,ucBuffer);
	if(ret != 0)
		return -5; //д��ʧ��
	return 0;
}

// ��������: �޸Ľ��׿���
// ��������: ���׿���
int __stdcall SMT_ChangeDealCardNo(int nDealCardNo)
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(1, 0, ucBuffer);
	if(ret != 0)
		return -6; //����ʧ��
	ucBuffer[0] = nDealCardNo/256/256;
	ucBuffer[1] = nDealCardNo/256;
	ucBuffer[2] = nDealCardNo%256;
	ret = SMT_WriteBlock(1,0,ucBuffer);
	if(ret != 0)
		return -5; //д��ʧ��
	return 0;
}

// ��������: �޸Ŀ�Ƭʹ�ý�������
// ��������: ��ֹ����ʱ��
int __stdcall SMT_ChangeDeadLineDate(unsigned char ucDeadLineDate[3])
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(1, 0, ucBuffer);
	if(ret != 0)
		return -6; //����ʧ��
	memcpy(ucBuffer + 4, ucDeadLineDate, 3);
	ret = SMT_WriteBlock(1, 0, ucBuffer);
	if(ret != 0)
		return -5; //д��ʧ��
	return 0;
}

// ��������: �޸Ŀ�ƬȨ������
// ��������: ��ƬȨ������
int __stdcall SMT_ChangeCardRightType (unsigned char ucCardRightType[1])
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(1,0,ucBuffer);
	if(ret != 0)
		return -6; //����ʧ��
	memcpy(ucBuffer+3,ucCardRightType,1);
	ret = SMT_WriteBlock(1,0,ucBuffer);
	if(ret != 0)
		return -5; //д��ʧ��
	return 0;
}

// �������ã�д�뿨Ƭ������Ϣ
// ��������: ucName(����), ucDutyNo(ְ�����), ucCertificateNo(֤������)
// ucDepartmentNo(���ű��), ucIdentifyNo(���ݴ���), ucSexNo(�Ա�), ucCardNo(����)
int __stdcall SMT_WritePersonalInfo(unsigned char ucName[8],
									unsigned char ucDutyNo[4],
									unsigned char ucCertificateNo[20],
									unsigned char ucDepartmentNo[10],
									unsigned char ucIdentifyNo[4],
									unsigned char ucSexNo[1],
									unsigned char ucCardNo[20])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(0);
	if(ret != 0)	return ret;
	memcpy(ucBuffer,ucCardNo,16);
	ret = SMT_WriteBlock(0,1,ucBuffer);
	if(ret != 0)	return -5;
	memset(ucBuffer,0xFF,16);
	memcpy(ucBuffer,ucCardNo+16,4);
	ret = SMT_WriteBlock(0,2,ucBuffer);
	if(ret != 0)	return -5;
	//��2����0�����
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	memcpy(ucBuffer,ucName,8);
	memcpy(ucBuffer+8,ucDutyNo,4);
	memcpy(ucBuffer+12,ucCertificateNo,4);
	ret = SMT_WriteBlock(2,0,ucBuffer);
	if(ret != 0)	return -5;
	memcpy(ucBuffer,ucCertificateNo+4,16);
	ret = SMT_WriteBlock(2,1,ucBuffer);
	if(ret != 0)	return -5;
	memcpy(ucBuffer,ucDepartmentNo,10);
	memcpy(ucBuffer+10,ucIdentifyNo,4);
	memcpy(ucBuffer+14,ucSexNo,1);
	ret = SMT_WriteBlock(2,2,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: ��ȡ��Ƭ������Ϣ
// ��������: ������ʾ
int __stdcall SMT_ReadPersonalInfo(unsigned char ucName[8],
								   unsigned char ucDutyNo[4],
								   unsigned char ucCertificateNo[20],
								   unsigned char ucDepartmentNo[10],
								   unsigned char ucIdentifyNo[4],
								   unsigned char ucSexNo[1],
								   unsigned char ucCardNo[20])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(0);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(0,1,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucCardNo,ucBuffer,16);
	ret = SMT_ReadBlock(0,2,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucCardNo+16,ucBuffer,4);

	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,0,ucBuffer);
	if(ret != 0 )	return -6;
	memcpy(ucName,ucBuffer,8);
	memcpy(ucDutyNo,ucBuffer+8,4);
	memcpy(ucCertificateNo,ucBuffer+12,4);
	ret = SMT_ReadBlock(2,1,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucCertificateNo+4,ucBuffer,16);
	ret = SMT_ReadBlock(2,2,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucDepartmentNo,ucBuffer,10);
	memcpy(ucIdentifyNo,ucBuffer+10,4);
	memcpy(ucSexNo,ucBuffer+14,1);
	return 0;
}

// ��������: �޸��û�����
// ��������: �û���
int __stdcall SMT_ChangeName(unsigned char ucName[8])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,0,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucBuffer,ucName,8);
	ret = SMT_WriteBlock(2,0,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: �޸�ְ�����
// ��������: ְ����
int __stdcall SMT_ChangeDutyNo(unsigned char ucDutyNo[4])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,0,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucBuffer+8,ucDutyNo,4);
	ret = SMT_WriteBlock(2,0,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// �������ã��޸�֤������
// ��������: ֤����
int __stdcall SMT_ChangeCerificateNo (unsigned char ucCertificateNo[20])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,0,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucBuffer+12,ucCertificateNo,4);
	ret = SMT_WriteBlock(2,0,ucBuffer);
	if(ret != 0)	return -5;
	memcpy(ucBuffer,ucCertificateNo+4,16);
	ret = SMT_WriteBlock(2,1,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: �޸Ĳ��ű��
// ��������: ���ű��
int __stdcall SMT_ChangeDepartmentNo(unsigned char ucDepartmentNo[10])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,2,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucBuffer,ucDepartmentNo,10);
	ret = SMT_WriteBlock(2,2,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: �޸����ݴ���
// ��������: ���ݴ���
int __stdcall SMT_ChangeIdentifyNo (unsigned char ucIdentifyNo[4])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,2,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucBuffer+10,ucIdentifyNo,4);
	ret = SMT_WriteBlock(2,2,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: �޸��Ա����
// ��������: �Ա����
int __stdcall SMT_ChangeSexNo (unsigned char ucSexNo[1])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(2);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(2,2,ucBuffer);
	if(ret != 0)	return -6;
	memcpy(ucBuffer+14,ucSexNo,1);
	ret = SMT_WriteBlock(2,2,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: �޸�(ѧ����)
// ��������: ѧ����
int __stdcall SMT_ChangeCardNo(unsigned char ucCardNo[20])
{
	int ret;
	unsigned char ucBuffer[16];
	//����ѧ�Ų���
	ret = Login_Card(0);
	if(ret != 0)	return ret;
	memcpy(ucBuffer,ucCardNo,16);
	ret = SMT_WriteBlock(0,1,ucBuffer);
	if(ret != 0)	return -6;
	memset(ucBuffer,0xFF,16);
	memcpy(ucBuffer,ucCardNo+16,4);
	ret = SMT_WriteBlock(0,2,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: д���������
// ��������: ��������
int __stdcall SMT_WritePersonalPassword(unsigned char PersonalPsd[3])
{
	int ret;
	unsigned char ucBuffer[16];
	
	ret = Login_Card(1);
	if(ret != 0)	
		return ret;
	ret = SMT_ReadBlock(1,1,ucBuffer);
	if(ret != 0)	
		return ret;
	memcpy(ucBuffer,PersonalPsd,3);
	ret = SMT_WriteBlock(1,1,ucBuffer);
	if(ret != 0)	
		return ret;
	return 0;
}

// ��������: ��ȡ��������
// ��������: ��������(3λ)
int __stdcall SMT_ReadPersonalPassword(unsigned char PersonalPsd[3])
{
	int ret;
	unsigned char ucBuffer[16];
	
	ret = Login_Card(1);
	if(ret != 0)	
		return ret;
	//memset(ucBuffer,0xFF,16);
	ret = SMT_ReadBlock(1,1,ucBuffer);
	if(ret != 0)	
		return -5;
	memcpy(PersonalPsd,ucBuffer,3);
	return 0;
}

// ��������: дͼ��֤��
// ��������: ͼ��֤��
int __stdcall SMT_WriteLibraryNo(unsigned char ucLibraryNo[13])
{
	int ret;
	unsigned char ucBuffer[16];
	
	ret = Login_Card(1);
	if(ret != 0)	
		return ret;
	ret = SMT_ReadBlock(1,1,ucBuffer);
	if(ret != 0)	
		return ret;
	memcpy(ucBuffer+3,ucLibraryNo,13);
	ret = SMT_WriteBlock(1,1,ucBuffer);
	if(ret != 0)	
		return ret;
	return 0;
}

// ��������: ��ͼ��֤��
// ��������: ͼ��֤��
int __stdcall SMT_ReadLibraryNo(unsigned char ucLibraryNo[13])
{
	int ret;
	unsigned char ucBuffer[16];
	
	ret = Login_Card(1);
	if(ret != 0)	
		return ret;
	//memset(ucBuffer,0xFF,16);
	ret = SMT_ReadBlock(1,1,ucBuffer);
	if(ret != 0)	
		return -5;
	memcpy(ucLibraryNo,ucBuffer+3,13);
	return 0;
}

// ��������: д���п���
// ��������: ���п���
int __stdcall SMT_WriteBankCardNo(unsigned char ucBankCardNo[10])
{
	int ret;
	unsigned char ucBuffer[16];
	
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	memset(ucBuffer,0xFF,16);
	memcpy(ucBuffer,ucBankCardNo,10);
	ret = SMT_WriteBlock(1,2,ucBuffer);
	if(ret != 0)	return -5;
	return 0;
}

// ��������: �����п���
// ��������: ���п���
int __stdcall SMT_ReadBankCardNo(unsigned char  ucBankCardNo[10])
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(1);
	if(ret != 0)	return ret;
	memset(ucBuffer,0xFF,16);
	ret = SMT_ReadBlock(1,2,ucBuffer);
	if(ret != 0)	return -5;
	memcpy(ucBankCardNo,ucBuffer,10);
	return 0;
}

// ��������: ��Ǯ����Ϣ
// ��������: nPacketNo(Ǯ����), LoopPurseInfo(����Ǯ����Ϣ)
int __stdcall SMT_ReadPacketInfo(int nPacketNo, LOOPPURSEINFO *LoopPurseInfo)   
{
	int ret,sector;
	unsigned char ucBuffer[16];
	if(nPacketNo<1)
		return -1;
	ret = Login_Card(3);
	if(ret != 0)	
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��¼3����ʧ��",__FILE__,__LINE__,ret);
		return ret;
	}
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��3����ʧ��",__FILE__,__LINE__,ret);	
		return -5;
	}
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
	{
		SetLastErrMsg(1,"File:%s\nLine:%d\nError:%d\nMessage:Ǯ��%dδ��ͨ",__FILE__,__LINE__,ret,nPacketNo);	
		return 1; //Ǯ��������ʹ��
	}
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��½��%d����ʧ��",__FILE__,__LINE__,ret,sector);	
		return ret;
	}

	unsigned char ucBuf1[16],ucBuf2[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d����ʧ��",__FILE__,__LINE__,ret,sector);	
		return -5;
	}
	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		LoopPurseInfo->RemainMoney = ucBuf1[0]+ucBuf1[1]*256+ucBuf1[2]*256*256;
		LoopPurseInfo->DealTimes   = ucBuf1[3]*256+ucBuf1[4];
		LoopPurseInfo->DealYear    = ucBuf1[5];
		LoopPurseInfo->DealMonth   = ucBuf1[6];
		LoopPurseInfo->DealDay     = ucBuf1[7];
		LoopPurseInfo->DealHour    = ucBuf1[8];
		LoopPurseInfo->DealMin     = ucBuf1[9];
		LoopPurseInfo->DealTimes_CurTime = ucBuf1[0x0D];
		LoopPurseInfo->DealTimes_CurDay  = ucBuf1[0X0E];
		return 0;
	}
	else
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d����У��ʹ���,��ʼ����������",__FILE__,__LINE__,0,sector);	
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)
		{
			SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d����ʧ��",__FILE__,__LINE__,ret,sector);	
			return -5;
		}
		ucCheck = 0;
		for(i=0;i<15;i++)
			ucCheck+=ucBuf2[i];
		if(ucCheck == ucBuf2[15])
		{
			LoopPurseInfo->RemainMoney = ucBuf2[0]+ucBuf2[1]*256+ucBuf2[2]*256*256;
			LoopPurseInfo->DealTimes   = ucBuf2[3]*256+ucBuf2[4];
			LoopPurseInfo->DealYear    = ucBuf2[5];
			LoopPurseInfo->DealMonth   = ucBuf2[6];
			LoopPurseInfo->DealDay     = ucBuf2[7];
			LoopPurseInfo->DealHour    = ucBuf2[8];
			LoopPurseInfo->DealMin     = ucBuf2[9];
			LoopPurseInfo->DealTimes_CurTime = ucBuf2[0x0D];
			LoopPurseInfo->DealTimes_CurDay  = ucBuf2[0X0E];
			return 0;
		}
		else
		{
			SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d�������ݿ�У��ʹ���",__FILE__,__LINE__,ret,sector);	
			return 2;
		}
	}
	return 2;
}

// ��������: Ǯ������Ǯ
// ��������: nPacketNo(Ǯ����), nMoney(Ǯ����)��
// ˵��: ��������б䶯, ��ʱ���滻��Ϊ��API����ʽ
int __stdcall SMT_PacketAddMomey(int nPacketNo,int nMoney)
{
	int ret,sector;
	if(nPacketNo<1)
		return -1;
	if(nMoney<0)
		return -1;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)	return -5;
	
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
		return 1; //Ǯ��������ʹ��
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
		return ret;
	unsigned char ucBuf1[16],ucBuf2[16],ucBufTmp[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)	
		return -6;
	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		memcpy(ucBufTmp,ucBuf1,sizeof(ucBufTmp));
		ret = SMT_WriteBlock(sector,2,ucBufTmp);
		if(ret != 0)	
			return -8;
		i = ucBuf1[0]+ucBuf1[1]*256+ucBuf1[2]*256*256;
		i = i + nMoney;
		ucBuf1[0] = i%256;
		ucBuf1[1] = i/256;
		ucBuf1[2] = i/256/256;
		i = ucBuf1[3]*256+ucBuf1[4];
		i = i + 1;
		ucBuf1[3] = i/256;
		ucBuf1[4] = i%256;
		SYSTEMTIME tm;
		GetLocalTime(&tm);
//		CTime tm = CTime::GetCurrentTime();                               �����޸Ĺ�
		//����ϴν��������뵱ǰʱ�䲻ͬ������ۼ����ѽ������
		if (tm.wDay != ucBuf1[7])                                   // (tm.GetDay()!=ucBuf1[7])
		{
			ucBuf1[10] = 0;
			ucBuf1[11] = 0;
			ucBuf1[12] = 0;		
		}
		ucBuf1[5] = (BYTE)(tm.wYear - 2000); // tm.GetYear()-2000;
		ucBuf1[6] = (BYTE)tm.wMonth;       // tm.GetMonth();
		ucBuf1[7] = (BYTE)tm.wDay;         // tm.GetDay();
		ucBuf1[8] = (BYTE)tm.wHour;        // tm.GetHour();
		ucBuf1[9] = (BYTE)tm.wMinute;      // tm.GetMinute();
		ucBuf1[15]=0;	
		for(i=0;i<15;i++)	
			ucBuf1[15] += ucBuf1[i];
		ret = SMT_WriteBlock(sector,0,ucBuf1);
		if(ret != 0)	
			return -8;
		return 0;
	}
	else
	{
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)	
			return -9;
		ucCheck = 0;
		for(i=0;i<15;i++)	
			ucCheck+=ucBuf2[i];
		if(ucCheck != ucBuf2[15])	
			return 2;
		i = ucBuf2[0]+ucBuf2[1]*256+ucBuf2[2]*256*256;
		i = i + nMoney;
		ucBuf2[0] = i%256;
		ucBuf2[1] = i/256;
		ucBuf2[2] = i/256/256;
		i = ucBuf2[3]*256+ucBuf2[4];
		i = i + 1;
		ucBuf2[3] = i/256;
		ucBuf2[4] = i%256;
//		CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//����ϴν��������뵱ǰʱ�䲻ͬ������ۼ����ѽ������
		if (tm.wDay != ucBuf2[7])                                  // (tm.GetDay()!=ucBuf2[7])
		{
			ucBuf2[10] = 0;
			ucBuf2[11] = 0;
			ucBuf2[12] = 0;		
		}
		ucBuf2[5] = (BYTE)(tm.wYear - 2000); // tm.GetYear()-2000;
		ucBuf2[6] = (BYTE)tm.wMonth;       // tm.GetMonth();
		ucBuf2[7] = (BYTE)tm.wDay;         // tm.GetDay();
		ucBuf2[8] = (BYTE)tm.wHour;       // tm.GetHour();
		ucBuf2[9] = (BYTE)tm.wMinute;      // tm.GetMinute();
		ucBuf2[15]=0;	
		for(i=0;i<15;i++)	
			ucBuf2[15] += ucBuf2[i];
		ret = SMT_WriteBlock(sector,0,ucBuf2);
		if(ret != 0)	
			return -12;
	}
	return 0;
}

// ��������: Ǯ����Ǯ
// ��������: ������ʾ
int __stdcall SMT_PacketDelMomey(int nPacketNo,int nMoney)
{
	int ret,sector;
	unsigned char ucBuffer[16];
	if(nPacketNo<1)
		return -1;
	if(nMoney<0)
		return -1;
	ret = Login_Card(3);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)	return -5;
	
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
		return 1; //Ǯ��������ʹ��
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
		return ret;
	unsigned char ucBuf1[16],ucBuf2[16],ucBufTmp[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)	
		return -5;
	int i;
	
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		memcpy(ucBufTmp,ucBuf1,sizeof(ucBufTmp));
		ret = SMT_WriteBlock(sector,2,ucBufTmp);
		if(ret != 0)	
			return -8;
		i = ucBuf1[0]+ucBuf1[1]*256+ucBuf1[2]*256*256;
		if(i<nMoney)	
			return 3;
		i = i - nMoney;
		ucBuf1[0] = i%256;
		ucBuf1[1] = i/256;
		ucBuf1[2] = i/256/256;
		i = ucBuf1[3]*256+ucBuf1[4];
		i = i + 1;
		ucBuf1[3] = i/256;
		ucBuf1[4] = i%256;
//		CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//����ϴν��������뵱ǰʱ�䲻ͬ������ۼ����ѽ������
		if (tm.wDay != ucBuf1[7])                                      //(tm.GetDay()!=ucBuf1[7])
		{
			ucBuf1[10] = 0;
			ucBuf1[11] = 0;
			ucBuf1[12] = 0;		
		}
		ucBuf1[5] = (BYTE)(tm.wYear - 2000);                           // tm.GetYear()-2000;
		ucBuf1[6] = (BYTE)(tm.wMonth);                                 // tm.GetMonth();
		ucBuf1[7] = (BYTE)(tm.wDay);                                   // tm.GetDay();
		ucBuf1[8] = (BYTE)tm.wHour;                                    // tm.GetHour();
		ucBuf1[9] = (BYTE)tm.wMinute;                                  // tm.GetMinute();
		ucBuf1[15]=0;	
		for(i=0;i<15;i++)	
			ucBuf1[15] += ucBuf1[i];
		ret = SMT_WriteBlock(sector,0,ucBuf1);
		if(ret != 0)	
			return -6;
		return 0;
	}
	else
	{
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)	
			return -5;
		ucCheck = 0;
		for(i=0;i<15;i++)	
			ucCheck+=ucBuf2[i];
		if(ucCheck != ucBuf2[15])	
			return 2;
		i = ucBuf2[0]+ucBuf2[1]*256+ucBuf2[2]*256*256;
		if(i<nMoney)	
			return 3;
		i = i - nMoney;
		ucBuf2[0] = i%256;
		ucBuf2[1] = i/256;
		ucBuf2[2] = i/256/256;
		i = ucBuf2[3]*256+ucBuf2[4];
		i = i + 1;
		ucBuf2[3] = i/256;
		ucBuf2[4] = i%256;
	//	CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//����ϴν��������뵱ǰʱ�䲻ͬ������ۼ����ѽ������
		if (tm.wDay != ucBuf2[7])                               // (tm.GetDay()!=ucBuf2[7])
		{
			ucBuf2[10] = 0;
			ucBuf2[11] = 0;
			ucBuf2[12] = 0;		
		}
		ucBuf2[5] = (BYTE)(tm.wYear - 2000);                   // tm.GetYear()-2000;
		ucBuf2[6] = (BYTE)(tm.wMonth);                         // tm.GetMonth();
		ucBuf2[7] = (BYTE)(tm.wDay);                           // tm.GetDay();
		ucBuf2[8] = (BYTE)(tm.wHour);                          // tm.GetHour();
		ucBuf2[9] = (BYTE)(tm.wMinute);                        // tm.GetMinute();
		ucBuf2[15]=0;	
		for(i=0;i<15;i++)	
			ucBuf2[15] += ucBuf2[i];
		ret = SMT_WriteBlock(sector,0,ucBuf2);
		if(ret != 0)	
			return -6;
		return 0;
	}
	return 0;
}


// ��������: �޸�Ǯ����Ϣ
// ��������: ������ʾ
int __stdcall SMT_PacketSetMoney(int nPacketNo,int nMoney)
{
	int ret,sector;
	unsigned char ucBuffer[16];
	if(nPacketNo<1)
		return -1;
	if(nMoney<0)
		return -1;
	ret = Login_Card(3);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)	return -5;
	
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
		return 1; //Ǯ��������ʹ��
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
		return ret;
	unsigned char ucBuf1[16],ucBuf2[16],ucBufTmp[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)	
		return -6;
	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		memcpy(ucBufTmp,ucBuf1,sizeof(ucBufTmp));
		ret = SMT_WriteBlock(sector,2,ucBufTmp);
		if(ret != 0)	
			return -8;
		i = nMoney;
		ucBuf1[0] = i%256;
		ucBuf1[1] = i/256;
		ucBuf1[2] = i/256/256;
		i = ucBuf1[3]*256+ucBuf1[4];
		i = i + 1;
		ucBuf1[3] = i/256;
		ucBuf1[4] = i%256;
// 		CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//����ϴν��������뵱ǰʱ�䲻ͬ������ۼ����ѽ������
		if (tm.wDay != ucBuf1[7])    // (tm.GetDay()!=ucBuf1[7])
		{
			ucBuf1[10] = 0;
			ucBuf1[11] = 0;
			ucBuf1[12] = 0;		
		}
		ucBuf1[5] = (BYTE)(tm.wYear - 2000);                        // tm.GetYear()-2000;
		ucBuf1[6] = (BYTE)tm.wMonth;								// tm.GetMonth();
		ucBuf1[7] = (BYTE)tm.wDay;									// tm.GetDay();
		ucBuf1[8] = (BYTE)tm.wHour;									// tm.GetHour();
		ucBuf1[9] = (BYTE)tm.wMinute;								// tm.GetMinute();
		ucBuf1[15]=0;	
		for(i=0;i<15;i++)	
			ucBuf1[15] += ucBuf1[i];
		ret = SMT_WriteBlock(sector,0,ucBuf1);
		if(ret != 0)	
			return -8;
		return 0;
	}
	else
	{
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)	
			return -9;
		ucCheck = 0;
		for(i=0;i<15;i++)	
			ucCheck+=ucBuf2[i];
		if(ucCheck != ucBuf2[15])	
			return 2;
		i = nMoney;
		ucBuf2[0] = i%256;
		ucBuf2[1] = i/256;
		ucBuf2[2] = i/256/256;
		i = ucBuf2[3]*256+ucBuf2[4];
		i = i + 1;
		ucBuf2[3] = i/256;
		ucBuf2[4] = i%256;
//		CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		//����ϴν��������뵱ǰʱ�䲻ͬ������ۼ����ѽ������
		if (tm.wDay != ucBuf2[7])                               // (tm.GetDay()!=ucBuf2[7])
		{
			ucBuf2[10] = 0;
			ucBuf2[11] = 0;
			ucBuf2[12] = 0;		
		}
		ucBuf2[5] = (BYTE)(tm.wYear - 2000);					// tm.GetYear()-2000;
		ucBuf2[6] = (BYTE)tm.wMonth;							// tm.GetMonth();
		ucBuf2[7] = (BYTE)tm.wDay;								// tm.GetDay();
		ucBuf2[8] = (BYTE)tm.wHour;								// tm.GetHour();
		ucBuf2[9] = (BYTE)tm.wMinute;							// tm.GetMinute();
		ucBuf2[15]=0;	
		for(i=0;i<15;i++)	
			ucBuf2[15] += ucBuf2[i];
		ret = SMT_WriteBlock(sector,0,ucBuf2);
		if(ret != 0)	
			return -12;
	}
	return 0;
}

// �������ã�Ǯ������
// ��������: Ǯ����
int __stdcall SMT_PacketReset(int nPacketNo)
{
	int ret,sector;
	if(nPacketNo<1)
		return -1;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)	return ret;
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)	return -5;

	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
		return 1; //Ǯ��������ʹ��
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	// 
	memset(ucBuffer,0,sizeof ucBuffer);
	ret = Login_Card(sector);
	if(ret != 0)
		return ret;
	ret = SMT_WriteBlock(sector,0,ucBuffer);
	if(ret != 0)
		return ret;
	memset(ucBuffer,0,sizeof ucBuffer);
	ret = SMT_WriteBlock(sector,2,ucBuffer);
	return ret;
}

// д������Ϣ
int __stdcall SMT_WriteAssistInfo (int nBatchNo[1],int nMoney[1])
{
	return 0;
}

// ��ȡ������Ϣ
int __stdcall SMT_ReadAssisInfo(int nBatchNo[1],int nMoney[1],unsigned char ucBatchTime[5],unsigned char ucMacNo[4])
{
	return 0;
}

// ��������: ���úڿ����, ��ͷ���ַ��ڿ�Ϊ00, �׿�ΪFF
// ��������: ��
int __stdcall SMT_SetBlackCardFlag()
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)	return ret;
	memset(ucBuffer,0xFF,16);
	ucBuffer[0] = 0x00;
	ret = SMT_WriteBlock(3,2,ucBuffer);
	if(ret != 0)	return -6;
	return 0;
}

// ��������: ������ͨ�����(�Ǻڿ�)
// ��������: ��
int __stdcall SMT_SetNormalFlag()
{
	int ret;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)	
		return ret;
//	CTime tm = CTime::GetCurrentTime();
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	unsigned char ucDateTime[20];
	unsigned char ucBcdDateTime[6];
	memset(ucDateTime,0,sizeof(ucDateTime));
	memset(ucBcdDateTime,0,sizeof(ucBcdDateTime));
//	sprintf((char*)ucDateTime,"%02d%02d%02d%02d%02d%02d",tm.GetYear()-2000,tm.GetMonth(),tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
	sprintf((char*)ucDateTime, "%02d%02d%02d%02d%02d%02d", (BYTE)(tm.wYear - 2000), (BYTE)tm.wMonth, (BYTE)tm.wDay, (BYTE)tm.wHour, (BYTE)tm.wMinute, (BYTE)tm.wSecond);
	dec2bcd(ucDateTime,ucBcdDateTime,6);
	memset(ucBuffer,0xFF,16);
	memcpy(ucBuffer+1,ucBcdDateTime,6);
	ret = SMT_WriteBlock(3,2,ucBuffer);
	if(ret != 0)	
		return -6;
	return 0;
}

// ��������: �ɲ�ͬ�汾������ͨ���
// ��������: �汾��
int __stdcall SMT_SetNormalFlagByBlkVerion(char *blkversion)
{
	int ret;
	unsigned char ucBlkVer[12];
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)	
		return ret;
	if(strlen(blkversion)!=12)
	{
		return -1;	
	}
	dec2bcd((unsigned char*)blkversion,ucBlkVer,6);
	memset(ucBuffer,0xFF,16);
	memcpy(ucBuffer+1,ucBlkVer,6);
	ret = SMT_WriteBlock(3,2,ucBuffer);
	if(ret != 0)	
		return -6;
	return 0;
}

// ��������: �������Ϣ
// ��������: ��
int __stdcall SMT_ClearCard()
{
	unsigned char ucSerialNo[5];
	unsigned char ucKeyA[6];
	unsigned char ucRawData[256];	//,ucDesData[256]
	
	int i,j,startSector=4;
	int ret;
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
		return -1;//��Ƭ���ڸ�Ӧ��
	//1��ʹ�ó��������½��Ƭ  ǰ32������
//	SMT_Cal_KeyA(ucRawData, ucSerialNo, ucDesData, ucKeyA);
	SMT_Cal_KeyA(SMT_UCWORKINGKEY, ucSerialNo, ucKeyA);
	// 	memcpy(ucRawData,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i+4] = ~ucRawData[i];
// 	des_code(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_KeyA(ucDesData,ucKeyA);
//	Cal_Key(ucDesData, ucKeyA);
	for(i=0;i<12;i++)
	{
		if(i==3)
			continue;
		ret = SMT_Login_With_UserKey(i,ucKeyA,0);
		if(ret != 0)
			return -2; //�ǳ�����
		for(j=0;j<3;j++)
		{
			if(i==0&&j==0)
				continue;
			if(i == 1 && j == 0)
			{
				//  ����ǵ�1������0�飬��ȡ��ʾ���ţ���Ҫɾ��������
				ret = SMT_ReadBlock(i,j,ucRawData);
				if(ret)
					return -3;
				// �������ʾ�����������������
				memset(ucRawData,0x00,7);
				memset(ucRawData+12,0x00,4);
			}
			else
			{
				memset(ucRawData,0x00,16);
			}
			ret=SMT_WriteBlock(i,j,ucRawData);
			if(ret)
				return -3;
		}
	}
	memset(ucRawData,0xFF,16);
	j = 0;
	for(i=0;i<4;i++)
	{
		ucRawData[j] = 0x00;
		j=j+1;
		ucRawData[j] = startSector;                        // ע��ֻ�ܸĵ��ĸ�
		startSector+=1;
		j=j+1;
	}
	//1 ʹ���û������½
	ret = SMT_Login_With_UserKey(3,ucKeyA,0);
	if(ret != 0)
		return -4;//ʹ���û�����KEYA���ܵ�½
	ret = SMT_WriteBlock(3,0,ucRawData);
	if(ret != 0)
		return -5;//�޷�д��
	memset(ucRawData,0xFF,16);

	for(i=0;i<4;i++)
	{
		ucRawData[i] = startSector;
		startSector += 1;
	}
	ret = SMT_WriteBlock(3,1,ucRawData);
	if(ret != 0)
		return -6;//�޷�д��
	memset(ucRawData,0xFF,16);  //20050305����
	ret = SMT_WriteBlock(3,2,ucRawData);
	if(ret != 0)
		return -7;//�޷�д��
	return 0;
}

// ��������: ��ȡ��֤�����
// ��������: flag(���λ��)
int __stdcall SMT_ReadAuthCardFlag(short * flag)
{
	int ret;
	unsigned char ucSerialNo[5]="";
	unsigned char ucInitKeyA[6]="",ucInitKeyB[6]="";	
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
	{
		MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
		return -1;//��Ƭ���ڸ�Ӧ��
	}
	memcpy(ucInitKeyA,ucSerialNo,4);
	//����7����
	sprintf((char*)&ucInitKeyA[4],"%02X",7);
	ret = SMT_Login_With_UserKey(7,ucInitKeyA,0);
	if(ret != 0)
		return -2; //�ǳ�����
	int  iWorkKeyAtBlockNo;
	int  iWorkKeyAtBlockStartPos;
	unsigned char ucAuthKeyA[6];
//	unsigned char ucAuthKeyB[6];
	unsigned char ucBuf1[16]="";
	ret = SMT_ReadBlock(7,1,ucBuf1);
	if(ret != 0)	
		return -3;	
//	memcpy(ucAuthKeyB,ucBuf1,6);
	iWorkKeyAtBlockNo=ucBuf1[6]-'0';
	iWorkKeyAtBlockStartPos=ucBuf1[7]-'0';

	unsigned char ucBuf2[16]="";
	unsigned char ucSector[3]="";
	int iWorkKeyAtSectorNo;
	//����15����
	sprintf((char*)&ucInitKeyA[4],"%02X",15);
	ret = SMT_Login_With_UserKey(15,ucInitKeyA,0);
	if(ret != 0)
		return -4; //�ǳ�����
	ret = SMT_ReadBlock(15,2,ucBuf2);
	if(ret != 0)	
		return -5;	
	//��������Կ����
	memset(ucSector,0,sizeof(ucSector));
	memcpy(ucSector,ucBuf2+14,2);
	iWorkKeyAtSectorNo=strtoul((char*)ucSector,NULL,16);
	memcpy(ucAuthKeyA,ucBuf2+8,6);
	ret = SMT_Login_With_UserKey(iWorkKeyAtSectorNo,ucAuthKeyA,0);
	if(ret != 0)
		return -6; //�ǳ�����
	unsigned char ucBuf3[256]="";
	memset(ucBuf3,0,sizeof(ucBuf3));
	ret = SMT_ReadBlock(iWorkKeyAtSectorNo,iWorkKeyAtBlockNo,ucBuf3);
	if(ret != 0)	
		return -7;
	*flag = (short)ucBuf3[iWorkKeyAtBlockStartPos+9];
	return 0;
}

// ��������: ��ȡ��֤��2
// ��������: ucWorkKey(������Կ), flag(���λ��)
int __stdcall SMT_ReadAuthCard2(unsigned char ucWorkKey[8],short * flag)
{
	int ret;
	unsigned char ucSerialNo[5]="";
	unsigned char ucInitKeyA[6]="",ucInitKeyB[6]="";
	//////////////////////////////////////////////////////////////////////////
	// ������
	ret = Load_Dll();
	if (ret != 0)
	{
		MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
	{
		MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
		return -1;//��Ƭ���ڸ�Ӧ��
	}
	memcpy(ucInitKeyA,ucSerialNo,4);
	//����7����
	sprintf((char*)&ucInitKeyA[4],"%02X",7);
	ret = SMT_Login_With_UserKey(7,ucInitKeyA,0);
	if(ret != 0)
		return -2; //�ǳ�����
	int  iWorkKeyAtBlockNo;
	int  iWorkKeyAtBlockStartPos;
	unsigned char ucAuthKeyA[6];
//	unsigned char ucAuthKeyB[6];
	unsigned char ucBuf1[16]="";
	ret = SMT_ReadBlock(7,1,ucBuf1);
	if(ret != 0)	
		return -3;	
//	memcpy(ucAuthKeyB,ucBuf1,6);
	iWorkKeyAtBlockNo=ucBuf1[6]-'0';
	iWorkKeyAtBlockStartPos=ucBuf1[7]-'0';

	unsigned char ucBuf2[16]="";
	unsigned char ucSector[3]="";
	int iWorkKeyAtSectorNo;
	//����15����
	sprintf((char*)&ucInitKeyA[4],"%02X",15);
	ret = SMT_Login_With_UserKey(15,ucInitKeyA,0);
	if(ret != 0)
		return -4; //�ǳ�����
	ret = SMT_ReadBlock(15,2,ucBuf2);
	if(ret != 0)	
		return -5;	
	//��������Կ����
	memset(ucSector,0,sizeof(ucSector));
	memcpy(ucSector,ucBuf2+14,2);
	iWorkKeyAtSectorNo=strtoul((char*)ucSector,NULL,16);
	memcpy(ucAuthKeyA,ucBuf2+8,6);
	ret = SMT_Login_With_UserKey(iWorkKeyAtSectorNo,ucAuthKeyA,0);
	if(ret != 0)
		return -6; //�ǳ�����
	unsigned char ucBuf3[256]="";
	memset(ucBuf3,0,sizeof(ucBuf3));
	ret = SMT_ReadBlock(iWorkKeyAtSectorNo,iWorkKeyAtBlockNo,ucBuf3);
	if(ret != 0)	
		return -7;	
	memcpy(ucWorkKey,ucBuf3+iWorkKeyAtBlockStartPos,8);
	*flag = (short)ucBuf3[iWorkKeyAtBlockStartPos+8];
	return 0;
}


// ��������: ��ȡ��֤��1
// ��������: ucWorkKey(������Կ)
int __stdcall SMT_ReadAuthCard(unsigned char ucWorkKey[8])
{
	int ret;
	unsigned char ucSerialNo[5]="";
	unsigned char ucInitKeyA[6]="",ucInitKeyB[6]="";	
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
	{
		MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
		return -1;//��Ƭ���ڸ�Ӧ��
	}
	memcpy(ucInitKeyA,ucSerialNo,4);
	//����7����
	sprintf((char*)&ucInitKeyA[4],"%02X",7);
	ret = SMT_Login_With_UserKey(7,ucInitKeyA,0);
	if(ret != 0)
		return -2; //�ǳ�����
	int  iWorkKeyAtBlockNo;
	int  iWorkKeyAtBlockStartPos;
	unsigned char ucAuthKeyA[6];
//	unsigned char ucAuthKeyB[6];
	unsigned char ucBuf1[16]="";
	ret = SMT_ReadBlock(7,1,ucBuf1);
	if(ret != 0)	
		return -3;	
//	memcpy(ucAuthKeyB,ucBuf1,6);
	iWorkKeyAtBlockNo=ucBuf1[6]-'0';
	iWorkKeyAtBlockStartPos=ucBuf1[7]-'0';

	unsigned char ucBuf2[16]="";
	unsigned char ucSector[3]="";
	int iWorkKeyAtSectorNo;
	//����15����
	sprintf((char*)&ucInitKeyA[4],"%02X",15);
	ret = SMT_Login_With_UserKey(15,ucInitKeyA,0);
	if(ret != 0)
		return -4; //�ǳ�����
	ret = SMT_ReadBlock(15,2,ucBuf2);
	if(ret != 0)	
		return -5;	
	//��������Կ����
	memset(ucSector,0,sizeof(ucSector));
	memcpy(ucSector,ucBuf2+14,2);
	iWorkKeyAtSectorNo=strtoul((char*)ucSector,NULL,16);
	memcpy(ucAuthKeyA,ucBuf2+8,6);
	/*
	{
		char szMsg[256]="";
		char keyA[17]="";
		sprintf(szMsg,"iWorkKeyAtSectorNo=%d",iWorkKeyAtSectorNo);
		::AfxMessageBox(szMsg);
		sprintf(szMsg,"iWorkKeyAtBlockNo=%d",iWorkKeyAtBlockNo);
		::AfxMessageBox(szMsg);
		sprintf(szMsg,"iWorkKeyAtBlockStartPos=%d",iWorkKeyAtBlockStartPos);
		::AfxMessageBox(szMsg);
		for(int i=0;i<6;i++)
			sprintf(&keyA[2*i],"%02X",ucAuthKeyA[i]);
		sprintf(szMsg,"ucAuthKeyA=%d",keyA);
		::AfxMessageBox(szMsg);
	}
	*/
	ret = SMT_Login_With_UserKey(iWorkKeyAtSectorNo,ucAuthKeyA,0);
	if(ret != 0)
		return -6; //�ǳ�����
	unsigned char ucBuf3[256]="";
	memset(ucBuf3,0,sizeof(ucBuf3));
	ret = SMT_ReadBlock(iWorkKeyAtSectorNo,iWorkKeyAtBlockNo,ucBuf3);
	if(ret != 0)	
		return -7;	
	memcpy(ucWorkKey,ucBuf3+iWorkKeyAtBlockStartPos,8);
	return 0;
}

// ��������: ��Ƭ����
// ��������: ��
int __stdcall SMT_ResetCard()
{
	unsigned char ucSerialNo[5];
	unsigned char ucKeyA[6],ucKeyB[6],ucInitKeyA[6],ucInitKeyB[6];
	unsigned char ucRawData[256],ucTmpRawData[16]; //,ucDesData[256];
	int ret;
	int i;
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
	{
		MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
		return -1;//��Ƭ���ڸ�Ӧ��
	}
	//1 --- ����KEYA
//	SMT_Cal_KeyA(ucRawData, ucSerialNo, ucDesData, ucKeyA);
// 	memcpy(ucRawData,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i+4] = ~ucRawData[i];
// 	des_code(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_KeyA(ucDesData,ucKeyA);
//	Cal_Key(ucDesData, ucKeyA);
	SMT_Cal_KeyA(SMT_UCWORKINGKEY, ucSerialNo, ucKeyA);
	//����KEYB
//	SMT_Cal_KeyB(ucRawData, ucSerialNo, ucDesData, ucKeyB);
	SMT_Cal_KeyB(SMT_UCWORKINGKEY, ucSerialNo, ucKeyB);
	// 	memcpy(ucRawData+4,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i] = ~ucRawData[i+4];
// 	TriDes(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_Key(ucDesData,ucKeyB);
	memset(ucRawData,0xFF,sizeof(ucRawData));
	memset(ucInitKeyA,0xFF,sizeof(ucInitKeyA));
	memset(ucInitKeyB,0,sizeof(ucInitKeyB));
//	ucRawData[6] = 0x7F;
//	ucRawData[7] = 0x07;
//	ucRawData[8] = 0x88;
//	ucRawData[9] = 0x00;

	ucRawData[6] = 0xFF;
	ucRawData[7] = 0x07;
	ucRawData[8] = 0x80;
	ucRawData[9] = 0x69;

	for(i=0;i<32;i++)
	{
		switch(1)
		{
		case 1:
			ret = MF_ReadSeriesNo(ucSerialNo);
			if(ret < 0)
			{
				MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
				return -1;//��Ƭ���ڸ�Ӧ��
			}
			ret = SMT_Login_With_UserKey(i,ucKeyB,1);//1
			if(ret != 0)
			{
				//char szMsg[256]="";
				//sprintf(szMsg,"ʹ�ü��ܺ�KeyB��¼��%d����ʧ��",i);
				//AfxMessageBox(szMsg);
			}
			else
			{
				memcpy(ucTmpRawData,ucRawData,16);
				ret = SMT_WriteBlock(i,3,ucTmpRawData);
				if(ret != 0)
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ü��ܺ�KeyBд��%d����ʧ��",i);
					//AfxMessageBox(szMsg);
				}
				else
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ü��ܺ�KeyBд��%d�����ɹ�",i);
					//AfxMessageBox(szMsg);

				}
			}
		case 2:
			if(ret==0)
				break;
			ret = MF_ReadSeriesNo(ucSerialNo);
			if(ret < 0)
			{
				MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
				return -1;//��Ƭ���ڸ�Ӧ��
			}
			ret = SMT_Login_With_UserKey(i,ucKeyA,0);//1
			if(ret != 0)
			{
				//char szMsg[256]="";
				//sprintf(szMsg,"ʹ�ü��ܺ�KeyA��¼��%d����ʧ��",i);
				//AfxMessageBox(szMsg);
			}
			else
			{
				memcpy(ucTmpRawData,ucRawData,16);
				ret = SMT_WriteBlock(i,3,ucTmpRawData);
				if(ret != 0)
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ü��ܺ�KeyAд��%d����ʧ��",i);
					//AfxMessageBox(szMsg);
				}
				else
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ü��ܺ�KeyAд��%d�����ɹ�",i);
					//AfxMessageBox(szMsg);
				}
			}
		case 3:
			if(ret==0)
				break;
			ret = MF_ReadSeriesNo(ucSerialNo);
			if(ret < 0)
			{
				MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
				return -1;//��Ƭ���ڸ�Ӧ��
			}
			ret = SMT_Login_With_UserKey(i,ucInitKeyA,0);//1
			if(ret != 0)
			{
				//char szMsg[256]="";
				//sprintf(szMsg,"ʹ�ó���KeyA=0xFF��¼��%d����ʧ��",i);
				//AfxMessageBox(szMsg);
			}
			else
			{
				memcpy(ucTmpRawData,ucRawData,16);
				ret = SMT_WriteBlock(i,3,ucTmpRawData);
				if(ret != 0)
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyA=0xFFд��%d����ʧ��",i);
					//AfxMessageBox(szMsg);
				}
				else
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyA=0xFFд��%d�����ɹ�",i);
					//AfxMessageBox(szMsg);
				}
			}
		case 4:
			if(ret==0)
				break;
			ret = MF_ReadSeriesNo(ucSerialNo);
			if(ret < 0)
			{
				MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
				return -1;//��Ƭ���ڸ�Ӧ��
			}
			ret = SMT_Login_With_UserKey(i,ucInitKeyA,1);//1
			if(ret != 0)
			{
				//char szMsg[256]="";
				//sprintf(szMsg,"ʹ�ó���KeyB=0xFF��¼��%d����ʧ��",i);
				//AfxMessageBox(szMsg);
			}
			else
			{
				memcpy(ucTmpRawData,ucRawData,16);
				ret = SMT_WriteBlock(i,3,ucTmpRawData);
				if(ret != 0)
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyB=0xFFд��%d����ʧ��",i);
					//AfxMessageBox(szMsg);
				}
				else
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyB=0xFFд��%d�����ɹ�",i);
					//AfxMessageBox(szMsg);
				}
			}
			break;
		case 5:
			if(ret==0)
				break;
			ret = MF_ReadSeriesNo(ucSerialNo);
			if(ret < 0)
			{
				MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
				return -1;//��Ƭ���ڸ�Ӧ��
			}
			ret = SMT_Login_With_UserKey(i,ucInitKeyB,0);//1
			if(ret != 0)
			{
				//char szMsg[256]="";
				//sprintf(szMsg,"ʹ�ó���KeyA=0x00��¼��%d����ʧ��",i);
				//AfxMessageBox(szMsg);
			}
			else
			{
				memcpy(ucTmpRawData,ucRawData,16);
				ret = SMT_WriteBlock(i,3,ucTmpRawData);
				if(ret != 0)
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ��KeyA=0x00д��%d����ʧ��",i);
					//AfxMessageBox(szMsg);
				}
				else
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyA=0x00д��%d�����ɹ�",i);
					//AfxMessageBox(szMsg);
				}
			}
		case 6:
			if(ret==0)
				break;
			ret = MF_ReadSeriesNo(ucSerialNo);
			if(ret < 0)
			{
				MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
				return -1;//��Ƭ���ڸ�Ӧ��
			}
			ret = SMT_Login_With_UserKey(i,ucInitKeyB,1);//1
			if(ret != 0)
			{
				//char szMsg[256]="";
				//sprintf(szMsg,"ʹ�ó���KeyB=0x00��¼��%d����ʧ��",i);
				//AfxMessageBox(szMsg);
			}
			else
			{
				memcpy(ucTmpRawData,ucRawData,16);
				ret = SMT_WriteBlock(i,3,ucTmpRawData);
				if(ret != 0)
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyB=0x00д��%d����ʧ��",i);
					//AfxMessageBox(szMsg);
				}
				else
				{
					//char szMsg[256]="";
					//sprintf(szMsg,"ʹ�ó���KeyB=0x00д��%d�����ɹ�",i);
					//AfxMessageBox(szMsg);
				}
			}
			break;
		default:
			break;
		}
		ret = MF_ReadSeriesNo(ucSerialNo);
		if(ret < 0)
		{
			MessageBox(NULL, "��Ƭ���ڸ�Ӧ��", NULL, MB_OK);
			return -1;//��Ƭ���ڸ�Ӧ��
		}
		ret = SMT_Login_With_UserKey(i,ucInitKeyA,0);
		if(ret != 0)
		{
			char szMsg[256]="";
			sprintf(szMsg,"��ʼ����%d�����ɹ������µ�¼ʧ��",i);
			MessageBox(NULL, szMsg, NULL, MB_OK);
			return -7; //�ǳ�����
		}
		else
		{
			//char szMsg[256]="";
			//sprintf(szMsg,"��ʼ����%d�����ɹ�",i);
			//AfxMessageBox(szMsg);
		}
	}
	return 0;
}


// ��������: ��������Ǯ����Ϣ
// ��������: Ǯ����
int __stdcall SMT_ResetPacketInfo(int nPacketNo)
{
	int ret,sector;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��¼3����ʧ��",__FILE__,__LINE__,ret);		
		return ret;
	}
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��3����ʧ��",__FILE__,__LINE__,ret);	
		return -5;
	}
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
	{
		SetLastErrMsg(1,"File:%s\nLine:%d\nError:%d\nMessage:Ǯ��%dδ��ͨ",__FILE__,__LINE__,ret,nPacketNo);		
		return 1; //Ǯ��������ʹ��
	}
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��½��%d����ʧ��",__FILE__,__LINE__,ret,sector);	
		return ret;
	}
	unsigned char ucBuf1[16];

	memset(ucBuf1,0,sizeof(ucBuf1));
	ret = SMT_WriteBlock(sector,0,ucBuf1);
	if(ret != 0)	
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:д��%d������0��ʧ��",__FILE__,__LINE__,ret,sector);	
		return -8;
	}
	memset(ucBuf1,0,sizeof(ucBuf1));
	ret = SMT_WriteBlock(sector,2,ucBuf1);
	if(ret != 0)	
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:д��%d�������ݿ�ʧ��",__FILE__,__LINE__,ret,sector);	
		return -8;
	}
	return 0;
}

// ��������: ���Ǯ����Ϣ
// ��������: nPacketNo(Ǯ����), pPcakInfo(Ǯ����Ϣָ��)
int __stdcall SMT_GetPacketInfo(int nPacketNo, MONEY_PACK_INFO *pPackInfo)
{
	int ret,sector;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)	
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��¼3����ʧ��",__FILE__,__LINE__,ret);
		return ret;
	}
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��3����ʧ��",__FILE__,__LINE__,ret);	
		return -5;
	}
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
	{
		SetLastErrMsg(1,"File:%s\nLine:%d\nError:%d\nMessage:Ǯ��%dδ��ͨ",__FILE__,__LINE__,ret,nPacketNo);	
		return 1; //Ǯ��������ʹ��
	}
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:��½��%d����ʧ��",__FILE__,__LINE__,ret,sector);	
		return ret;
	}

	unsigned char ucBuf1[16],ucBuf2[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d����ʧ��",__FILE__,__LINE__,ret,sector);	
		return -5;
	}
	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		pPackInfo->bala = ucBuf1[0]+ucBuf1[1]*256+ucBuf1[2]*256*256;
		pPackInfo->total_cnt   = ucBuf1[3]*256+ucBuf1[4];
		pPackInfo->date    = (ucBuf1[5]+2000)*10000+ucBuf1[6]*100+ucBuf1[7];
		pPackInfo->time    = ucBuf1[8]*100+ucBuf1[9];
		pPackInfo->daybala =ucBuf1[10]+ucBuf1[11]*256+ucBuf1[12]*256*256;
		pPackInfo->time_cnt=ucBuf1[0X0D];
		pPackInfo->day_cnt=ucBuf1[0X0E];
		return 0;
	}
	else
	{
		SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d����У��ʹ���,��ʼ����������",__FILE__,__LINE__,0,sector);	
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)
		{
			SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d����ʧ��",__FILE__,__LINE__,ret,sector);	
			return -5;
		}
		ucCheck = 0;
		for(i=0;i<15;i++)
			ucCheck+=ucBuf2[i];
		if(ucCheck == ucBuf2[15])
		{
			pPackInfo->bala = ucBuf2[0]+ucBuf2[1]*256+ucBuf2[2]*256*256;
			pPackInfo->total_cnt   = ucBuf2[3]*256+ucBuf2[4];
			pPackInfo->date    = (ucBuf2[5]+2000)*10000+ucBuf2[6]*100+ucBuf2[7];
			pPackInfo->time    = ucBuf2[8]*100+ucBuf2[9];
			pPackInfo->daybala =ucBuf2[10]+ucBuf2[11]*256+ucBuf2[12]*256*256;
			pPackInfo->time_cnt=ucBuf2[0X0D];
			pPackInfo->day_cnt=ucBuf2[0X0E];
			return 0;
		}
		else
		{
			SetLastErrMsg(ret,"File:%s\nLine:%d\nError:%d\nMessage:����%d�������ݿ�У��ʹ���",__FILE__,__LINE__,ret,sector);	
			return 2;
		}
	}
	return 2;
}


// ��������: ����Ǯ����Ϣ
// ��������: nPacketNo(Ǯ����), pPackInfo(Ǯ����Ϣָ��)
int __stdcall SMT_SetPacketInfo(int nPacketNo,MONEY_PACK_INFO *pPackInfo)
{
	int ret,sector;
	unsigned char ucBuffer[16];
	if(nPacketNo<1)
		return -1;
	ret = Login_Card(3);
	if(ret != 0)
	{
		return ret;
	}
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)
	{
		return -5;
	}
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
	{
		return 1; //Ǯ��������ʹ��
	}
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
	{
		return ret;
	}
	unsigned char ucBuf1[16],ucBuf2[16],ucBufTmp[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)
	{
		return -6;
	}
	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		memcpy(ucBufTmp,ucBuf1,sizeof(ucBufTmp));
		ret = SMT_WriteBlock(sector,2,ucBufTmp);
		if(ret != 0)	
			return -8;
		if(pPackInfo->bala>=0)
		{
			i = pPackInfo->bala;
			ucBuf1[0] = i%256;
			ucBuf1[1] = i/256;
			ucBuf1[2] = i/256/256;
		}
		if(pPackInfo->total_cnt>=0)
		{
			i = pPackInfo->total_cnt;
			ucBuf1[3] = i/256;
			ucBuf1[4] = i%256;
		}
		/*
		 *	date: -1 ���޸�,0-�޸�Ϊϵͳʱ��,>0-�޸�Ϊ����ʱ��
		 */
//		CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		if(0==pPackInfo->date)
		{
			ucBuf1[5] = (BYTE)(tm.wYear - 2000); // tm.GetYear()-2000;
			ucBuf1[6] = (BYTE)(tm.wMonth);		 // tm.GetMonth();
			ucBuf1[7] = (BYTE)(tm.wDay);		 // tm.GetDay();
		}
		else if(pPackInfo->date>20000000&&pPackInfo->date<30000000)
		{
			int day  = pPackInfo->date%100;
			int mon  = pPackInfo->date/100%100;
			int year = pPackInfo->date/10000%100;
		
			ucBuf1[5] = year;
			ucBuf1[6] = mon;
			ucBuf1[7] = day;
		}
		if(0==pPackInfo->time)
		{
			ucBuf1[8] = (BYTE)tm.wHour;			 // tm.GetHour();
			ucBuf1[9] = (BYTE)tm.wMinute;		 // tm.GetMinute();
		}
		else if(pPackInfo->time>0)
		{
			int hour=pPackInfo->time/100;
			int minute=pPackInfo->time%100;
			ucBuf1[8] = hour;
			ucBuf1[9] = minute;
		}
		if(pPackInfo->daybala>=0)
		{
			i = pPackInfo->daybala;
			ucBuf1[10] = i%256;
			ucBuf1[11] = i/256;
			ucBuf1[12] = i/256/256;
		}
		ucBuf1[15]=0;
		for(i=0;i<15;i++)	
			ucBuf1[15] += ucBuf1[i];
		ret = SMT_WriteBlock(sector,0,ucBuf1);
		if(ret != 0)	
			return -8;
		return 0;
	}
	else
	{
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)	
			return -9;
		ucCheck = 0;
		for(i=0;i<15;i++)	
			ucCheck+=ucBuf2[i];
		if(ucCheck != ucBuf2[15])	
			return 2;
		if(pPackInfo->bala>=0)
		{
			i = pPackInfo->bala;
			ucBuf2[0] = i%256;
			ucBuf2[1] = i/256;
			ucBuf2[2] = i/256/256;
		}
		if(pPackInfo->total_cnt>=0)
		{
			i = pPackInfo->total_cnt;
			ucBuf2[3] = i/256;
			ucBuf2[4] = i%256;
		}
		/*
		 *	date: -1 ���޸�,0-�޸�Ϊϵͳʱ��,>0-�޸�Ϊ����ʱ��
		 */
//		CTime tm = CTime::GetCurrentTime();
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		if(0==pPackInfo->date)
		{
			ucBuf2[5] = (BYTE)tm.wYear;					// tm.GetYear()-2000;
			ucBuf2[6] = (BYTE)tm.wMonth;				// tm.GetMonth();
			ucBuf2[7] = (BYTE)tm.wDay;					// tm.GetDay();
		}
		else if(pPackInfo->date>20000000 && pPackInfo->date<30000000)
		{
			int day  = pPackInfo->date%100;
			int mon  = pPackInfo->date/100%100;
			int year = pPackInfo->date/10000%100;
			
			ucBuf2[5] = year;
			ucBuf2[6] = mon;
			ucBuf2[7] = day;
		}
		if(0==pPackInfo->time)
		{
			ucBuf2[8] = (BYTE)tm.wHour;					// tm.GetHour();
			ucBuf2[9] = (BYTE)tm.wMinute;				// tm.GetMinute();
		}
		else if(pPackInfo->time>0)
		{
			int hour=pPackInfo->time/100;
			int minute=pPackInfo->time%100;
		
			ucBuf2[8] = hour;
			ucBuf2[9] = minute;
		}
		if(pPackInfo->daybala>=0)
		{
			i = pPackInfo->daybala;
			ucBuf2[10] = i%256;
			ucBuf2[11] = i/256;
			ucBuf2[12] = i/256/256;
		}
		ucBuf2[15]=0;
		for(i=0;i<15;i++)	
			ucBuf2[15] += ucBuf2[i];

		ret = SMT_WriteBlock(sector,0,ucBuf2);
		if(ret != 0)	
			return -12;
	}
	return 0;
}

// ��������: ���ý��״���
// ��������: nPacketNo(Ǯ����), tx_cnt(???)
int __stdcall SMT_SetTxCnt(int nPacketNo,int tx_cnt)
{
	int ret,sector;
	unsigned char ucBuffer[16]="";
	if(nPacketNo<1)
		return -1;
	if(tx_cnt<0)
		return -1;
	ret = Login_Card(3);
	if(ret != 0)
	{
		return ret;
	}
	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)
	{
		return -5;
	}
	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
	{
		return 1; //Ǯ��������ʹ��
	}
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	
	ret = Login_Card(sector);
	if(ret != 0)
	{
		return ret;
	}
	unsigned char ucBuf1[16]="";
	unsigned char ucBuf2[16]="";
	unsigned char ucBufTmp[16]="";
	unsigned char ucCheck=0;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)
	{
		return -6;
	}
	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		memcpy(ucBufTmp,ucBuf1,sizeof(ucBufTmp));
		ret = SMT_WriteBlock(sector,2,ucBufTmp);
		if(ret != 0)	
			return -8;
		ucBuf1[3] = tx_cnt/256;
		ucBuf1[4] = tx_cnt%256;
		ucBuf1[15]=0;
		for(i=0;i<15;i++)	
			ucBuf1[15] += ucBuf1[i];
		ret = SMT_WriteBlock(sector,0,ucBuf1);
		if(ret != 0)	
			return -8;
	}
	else
	{
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)	
			return -9;
		ucCheck = 0;
		for(i=0;i<15;i++)	
			ucCheck+=ucBuf2[i];
		if(ucCheck != ucBuf2[15])	
			return 2;
		ucBuf2[3] = tx_cnt/256;
		ucBuf2[4] = tx_cnt%256;
		ucBuf2[15]=0;
		for(i=0;i<15;i++)	
			ucBuf2[15] += ucBuf2[i];
		ret = SMT_WriteBlock(sector,0,ucBuf2);
		if(ret != 0)	
			return -10;
	}
	return 0;
}

// ��ȡ�������б��汾
int __stdcall SMT_ReadBlackNameListVersion(int *flag,char *version)
{
	int ret;
	unsigned char ucBuffer[16]="";
	char sVersion[13]="";
	ret = Login_Card(3);
	if(ret != 0)	
		return ret;
	ret = SMT_ReadBlock(3,2,ucBuffer);
	if(ret != 0)	
		return -6;
	if(0==ucBuffer[0])
		*flag=1;
	else
		*flag=0;
	dec2hex(ucBuffer + 1, 6, sVersion);
	strcpy(version, sVersion);
	return 0;
}

// ��ȡǮ�����
int __stdcall ReadBalance(int nPacketNo,int nBalance[1],int nCnt[1])
{

	char line[10]="";
	int ret,sector;
	unsigned char ucBuffer[16];
	ret = Login_Card(3);
	if(ret != 0)
		return ret;

	ret = SMT_ReadBlock(3,0,ucBuffer);
	if(ret != 0)
		return ret;

	if(ucBuffer[(nPacketNo-1)*2] != 0x00)
	{
		return 1; //Ǯ��������ʹ��
	}
	sector = ucBuffer[(nPacketNo-1)*2+1]; //Ǯ����������
	if(sector<4)
	{
		return -1;
	}
	ret = Login_Card(sector);
	if(ret != 0)	
		return ret;

	unsigned char ucBuf1[16],ucBuf2[16];
	unsigned char ucCheck;
	ret = SMT_ReadBlock(sector,0,ucBuf1);
	if(ret != 0)
		return ret;

	int i;
	ucCheck = 0;
	for(i=0;i<15;i++)
		ucCheck+=ucBuf1[i];
	if(ucCheck == ucBuf1[15])
	{
		nBalance[0] = ucBuf1[0]+ucBuf1[1]*256+ucBuf1[2]*256*256;
		nCnt[0]=ucBuf1[3]*256+ucBuf1[4];
	}
	else 
	{
		ret = SMT_ReadBlock(sector,2,ucBuf2);
		if(ret != 0)	
			return ret;

		ucCheck = 0;
		for(i=0;i<15;i++)
			ucCheck+=ucBuf2[i];
		if(ucCheck == ucBuf2[15])
		{
			nBalance[0] = ucBuf2[0]+ucBuf2[1]*256+ucBuf2[2]*256*256;
			nCnt[0]=ucBuf2[3]*256+ucBuf2[4];
			return 0;
		}
		else
		{
			return 2;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
#define WATER_SECT_NO  0x0E
#define WATER_SECT_BLK_NO 1
// static unsigned char s_water_temp_key[4] = {0x22,0x33,0x44,0x55};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ���㺯��
//////////////////////////////////////////////////////////////////////////
static unsigned char add_2(unsigned char add1, unsigned char add2)
{
	unsigned char ad1, ad2, sum;
	ad1 = add1;
	ad2 = add2;
	sum = (ad1 + ad2) % 0x100;
	return sum;
}

static void dess_ming(unsigned char key[4],unsigned char Source[4],unsigned char dest[4])
{
	unsigned char x0, x1, x2, x3, y0, y1, y2, y3, m, n, add1, add2;
	x0 = key[0];
	x1 = key[1];
	x2 = key[2];
	x3 = key[3];
	y0 = Source[0];
	y1 = Source[1];
	y2 = Source[2];
	y3 = Source[3];
	m = x0 ^ x1;
	add1 = m;
	add2 = y2;
	m = add_2(add1, add2);
	add1 = x0;
	add2 = x1;
	n = add_2(add1, add2);
	add1 = n;
	add2 = y3;
	n = add_2(add1, add2);
	m = m ^ y0;
	n = n ^ y1;
	y0 = m;
	y1 = n;
	add1 = m;
	add2 = x1 ^ x2;
	m = add_2(add1, add2);
	add1 = x1;
	add2 = x2;
	add1 = add_2(add1, add2);
	add2 = n;
	n = add_2(add1, add2);
	m = m ^ y2;
	n = n ^ y3;
	y2 = m;
	y3 = n;
	add1 = y1;
	add2 = x2 ^ x3;
	m = add_2(add1, add2);
	add1 = x2;
	add2 = x3;
	add1 = add_2(add1, add2);
	add2 = y0;
	n = add_2(add1, add2);
	m = m ^ y3;
	n = n ^ y2;
	y2 = m;
	y3 = n;
	add1 = m;
	add2 = x0 ^ x3;
	m = add_2(add1, add2); 
	add1 = x0;
	add2 = x3;
	add1 = add_2(add1, add2);
	add2 = n;
	n = add_2(add1, add2);
	m = m ^ y1;
	n = n ^ y0;
	y0 = m;
	y1 = n;
	add1 = y3;
	add2 = x0 ^ x3;
	m = add_2(add1, add2);
	add1 = x0;
	add2 = x3;
	add1 = add_2(add1, add2);
	add2 = y2;
	n = add_2(add1, add2);
	m = m ^ y1;
	n = n ^ y0;
	y0 = m;
	y1 = n;
	add1 = m;
	add2 = x2 ^ x3;
	m = add_2(add1, add2);
	add1 = x2;
	add2 = x3;
	add1 = add_2(add1, add2);
	add2 = n;
	n = add_2(add1, add2);
	m = m ^ y3;
	n = n ^ y2;
	y2 = m;
	y3 = n;
	add1 = y1;
	add2 = x1 ^ x2;
	m = add_2(add1, add2);
	add1 = x1;
	add2 = x2;
	add1 = add_2(add1, add2);
	add2 = y0;
	n = add_2(add1, add2);
	m = m ^ y3;
	n = n ^ y2;
	y2 = m;
	y3 = n;
	add1 = m;
	add2 = x0 ^ x1;
	m = add_2(add1, add2);
	add1 = x0;
	add2 = x1;
	add1 = add_2(add1, add2);
	add2 = n;
	n = add_2(add1, add2);
	m = m ^ y1;
	n = n ^ y0;
	y0 = m;
	y1 = n;
	dest[0] = y0;
	dest[1]= y1;
	dest[2] = y2;
	dest[3] = y3;
}

static int gen_extra_keyA(unsigned char* key,unsigned char* text,unsigned char* keyA)
{
	unsigned int i;
	unsigned char sPlain[9] = "";
	unsigned char workKey[4] = "";
	memcpy(workKey,key,sizeof(workKey));
	for(i = 0;i < 4;++i)
	{
		sPlain[i] = text[i];
		sPlain[i+4] = ~text[i];
	}
	dess_ming(workKey,sPlain,keyA);
	dess_ming(workKey,sPlain+4,keyA+4);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ������ˮ�Ĳ���
//////////////////////////////////////////////////////////////////////////

// ��½��ˮƬ��
static int LoginWaterSection()
{
	unsigned char ucSerialNo[5];// = {0x11,0x22,0x33,0x44};
	unsigned char ucKey[8] = "";
	int ret = 1;
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
		return -1;//��Ƭ���ڸ�Ӧ��
	/*
	unsigned char workey[4] = {0x8E,0x8B,0xFC,0x8F};
	if(gen_extra_keyA(workey,ucSerialNo,ucKey))
		return -1;
	*/
	// FIXME : 
	if(gen_extra_keyA(SMT_UCWORKINGKEY,ucSerialNo,ucKey))
		return -1;
	
	if(SMT_Login_With_UserKey(WATER_SECT_NO,ucKey,0))
		return -2;
	return 0;
}

// ��ȡ��ˮ����Ϣ
int __stdcall SMT_ReadWaterPackInfo(WATER_PACK_INFO* info)
{
	unsigned char ucBuffer[16] = "";
	int ret;
	ret = LoginWaterSection();
	if(ret)
		return -1;
	ret = SMT_ReadBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucBuffer);
	if(ret)
		return -6;
	unsigned char termid[2] = "";
	memcpy(termid,ucBuffer,sizeof(info->termid));
	char buf[12] = "";
	if(bcd2decA(ucBuffer+4,3,buf))
		return -7;
	info->remain = atoi(buf);
	
	if(memcmp(termid,s_water_clear_card,2) == 0)
	{
		info->cardtype = WATER_CLEAR_CARD;
	}
	else if(memcmp(termid,s_water_new_card,2) == 0)
	{
		info->cardtype = WATER_NEW_CARD;
	}
	else if(memcmp(termid,s_water_common_card,2) == 0)
	{
		info->cardtype = WATER_COMMON_CARD;
	}
	else
	{
		info->cardtype = WATER_FIXED_CARD;
		memset(buf,0,sizeof(buf));
//		bcd2decA(termid,2,buf);
		info->termid = (termid[0] << 8 & 0xFF00);
		info->termid |= termid[1] & 0xFF;		
//		info->termid = atoi(buf);
	}
	memcpy(info->oldSerialNo, ucBuffer + 8, 4);
	return 0;
}

// ��ȡˮ��Ǯ��������Ϣ, ̩�ݲ�����........................
int __stdcall SMT_ReadWaterPackBackUp(int *WaterPackBackUp)
{
	unsigned char ucBuffer[16] = "";
	char cBuf[12] = "";
	int ret;
	ret = LoginWaterSection();
	if(ret)
		return -1;
	ret = SMT_ReadBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucBuffer);
	if(ret)
		return -6;
	if(bcd2decA(ucBuffer+13,3,cBuf))
		return -7;
	*WaterPackBackUp = atoi(cBuf);
	return 0;
}
//.........................................................

// д����ˮ��Ϣ
int __stdcall SMT_WriteWaterPackInfo(WATER_PACK_INFO* info)
{
	unsigned char ucRawData[256] = "";
	char bcdtmp[16] = "";
	unsigned char termid[2] = "";

	switch(info->cardtype)
	{
	case WATER_CLEAR_CARD:
		memcpy(termid,s_water_clear_card,2);
		break;
	case WATER_NEW_CARD:
		memcpy(termid,s_water_new_card,2);
		break;
	case WATER_COMMON_CARD:
		memcpy(termid,s_water_common_card,2);
		break;
	case WATER_FIXED_CARD:
		termid[0] = ((info->termid&0xFF00)>>8);
		termid[1] = (info->termid&0xFF);
// 		sprintf(bcdtmp,"%d",info->termid);
// 		if(dec2bcd((unsigned char*)bcdtmp,termid,2))
// 			return -2;
		break;
	default:
		return -1;
	}

	if(LoginWaterSection())
	{
		return -2;
	}
	// д����
	memset(ucRawData,0,sizeof(ucRawData));
	memcpy(ucRawData,termid,sizeof(termid));
	memset(bcdtmp,0,sizeof(bcdtmp));
	sprintf(bcdtmp,"%.06d",info->remain);
	if(dec2bcd((unsigned char*)bcdtmp,ucRawData+4,strlen(bcdtmp)))
		return -3;
	// �¼���Ķ���
	memcpy(ucRawData + 8, info->oldSerialNo,sizeof(info->oldSerialNo));
	if(SMT_WriteBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucRawData))
	{
		return -4;
	}
	return 0;
}

// ������ˮ����
int __stdcall SMT_WaterSetCapability(int value)
{
	if(value < 0 || value > 99999)
		return -2;
	unsigned char ucBuffer[16] = "";
	int ret;
	ret = LoginWaterSection();
	if(ret)
		return -1;
	ret = SMT_ReadBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucBuffer);
	if(ret)
		return -3;
	char buffer[20] = "";
	sprintf(buffer,"%.06d",value);
	if(dec2bcd((unsigned char*)buffer,ucBuffer+4,3))
		return -3;
	ret = SMT_WriteBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucBuffer);
	if(ret)
		return -4;
	return 0;
}

// ��½��ʼ��
static int LoginOriginalCard(int sect,int keytype = 1)
{
	unsigned char uckey[8];
	unsigned char ucSerialNo[5] = "";
	if(MF_ReadSeriesNo(ucSerialNo) < 0)
		return -1;
	memset(uckey,0xFF,sizeof(uckey));
	if(SMT_Login_With_UserKey(sect,uckey,keytype))
		return -1;
	return 0;
}

// ��ʼ����ˮ��Ϣ
int __stdcall SMT_InitWaterInfo(const WATER_PACK_INFO* info)
{
	unsigned char ucRawData[256] = "";
	unsigned char ucDesData[256] = "";
	unsigned char ucKeyA[8] = "";
	unsigned char ucKeyB[8] = "";
	unsigned char ucSerialNo[5] = "";
	char bcdtmp[16] = "";
	unsigned char termid[2] = "";
//	int i;
	switch(info->cardtype)
	{
	case WATER_CLEAR_CARD:
		memcpy(termid,s_water_clear_card,2);
		break;
	case WATER_CLEAR_CARD2:
		memcpy(termid,s_water_clear_card2,2);
		break;
	case WATER_NEW_CARD:
		memcpy(termid,s_water_new_card,2);
		break;
	case WATER_COMMON_CARD:
		memcpy(termid,s_water_common_card,2);
		break;
	case WATER_FIXED_CARD:
		termid[0] = ((info->termid&0xFF00)>>8);
		termid[1] = (info->termid&0xFF);
		//if(dec2bcd((unsigned char*)bcdtmp,termid,2))
		//	return -2;
		break;
	default:
		return -1;
	}
	int ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
		return -1;//��Ƭ���ڸ�Ӧ��
	// FIXME : ʹ����Ȩ��Կ
	if(gen_extra_keyA(SMT_UCWORKINGKEY,ucSerialNo,ucKeyA))
		return -1;

	int needInit = 0;
	if(!Login_Card(WATER_SECT_NO))
	{
		//����KEYB	
//		SMT_Cal_KeyB(ucRawData, ucSerialNo, ucRawData, ucKeyB);
		SMT_Cal_KeyB(SMT_UCWORKINGKEY, ucSerialNo, ucKeyB);
		// 		memcpy(ucRawData+4,ucSerialNo,4);
// 		for(i=0;i<4;i++)
// 			ucRawData[i] = ~ucRawData[i+4];
// 		TriDes(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 		Cal_Key(ucDesData,ucKeyB);
//		Cal_Key(ucDesData, ucKeyA);
		// ʹ�� KeyB ��¼��
		if(SMT_Login_With_UserKey(WATER_SECT_NO,ucKeyB,1))
			return -1;
		needInit = 1;
	}
	else if(!LoginOriginalCard(WATER_SECT_NO))
	{
		needInit = 1;
	}
	if(needInit)
	{
		// ��ʼ����Կ����
		memset(ucRawData,0xFF,sizeof(ucRawData));
		memcpy(ucRawData,ucKeyA,sizeof(ucKeyA));
		memcpy(ucRawData+10,ucKeyB,sizeof(ucKeyB));
		ucRawData[6] = 0x7F;
		ucRawData[7] = 0x07;
		ucRawData[8] = 0x88;
		ucRawData[9] = 0x00;
		// д��Կ
		if(SMT_WriteBlock(WATER_SECT_NO,3,ucRawData))
		{
			return -1;
		}
	}
	if(LoginWaterSection())
	{
		return -2;
	}
	// д����
	memset(ucRawData,0,sizeof(ucRawData));
	memcpy(ucRawData,termid,sizeof(termid));
	memset(bcdtmp,0,sizeof(bcdtmp));
	sprintf(bcdtmp,"%.06d",info->remain);
	if(dec2bcd((unsigned char*)bcdtmp,ucRawData+4,3))
		return -3;
	memcpy(ucRawData + 8, info->oldSerialNo,sizeof(info->oldSerialNo));
	if(SMT_WriteBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucRawData))
	{
		return -4;
	}
	return 0;
}

// ��ˮ��֤
int __stdcall SMT_WaterNewAuthCard()                                      // �����޸Ĺ�_stdcall
{
	unsigned char keyA[6] = {0x48, 0x43, 0x43, 0x48, 0x43, 0x43};
	unsigned char keyB[6] = "";
	unsigned char ucSerialNo[5] = "";
	int ret;                                                              // ,i;
	unsigned char ucRawData[24] = "";
	unsigned char ucDesData[24] = "";
	
	ret = MF_ReadSeriesNo(ucSerialNo);
	if(ret < 0)
		return -1;

	if(LoginOriginalCard(WATER_SECT_NO))
	{
		return -4;
	}
	
	// ���� keyB
	SMT_Cal_KeyB(SMT_UCWORKINGKEY, ucSerialNo, keyB);
// 	memcpy(ucRawData+4,ucSerialNo,4);
// 	for(i=0;i<4;i++)
// 		ucRawData[i] = ~ucRawData[i+4];
// 	TriDes(UCWORKINGKEY,ucRawData,ucDesData,'0');
// 	Cal_Key(ucDesData,keyB);
	memset(ucRawData,0xFF,sizeof(ucRawData));
	memcpy(ucRawData,keyA,sizeof(keyA));
	memcpy(ucRawData+10,keyB,sizeof(keyB));
	ucRawData[6] = 0x7F;
	ucRawData[7] = 0x07;
	ucRawData[8] = 0x88;
	ucRawData[9] = 0x00;
	SMT_WriteBlock(WATER_SECT_NO,3,ucRawData);
	if(SMT_Login_With_UserKey(WATER_SECT_NO,keyA,0))
	{
		if(LoginOriginalCard(WATER_SECT_NO,0))
		{
			return -4;
		}
		
		memset(ucRawData,0xFF,sizeof(ucRawData));
		memcpy(ucRawData,keyA,sizeof(keyA));
		memcpy(ucRawData+10,keyB,sizeof(keyB));
		ucRawData[6] = 0x7F;
		ucRawData[7] = 0x07;
		ucRawData[8] = 0x88;
		ucRawData[9] = 0x00;
		SMT_WriteBlock(WATER_SECT_NO,3,ucRawData);
		if(SMT_Login_With_UserKey(WATER_SECT_NO,keyA,0))
		{
			return -2;
		}
	}
	memset(ucRawData,0,sizeof(ucRawData));
	memcpy(ucRawData,s_water_auth_card,2);
	//FIXME: 
	memcpy(ucRawData+8,SMT_UCWORKINGKEY,4);
	if(SMT_WriteBlock(WATER_SECT_NO,WATER_SECT_BLK_NO,ucRawData))
	{
		return -3;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////
//	��ʼ��������
//  2006-8-14
///////////////////////////////////////////////////////////////////////////

static BOOL Is_Com_open()
{
	return com_is_open;
}

static BOOL Read_Author_Key()
{
	if( TRUE == has_read_main_key)
	{
		return TRUE;
	}
	if( SMT_ReadAuthCard2(SMT_UCWORKINGKEY,&AUTH_CARD_FLAG) == 0 )
	{
		has_read_main_key = TRUE;
		SMT_AutoDll(SMT_UCWORKINGKEY);
		SMT_ControlBuzzer();
		return TRUE;
	}
	return FALSE;
}

/** 
 * @brief ʹ����Կ��¼������
 * 
 * @param sect - Ҫ��¼������
 * @param key - ��¼��Կ
 * @param flag - ��Կ��KEY-A����KEY-B
 * @param card_type - ������
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */
static int LoginCardSect(long sect,BYTE * key,long flag,BYTE * card_type)
{
	//char msg[500] = "";
	BYTE tmp_card_id[5] = "";
	BYTE tmp_card_type[2] = "";
	// ����ϴε�¼���ɹ�������Ҫ���¼��һ�¿������ܼ�����¼
	static bool lastLoginSuccess = false;
	
	assert(sect >= 0 || sect < SMARD_CARD_SECT_NO);

//	if( !lastLoginSuccess )
//	{
	if (SMT_RequestCardExist(tmp_card_id,tmp_card_type) !=0)
	{
		return -1;
	}
//	}
	if( SMT_Login_With_UserKey(sect,key,flag) == 0 )
	{
		// ��¼�ɹ�
		lastLoginSuccess = true;
		//printf("Login card sect[%d] key [%s]",sect,key);
		//TRACE0(msg);
		return 0;
	}
	// ��¼ʧ��
	lastLoginSuccess = false;
	return -1;
}

static int WriteCardBlock(int *err_code,PWriteDataFunc pFunc,LPProcessCallBack lpCallBack)
{
	BYTE card_id[5] = "";
	BYTE card_type[2] = "";
	BYTE card_key_a[7] = "";
	BYTE card_key_b[7] = "";
	int current_key, old_key;
	int ret;
	BOOL login_sect_succ = FALSE;
	
	CardKeyData card_key[3];
	
	int sect = 0;
	// �жϴ򿪴���
	if (!Is_Com_open())
	{
		*err_code = E_COM_NOT_OPEN;
		return -1;
	}
	// ��ȡ��Ȩ��Կ
	if( FALSE == Read_Author_Key())
	{
		*err_code = E_MAIN_KEY_ERROR;
		return -1;
	}
	// ��鿨�Ƿ����
	if ( SMT_RequestCardExist(card_id,card_type) != 0 )
	{
		*err_code = E_READ_CARD_ERROR;
		return -1;
	}

	// ���㿨�� keyA , keyB
	if( gen_user_card_keyA(SMT_UCWORKINGKEY,card_id
			,card_key_a) != 0 )
	{
		*err_code = E_GEN_CARD_KEY_A_ERROR;
		return -1;
	}
	if( gen_user_card_keyB(SMT_UCWORKINGKEY,card_id
			,card_key_b) != 0 )
	{
		*err_code = E_GEN_CARD_KEY_B_ERROR;
		return -1;
	}
	// ��ʼ�� key ������
	memset((void *)card_key,0,sizeof(card_key));

	card_key[0].flag = CARD_KEY_A;
	// ����λ�ÿ�����Ϊ��ʼ����ʱ����޸Ĺ���KEYA
	memcpy(card_key[0].key,CARD_INIT_KEY,sizeof(CARD_INIT_KEY));

	card_key[1].flag = CARD_KEY_A;
	memcpy(card_key[1].key,card_key_a,sizeof(card_key_a));

	card_key[2].flag = CARD_KEY_B;
	memcpy(card_key[2].key,card_key_b,sizeof(card_key_b));
	
	// �ٶ�һ�ο����һ�¿� ID
	BYTE card_id_2[5] = "";
	SMT_RequestCardExist(card_id_2,card_type);
	if( memcmp(card_id,card_id_2,4) != 0)
	{
		* err_code = E_READ_CARD_ERROR;
		return -1;
	}
	// ������Ե�¼�� key ������
	current_key = 0;
//	current_key = 1;					 // �޸Ĺ�, �����˻��÷���ȥ, ��keyA��½ 
	for(sect = 0;sect < SMARD_CARD_SECT_NO; ++sect)
	{
		// ���ûص�����
		if( lpCallBack != NULL )
			lpCallBack(sect);
		// ���γ�ʼ��ÿ������
		old_key = current_key;
		login_sect_succ = FALSE;
		// ���ҿ��Ե�¼�� key 
		while(TRUE)
		{
			// �����¼�ɹ� ����ѭ��
			if( LoginCardSect(sect,card_key[current_key].key
				,card_key[current_key].flag,card_type) == 0)
			{
				login_sect_succ = TRUE;
				break;
			}
			current_key--;
			if( current_key < 0 )
				current_key = 2;
			if (current_key == old_key)
				break;
		}
		// �жϵ�ǰ�����Ƿ�ɹ���¼
		if (login_sect_succ == FALSE)
		{
			*err_code = E_CARD_LOGIN_ERROR;
			return -1;
		}
		// ����д���ص�����
		if( pFunc != NULL )
		{
			if( (ret = pFunc(sect,&card_key[1],&card_key[2],card_type,
				&card_key[current_key])) != 0)
			{
				// д�����ɹ�
				*err_code = E_CARD_WRITE_ERROR;
				return ret;
			}
		}
	}
	return 0;
}

static int WriteCardData(long sect,long block,BYTE * buf)
{
	assert(buf != NULL);
	return SMT_WriteBlock(sect,block,buf);
}

static int WriteInitNewCardData(long sect,CardKeyData * keya,CardKeyData *keyb
								,BYTE * card_type,CardKeyData * login_key)
{
	BYTE sect_data[17] = "";
	// ��ʼ��д�������λ
	memset(sect_data,0,sizeof(sect_data));
	// key A
	memcpy(sect_data,keya->key,CARD_KEY_LEN);
	// control bit
	memcpy(sect_data+CARD_KEY_LEN,CARD_NEW_CTRL_BIT,sizeof(CARD_NEW_CTRL_BIT));
	// key B
	memcpy(sect_data+CARD_KEY_LEN+sizeof(CARD_NEW_CTRL_BIT),keyb->key,CARD_KEY_LEN);
	
	// if(WriteCardData(sect,PASSWD_BLOCK_NUM,sect_data) == 0)
	WriteCardData(sect,PASSWD_BLOCK_NUM,sect_data);
	{
		// д���ɹ��������Ƿ��ʼ���ɹ�
		// ����ʹ�� keyA ��¼
		// �޸�һ�� 20080804
		if(LoginCardSect(sect,keya->key
			,keya->flag,card_type) != 0)
		{
			return -2;
		}
		
		// ����ʹ�� keyB ��¼
		if(LoginCardSect(sect,keyb->key
			,keyb->flag,card_type) != 0)
		{
			return -3;
		}
		return 0;
	}
	return -1;
}

int __stdcall InitNewCard(int * err_code,const char* showcardno
								   ,LPProcessCallBack lpCallBack)
{
	int ret = 0;
	if((ret = WriteCardBlock(err_code,WriteInitNewCardData,lpCallBack)) != 0)
	{
		//*err_code = E_CARD_WRITE_ERROR;
		return ret;
	}
	
	if(showcardno != NULL)
	{
		// ����BCD��
		unsigned char bcdshowcard[5] = "";
		for(int i = 0;i < 10;i+=2)
		{
			bcdshowcard[i/2] = (((showcardno[i] - '0') << 4) & 0xF0)
				| ((showcardno[i+1] - '0') & 0x0F);
		}
		// д��ʾ����
		if(SMT_ChangeShowCardNo(bcdshowcard) != 0)
		{
			return -1;
		}
	}
	// ����ˮ��Ǯ������ͨ�ÿ�
	WATER_PACK_INFO water, tempWater;
	memset(&water,0,sizeof(water));
	memset(&tempWater, 0, sizeof(tempWater));
	SMT_ReadWaterPackInfo(&tempWater); 	
	water.remain = tempWater.remain;
	water.cardtype = WATER_COMMON_CARD;
	if(SMT_InitWaterInfo(&water))
	{
		MessageBox(NULL, "��ʼ����ʧ��", "", NULL);
		return -1;
	}
	SMT_ControlBuzzer();
	return 0;
}

int __stdcall ReadCardMainKey(BYTE * main_key,short wantFlag,
									   short * readFlag)
{
	int ret = -1;
	if( NULL == readFlag )
	{
		return -1;
	}
	if( TRUE == has_read_main_key 
		&& (AUTH_CARD_FLAG == wantFlag || -1 == wantFlag ))
	{
		return 0;
	}
	unsigned char work_key[9] = "";
	if( SMT_ReadAuthCard2(work_key,&AUTH_CARD_FLAG) == 0 )
	{
		if(AUTH_CARD_FLAG == wantFlag || -1 == wantFlag)
		{
			has_read_main_key = TRUE;
			ret = 0;
			SMT_AutoDll(work_key);
			SMT_ControlBuzzer();
			memcpy(main_key,work_key,sizeof(main_key));
			*readFlag = AUTH_CARD_FLAG;
		}
	}
	return ret;
}

int __stdcall ReadCardPhyID(BYTE card_id[4],BYTE card_type[2])
{
	return SMT_RequestCardExist(card_id,card_type);
}

/** 
 * @brief ���տ��Ļص�����
 * 
 * @param sect - ������
 * @param keya - KEYA
 * @param keyb - KEYB
 * @param card_type - ������
 * @param login_key - ��¼��Կ
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */
static int WriteRefineCardData(long sect,CardKeyData * keya,CardKeyData * keyb
							   ,BYTE * card_type,CardKeyData * login_key)
{
	
	BYTE sect_data[17] = "";
	int ret;
	// ��ʼ��д�������λ
	memset(sect_data,0,sizeof(sect_data));
	// key A, ��������ڻ��յ�ʱ��д��KEYA
	memcpy(sect_data,CARD_INIT_KEY,CARD_KEY_LEN);
	// control bit
	memcpy(sect_data+CARD_KEY_LEN,CARD_INIT_CTRL_BIT,sizeof(CARD_INIT_CTRL_BIT));
	// key B
	memcpy(sect_data+CARD_KEY_LEN+sizeof(CARD_INIT_CTRL_BIT),CARD_INIT_KEY,CARD_KEY_LEN);
	// ����� ������ ��
//	memcmp(login_key->key,keyb->key,sizeof(keyb->key));
	bool ctrl_bit = false;
	if(!memcmp(login_key->key,keyb->key,sizeof(keyb->key))
		&& card_type[0] == PHILIPS_CARD_TYPE[0])
	{
		// ������Keyb ��¼������Ϊ ������ ��
		ctrl_bit = true;
	}	
	ret = -1;
	WriteCardData(sect,PASSWD_BLOCK_NUM,sect_data);
	// ����ʹ�� keyB ��¼
	if((LoginCardSect(sect,CARD_INIT_KEY
		,CARD_KEY_A,card_type) == 0)|| ctrl_bit) // ���λ����Ҫ��һ��
	{
		ret = 0;
	}
	return ret;
}

int __stdcall RefineCard(int *err_code,LPProcessCallBack lpFunc)
{
	if(SMT_ClearCard() != 0)
	{
		*err_code = E_CARD_WRITE_ERROR;
		return -1;
	}

	if(WriteCardBlock(err_code,WriteRefineCardData,lpFunc) != 0)
	{
		*err_code = E_CARD_WRITE_ERROR;
		return -1;
	}
	SMT_ControlBuzzer();
	return 0;
}

int __stdcall ReadCardBlock(int startSect,int endSect
									 ,short block,LPGetBlockData lpFunc)
{
	BYTE card_id[5] = "";
	BYTE card_type[2] = "";
	BYTE card_key_a[7] = "";
	BYTE card_key_b[7] = "";
	BYTE buf[16] = "";
	int current_key, old_key;
	BOOL login_sect_succ = FALSE;
	
	CardKeyData card_key[3];
	
	int sect = 0;
	// �жϴ򿪴���
	if (!Is_Com_open())
	{
		return -1;
	}
	// ��ȡ��Ȩ��Կ
	if( FALSE == Read_Author_Key())
	{
		return -1;
	}
	// ��鿨�Ƿ����
	if ( SMT_RequestCardExist(card_id,card_type) != 0 )
	{
		return -1;
	}

	// ���㿨�� keyA , keyB
	if( gen_user_card_keyA(SMT_UCWORKINGKEY,card_id
			,card_key_a) != 0 )
	{
		return -1;
	}
	if( gen_user_card_keyB(SMT_UCWORKINGKEY,card_id
			,card_key_b) != 0 )
	{
		return -1;
	}
	// ��ʼ�� key ������
	memset((void *)card_key,0,sizeof(card_key));

	card_key[0].flag = CARD_KEY_A;
	// ������Զ������պ��KEYA
	memcpy(card_key[0].key,CARD_INIT_KEY,sizeof(CARD_INIT_KEY));

	card_key[1].flag = CARD_KEY_A;
	memcpy(card_key[1].key,card_key_a,sizeof(card_key_a));

	card_key[2].flag = CARD_KEY_B;
	memcpy(card_key[2].key,card_key_b,sizeof(card_key_b));
	// ������Ե�¼�� key ������
	current_key = 1;
	for(sect = startSect;sect < endSect + 1; ++sect)
	{
		// ���γ�ʼ��ÿ������
		old_key = current_key;
		login_sect_succ = FALSE;
		// ���ҿ��Ե�¼�� key 
		while(TRUE)
		{
			// �����¼�ɹ� ����ѭ��
			if( LoginCardSect(sect,card_key[current_key].key
				,card_key[current_key].flag,card_type) == 0)
			{
				login_sect_succ = TRUE;
				break;
			}
			current_key++;
			if( current_key > 2 )
				current_key = 0;
			if (current_key == old_key)
				break;
		}
		// �жϵ�ǰ�����Ƿ�ɹ���¼
		if (login_sect_succ == FALSE)
		{
			return -1;
		}
		// ��ȡ����������
		for(int b = 0;b < 4;++b)
		{
			if(block & (1 << b))
			{
				memset(buf,0,sizeof(buf));
				if(SMT_ReadBlock(sect,b,buf) != 0)
				{
					return -1;
				}
				// ���ö����ص�����
				if( lpFunc != NULL )
				{
					lpFunc(sect,b,buf,
						card_key[current_key].key,
						card_key[current_key].flag);
					
				}
			}
		}
		
	}
	return 0;
}

int __stdcall ResetAuthCard(int * err_code,LPProcessCallBack lpCallBack)
{
	BYTE card_id[5] = "";
	BYTE card_type[2] = "";
	BYTE card_key[7] = "";
	BYTE buf[17] = "";
	BYTE tmpBuf[17] = "";
	BYTE priKeyA[7] = "";
	BYTE priKeyB[7] = "";
	BYTE tmpSect[3] = "";
	char tmp[3] = "";
	short priBlock,priStartPos,priSect;
	unsigned short sect;

	if( !Is_Com_open() )
	{
		* err_code = E_COM_NOT_OPEN;
		return -1;
	}

	if(SMT_RequestCardExist(card_id,card_type) != 0)
	{
		* err_code = E_READ_CARD_ID_ERROR;
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ 7 ����
	sect = 7;
	memset(card_key,0,sizeof(card_key));
	memcpy(card_key,card_id,sizeof(card_id));
	// �� ID Ϊ 4 λ
	sprintf((char*)card_key + 4,"%02X",sect);
	//memcpy(card_key+4,((char*)&sect)+1,1);
	//memcpy(card_key+5,((char*)&sect),1);
	
	// ��ȡ Key B
	if( LoginCardSect(sect,card_key,CARD_KEY_A,card_type) != 0)
	{
		//AfxMessageBox("��¼����ʧ�ܣ�");
		MessageBox(NULL, "��¼����ʧ�ܣ�", "", NULL);
		* err_code = E_CARD_LOGIN_ERROR;
		return -2;
	}
	if( SMT_ReadBlock(sect,1,buf) != 0)
	{
		//AfxMessageBox("��ȡ����ʧ�ܣ�");
		MessageBox(NULL, "��ȡ����ʧ�ܣ�", "", NULL);
		* err_code = E_CARD_WRITE_ERROR;
		return -3;
	}
	// key b
	memcpy(priKeyB,buf,CARD_KEY_LEN);
	// block
	priBlock = buf[6] - '0';
	if( priBlock < 0 || priBlock > 2)
	{
		//AfxMessageBox("���ݲ��Ϸ���");
		MessageBox(NULL, "���ݲ��Ϸ���", "", NULL);
		* err_code = E_INVALID_CARD;
		return -3;
	}
	// start pos
	// priStartPos = char2int((const char*)&buf[7],1,true);
	tmp[0] = buf[7];
	priStartPos = (short)strtoul(tmp,NULL,16);
	if( priStartPos < 0 || priStartPos > 10)
	{
		//AfxMessageBox("���ݲ��Ϸ���");
		MessageBox(NULL, "���ݲ��Ϸ���", "", NULL);
		* err_code = E_INVALID_CARD;
		return -3;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// �� 15 ����
	sect = 15;
	memset(card_key,0,sizeof(card_key));
	memcpy(card_key,card_id,sizeof(card_id));
	// �� ID Ϊ 4 λ
	sprintf((char*)card_key + 4,"%02X",sect);
	// ��ȡ Key a
	if( LoginCardSect(sect,card_key,CARD_KEY_A,card_type) != 0)
	{
		//AfxMessageBox("��¼����ʧ�ܣ�");
		MessageBox(NULL, "��¼����ʧ�ܣ�", "", NULL);
		* err_code = E_CARD_LOGIN_ERROR;
		return -2;
	}
	if( SMT_ReadBlock(sect,2,buf) != 0)
	{
		//AfxMessageBox("��ȡ����ʧ�ܣ�");
		MessageBox(NULL, "��ȡ����ʧ�ܣ�", "", NULL);
		* err_code = E_CARD_WRITE_ERROR;
		return -3;
	}
	// key a
	strncpy((char*)priKeyA,(char*)buf+8,CARD_KEY_LEN);
	// private key section no
	priSect = (short)strtoul((char*)buf+14,NULL,16);
	//////////////////////////////////////////////////////////////////////////
	// ���ɳ�ʼ״̬
	memset((char*)buf,255,sizeof(buf) - 1);
	strncpy((char*)buf + sizeof(CARD_INIT_KEY),(char*)CARD_INIT_CTRL_BIT,sizeof(CARD_INIT_CTRL_BIT));

	if(lpCallBack)
		lpCallBack(0);
	bool isClear = false;
	// ������Կ����
	if(LoginCardSect(priSect,priKeyA,CARD_KEY_A,card_type) == 0)
	{
		memset(tmpBuf,0,sizeof(tmpBuf));
		
		if(SMT_WriteBlock(priSect,priBlock,tmpBuf) != 0)
		{
			isClear = false;
			goto L_TRY_KEY_B;
		}
		// д����λ����
		strncpy((char*)tmpBuf,(char*)buf,sizeof(tmpBuf));
		if(SMT_WriteBlock(priSect,3,tmpBuf) != 0)
		{
			// д��ʧ�ܣ�ʹ��Key B ����
			isClear = false;
		}
		else
			isClear = true;
		
	}
L_TRY_KEY_B:
	if(lpCallBack)
		lpCallBack(0);
	if(!isClear)
	{
		// �� key B ����
		if(LoginCardSect(priSect,priKeyB,CARD_KEY_B,card_type) == 0)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			
			if(SMT_WriteBlock(priSect,priBlock,tmpBuf) != 0)
			{
			//	AfxMessageBox("д��������ʧ�ܣ�");
				MessageBox(NULL, "д��������ʧ�ܣ�", "", NULL);
				* err_code = E_CARD_WRITE_ERROR;
				return -2;
			}
			// д����λ����
			strncpy((char*)tmpBuf,(char*)buf,sizeof(tmpBuf));
			if(SMT_WriteBlock(priSect,3,tmpBuf) != 0)
			{
				// key B Ҳ�޷�д��
				//AfxMessageBox("д��������ʧ�ܣ�");
				MessageBox(NULL, "д��������ʧ�ܣ�", "", NULL);
				* err_code = E_CARD_WRITE_ERROR;
				return -2;
			}
			
		}
		else
		{
			// �޷��� key b ��¼��
		//	AfxMessageBox("��¼����ʧ�ܣ�");
			MessageBox(NULL, "��¼����ʧ�ܣ�", "", NULL);
			*err_code = E_CARD_LOGIN_ERROR;
			return -1;
		}
	}

	// �����Ƿ���ճɹ�
	if( LoginCardSect(priSect,CARD_INIT_KEY,CARD_KEY_A,card_type) != 0)
	{
		//AfxMessageBox("��������ʧ�ܣ�");
		MessageBox(NULL, "��������ʧ�ܣ�", "", NULL);
		* err_code = E_CARD_WRITE_ERROR;
		return -2;
	}
	char msg[128] = "";
	// ������������
	for(sect = 0;sect < AUTH_CARD_SECT_NO;++sect)
	{
		if(lpCallBack)
			lpCallBack(0);
		// ��Կ�������ó�ʼ��
		if( sect == priSect)
			continue;
		memset(card_key,0,sizeof(card_key));
		memcpy(card_key,card_id,sizeof(card_id));
		// �� ID Ϊ 4 λ
		sprintf((char*)(card_key+4),"%02X",sect);
		
		if( LoginCardSect(sect,card_key
				,CARD_KEY_A,card_type) == 0)
		{
			strncpy((char*)tmpBuf,(char*)buf,sizeof(tmpBuf));
			// ��¼�ɹ�
			// ���տ���λ
			if( SMT_WriteBlock(sect,3,tmpBuf) != 0)
			{
				sprintf(msg,"����[%d]����ʧ�ܣ�",sect);
				//AfxMessageBox(msg);
				MessageBox(NULL, msg, "", NULL);
				* err_code = E_CARD_WRITE_ERROR;
				return -2;
			}
			// ��Key A ��¼����
			if( LoginCardSect(sect,CARD_INIT_KEY,CARD_KEY_A,card_type) != 0)
			{
				sprintf(msg,"���Ե�¼%d����ʧ�ܣ�",sect);
			//	AfxMessageBox(msg);
				MessageBox(NULL, msg, "", NULL);
				* err_code = E_CARD_LOGIN_ERROR;
				return -1;
			}
		}
		else if( LoginCardSect(sect,CARD_INIT_KEY,CARD_KEY_A,card_type) == 0)
		{
			// ʹ�� ��ʼ�� ��Կ���е�¼����
			sprintf(msg,"����%dʹ�ó�ʼ��Կ��¼�ɹ���\n�Ƿ�������գ�",sect);
		//	if(AfxMessageBox(msg,MB_OKCANCEL) == IDCANCEL)
			if (MessageBox(NULL, msg, "", MB_OKCANCEL) == IDCANCEL)
			{
				// �������������,�򷵻�
				* err_code = E_CARD_LOGIN_ERROR;
				return -2;
			}
		}
		else
		{
			sprintf(msg,"��¼����%d����ʧ��",sect);
			//AfxMessageBox(msg);
			MessageBox(NULL, msg, "", NULL);
			* err_code = E_CARD_LOGIN_ERROR;
			return -2;
		}
	}
	return 0;
}

/** 
 * @brief ��ӿں��������¶�ȡ��Ȩ��
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */

int __stdcall ResetMainKey()
{
//	if( ReaderAuthor((char *)CARD_MAIN_KEY) == 0 )
	if( ReaderAuthor(CARD_MAIN_KEY) == 0 )	
	{
		SMT_AutoDll(CARD_MAIN_KEY);
		has_read_main_key = TRUE;
		return TRUE;
	}
	return FALSE;
}

/** 
 * @brief ��ʼ����Ȩ����ÿ��������Կ
 * 
 * @param card_id - ������ID��
 * @param card_type - ������
 * @param lpCallBack - �ص�������������ʱ���� ������ΪNULL
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */
static int NewAuthCard_Init_Sect(BYTE card_id[4],BYTE card_type[2],
								 LPProcessCallBack lpCallBack)
{
	int sect;
	char msg[256] = "";
	BYTE buf[16] = "";
	BYTE card_key[6] = "";
	memcpy(card_key,card_id,sizeof(card_id));
	// ��ȡһ�ο�,�����û�ͬʱ�Ŷ��ſ�
	for(sect = 0;sect < AUTH_CARD_SECT_NO;++sect)
	{
		if(lpCallBack)
			lpCallBack(sect);
		// һ��Ҫʹ�ô�д
		sprintf((char*)card_key+4,"%02X",sect);

		//////////////////////////////////////////////////////////////////////////
		if( LoginCardSect(sect,CARD_INIT_KEY,CARD_KEY_A,card_type) != 0)
		{
			if(LoginCardSect(sect,card_key,CARD_KEY_B,card_type) != 0)
			{
				sprintf(msg,"��¼����%d����ʧ��",sect);
				MessageBox(NULL, msg, NULL, NULL);
				return -1;
			}
			continue;
		}
		
		//////////////////////////////////////////////////////////////////////////
		memcpy(buf,card_key,sizeof(card_key));
		memcpy(buf+6,CARD_INIT_CTRL_BIT,sizeof(CARD_INIT_CTRL_BIT));
		memcpy(buf+10,card_key,sizeof(card_key));
		// д������Ϣ
		SMT_WriteBlock(sect,3,buf);
		/*
		{
			sprintf(msg,"��ʼ����%d����ʧ��",sect);
			AfxMessageBox(msg);
			return -1;
		}
		*/
		if( LoginCardSect(sect,card_key,CARD_KEY_A,card_key) != 0)
		{
			sprintf(msg,"���Ե�%d������¼ʧ��!\nд����%d������Կʧ��!"
				,sect,sect);
			MessageBox(NULL, msg, NULL, NULL);
			return -1;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	return 0;
}

/** 
 * @brief ������Ȩ��CRCУ��ֵ
 * 
 * @param keya - ��Ȩ����Կ����KEYA
 * @param keyb - ��Ȩ����Կ����KEYB
 * @param sect - ��Ȩ����Կ������
 * @param block - ��Ȩ����Կ���
 * @param pos -   ��Ȩ����Կ����ʼλ��
 * 
 * @return - ����CRCУ��ֵ 
 */
static long CalAuthCardCRC(BYTE keya[6],BYTE keyb[6],int sect,int block,int pos)
{
	BYTE buf[17] = "";
	long crc;
	int i;
	memcpy(buf,keyb,CARD_KEY_LEN);
	i = sprintf((char*)buf+CARD_KEY_LEN,"%c%c%02X",
		block+0x30,pos+0x30,sect);
	i += CARD_KEY_LEN;
	memcpy((char *)buf + i,keya,CARD_KEY_LEN);
	// ����У��λ
	crc = get_check_char(buf);
	return crc;
}

/** 
 * @brief ������Ȩ��������Կ����
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */
static int ResetAuthCardKeySeed()
{
	int i;
	srand((unsigned)time( NULL ));
	for(i = 0;i < sizeof(AUTH_CARD_SEED);++i)
	{
		AUTH_CARD_SEED[i] = rand() % 255;
	}
	return 0;
}

/** 
 * @brief ��ӿں�����������Ȩ��
 * 
 * @param main_key - ��Ȩ��Կ
 * @param pri_sect - ��Ȩ��Կ��������
 * @param pri_block- ��Ȩ��Կ���ڿ��
 * @param pri_startpos - ��Ȩ��Կ��ʼλ��
 * @param flag - ��Ȩ�������־����־���ݼ���Ȩ���ṹ�ĵ�
 * @param lpCallBack - �ص�������������ʱ���ã�����ΪNULL
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */
int __stdcall NewAuthCard(BYTE main_key[8],int pri_sect
								   ,int pri_block,int pri_startpos
								   ,int flag,LPProcessCallBack lpCallBack)
{
	// ������Ȩ��
	// modify by Cash 2005-8-8
	// Ϊ��Ȩ�������˱�־λ
	if((pri_sect == 7) || (pri_sect == 15 ) 
		|| (pri_sect == 30) || (pri_sect < 1)
		|| (pri_sect > 31)
		||(pri_block < 0) || (pri_block > 2)
		||(pri_startpos < 0) || (pri_startpos > 7))
	{
		MessageBox(NULL, "��Ȩ����������", NULL, NULL);
		return -1;
	}
	BYTE card_id[5] = "";
	BYTE card_type[2] = "";
	BYTE card_key[6] = "";
	BYTE buf[17] = "";
	long crc;
	short sect;
	if( SMT_RequestCardExist(card_id,card_type) != 0 )
	{
		MessageBox(NULL, "��ſ���", NULL, NULL);
		return -1;
	}
	// ������Կ
	BYTE auth_key[8] = "";
	if( decrypt_work_key(main_key,auth_key) != 0)
	{
		MessageBox(NULL, "������Կʧ�ܣ�", NULL, NULL);
		return -1;
	}
	// ��ʼ����������
	if( NewAuthCard_Init_Sect(card_id,card_type,lpCallBack) != 0)
	{
		return -1;
	}
	ResetAuthCardKeySeed();
	BYTE auth_key_a[6] = "";
	BYTE auth_key_b[6] = "";
	if(gen_auth_card_keyA(AUTH_CARD_SEED,auth_key_a) != 0 )
	{
		MessageBox(NULL, "������ԿAʧ�ܣ�", NULL, NULL);
		return -2;
	}
	if(gen_auth_card_keyB(AUTH_CARD_SEED,auth_key_b) != 0)
	{
		MessageBox(NULL, "������ԿBʧ�ܣ�", NULL, NULL);
		return -2;
	}
	if( lpCallBack )
		lpCallBack(0);
	//////////////////////////////////////////////////////////////////////////
	// �� 7 ����
	sect = 7;
	memset(buf,0,sizeof(buf));
	memcpy(buf,auth_key_b,sizeof(auth_key_b));
	sprintf((char*)buf+6,"%01X",pri_block);
	//buf[6] = (char)pri_block;
	sprintf((char*)buf+7,"%01X",pri_startpos);
	//buf[7] = (char)pri_startpos;
	memcpy(card_key,card_id,sizeof(card_id));
	sprintf((char*)card_key+4,"%02X",sect);
	if(LoginCardSect(sect,card_key,CARD_KEY_A,card_type) !=0)
	{
		MessageBox(NULL, "��¼����ʧ�ܣ�", NULL, NULL);
		return -2;
	}
	if( SMT_WriteBlock(sect,1,buf) !=0 )
	{
		MessageBox(NULL, "д��ʧ�ܣ�", NULL, NULL);
		return -3;
	}
	if( lpCallBack )
		lpCallBack(0);
	//////////////////////////////////////////////////////////////////////////
	// �� 15 ����
	sect = 15;
	memset(buf,0,sizeof(buf));
	memcpy(buf+8,auth_key_a,sizeof(auth_key_a));
	sprintf((char*)buf+14,"%02X",pri_sect);
	
	memcpy(card_key,card_id,sizeof(card_id));
	sprintf((char*)card_key+4,"%02X",sect);
	
	if(LoginCardSect(sect,card_key,CARD_KEY_A,card_type) !=0)
	{
		MessageBox(NULL, "��¼����ʧ�ܣ�", NULL, NULL);
		return -2;
	}
	if( SMT_WriteBlock(sect,2,buf) !=0 )
	{
		MessageBox(NULL, "д��ʧ�ܣ�", NULL, NULL);
		return -3;
	}
	if( lpCallBack )
		lpCallBack(0);
	//////////////////////////////////////////////////////////////////////////
	// дУ��λ
	sect = 30;
	memset(buf,0,sizeof(buf));
	sprintf((char*)card_key+4,"%02X",sect);
	if(LoginCardSect(sect,card_key,CARD_KEY_A,card_type) !=0)
	{
		MessageBox(NULL, "��¼����ʧ�ܣ�", NULL, NULL);
		return -2;
	}
	crc = CalAuthCardCRC(auth_key_a,auth_key_b,pri_sect,pri_block,pri_startpos);
	buf[11] = (unsigned char)crc;
	if(SMT_WriteBlock(sect,0,buf) != 0)
	{
		MessageBox(NULL, "дУ��λʧ��", NULL, NULL);
	}
	//////////////////////////////////////////////////////////////////////////
	// �� pri_sect ����
	sect = pri_sect;
	memset(buf,0,sizeof(buf));
	memcpy(buf+pri_startpos,auth_key,sizeof(auth_key));
	// modify by Cash 2005-8-8
	// ���ӿ��Ʊ�־λ
	*(buf+pri_startpos+sizeof(auth_key)) = (BYTE)flag;
	sprintf((char*)card_key+4,"%02X",sect);
	// д��Կ
	if(LoginCardSect(sect,card_key,CARD_KEY_A,card_type) !=0)
	{
		MessageBox(NULL, "��¼����ʧ�ܣ�", NULL, NULL);
		return -2;
	}
	if( SMT_WriteBlock(sect,pri_block,buf) !=0 )
	{
		MessageBox(NULL, "д��ʧ�ܣ�", NULL, NULL);
		return -3;
	}
	// д����λ
	memset(buf,0,sizeof(buf));
	memcpy(buf,auth_key_a,sizeof(auth_key_a));
	memcpy(buf+sizeof(auth_key_a),CARD_INIT_CTRL_BIT,
		sizeof(CARD_INIT_CTRL_BIT));
	memcpy(buf+sizeof(auth_key_a)+sizeof(CARD_INIT_CTRL_BIT),
		auth_key_b,sizeof(auth_key_b));
	
	if( SMT_WriteBlock(sect,3,buf) != 0)
	{
		MessageBox(NULL, "д��Կ����ʧ�ܣ�", NULL, NULL);
		return -3;
	}
	
	/*
	if( !Read_Author_Key() )
	{
		AfxMessageBox("��ʼ����Ȩ��ʧ�ܣ�");
		return -4;
	}
	*/
	return 0;
}

/** 
 * @brief ��ӿں�����ʹ����ԿΪ����Ȩ
 * 
 * @param main_key - ��Ȩ��Կ������Ϊ8�ֽ�
 * 
 * @return - �ɹ�����0��ʧ�ܷ���-1
 */
int __stdcall SetCardMainKey(BYTE main_key[8])
{
	if(!Is_Com_open())
	{
		MessageBox(NULL, "��򿪴���!", NULL, NULL);
		return -1;
	}
	memcpy(CARD_MAIN_KEY,main_key,sizeof(CARD_MAIN_KEY));
	SMT_AutoDll(CARD_MAIN_KEY);
	SMT_ControlBuzzer();
	has_read_main_key = TRUE;
	return 0;
}