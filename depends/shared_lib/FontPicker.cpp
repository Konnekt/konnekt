
#include <windows.h>
#include <string>
#define FUNC_NOTHREAD
#include "func.h"
#pragma hdrstop

#include "FontPicker.h"
#include "ColorPicker.h"

UINT_PTR CALLBACK CFHookProc(HWND hwnd,  UINT message, WPARAM wParam, LPARAM lParam);

DWORD PXToHeight(int px) {return 15 * px;}



void CFontPicker::ChooseFont() {
    CHOOSEFONT cf;
    memset(&cf , 0 , sizeof(cf));
    cf.lStructSize = sizeof(cf);
    cf.hwndOwner=_hwnd;
    LOGFONTEX font_copy = _font;
    cf.lpLogFont = &_font.lf;
    cf.lCustData=(LPARAM)this;
    cf.lpfnHook = CFHookProc;
    cf.Flags=CF_INITTOLOGFONTSTRUCT | CF_ENABLEHOOK | /*CF_FORCEFONTEXIST |*/ CF_SCREENFONTS;
    if (::ChooseFont(&cf)) {
        _font.lf = *cf.lpLogFont;
//        if ((_style & CF_NOFACESEL)) strcpy(_font.lf.lfFaceName,"Tahoma");
//        if ((_style & CF_NOSIZESEL)) _font.lf.lfHeight = 120;
//        if (_style & CF_NOCOLORSEL) {_font.color=GetSysColor(COLOR_WINDOWTEXT);_font.bgColor=GetSysColor(COLOR_WINDOW); }
        Refresh();
        SendMessage(GetParent(this->_hwnd) , WM_COMMAND , MAKELPARAM((WORD)GetMenu(_hwnd) , EN_UPDATE) , (LPARAM)_hwnd);
    } else {
      _font = font_copy;
    }

}

void CFontPicker::Refresh() {
    RedrawWindow(_hwnd,0,0,RDW_UPDATENOW|RDW_INVALIDATE);
}

int CALLBACK CFontPickerWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
//   static char str_buff2 [MAX_STRBUFF];
   CFontPicker * FP;
   switch (message)
   {
        case WM_SETTEXT:
            if (!lParam) return 0;
            FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
            FP->_font = StrToLogFont((char*)lParam);
            FP->Refresh();
            return 1;
        case WM_GETTEXT:
            if (!lParam) return 0;
            FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
            strncpy((char*)lParam , LogFontToStr(FP->_font).c_str() ,wParam);
            return strlen((char*)lParam);
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_NCDESTROY:
            FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
            delete FP;
            return 0;
        case WM_PAINT: {
                FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
                PAINTSTRUCT ps; 
                HDC hdc = BeginPaint(hwnd, &ps);
                RECT rc;
                GetClientRect(hwnd , &rc);
                HBRUSH hBrush = CreateSolidBrush(FP->_font.bgColor);
                FillRect(hdc , &rc , hBrush);
                HFONT hFont = CreateFontIndirect(&FP->_font.lf);
                HGDIOBJ oldFont = SelectObject(hdc , hFont);
//                bool bright = GetRValue(CP->_color)>127||GetGValue(CP->_color)>127||GetBValue(CP->_color)>127;
                SetBkMode(hdc , TRANSPARENT);
                SetTextColor(hdc , FP->_font.color);
                DrawText(hdc, FP->_preview.c_str() , -1 , &rc , DT_CENTER | DT_VCENTER | DT_SINGLELINE); 
                DeleteObject(hBrush);
				SelectObject(hdc , oldFont);
                DeleteObject(hFont);
                EndPaint(hwnd, &ps); 
                return 0;
              }

        case WM_LBUTTONUP:
            FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
            FP->ChooseFont();
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
   FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
   return CallWindowProc(FP->_subclass , hwnd , message , wParam , lParam); 
}
UINT_PTR CALLBACK SampleProc(HWND hwnd,  UINT message, WPARAM wParam, LPARAM lParam) {
    CFontPicker * FP;  //0xd   0x87   0xc !!!!!!
    static RECT rcPos = {0,0,0,0};
    switch(message) {
        case WM_COLORPICKER_CHANGE: 
            RedrawWindow(hwnd,0,0,RDW_UPDATENOW|RDW_INVALIDATE);
            return 0;

        
        case WM_PAINT: {
            PAINTSTRUCT ps; 
            if (!rcPos.left) {
                GetChildRect(GetDlgItem(hwnd,0x431) , &rcPos);
                rcPos.right-=15;
                rcPos.top+=20;
                rcPos.left+=15;
                rcPos.bottom-=12;
                
            }
            HDC hdc = BeginPaint(hwnd, &ps);
            if (RectVisible(hdc , &rcPos)) {
                InvalidateRect(hwnd , &rcPos, 0);
                FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
                HBRUSH hBrush = CreateSolidBrush(((CColorPicker*)GetProp(hwnd,"CP2"))->_color);
                SelectObject(hdc , hBrush);
                COLORREF color = ((CColorPicker*)GetProp(hwnd,"CP1"))->_color;
                HPEN hPen1 = CreatePen(PS_SOLID , 0 , RGB_ALPHA(0x0 , color,50));
                color = ((CColorPicker*)GetProp(hwnd,"CP2"))->_color;
                HPEN hPen2 = CreatePen(PS_SOLID , 0 , RGB_ALPHA(0xFFFFFF , color,50));
                LOGFONT lf;
                SendMessage(hwnd , WM_CHOOSEFONT_GETLOGFONT , 0 , (LPARAM)&lf);
                HFONT hFont = CreateFontIndirect(&lf);
                SelectObject(hdc , hFont);
                FillRect(hdc , &rcPos , hBrush);
                SelectObject(hdc , hPen1);
                MoveToEx(hdc , rcPos.left , rcPos.bottom , 0);
                LineTo(hdc , rcPos.left , rcPos.top);
                LineTo(hdc , rcPos.right , rcPos.top);
                SelectObject(hdc , hPen2);
                LineTo(hdc , rcPos.right , rcPos.bottom);
                LineTo(hdc , rcPos.left , rcPos.bottom);
                SetTextColor(hdc , ((CColorPicker*)GetProp(hwnd,"CP1"))->_color);
                SetBkMode(hdc , TRANSPARENT);
                RECT rc = rcPos;
                rc.left+=2;
                rc.top+=2;
                rc.right-=2;
                rc.bottom-=2;
                DrawText(hdc, FP->_preview.c_str() , -1 , &rc , DT_CENTER | DT_VCENTER | DT_SINGLELINE); 
                DeleteObject(hBrush);
                DeleteObject(hFont);
                DeleteObject(hPen1);
                DeleteObject(hPen2);
            }
            EndPaint(hwnd , &ps);
            ValidateRect(hwnd , &rcPos);
            break;}
    }
   return CallWindowProc((WNDPROC)GetProp(hwnd , "SUBCLASS") , hwnd , message , wParam , lParam); 
}
//----------------------------
UINT_PTR CALLBACK CFHookProc(HWND hwnd,  UINT message, WPARAM wParam, LPARAM lParam) {
    CFontPicker * FP;
    switch(message) {
        case WM_INITDIALOG: {
            FP = (CFontPicker*)((CHOOSEFONT*)lParam)->lCustData;
            SetProp(hwnd , "CFontPicker" , (HANDLE)FP);
            RECT rc;
            GetChildRect(GetDlgItem(hwnd,0x470) , &rc);
            rc.bottom +=20;
            //SetProp(GetDlgItem(hwnd , 0x444) , "SUBCLASS" , (HANDLE)SetWindowLong(GetDlgItem(hwnd , 0x444),GWL_WNDPROC,(LONG)SampleProc));
            SetProp(hwnd , "SUBCLASS" , (HANDLE)SetWindowLong(hwnd ,GWL_WNDPROC,(LONG)SampleProc));
            //SetProp(GetDlgItem(hwnd , 0x444) , "CFontPicker" , FP);   0x431
            HWND item; 
            item = CreateWindow("STATIC" , "Kolor czcionki:", WS_CHILD|((FP->_style & CF_NOCOLORSEL)?0:WS_VISIBLE) , rc.left , rc.bottom , 100
                ,  18 , hwnd , 0 , 0 , 0);
            rc.bottom +=16;
            SendMessage(item , WM_SETFONT , SendMessage(hwnd , WM_GETFONT , 0 , 0) , 0);
            SetProp (hwnd , "CP1" , new CColorPicker(FP->_font.color , WS_CHILD|((FP->_style & CF_NOCOLORSEL)?0:WS_VISIBLE) , rc.left , rc.bottom 
                ,  100 , 18 , hwnd , (HMENU)0x1000 , 0 , 0));
            rc.bottom +=20;
            item = CreateWindow("STATIC" , "Kolor t³a:", WS_CHILD|((FP->_style & (CF_NOBGCOLORSEL|CF_NOCOLORSEL))?0:WS_VISIBLE) , rc.left , rc.bottom , 100
                ,  18 , hwnd , 0 , 0 , 0);
            rc.bottom +=16;
            SendMessage(item , WM_SETFONT , SendMessage(hwnd , WM_GETFONT , 0 , 0) , 0);
            SetProp (hwnd , "CP2" , new CColorPicker(FP->_font.bgColor , WS_CHILD|((FP->_style & (CF_NOBGCOLORSEL|CF_NOCOLORSEL))?0:WS_VISIBLE) , rc.left , rc.bottom 
                ,  100 , 18 , hwnd , (HMENU)0x1001 , 0 , 0));
            // wylaczamy niepotrzebne kontrolki
            if (FP->_style & CF_NOFACESEL) {
                EnableWindow(GetDlgItem(hwnd , 0x470) , 0);
                EnableWindow(GetDlgItem(hwnd , 0x474) , 0);
            }
            if (FP->_style & CF_NOSTYLESEL)
                EnableWindow(GetDlgItem(hwnd , 0x471) , 0);
            if (FP->_style & CF_NOSIZESEL)
                EnableWindow(GetDlgItem(hwnd , 0x472) , 0);
            

            return 0;}

        case WM_COMMAND: 
            FP = (CFontPicker*)GetProp(hwnd , "CFontPicker");
            switch (HIWORD(wParam)) {
               case BN_CLICKED: // Control
                    switch(LOWORD(wParam)) {
                            case IDOK:{
                                if (!(FP->_style & CF_NOCOLORSEL)) {
                                    FP->_font.color = ((CColorPicker*)GetProp(hwnd,"CP1"))->_color;
                                    FP->_font.bgColor = ((CColorPicker*)GetProp(hwnd,"CP2"))->_color;
                                }
                                return 0;}
                    }
            }
            return 0;
        
    }
    return 0;
}

// ------------------------------------------------------------------

CFontPicker::CFontPicker(const char * preview , const char * font , DWORD fdlStyle, DWORD style , int x , int y , int w , int h , HWND parent , HMENU id , HINSTANCE hInst , LPARAM param) {
   _font=StrToLogFont(font);
   _preview = (preview && *preview)? preview : "Text";
   _style = fdlStyle;
   _subclass = 0;
   _hwnd = CreateWindowEx(0 , "STATIC" , "" , SS_SUNKEN | SS_NOTIFY | style , x , y , w , h , parent , id , hInst , (LPVOID)param);
   SetProp(_hwnd , "CFontPicker" , this);
   _subclass = (WNDPROC) SetWindowLong(_hwnd , GWL_WNDPROC , (LONG)CFontPickerWndProc);
   Refresh();
}
CFontPicker::~CFontPicker() {

}

/* Zapis: 
face;charset;size;flags;color;bck;
np.
Tahoma;;10;ibu;0;16767623;
*/

LOGFONTEX StrToLogFont(std::string str) {
    LOGFONTEX font;
    font.bgColor = RGB(0xFF , 0xFF , 0xFF);
    memset(&font , 0 , sizeof(font));
    int start=0;
    int pos = str.find(';');
    if (pos<0) return font;
    strncpy(font.lf.lfFaceName , str.substr(0,pos - start).c_str() , LF_FACESIZE);
    //charset
    start = pos+1;
    pos = str.find(';',start);
    if (pos<0) return font;
    font.lf.lfCharSet = atoi(str.substr(start , pos - start).c_str());
    if (!font.lf.lfCharSet) font.lf.lfCharSet = DEFAULT_CHARSET;
    //size
    start = pos+1;
    pos = str.find(';',start);
    if (pos<0) return font;
    font.lf.lfHeight = atoi(str.substr(start , pos - start).c_str());
    //flags
    start = pos+1;
    pos = str.find(';',start);
    if (pos<0) return font;
    std::string flags = str.substr(start , pos - start);
    font.lf.lfItalic = flags.find('i')!=flags.npos;
    font.lf.lfStrikeOut = flags.find('s')!=flags.npos;
    font.lf.lfUnderline = flags.find('u')!=flags.npos;
    font.lf.lfWeight = (flags.find('b')!=flags.npos)?FW_BOLD:FW_NORMAL;
    //color
    start = pos+1;
    pos = str.find(';',start);
    if (pos<0) return font;
	if (pos != start) { // tylko gdy coœ jest...
		if (pos-start > 2 && str[start+1]=='x') { // hexy
			start+=2;
			font.color = chtoint(str.substr(start , pos - start).c_str() , 16);
		} else font.color = atoi(str.substr(start , pos - start).c_str());
	}
    //bgcolor
    start = pos+1;
    pos = str.find(';',start);
    if (pos<0) return font;
	if (pos != start) { // tylko gdy coœ jest...
		if (pos-start > 2 && str[start+1]=='x') { // hexy
			start+=2;
			font.bgColor = chtoint(str.substr(start , pos - start).c_str() , 16);
		} else font.bgColor = atoi(str.substr(start , pos - start).c_str());
	}
    return font;
}

std::string LogFontToStr(LOGFONTEX font) {
    std::string str = font.lf.lfFaceName;
    char buff [32]; // buforek
    buff[0]=';';
    char * ch = buff+1; // do ch ladujemy informacje, ktore potem ida do str
    itoa(font.lf.lfCharSet , ch , 10);
    str+=buff;
    itoa(font.lf.lfHeight , ch , 10);
    str+=buff;

    if (font.lf.lfWeight>=FW_BOLD) {*ch='b';ch++;}
    if (font.lf.lfItalic) {*ch = 'i';ch++;}
    if (font.lf.lfUnderline) {*ch = 'u';ch++;}
    if (font.lf.lfStrikeOut) {*ch = 's';ch++;}
    *ch=0;
    str+=buff;
	buff[1]='0';
	buff[2]='x'; // zapiszemy sobie hexem
	ch = buff+3;
	if (font.color != -1) {
		itoa(font.color , ch , 16);
	    str+=buff;
	} else 
		str+=buff[0];
	if (font.bgColor != -1) {
		itoa(font.bgColor , ch , 16);
	    str+=buff;
	} else
		str+=buff[0];
    str+=";";
    return str;
}
