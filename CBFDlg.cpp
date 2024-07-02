
// CBFDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CBF.h"
#include "CBFDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCBFDlg 对话框



CCBFDlg::CCBFDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CBF_DIALOG, pParent)
	, m_strSrcFile(_T(""))
	, m_strDstFile(_T(""))
	, m_hThread(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC_FILE, m_strSrcFile);
	DDX_Text(pDX, IDC_EDIT_DST_FILE, m_strDstFile);
	DDX_Control(pDX, IDC_LIST_ERROR, m_ctlErrorList);
	DDX_Control(pDX, IDC_BUTTON_START_STOP, m_btnStartStop);
	DDX_Control(pDX, IDC_PROGRESS_TRANSFER, m_ctlProgress);
}

BEGIN_MESSAGE_MAP(CCBFDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UPDATE_PROGRESS, OnUpdateProgress)
	ON_MESSAGE(WM_UPDATE_BAD, OnUpdateBad)
	ON_BN_CLICKED(IDOK, &CCBFDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCBFDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SRC_FILE, &CCBFDlg::OnBnClickedButtonSrcFile)
	ON_BN_CLICKED(IDC_BUTTON_DST_FILE, &CCBFDlg::OnBnClickedButtonDstFile)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP, &CCBFDlg::OnBnClickedButtonStartStop)
END_MESSAGE_MAP()


// CCBFDlg 消息处理程序

BOOL CCBFDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_ctlProgress.SetRange(0, 100);
	CString strCol;
	strCol.LoadString(IDS_DATA_RANGE);
	m_ctlErrorList.InsertColumn(0, strCol, 0, 300);
	strCol.LoadString(IDS_CORRUPT_LENGTH);
	m_ctlErrorList.InsertColumn(1, strCol, 0, 100);
	strCol.LoadString(IDS_START);
	m_btnStartStop.SetWindowText(strCol);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCBFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCBFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCBFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCBFDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// CDialog::OnOK();
}


void CCBFDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CCBFDlg::OnBnClickedButtonSrcFile()
{
	CFileDialog dlgOpen(TRUE);
	if (dlgOpen.DoModal() == IDOK) {
		this->m_strSrcFile = dlgOpen.GetPathName();
		this->UpdateData(FALSE);
	}
}


void CCBFDlg::OnBnClickedButtonDstFile()
{
	CFileDialog dlgOpen(FALSE);
	if (dlgOpen.DoModal() == IDOK) {
		this->m_strDstFile = dlgOpen.GetPathName();
		this->UpdateData(FALSE);
	}
}

void CCBFDlg::FormatError(DWORD dwError, CString & strRet)
{
	LPTSTR lpMsgBuf = NULL;
	::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	strRet = lpMsgBuf;

	LocalFree(lpMsgBuf);
}

void CCBFDlg::ReadFileAnyway(
	HANDLE hFile,
	LONGLONG nOffset,
	LPBYTE Buffer,
	DWORD dwSize,
	DWORD* dwRead,
	DWORD* dwBad,
	BOOL * bRunning
)
{
	BYTE buf[512]; /* once a sector...*/
	LARGE_INTEGER nIndex;
	DWORD dwJunk = 0, dwIndex = 0, dwRealWant = 0;
	nIndex.QuadPart = nOffset;
	*dwRead = 0;
	*dwBad = 0;

	ZeroMemory(Buffer, dwSize);

	/* seek to location */
	if (!::SetFilePointerEx(hFile, nIndex, NULL, FILE_BEGIN)) {
		*dwBad = dwSize;
		*dwRead = dwSize;
		nIndex.QuadPart = nOffset +  dwSize;
		::SetFilePointerEx(hFile, nIndex, NULL, FILE_BEGIN);
		return;
	}

	/* read once a sector */
	while (dwSize != (*dwRead) && (*bRunning)) {
		dwRealWant = dwSize - (*dwRead);
		if (dwRealWant > sizeof(buf))
			dwRealWant = sizeof(buf);
		if (::ReadFile(hFile, &buf, sizeof(buf), &dwJunk, NULL)) {
			CopyMemory(Buffer + (*dwRead), buf, dwJunk);
			*dwRead += dwJunk;
		}
		else {
			ZeroMemory(buf, sizeof(buf));
			CopyMemory(Buffer + (*dwRead), buf, dwRealWant);
			*dwRead += dwRealWant;
			(*dwBad)+= dwRealWant;
			nIndex.QuadPart = nOffset + *dwRead;
			::SetFilePointerEx(hFile, nIndex, NULL, FILE_BEGIN);
		}
	}
}

LRESULT  CCBFDlg::OnUpdateProgress(WPARAM wProgress, LPARAM lDone)
{
	CString strCaption;
	if (!lDone) {
		m_ctlProgress.SetPos((INT)wProgress);
		strCaption.Format(_T("%d%%"), (INT)wProgress);
		this->SetWindowText((LPCTSTR)strCaption);
	}
	else {
		m_ctlProgress.SetPos(0);
		::WaitForSingleObject(m_hThread, INFINITE);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		strCaption.LoadString(IDS_START);
		m_btnStartStop.SetWindowText(strCaption);
		m_btnStartStop.EnableWindow(TRUE);
		if(m_Context.eState == CCBFContext::THREAD_END_STATE_OK)
			AfxMessageBox(IDS_THREAD_END_STATE_OK, MB_OK | MB_ICONINFORMATION);
		else {
			CString strTemp = m_Context.GetStateStr();
			CString strError;
			FormatError(m_Context.dwLastError, strError);
			strTemp += _T(": ");
			strTemp += strError;
			AfxMessageBox(strTemp, MB_OK);
		}
		GetDlgItem(IDC_EDIT_SRC_FILE)->EnableWindow();
		GetDlgItem(IDC_EDIT_DST_FILE)->EnableWindow();
		GetDlgItem(IDC_BUTTON_SRC_FILE)->EnableWindow();
		GetDlgItem(IDC_BUTTON_DST_FILE)->EnableWindow();
		m_Context.Reset();
	}
	return 0;
}

LRESULT CCBFDlg::OnUpdateBad(WPARAM wLength, LPARAM lJunk)
{
	CString strOffset, strLength;
	INT nItem = m_ctlErrorList.GetItemCount();
	strOffset.Format(_T("0x%016x-0x%016x"), m_Context.nDone, m_Context.nDone + CBF_COPY_BUFFER_SIZE);
	strLength.Format(_T("%u"), wLength);
	m_ctlErrorList.InsertItem(nItem, strOffset);
	m_ctlErrorList.SetItemText(nItem, 1, strLength);
	return 0;
}

DWORD WINAPI CCBFDlg::fnCopyFunction(
	_In_ LPVOID lpParameter
)
{
	CCBFContext* pContext = (CCBFContext*)lpParameter;
	HANDLE hSrcFile = INVALID_HANDLE_VALUE, hDstFile = INVALID_HANDLE_VALUE;
	LONGLONG nLeft = 0;
	DWORD dwRead = 0, dwWrite = 0, dwBad = 0;
	INT nLastProgress = 0, nNewProgress = 0;

	hSrcFile = ::CreateFile(
		(LPCTSTR)pContext->strSrcFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
		NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE) {
		pContext->eState = CCBFContext::THREAD_END_STATE_OPEN_SRC_ERROR;
		pContext->dwLastError = ::GetLastError();
		goto error;
	}
	hDstFile = ::CreateFile(
		(LPCTSTR)pContext->strDstFile,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hDstFile == INVALID_HANDLE_VALUE) {
		pContext->eState = CCBFContext::THREAD_END_STATE_OPEN_DST_ERROR;
		pContext->dwLastError = ::GetLastError();
		goto error;
	}

	/* get src size */
	if (!::GetFileSizeEx(hSrcFile, &(pContext->nFileSize))) {
		pContext->eState = CCBFContext::THREAD_END_STATE_GET_SRC_SIZE_ERROR;
		pContext->dwLastError = ::GetLastError();
		goto error;
	}
	nLeft = pContext->nFileSize.QuadPart;
	pContext->nDone = 0;
	while (nLeft > 0 && pContext->bRunning) {
		dwRead = sizeof(pContext->Buffer);
		dwBad = 0;
		if (!::ReadFile(hSrcFile, pContext->Buffer, dwRead, &dwRead, NULL)) {
			dwRead = nLeft > sizeof(pContext->Buffer) ? sizeof(pContext->Buffer) : nLeft;
			ReadFileAnyway(hSrcFile, pContext->nDone, pContext->Buffer,
				dwRead, 
				&dwRead,
				&dwBad,
				&pContext->bRunning);
		}

		if (!pContext->bRunning) {
			break;
		}

		if (dwRead > 0) {
			if (::WriteFile(hDstFile, pContext->Buffer, dwRead, &dwWrite, NULL)) {
				if (dwRead == dwWrite) {
					nLeft -= dwRead;
					/*  pContext->nDone is offset, dwBad is length */
					if(dwBad)
						::PostMessage(pContext->hwOwner, WM_UPDATE_BAD, dwBad, 0);
					pContext->nDone += dwRead;
					pContext->nBad += dwBad;
				}
				else {
					/*  read write length mismatch */
					pContext->eState = CCBFContext::THREAD_END_STATE_DISK_FULL;
					pContext->dwLastError = ::GetLastError();
					break;
				}
			}
			else {
				/* write error */
				pContext->eState = CCBFContext::THREAD_END_STATE_WRITE_DST_ERROR;
				pContext->dwLastError = ::GetLastError();
				break;
			}
		}
		else {
			continue;
		}
		/* report progress to hOwner*/
		nNewProgress = (pContext->nDone * 100) / pContext->nFileSize.QuadPart;
		if (nNewProgress != nLastProgress) {
			::PostMessage(pContext->hwOwner, WM_UPDATE_PROGRESS, nNewProgress, 0);
			nLastProgress = nNewProgress;
		}
	}

	if (!pContext->bRunning) {
		pContext->eState = CCBFContext::THREAD_END_STATE_USER_CANCEL;
	} 

	if (pContext->nDone == pContext->nFileSize.QuadPart && pContext->nDone > 0) {
		pContext->eState = CCBFContext::THREAD_END_STATE_OK;
	}

error:
	/* report we have done to hOwner */
	::PostMessage(pContext->hwOwner, WM_UPDATE_PROGRESS, nNewProgress, 1);

	if (hSrcFile != INVALID_HANDLE_VALUE) {
		::CloseHandle(hSrcFile);
	}
	if (hDstFile != INVALID_HANDLE_VALUE) {
		::CloseHandle(hDstFile);
	}
	return 0;
}


void CCBFDlg::OnBnClickedButtonStartStop()
{
	CString strText;
	if (m_Context.bRunning) {
		m_Context.bRunning = FALSE;
		m_btnStartStop.EnableWindow(FALSE);
	}
	else {
		UpdateData();
		m_Context.hwOwner = GetSafeHwnd();
		m_Context.strSrcFile = m_strSrcFile;
		m_Context.strDstFile = m_strDstFile;
		m_hThread = ::CreateThread(NULL, 0, fnCopyFunction, &m_Context, CREATE_SUSPENDED, NULL);
		if (m_hThread != NULL) {
			m_ctlProgress.SetPos(0);
			m_ctlErrorList.DeleteAllItems();
			GetDlgItem(IDC_EDIT_SRC_FILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_DST_FILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SRC_FILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DST_FILE)->EnableWindow(FALSE);
			strText.LoadString(IDS_CANCEL);
			m_btnStartStop.SetWindowText(strText);
			m_Context.bRunning = TRUE;
			::ResumeThread(m_hThread);
		}
		else {
			CString strTemp;
			CString strError;
			strTemp.LoadString(IDS_START_THREAD_ERROR);
			FormatError(::GetLastError(), strError);
			strTemp += _T(": ");
			strTemp += strError;
			AfxMessageBox(strTemp);
		}
	}
}

