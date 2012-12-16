#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <commctrl.h>
#include <stdstring.h>

#include "ButtonX.h"
#include "func.h"

WNDPROC ButtonX::buttonClassProc = 0;

ButtonX::ButtonX(const char * title ,int style, int x , int y , int w , int h , HWND parent , HMENU id , HINSTANCE hinst , LPARAM param) {
	this->_hwnd = CreateWindowEx(0,"BUTTON",title , BS_PUSHBUTTON |  style
            ,x,y,w,h,parent,id,hinst, (HANDLE)param);
	this->initWindow();
	
}
ButtonX::ButtonX(HWND hwnd) {
	this->_hwnd = hwnd;
	this->initWindow();
}
ButtonX::~ButtonX() {
	if (this->_hwnd) {
		SetProp(this->_hwnd, "ButtonX*", 0);
	}
}
void ButtonX::initWindow() {
	this->_duringPaint = false;
	this->_icon = 0;
	this->_sharedIcon = false;
	//this->_text = "";
	this->_iconW = this->_iconH = 0;
	this->_iconMargin = 5;
	this->_flat = (GetWindowLong(this->_hwnd, GWL_STYLE) & BS_TYPEMASK) == BS_OWNERDRAW;

	int length = GetWindowTextLength(this->_hwnd);
	GetWindowText(this->_hwnd, this->_text.GetBuffer(length+1), length); 
	this->_text.ReleaseBuffer();
//	SetWindowText(this->_hwnd, "");
	this->buttonClassProc = (WNDPROC)GetWindowLong(this->_hwnd, GWL_WNDPROC);
	SetProp(this->_hwnd, "ButtonX*", (HANDLE)this);
	SetWindowLong(this->_hwnd, GWL_WNDPROC, (LONG)ButtonX::windowProc);
	RedrawWindow(this->_hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);

}

ButtonX * ButtonX::fromHWND(HWND wnd) {
	return (ButtonX *) GetProp(wnd, "ButtonX*");
}
void ButtonX::setIcon(HICON icon, bool shared) {
	this->freeIcon();
	this->_sharedIcon = shared;
	this->_icon = icon;
    ICONINFO ii;
	BITMAP b;
	if (GetIconInfo(icon , &ii) && GetObject(ii.hbmColor , sizeof(BITMAP),&b)) {
		this->_iconW = b.bmWidth;
		this->_iconH = b.bmHeight;
		DeleteObject(ii.hbmColor);
		DeleteObject(ii.hbmMask);
	} else {
		this->_iconW = 16;
		this->_iconH = 16;
	}
	RedrawWindow(this->_hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}
void ButtonX::setIcon(HIMAGELIST list, int pos) {
	this->setIcon(ImageList_ExtractIcon(0, list, pos), false);
}
void ButtonX::freeIcon() {
	if (!this->_icon) return;
	if (!this->_sharedIcon) 
		DestroyIcon(this->_icon);
	this->_icon = 0;
	this->_iconW = 0;
	this->_iconH = 0;
}

void ButtonX::drawCaption(HDC hdc, RECT updRect) {
	//if (!this->_icon) return;
	RECT textRc, clientRc;
	GetClientRect(this->_hwnd, &clientRc);
	bool enabled = IsWindowEnabled(this->_hwnd);
	int state = SendMessage(this->_hwnd, BM_GETSTATE, 0, 0);
	
	if (this->_flat && state == BST_FOCUS) {
		RECT focusRc = clientRc;
		InflateRect(&focusRc, -2, -2);
		DrawFocusRect(hdc, &focusRc);
	}

	/*if (state == BST_PUSHED) {
		clientRc.top += 2;
		clientRc.left += 2;
	}*/
	textRc = clientRc;
	int color = GetSysColor(COLOR_BTNTEXT);
	if (!this->_text.empty()) {
		if (!enabled)  {
			color = GetSysColor(COLOR_BTNSHADOW);
		} else if (state==0) {
			/*POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(this->_hwnd, &pt);
			if (PtInRect(&clientRc, pt)) {
				color = GetSysColor(COLOR_HOTLIGHT);
			}*/
		}
		SetBkMode(hdc , TRANSPARENT);
		SetTextColor(hdc, color);
		textRc.bottom = 0;
		textRc.right -=	6;
		if (this->_icon)
			textRc.right -=	this->_iconMargin + this->_iconW;
		HFONT oldFont = (HFONT) SelectObject(hdc, (HGDIOBJ)SendMessage(this->_hwnd, WM_GETFONT, 0, 0));
		DrawText(hdc, this->_text, -1, &textRc, DT_LEFT | DT_NOPREFIX | DT_CALCRECT | DT_WORDBREAK);
		int x = (clientRc.right - textRc.right + (this->_icon ? (this->_iconW + this->_iconMargin) : 0))/2;
		int y = (clientRc.bottom - textRc.bottom)/2;
		textRc.left = x;
		textRc.top = y;
		textRc.right += x;
		textRc.bottom += y;
		DrawText(hdc, this->_text, -1, &textRc, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
		SelectObject(hdc, (HGDIOBJ)oldFont);
		if (this->_icon)
			DrawIconEx(hdc , x - this->_iconW - this->_iconMargin , (clientRc.bottom - this->_iconH)/2 , this->_icon , 0,0,0,0,DI_NORMAL);
	} else {
		DrawIconEx(hdc , (clientRc.right - this->_iconW)/2 , (clientRc.bottom - this->_iconH)/2 , this->_icon , 0,0,0,0,DI_NORMAL);
	}
}

int CALLBACK ButtonX::windowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam) {
	switch(message) {

		case WM_SETTEXT:
			ButtonX::fromHWND(hwnd)->_text = (char*) lParam;
			//break;
			return 0;
		case WM_GETTEXT:
			if (ButtonX::fromHWND(hwnd)->_duringPaint) {
				((char*)lParam)[0] = 0;
				return 0;
			}
			//break;
//			strncpy((char*) lParam,ButtonX::fromHWND(hwnd)->_text , wParam);
			strncpy((char*) lParam,ButtonX::fromHWND(hwnd)->_text , wParam);
			return min(ButtonX::fromHWND(hwnd)->_text.length(), wParam-1);
		case WM_GETTEXTLENGTH:
			if (ButtonX::fromHWND(hwnd)->_duringPaint) return 0;
			//break;
			return ButtonX::fromHWND(hwnd)->_text.length();
			
		case WM_PAINT: {
			ButtonX * bt = ButtonX::fromHWND(hwnd);
			//if (!bt->_icon) break; // standardowa obs³uga...
			//int r = 1;
			HRGN hrgn=CreateRectRgn(0, 0, 0, 0);
			bt->_duringPaint=true;
			GetUpdateRgn(hwnd, hrgn, false);
			int r = 1;
			if (!bt->_flat) {
				r = CallWindowProc(ButtonX::buttonClassProc, hwnd, message, wParam, lParam);
			}
			InvalidateRgn(hwnd, hrgn, false);

			PAINTSTRUCT ps;
			HDC hdc;
   			hdc = BeginPaint(hwnd, &ps);
			if (bt->_flat) {
				FillRgn(hdc, hrgn, GetSysColorBrush(COLOR_BTNFACE));
			}
			RECT rc = {0,0,0,0};
			bt->drawCaption(hdc, rc);
			EndPaint(hwnd, &ps);
			bt->_duringPaint=false;
			DeleteObject(hrgn);
			return r;}
		case WM_ENABLE: case WM_UPDATEUISTATE: {
			int r = CallWindowProc(ButtonX::buttonClassProc, hwnd, message, wParam, lParam);
			RepaintWindow(hwnd);
			return r;}
		case WM_ERASEBKGND:
			return 0;
		/*case WM_MOUSEMOVE:
			if (!GetProp(hwnd, "MOUSEOVER")) {
				SetProp(hwnd, "MOUSEOVER", (void*)1);
				RepaintWindow(hwnd);
			}
			break;
		case WM_MOUSELEAVE:
			SetProp(hwnd, "MOUSEOVER", 0);
			RepaintWindow(hwnd);
			break;*/
		case WM_SETFOCUS:
		//case BM_SETSTYLE:
		case BM_SETSTATE:
			// specjalnie dla W98
			RepaintWindow(hwnd);
			break;

		case WM_SETCURSOR:
			if (fromHWND(hwnd)->_flat) {
				SetCursor( LoadCursor(0, IDC_HAND) );
				return true;
			}
			break;

		case WM_DESTROY:
			delete ButtonX::fromHWND(hwnd);
			break;

	};
	
	return CallWindowProc(ButtonX::buttonClassProc, hwnd, message, wParam, lParam);
}

