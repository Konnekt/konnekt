#ifndef WINLISTVIEW_H
#define WINLISTVIEW_H

LPARAM ListView_GetItemData(HWND hwnd , int pos);


int ListView_ItemFromPoint(HWND hwnd , POINT pt);
int ListView_Deselect(HWND hwnd);
int ListView_GetSelItems(HWND hwnd , int count , int * buff);
int ListView_SetCurSel (HWND hwnd , int pos);
int ListView_AddString(HWND hwnd , const char * txt , int image = 0 , LPARAM param = 0);
int ListView_SetString(HWND hwnd , int item , int subitem , char * txt);
int ListView_AddColumn(HWND hwnd , const char * txt , int cx);
int ListView_MoveItem(HWND hwnd , int pos , int newPos);
#endif