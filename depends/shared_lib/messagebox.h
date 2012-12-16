#ifndef MESSAGEBOX_TIMEOUT_H
#define MESSAGEBOX_TIMEOUT_H

#define IDCLOSED_BY_TIMEOUT	  50

int MessageBoxTimeout(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, UINT uElapse, UINT uBlinkingTime=0);



#endif