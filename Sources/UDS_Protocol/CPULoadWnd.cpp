#include "CPULoadWnd.h"

#include "DIL_Interface/BaseDIL_CAN.h"

static DWORD g_dwClientID = 0;
static CBaseDIL_CAN* g_pouDIL_CAN_Interface;

CCPULoadWnd* CCPULoadWnd::m_spodInstance = NULL;

IMPLEMENT_DYNAMIC(CCPULoadWnd, CDialog)

CCPULoadWnd::CCPULoadWnd( CWnd* pParent)
	:CDialog(CCPULoadWnd::IDD, pParent)
{

}

CCPULoadWnd::~CCPULoadWnd(void)
{
}

void CCPULoadWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_InsertCycleTime, m_CycleTime);
}

BEGIN_MESSAGE_MAP(CCPULoadWnd, CDialog)
	ON_BN_CLICKED(ID_Start_Button, OnBnClickedStart)
	ON_BN_CLICKED(ID_Stop_Button, OnBnClickedStop)
END_MESSAGE_MAP()


BOOL CCPULoadWnd::OnInitDialog(){
	CDialog::OnInitDialog();
	m_CycleTime.vSetBase(BASE_HEXADECIMAL);
	m_CycleTime.vSetValue(0);
	m_CycleTime.SetLimitText(4);
	return TRUE;
}


void CCPULoadWnd::evaluateValue(int ActualValue){

  if (ActualValue < CPULoad_min)
    {
        CPULoad_min = ActualValue;
        maxOc = 1;				// This variable is used to indicate how many messages with a lower idle has been received  
        FirstTimeMinValue = this.TIME;

        TimeValMin = FirstTimeMinValue ;

        maxOcTime = (FirstTimeMinValue/100000);

        write("Min value %d", CPULoad_min);
        //write("valoare lu timpu al I %2f cu CPULoad_min %d", maxOcTime, CPULoad_min);        
      
    }
    else if (ActualValue == CPULoad_min)
        {
            if (maxOc==0)
            {
                TimeValMin = this.TIME;
            }else {
                //do nothing
            }
            
            maxOc++;
        }
        else if (ActualValue > CPULoad_min)
            {
                if(maxOc > OcuranceMin)
                {
                    OcuranceMin = maxOc;
                    FirstTimeMinValue = TimeValMin;
                    maxOcTime = (FirstTimeMinValue/100000);

                    maxOc = 0;
                    //write("valoare lu timpu al II %2f cu intervalu %d", maxOcTime, OcuranceMin);        

                }else{
                        maxOc = 0;
                    }
            }
    

    MaximumTimeWithMinimValues = (OcuranceMin*getvalue(evCycleTime));


    if (ActualValue > max)
    {
        max = ActualValue;
    }


    if(contor < 50000 )
    {
        
        average = (((contor-1)*average)+ActualValue)/contor;  

        contor++;
    }
    else
    {
       write("Counter full !!!!");
    }

/*
    Checking if the value to be checked is comming, and counting it
*/
    if (ActualValue <= getvalue(evCheckValue))
    {

        CheckValueCounter++;        
        CheckValueTime = this.TIME/100;

        fCheckWindowsMoving();

        fWriteCheckValueInFile(CheckValueTime,ActualValue);

    }else{
        //nothing       
    }

// Put Values in Envars
    putvalue(evValueMin,CPULoad_min);
    putvalue(evValueMax,max);
    putvalue(evValueAverage,average);
    putvalue(evMaxTimeMinValue,MaximumTimeWithMinimValues);
    putvalue(evTimeMaxValueMinTime, maxOcTime);
    putvalue(evCounterCheckValue, CheckValueCounter);
    
}



void CCPULoadWnd::OnBnClickedStart(){
	UpdateData();
	psTxCanMsgCPULoad = NULL;
	if (psTxCanMsgCPULoad ==NULL) 	psTxCanMsgCPULoad  = new mSTXSELMSGDATA;
	if (psTxCanMsgCPULoad!= NULL){
		psTxCanMsgCPULoad->m_unCount=1;
		psTxCanMsgCPULoad->m_psTxMsg = new STCAN_MSG[1];
		if(psTxCanMsgCPULoad->m_psTxMsg != NULL )
		{
			psTxCanMsgCPULoad->m_psTxMsg->m_ucChannel=1;
			psTxCanMsgCPULoad->m_psTxMsg->m_bCANFD= FALSE; 
			psTxCanMsgCPULoad->m_psTxMsg->m_ucDataLen = 8;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucEXTENDED = FALSE;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucRTR = FALSE;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[0] = 0x40;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[1] = 0x06;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[2] = 0xBF;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[3] = 0xFF;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[4] = 0x77;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[5] = 0x01;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[6] = (int)(m_CycleTime.lGetValue())>>8;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[7] = (int) m_CycleTime.lGetValue();
			psTxCanMsgCPULoad->m_psTxMsg->m_unMsgID = 0x6F1;
			CWinThread* pomThread = NULL ;
			pomThread = AfxBeginThread( OnSendSelectedMsg, psTxCanMsgCPULoad );

		}
	}

}

void CCPULoadWnd::OnBnClickedStop(){
	UpdateData();
	psTxCanMsgCPULoad = NULL;
	if (psTxCanMsgCPULoad ==NULL) 	psTxCanMsgCPULoad  = new mSTXSELMSGDATA;
	if (psTxCanMsgCPULoad!= NULL){
		psTxCanMsgCPULoad->m_unCount=1;
		psTxCanMsgCPULoad->m_psTxMsg = new STCAN_MSG[1];
		if(psTxCanMsgCPULoad->m_psTxMsg != NULL )
		{
			psTxCanMsgCPULoad->m_psTxMsg->m_ucChannel=1;
			psTxCanMsgCPULoad->m_psTxMsg->m_bCANFD= FALSE; 
			psTxCanMsgCPULoad->m_psTxMsg->m_ucDataLen = 6;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucEXTENDED = FALSE;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucRTR = FALSE;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[0] = 0x40;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[1] = 0x04;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[2] = 0xBF;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[3] = 0xFF;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[4] = 0x77;
			psTxCanMsgCPULoad->m_psTxMsg->m_ucData[5] = 0xFF;
			//psTxCanMsgCPULoad->m_psTxMsg->m_ucData[6] = m_CycleTime >> 8;
			//psTxCanMsgCPULoad->m_psTxMsg->m_ucData[7] = m_CycleTime;
			psTxCanMsgCPULoad->m_psTxMsg->m_unMsgID = 0x6F1;
			CWinThread* pomThread = NULL ;
			pomThread = AfxBeginThread( OnSendSelectedMsg, psTxCanMsgCPULoad );

		}
	}

}


/***********************************************************************************************************/
UINT CCPULoadWnd::OnSendSelectedMsg(LPVOID pParam)
{
	// s_omState.ResetEvent();
	mPSTXSELMSGDATA psTxCanMsgCPULoad = static_cast <mPSTXSELMSGDATA> (pParam);
	int nReturn = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, psTxCanMsgCPULoad->m_psTxMsg[0]);
	if (nReturn != S_OK)
	{
		// Message not sent.  
	}
	return 0;
}

/***********************************************************************************************************/

void CCPULoadWnd::vSetDILInterfacePtr(void* ptrDILIntrf){

	g_pouDIL_CAN_Interface = (CBaseDIL_CAN*)ptrDILIntrf;
	g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _T("CAN_MONITOR"));
	//g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, g_dwClientID, &m_ouMCCanBufFSE);
}
/***********************************************************************************************************/
void* CCPULoadWnd::pGetDILInterfacePtr()
{
	return (void*)g_pouDIL_CAN_Interface;
}

/***********************************************************************************************************/

CCPULoadWnd* CCPULoadWnd::s_podGetUdsMsgManager()
{
	if( m_spodInstance == NULL)	 {
	//CWnd objParent;
	//objParent.Attach(hParent);

		m_spodInstance =  new CCPULoadWnd(NULL);	   
		/*	m_spodInstance->objPrue =1;*/								   
		// Handling NULL condition is caller's duty
		if( m_spodInstance == NULL )
		{
			// Help debugging
			ASSERT( FALSE );
		}
	}
	// Return the pointer or NULL in case of failure
	return m_spodInstance;
}