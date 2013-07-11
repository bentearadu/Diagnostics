#if !defined DILI_UDS_Protocol_H__INCLUDED_
#define DILI_UDS_Protocol_H__INCLUDED_

class DILI_UDS_Protocol						//  Cambiar Nombre, poner una C adelante
{
public:
	DILI_UDS_Protocol(void);
	~DILI_UDS_Protocol(void);
	BOOL InitInstance(void);

	
	 virtual HRESULT DisplayUdsMainWnd(HWND hParent);
};
#endif //DILI_UDS_Protocol_H__INCLUDED_