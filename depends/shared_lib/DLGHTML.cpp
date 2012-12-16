// DLGHTML.cpp: implementation of the CHtmlDialog class.
//
//////////////////////////////////////////////////////////////////////

#include <mshtmhst.h>
#include <comdef.h>

#include "DLGHTML.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlDialog::CHtmlDialog()
{
}

CHtmlDialog::~CHtmlDialog()
{
	//Free the Mshtml.dll
	::FreeLibrary(m_hInstMSHTML);
}

CHtmlDialog::CHtmlDialog(UINT nResID, HWND pParent/*= NULL*/)
{
	m_hWndParent = pParent;

	CString strTemp;
	strTemp.Format("%d", nResID);
	
	ResourceToURL(strTemp);
	CommonConstruct();
}

CHtmlDialog::CHtmlDialog(LPCTSTR lpszURL, BOOL bRes, HWND pParent/*= NULL*/)
{
	m_hWndParent = pParent;

	if (bRes)
	{
		ResourceToURL(lpszURL);
	}
	else
		m_strURL = lpszURL;

	CommonConstruct();
}

int CHtmlDialog::DoModal(bool modal , DWORD flags)
{
	//First get the proc ShowHTMLDialog
	SHOWHTMLDIALOGEXFN  *pfnShowHTMLDialogEx;

	pfnShowHTMLDialogEx = (SHOWHTMLDIALOGEXFN*)GetProcAddress(m_hInstMSHTML, TEXT("ShowHTMLDialogEx"));
	
	if (!pfnShowHTMLDialogEx)
		return -1;
	

	//Now create a URL Moniker
	IMoniker* pmk = NULL;
	BSTR bstrURL = m_strURL.AllocSysString();
	CreateURLMoniker(NULL, bstrURL, &pmk);
	if (!pmk)
		return -1;
	
	WCHAR* pchOptions = m_strOptions.IsEmpty() ? NULL : m_strOptions.GetBuffer(0);
	
	
	//Now show the HTML Dialog
    HRESULT hr = (*pfnShowHTMLDialogEx)(m_hWndParent, pmk, (modal?HTMLDLG_MODAL:HTMLDLG_MODELESS) | flags , /*m_varArgs*/ 0, pchOptions, /*m_varReturn*/ 0);
	if (FAILED(hr))
		return -1;

	return 0;
}

void CHtmlDialog::CommonConstruct()
{
	m_hInstMSHTML = ::LoadLibrary(TEXT("MSHTML.DLL"));
	
}



inline void CHtmlDialog::ResourceToURL(LPCTSTR lpszURL)
{
	HINSTANCE hInstance = 0; //AfxGetResourceHandle();
	ASSERT(hInstance != NULL);
	
	LPTSTR lpszModule = new TCHAR[_MAX_PATH];
	
	if (GetModuleFileName(hInstance, lpszModule, _MAX_PATH))
	{
		m_strURL.Format(_T("res://%s/%s"), lpszModule, lpszURL);
	}
	
	delete []lpszModule;
}


void CHtmlDialog::SetParam(VARIANT *pvarArgs)
{
//	m_varArgs = pvarArgs;
}

void CHtmlDialog::SetParam(LPCTSTR lpszArgs)
{
//	m_varArgs = lpszArgs;
}

CString CHtmlDialog::GetReturnString()
{
//	ASSERT(m_varReturn.vt == VT_BSTR);
//	CString str = m_varReturn.bstrVal;
//	return str;
    return "";
}

/*LPCVARIANT CHtmlDialog::GetReturnVariant()
{
	return (LPCVARIANT)m_varReturn;
}
  */
void CHtmlDialog::SetSize(int x, int y)
{
	m_strOptions.Format(L"dialogWidth: %d; dialogHeight: %d", x, y);
}

void CHtmlDialog::SetDlgOptions(LPCTSTR lpszOptions)
{
	m_strOptions = lpszOptions;
}
