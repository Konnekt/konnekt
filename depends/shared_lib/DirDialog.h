////////////////////////////////////////////////////////////////////////
// DirDialog.h: interface for the CDirDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_)
#define AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_

#include <windows.h>
#include <stdstring.h>

class CDirDialog
{
  public:
    CDirDialog();
    virtual ~CDirDialog();
    int DoBrowse();

    CStdString m_strPath;
    CStdString m_strInitDir;
    CStdString m_strSelDir;
    CStdString m_strTitle;
    int  m_iImageIndex;
};

#endif // !defined(AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_)


