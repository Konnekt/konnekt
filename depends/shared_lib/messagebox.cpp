

#include <windows.h>
#include <process.h>
#include <tchar.h>
#include <stdio.h>

#include "messagebox.h"

#define TIME_TO_APPEAR		  250
#define TIME_INTERVAL		  500

//static BOOL bUserIsHere = FALSE;

struct trMyData
{
  DWORD	  CurrentThreadID;
  int	  uElapse;
  int	  uBlinkingTime;
  WNDPROC oldWndProc;
  char * caption;
  char * captionCD; // countdown
};
typedef trMyData cMBoxInfo;

LRESULT CALLBACK MsgBoxWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
/*  if ( (WM_KEYFIRST <= uMsg && uMsg <= WM_KEYLAST) || 
	 (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST) ||
	 (WM_NCMOUSEMOVE <= uMsg && uMsg <= WM_NCMBUTTONDBLCLK) )
	bUserIsHere = TRUE;
*/
    cMBoxInfo * mbi = (cMBoxInfo*)GetProp(hwnd , "cMBoxInfo");  //GetWindowLong(hwnd, GWL_USERDATA);
    switch (message) {
    case WM_TIMER: {
        mbi->uElapse -= TIME_INTERVAL;
        if (mbi->uElapse <= 0) {
            EndDialog(hwnd, IDCLOSED_BY_TIMEOUT);
            return 0;
        }
        // Zmiana tytu³u
        sprintf(mbi->captionCD , " [%d]" , mbi->uElapse / 1000);
        SetWindowText(hwnd , mbi->caption);
        if (mbi->uElapse <= mbi->uBlinkingTime)
	        FlashWindow(hwnd, TRUE);
        return 0;}
    case WM_DESTROY: {
        KillTimer(hwnd , 1);
        break;}
    }
    return CallWindowProc(mbi->oldWndProc, hwnd, message, wParam, lParam);
}

BOOL CALLBACK FindMsgBox(HWND hwnd, LPARAM lParam)
{
  trMyData *prMyData = (trMyData *)lParam;
  TCHAR ClassNameBuf[256];
  BOOL RetVal = TRUE;
  int Ret = GetClassName(hwnd, ClassNameBuf, 100);
  if (!_tcscmp(ClassNameBuf, _T("#32770")))
  { // Znalazl szukane okno... Przechwytujemy je i ustawiamy Timer...
      //SetWindowLong(hwnd, GWL_USERDATA, lParam);
	  SetProp(hwnd , "cMBoxInfo" , (HANDLE)lParam);
      prMyData->oldWndProc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (long)MsgBoxWindowProc);
      SetTimer(hwnd , 1 , TIME_INTERVAL , 0);
      RetVal = FALSE;
  }
  return RetVal;
}


unsigned int _stdcall TimeoutMsgBox(void *pParam)
{
  trMyData *prMyData = (trMyData *)pParam;

  do {
      Sleep(TIME_TO_APPEAR);
//      prMyData->uElapse -= TIME_TO_APPEAR;
      EnumThreadWindows(prMyData->CurrentThreadID, FindMsgBox, (LPARAM)pParam);
  } while (prMyData->uElapse && !prMyData->oldWndProc);


  // Koniec...
  return 0;
}

int MessageBoxTimeout(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, UINT uElapse, UINT uBlinkingTime)
{
  trMyData rMyData;
  rMyData.CurrentThreadID = GetCurrentThreadId();
  rMyData.uElapse = uElapse;
  rMyData.uBlinkingTime = uBlinkingTime;
  rMyData.oldWndProc = 0;
  
  unsigned int uThreadID;
  // Rezerwujemy wiekszy Caption
  rMyData.caption = (char*)malloc(strlen(lpCaption) + 1 + 5);
  strcpy(rMyData.caption , lpCaption);
  rMyData.captionCD = strchr(rMyData.caption , 0);

  // Wlaczamy watek , ktory dopnie sie do messagebox'a
  HANDLE hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, TimeoutMsgBox,	
											(void*)&rMyData, 0, &uThreadID);

  int Res = MessageBox(hWnd, lpText, lpCaption, uType);
  rMyData.uElapse = 0; // Oznaczamy w którym momencie ma siê wy³¹czyæ
  // Czekamy a¿ skoñczy siê w¹tek...
  if (hThreadHandle && hThreadHandle != (HANDLE)-1) {
    WaitForSingleObject( hThreadHandle, INFINITE );
    CloseHandle(hThreadHandle);
  }

  free(rMyData.caption);

  return Res;
}

