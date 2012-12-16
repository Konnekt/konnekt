// DLGHTML.h: interface for the CHtmlDialog class.
//
//////////////////////////////////////////////////////////////////////

#include <stdstring.h>
#define CString CStdString

#if !defined(AFX_DLGHTML_H__00C54337_702E_11D2_ABF7_CCFAF0973072__INCLUDED_)
#define AFX_DLGHTML_H__00C54337_702E_11D2_ABF7_CCFAF0973072__INCLUDED_

#pragma once

class CHtmlDialog  
{
public:
	void SetDlgOptions(LPCTSTR lpszOptions);
	void SetSize(int x, int y);
	//LPCVARIANT GetReturnVariant();
	CString GetReturnString();
	void SetParam(LPCTSTR lpszArgs);
	void SetParam(VARIANT* pvarArgs);
	virtual int DoModal(bool modal , DWORD flags = 0);
	CHtmlDialog(LPCTSTR lpszURL, BOOL bRes = FALSE, HWND pParent = NULL);
	CHtmlDialog(UINT nResID, HWND pParent = NULL);
	CHtmlDialog();
	virtual ~CHtmlDialog();

private:
	inline void ResourceToURL(LPCTSTR lpszURL);
	void CommonConstruct();
protected:
	HWND m_hWndParent;
	CStdStringW m_strOptions;
	//COleVariant m_varArgs, m_varReturn;
	CString m_strURL;
	HINSTANCE m_hInstMSHTML;
};

#endif // !defined(AFX_DLGHTML_H__00C54337_702E_11D2_ABF7_CCFAF0973072__INCLUDED_)
