#include "DIL_Interface_stdafx.h"
#include "Include/BaseDefs.h"
//#include "UDS_Protocol/UDS_Extern.h"
//#include "UDS_Protocol/Uds_Protocol.h"
#include "DILI_UDS_Protocol.h"

//__declspec(dllimport) HRESULT DIL_UDS_ShowWnd(void);

/**
 * Constructor
 */
DILI_UDS_Protocol::DILI_UDS_Protocol(void)
{
}

/**
 * Destructor
 */
DILI_UDS_Protocol::~DILI_UDS_Protocol(void)
{
}

BOOL DILI_UDS_Protocol::InitInstance(void)
{
    return TRUE;
}


HRESULT DILI_UDS_Protocol::DisplayUdsMainWnd(HWND hParent)     //Esta función llamará a una función definida dentro del proyecto UDS_Protocol
{	
	HRESULT hResult = S_OK;
	return hResult;
	//return DIL_UDS_ShowWnd(hParent);			
}