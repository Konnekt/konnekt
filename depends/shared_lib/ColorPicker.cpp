
#include <windows.h>
#include <stdlib.h>
#define FUNC_NOTHREAD
#include "func.h"
#pragma hdrstop

#include "ColorPicker.h"

COLORREF CustColor [16];

void CColorPicker::ChooseColor() {
    CHOOSECOLOR cc;
    memset(&cc , 0 , sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner=_hwnd;
    cc.rgbResult = _color;
    cc.lpCustColors=CustColor;
    cc.Flags=CC_ANYCOLOR|CC_FULLOPEN|CC_RGBINIT;
    if (::ChooseColor(&cc)) {
        _color = cc.rgbResult;
        Refresh();
        SendMessage(GetParent(this->_hwnd) , WM_COMMAND , MAKELPARAM((WORD)GetMenu(_hwnd) , EN_UPDATE) , (LPARAM)_hwnd);
        SendMessage(GetParent(_hwnd) , WM_COLORPICKER_CHANGE , 0 , 0);
    }
}


void CColorPicker::Refresh() {
    RedrawWindow(_hwnd,0,0,RDW_UPDATENOW|RDW_INVALIDATE);
}

int CALLBACK CColorPickerWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
//   static char str_buff2 [MAX_STRBUFF];
   CColorPicker * CP;
   switch (message)
   {
        case WM_SETTEXT:
            if (!lParam) return 0;
            CP = (CColorPicker*)GetProp(hwnd , "CColorPicker");
            CP->_color = atoi((char*)lParam);
            CP->Refresh();
            return 1;
        case WM_GETTEXT:
            if (!lParam) return 0;
            CP = (CColorPicker*)GetProp(hwnd , "CColorPicker");
            itoa(CP->_color , (char*)lParam , 10);
            return strlen((char*)lParam);
        case WM_CLOSE:
                DestroyWindow(hwnd);
                return 0;
        case WM_NCDESTROY:
                CP = (CColorPicker*)GetProp(hwnd , "CColorPicker");
                delete CP;
                return 0;
        case WM_PAINT: {
                CP = (CColorPicker*)GetProp(hwnd , "CColorPicker");
                PAINTSTRUCT ps; 
                HDC hdc = BeginPaint(hwnd, &ps);
                char buff [10];
                char * ch = buff+1;
                CP->_color &=0x00FFFFFF;
                itoa(CP->_color , ch , 16);
                int count = strlen(ch);
                if (count < 6) {
                    for (int i=5; i>=6-count; i--) {
                        ch[i]=ch[i - (6 - count)];
                    }
                  memset(ch , '0' , 6 - count);
                }
                memcpy(buff+7,buff+1,2);
                memcpy(buff+1,buff+5,2);
                memcpy(buff+5,buff+7,2);
                buff[0] = '#';
                buff[7] = 0;
                _strupr(buff);
                RECT rc;
                GetClientRect(hwnd , &rc);
                HBRUSH hBrush = CreateSolidBrush((COLORREF)CP->_color);
                FillRect(hdc , &rc , hBrush);
                SelectObject(hdc , CP->_font);
//                bool bright = GetRValue(CP->_color)>127||GetGValue(CP->_color)>127||GetBValue(CP->_color)>127;
                bool bright = (GetRValue(CP->_color)+GetGValue(CP->_color)*1.5+GetBValue(CP->_color)) > 450;
                SetBkMode(hdc , TRANSPARENT);
                RECT rc2 = rc;
                SetTextColor(hdc , (!bright)?RGB_ALPHA(0 , CP->_color , 50):RGB_ALPHA(0xFFFFFF , CP->_color , 50));
                OffsetRect(&rc2 , (!bright)?1:-1 , (!bright)?1:-1);
                DrawText(hdc, buff , -1 , &rc2 , DT_CENTER | DT_VCENTER | DT_SINGLELINE); 
                SetTextColor(hdc , (bright)?RGB(0,0,30):RGB(255,255,255));
                DrawText(hdc, buff , -1 , &rc , DT_CENTER | DT_VCENTER | DT_SINGLELINE); 
                DeleteObject(hBrush);
                EndPaint(hwnd, &ps); 
                return 0;
              }

        case WM_LBUTTONUP:
            CP = (CColorPicker*)GetProp(hwnd , "CColorPicker");
            CP->ChooseColor();
            return 0;
/*        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                   DestroyWindow(hwnd);
                   break;
                case IDYES:
                   ServerEventDialogFill();
            }
            return 1;
  */
/*        case WM_NOTIFY:
             NMHDR * nm;nm=(NMHDR*)lParam;
             switch (nm->code) {
             }
             return 0;
             */
   }
   CP = (CColorPicker*)GetProp(hwnd , "CColorPicker");
   return CallWindowProc(CP->_subclass , hwnd , message , wParam , lParam); 
}

// ------------------------------------------------------------------

CColorPicker::CColorPicker(DWORD color , DWORD style , int x , int y , int w , int h , HWND parent , HMENU id , HINSTANCE hInst , LPARAM param) {
   _font=GetLogFont("Tahoma" , 11 , 0);
   _subclass = 0;
   _color = color;
   _hwnd = CreateWindowEx(0 , "STATIC" , "" , SS_SUNKEN | SS_NOTIFY | style , x , y , w , h , parent , id , hInst , (LPVOID)param);
   SetProp(_hwnd , "CColorPicker" , this);
   _subclass = (WNDPROC) SetWindowLong(_hwnd , GWL_WNDPROC , (LONG)CColorPickerWndProc);
   Refresh();
}
CColorPicker::~CColorPicker() {

}
