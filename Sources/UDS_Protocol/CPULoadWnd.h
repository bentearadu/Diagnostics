#pragma once
#include "afxwin.h"
#include "UDS_Resource.h"
#include "UDSWnd_Defines.h"
#include "UDSMainWnd.h"

class CCPULoadWnd :	public CDialog
{
	DECLARE_DYNAMIC(CCPULoadWnd) ;

public:
	CCPULoadWnd(CWnd* pParent =NULL);
	~CCPULoadWnd(void);

	int CPULoad_min;

	CRadixEdit m_CycleTime;

	static CCPULoadWnd* m_spodInstance;
	CWnd* pomGetTxMsgViewPointers() const;

	void CCPULoadWnd::OnBnClickedStart();
	void CCPULoadWnd::OnBnClickedStop();

		/** Structure that contains the message to be sent    */
   	mPSTXSELMSGDATA psTxCanMsgCPULoad;
												
	void vSetDILInterfacePtr(void* ptrDILIntrf);
	void* pGetDILInterfacePtr();
	static CCPULoadWnd* s_podGetUdsMsgManager();
	void evaluateValue(int ActualValue);
	enum { IDD = IDD_CPULoadBMWBDC };

protected: 
	BOOL CCPULoadWnd::OnInitDialog();
	static UINT OnSendSelectedMsg(LPVOID pParam);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
