
// CBFDlg.h: 头文件
//

#pragma once

#define WM_UPDATE_PROGRESS  WM_USER + 1
#define WM_UPDATE_BAD       WM_USER + 2

#define CBF_COPY_BUFFER_SIZE 4096

class CCBFContext
{
public:
	CCBFContext() 
		: strSrcFile(_T(""))
		, strDstFile(_T(""))
		, bRunning(FALSE)
		, nDone(0L)
		, nBad(0L)
		, dwLastError(0)
		, hwOwner(NULL)
		, eState(THREAD_END_STATE_OK)

	{
		nFileSize.QuadPart = 0;
		ZeroMemory(Buffer, sizeof(Buffer));
	}

	void Reset()
	{
		ZeroMemory(Buffer, sizeof(Buffer));
		nFileSize.QuadPart = 0;
		eState = THREAD_END_STATE_OK;
		hwOwner = NULL;
		dwLastError = 0;
		nBad = nDone = 0;
		bRunning = FALSE;
		strDstFile = _T("");
		strSrcFile = _T("");
	}

	const CString & GetStateStr()
	{
		strState = _T("UNKNOWN");
		switch (eState) {
		case THREAD_END_STATE_OK:
			strState.LoadString(IDS_THREAD_END_STATE_OK);
			break;
		case THREAD_END_STATE_OPEN_SRC_ERROR:
			strState.LoadString(IDS_THREAD_END_STATE_OPEN_SRC_ERROR);
			break;
		case THREAD_END_STATE_OPEN_DST_ERROR:
			strState.LoadString(IDS_THREAD_END_STATE_OPEN_DST_ERROR);
			break;
		case THREAD_END_STATE_WRITE_DST_ERROR:
			strState.LoadString(IDS_THREAD_END_STATE_WRITE_DST_ERROR);
			break;
		case THREAD_END_STATE_GET_SRC_SIZE_ERROR:
			strState.LoadString(IDS_THREAD_END_STATE_GET_SRC_SIZE_ERROR);
			break;
		case THREAD_END_STATE_DISK_FULL:
			strState.LoadString(IDS_THREAD_END_STATE_DISK_FULL);
			break;
		case THREAD_END_STATE_USER_CANCEL:
			strState.LoadString(IDS_THREAD_END_STATE_USER_CANCEL);
			break;
		default:
			break;
		}
		return strState;
	}

	enum THREAD_END_STATE {
		THREAD_END_STATE_OK = 0,
		THREAD_END_STATE_OPEN_SRC_ERROR,
		THREAD_END_STATE_OPEN_DST_ERROR,
		THREAD_END_STATE_WRITE_DST_ERROR,
		THREAD_END_STATE_GET_SRC_SIZE_ERROR,
		THREAD_END_STATE_DISK_FULL,
		THREAD_END_STATE_USER_CANCEL
	};

public:
	CString       strSrcFile;
	CString       strDstFile;
	BOOL          bRunning;
	LARGE_INTEGER nFileSize;
	LONGLONG      nDone;
	LONGLONG      nBad;
	DWORD         dwLastError;
	HWND		  hwOwner;
	BYTE          Buffer[CBF_COPY_BUFFER_SIZE];
	THREAD_END_STATE eState;

private:
	CString       strState;
};

// CCBFDlg 对话框
class CCBFDlg : public CDialog
{
// 构造
public:
	CCBFDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBF_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateProgress(WPARAM wProgress, LPARAM lDone);
	afx_msg LRESULT OnUpdateBad(WPARAM wLength, LPARAM lJunk);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSrcFile();
	afx_msg void OnBnClickedButtonDstFile();
	afx_msg void OnBnClickedButtonStartStop();
protected:
	CString m_strSrcFile;
	CString m_strDstFile;
	CString m_strSrcFileName;
	CListCtrl m_ctlErrorList;
	HANDLE m_hThread;
	static DWORD WINAPI fnCopyFunction(_In_ LPVOID lpParameter);
	static void CCBFDlg::ReadFileAnyway(
		HANDLE hFile,
		LONGLONG nOffset,
		LPBYTE Buffer,
		DWORD dwSize,
		DWORD* dwRead,
		DWORD* dwBad,
		BOOL* bCancel
	);
	static void FormatError(DWORD dwError, CString& strRet);
	CButton m_btnStartStop;
	CCBFContext  m_Context;
	CProgressCtrl m_ctlProgress;
};
