#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>
#include <string>
#include "func.h"
#include "win_registry.h"
#include "win_listview.h"

std::string RegQuerySZ(HKEY & hKey , const char * name) {
   unsigned long type,count=MAX_STRBUFF;
   if (!RegQueryValueEx(hKey,(LPCTSTR)name,0,&type,(LPBYTE)TLS().buff,&count))
     if (type==REG_SZ)
       return TLS().buff;
   return "";
}

DWORD RegQueryDW(HKEY & hKey , const char * name , DWORD def) {
   unsigned long type,count=sizeof(DWORD);
   if (!RegQueryValueEx(hKey,name,0,&type,(LPBYTE)TLS().buff,&count))
     if (type==REG_DWORD)
       return *((DWORD*)TLS().buff);
   return def;
}


long RegSetSZ(HKEY & hKey , const char * name , std::string val) {
 return RegSetValueEx(hKey,name,0,REG_SZ,(LPBYTE)val.c_str(),val.size()+1);
 
}

long RegSetDW(HKEY & hKey , const char * name , DWORD val) {
 return RegSetValueEx(hKey,name,0,REG_DWORD,(LPBYTE)&val,sizeof(DWORD));
}





// ----------------------------------------------------------------------
LPARAM ListView_GetItemData(HWND hwnd , int pos) {
  if (pos<0) return 0;
  LVITEM lvi;
  lvi.mask = LVIF_PARAM;
  lvi.iItem = pos;
  lvi.iSubItem = 0;
  if (ListView_GetItem(hwnd , &lvi))
    return lvi.lParam;
    else
    return 0;
}


int ListView_ItemFromPoint(HWND hwnd , POINT pt) {
   LVHITTESTINFO vhti;
   vhti.pt = pt;
   ListView_HitTest(hwnd , &vhti);
   return vhti.iItem;
}

int ListView_Deselect(HWND hwnd) {
   int pos;
   int c = ListView_GetItemCount(hwnd);
//   while ((pos = ListView_GetSelectionMark(hwnd))>=0)
   for (pos = 0; pos < c; pos++)
     ListView_SetItemState(hwnd , pos , 0 , LVIS_SELECTED);
   return 1;
}


int ListView_GetSelItems(HWND hwnd , int count , int * buff) {
   int i = 0;
   int sc = ListView_GetSelectedCount(hwnd);
   int c = ListView_GetItemCount(hwnd);
   for (int j = 0; j < c; j++) {
     if (ListView_GetItemState(hwnd , j , LVIS_SELECTED)) {
       buff[i]=j;
       i++;
       if (i>=count || i>=sc) return i;
     }
   }
   return i;
}

int ListView_SetCurSel (HWND hwnd , int pos) {
   ListView_Deselect(hwnd);
   ListView_SetItemState(hwnd , pos , LVIS_SELECTED , LVIS_SELECTED);
   ListView_SetSelectionMark(hwnd , pos);
   ListView_SetHotItem(hwnd , pos);
   ListView_EnsureVisible(hwnd , pos , 0);
   return 1;
}

int ListView_AddString(HWND hwnd , const char * txt , int image , LPARAM param ) {
   LVITEM lvi;
   lvi.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE;
   lvi.iItem = 0xFFFF;
   lvi.iSubItem = 0;
   lvi.lParam = param;
   lvi.iImage = image;
   lvi.pszText = (char*)txt;
   return ListView_InsertItem(hwnd , &lvi);
}

int ListView_SetString(HWND hwnd , int item , int subitem , char * txt) {
   LVITEM lvi;
   lvi.mask = LVIF_TEXT;
   lvi.iItem = item;
   lvi.iSubItem = subitem;
   lvi.pszText = txt;
   ListView_SetItem(hwnd , &lvi);
   return 1;
}


int ListView_AddColumn(HWND hwnd , const char * txt , int cx) {
    LVCOLUMN lvc;
    lvc.mask = LVCF_TEXT | LVCF_WIDTH;
    lvc.iSubItem = 0;
    lvc.pszText = (char*)txt;
    lvc.cx = cx;
    return ListView_InsertColumn(hwnd, 0x7FFF, &lvc);
//    ListView_SetColumnWidth(hwnd , 0 , LVSCW_AUTOSIZE_USEHEADER);
}

int ListView_MoveItem(HWND hwnd , int pos , int newPos) {
    char buff [2000];
    int cols = Header_GetItemCount(ListView_GetHeader(hwnd));
    LVITEM lvi;
    lvi.mask = LVIF_IMAGE|LVIF_INDENT|LVIF_PARAM|LVIF_TEXT|LVIF_STATE;
    lvi.iItem = pos;
    lvi.iSubItem = 0;
    lvi.stateMask= (UINT)-1;
    lvi.pszText = buff;
    lvi.cchTextMax = 2000;
    ListView_GetItem(hwnd , &lvi);
    int check = ListView_GetCheckState(hwnd , pos);

    if (newPos>pos) newPos++; else pos++;
    lvi.iItem = newPos;
    newPos = ListView_InsertItem(hwnd , &lvi);
    ListView_SetCheckState(hwnd , newPos , check);

    lvi.mask = LVIF_TEXT;
    for (int i=1; i<cols; i++) {
      lvi.iSubItem=i;
      lvi.iItem=pos;
      ListView_GetItem(hwnd , &lvi);
      lvi.iItem=newPos;
      ListView_SetItem(hwnd , &lvi);
    }
//    ListView_SetSelectionMark(hwnd , newPos);
    ListView_EnsureVisible(hwnd , newPos , 0);
    ListView_DeleteItem(hwnd , pos);
    return 1;
}

