#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x600
#define _WIN32_WINDOWS 0x0490
#include <windows.h>
#include <commctrl.h>
#include <stdstring.h>
#include <math.h>
#include <map>
#include "func.h"

using namespace std;
#include "ToolTipX.h"





//    ------------------------------------------------------------
ToolTipX::ToolTipX(HWND parent, HINSTANCE hInst) {
	this->create(parent, hInst);
}
ToolTipX::ToolTipX(){
	this->_hwnd = 0;
	this->_active = false;
}
void ToolTipX::create(HWND parent, HINSTANCE hInst){
	this->registerClass(hInst);
	this->_timer = CreateWaitableTimer(0 , 0 , 0);
	_hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,"ToolTipX_Class","",
        WS_POPUP/* | WS_BORDER*/,
        0,0,100,108,
        parent,NULL,hInst,NULL);
	if (!_hwnd) {
		ShowLastError();
		MessageBox(0, "Nie utworzy³em tooltipa!", "", MB_OK);
	}
	SetProp(_hwnd , "ToolTipX*" , (HANDLE)this);
	this->_active = false;
	this->hideTimeout = 500;
	this->enterWait = 500;
	this->enterSiblingTimeout = 500;
	this->_lastShown = 0;
	this->_lastPositioned = 0;
	this->_region = 0;
	this->_defaultPosition = enPositioning(positionFloatSlow | positionFirst | positionRefresh);
	this->_automated = false;
	this->_attached = false;
	this->_placement = pLeftBottom;
	this->_size.cx = 0;
	this->_size.cy = 0;
	this->_lastPlacementSwitch = 0;
	this->_dontReset = 0;
}
ToolTipX::~ToolTipX() {
	DestroyWindow(_hwnd);
	CloseHandle(_timer);
	if (this->_region)
		DeleteObject(this->_region);
}
ToolTipX * ToolTipX::fromHWND(HWND hwnd) {
	return (ToolTipX*)GetProp(hwnd, "ToolTipX*");
}


void ToolTipX::mouseMovement(HWND window, int scrollX, int scrollY) {
	mouseMovement(window, getScreenPoint(), scrollX, scrollY);
}

void ToolTipX::mouseMovement(HWND window, POINT screen, int scrollX, int scrollY) {
	if (!this->_hwnd) return;
	mouseMovement(window, screen, getLocalPoint(window, screen, scrollX, scrollY));
}
POINT ToolTipX::getPlacementPos(POINT screen, int offset, enPlacement place) {
	POINT pt;
	if (place & pLeft)
		pt.x = screen.x - this->_size.cx - offset;
	else
		pt.x = screen.x + offset;
	if (place & pTop)
		pt.y = screen.y - this->_size.cy - offset;
	else
		pt.y = screen.y + offset;
	return pt;
}

void ToolTipX::setPos(POINT screen, bool offset, enPositioning position, enPlacement place) {
	if (!this->_currentTarget) return;
	enPositioning allowPos = this->_currentTarget->getPositioning();
	if (allowPos == positionAuto) { 
		allowPos = this->_defaultPosition;
	}
	if ((position & positionFloat) && (allowPos & positionFloatSlow)) {
		if (GetTickCount() - this->_lastPositioned < 50) 
			return;
		this->_lastPositioned = GetTickCount();
	}
	if (!(position & allowPos)) return;

	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rc,0);
	expandRect(rc, -5, -5);
	int offsetV = offset ? (this->_defaultPosition & positionFloat ? cursorOffset : cursorOffset / 2) : 0;
	POINT pt = getPlacementPos(screen, offsetV, place == pNone ? this->_placement : place);
	RECT trc = Rect(pt.x, pt.y, _size.cx, _size.cy, true);
	//fitInRect(trc, rc);
	if (place == pNone && (!InsideRect(rc, trc) || this->_placement == pNone)) {
		DWORD ticks = GetTickCount();
/*		if (this->_placement != pNone && abs(ticks - this->_lastPlacementSwitch) < 500) {
			fitInRect(trc, rc);
		} else { */
			// Zmieniamy pozycjonowanie...
		POINT pt1 = getPlacementPos(screen, offsetV, pLeftTop);
		POINT pt2 = getPlacementPos(screen, offsetV, pRightBottom);
		RECT wrc = Rect(pt1.x, pt1.y, pt2.x + _size.cx, pt2.y + _size.cy);
		this->_lastPlacementSwitch = ticks;
		place = pNone;
		place = (enPlacement) (place | ((wrc.right > rc.right) ? pLeft : pRight));
		place = (enPlacement) (place | ((wrc.bottom > rc.bottom) ? pTop : pBottom));
//		}
	}
	if (place) {
		pt = getPlacementPos(screen, offsetV, place);
		this->_placement = place;
		trc = Rect(pt.x, pt.y, _size.cx, _size.cy, true);
		fitInRect(trc, rc);
		this->refreshRegion();
	}
	//OutputDebugString(_sprintf("\nSWP %d - %d %d", position, trc.left, trc.top));
	SetWindowPos(this->_hwnd, 0, trc.left, trc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void ToolTipX::mouseMovement(HWND window, POINT screen, POINT local) {
	if (!this->_hwnd) return;
	if (this->_automated && screen.x != -1 && sqrt(pow(double(screen.x - this->_lastScreen.x), 2)+pow(double(screen.y - this->_lastScreen.y), 2)) > 10 ) {
		this->hide(true);
		OutputDebugString("Speeding\n");
		this->_lastScreen = screen;
		this->_lastShown = 0;
		return;
	}
	this->_lastScreen = screen;
	if (this->_currentTarget) {
		if (this->_defaultPosition & positionFloat) {
			setPos(screen, true, positionFloat);
		}
		// sprawdzamy czy aktualny siê mieœci...
		if ((!this->_currentWindow || this->_currentWindow == window)
			&& this->_currentTarget->mouseAtTarget(this, window,screen, local)) 
		{
			//this->_lastScreen = screen;
			if (!this->visible() && !this->_active) {
				//OutputDebugString(_sprintf("\nShowActive %d < %d", (GetTickCount() - this->_lastShown), this->enterSiblingTimeout));
				this->show((GetTickCount() - this->_lastShown) > this->enterSiblingTimeout);
			}
			return; // skoro siê mieœci - zostawiamy aktualny...
		}
	}
    // szukamy nowego do pokazania...
	tTargets::iterator it = _targets.find(window);
	while (it != _targets.end() && it->first == window) {
		if (it->second->mouseAtTarget(this, window,screen, local)) {
			OutputDebugStr("CancelHideTimer1\n");
			CancelWaitableTimer(this->_timer);
			//this->_lastScreen = screen;
			bool currentlyShowing = this->_currentTarget && this->visible();
			// wybieramy go i nie szukamy dalej...
			this->setTip(window, it->second, true);
			this->show(!currentlyShowing && ((GetTickCount() - this->_lastShown) > this->enterSiblingTimeout));
			OutputDebugString("ShowNew\n");
			return;
		}
		it++;
	}
	// nic nie znalaz³ wiêc chowamy...
	//this->_lastScreen = Point(-1,-1);
	if (this->_defaultPosition & positionFloat) {
//		CancelWaitableTimer(this->_timer);
		this->hide(true);
	}  else if (!this->visible()) {
		this->_active = false;
	}
	// mo¿emy nie kasowaæ poprzedniego - mo¿e jeszcze siê przydaæ...
}


VOID CALLBACK  ToolTipX::timerShowHideProc(ToolTipX * tip , DWORD low , DWORD high) {
	if (!tip || !tip->_currentTarget) {
		OutputDebugString("Timer-notip\n");
		return;
	}
	POINT screen = getScreenPoint();
	HWND window = tip->_currentWindow;
	POINT local = getLocalPoint(window, screen);
	if (tip->visible()) { // HIDE proc
		if (((tip->_lastScreen.x != screen.x || tip->_lastScreen.y != screen.y) &&  !WindowBelongsTo(WindowFromPoint(screen), tip->_hwnd) &&  !tip->_currentTarget->mouseAtTarget(tip, window,screen, local))) {
			tip->hide(true);
			OutputDebugString("HideTimer-hide\n");
		} else {
			OutputDebugString("HideTimer\n");
		}
	} else if (tip->_active) { // SHOW proc
		if (tip->_currentTarget->mouseAtTarget(tip, window,screen, local)) {
			OutputDebugString("ShowTimer\n");
			tip->show(false);
		} else {
			OutputDebugString("ShowTimer-Hide\n");
			//tip->reset();
			tip->_active = false;
			OutputDebugStr("CancelHideTimer2\n");
			CancelWaitableTimer(tip->_timer);
		}
	}  else {
		OutputDebugStr("EmptyTimer\n");
	}
	tip->_lastScreen = screen;
}


//void ToolTipX::mouseMovementInTarget(HWND window, int x, int y) {
	/*TODO: mouseMovementInTarget*/
//}
void ToolTipX::hide(bool deactivate){
	if (this->visible()) {
		OutputDebugStr("CancelHideTimer3\n");
		CancelWaitableTimer(this->_timer);
		ShowWindow(this->_hwnd, SW_HIDE);
		if (deactivate)
			this->_active = false;
		this->_lastShown = GetTickCount();
		if (this->_currentTarget)
			this->_currentTarget->onHide(this);
	}
}// ukrywa tip'a
void ToolTipX::hideAuto() {
	if (this->_automated) {
		hide(false);
		this->_lastShown = 0;
	}
}
void ToolTipX::attach() {
	if (!this->_attached && this->_currentTarget && this->_currentTarget->getTip(this)) {
		this->_currentTarget->attach(this);
		this->_attached = true;
		this->refresh();
	}
}
void ToolTipX::show(bool wait){
	this->_active = true;
	LARGE_INTEGER lDueTime;
	if (wait) {
		// Odpala wait'a, który sprawdzi czy wybrana kontrolka spe³nia jeszcze warunek...
		if (!this->_currentTarget) return;
		lDueTime.QuadPart = -10000 * this->_currentTarget->getDelay(this);
		SetWaitableTimer(this->_timer , &lDueTime , 0,(PTIMERAPCROUTINE)timerShowHideProc, this , FALSE);
	} else {
		this->attach();
		setPos(getScreenPoint(), true, positionFirst);
		this->_currentTarget->onShow(this);
		ShowWindow(this->_hwnd, SW_SHOWNOACTIVATE);
		SetWindowPos(this->_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		// Odpala wait'a, który sprawdzi czy wybrana kontrolka spe³nia jeszcze warunek...
		lDueTime.QuadPart = -10000 * this->hideTimeout;
		CancelWaitableTimer(this->_timer);
		if (SetWaitableTimer(this->_timer , &lDueTime , this->hideTimeout,(PTIMERAPCROUTINE)timerShowHideProc, this , FALSE)) {
			OutputDebugStr("SetHideTimer\n");
		} else {
			int r = GetLastError();
			OutputDebugStr(_sprintf("SetHideTimer failed! %d - %s\n", r, GetLastErrorMsg(r)));
		}

	}
}// pokazuje tip'a
bool ToolTipX::visible() {
	return IsWindowVisible(this->_hwnd) != 0;
}
ToolTipX::tTargets::iterator ToolTipX::findTarget(HWND window, const iTarget & target) {
	tTargets::iterator it = _targets.find(window);
	while (it != _targets.end() && it->first == window) {
		if (it->second.get()->operator==(target)) return it;
		it++;
	}
	return _targets.end();
}

void ToolTipX::addTarget(HWND window, TargetPtr & target) {
	tTargets::iterator it = this->findTarget(window, target);
	if (it != this->_targets.end()) {
		this->_targets.erase(it);
	}
	this->_targets.insert(pair<HWND, TargetPtr>(window, target));
}
void ToolTipX::removeTarget(HWND window, const TargetPtr & target) {
	tTargets::iterator it = this->findTarget(window, target);
	if (it != this->_targets.end()) {
		this->_targets.erase(it);
	}
}
void ToolTipX::clearTargets(HWND window) {
	/*tTargets::iterator start = _targets.find(window);
	if (start == _targets.end) return;
	tTargets::iterator end = start;
	while (end != _targets.end() && end->first == window) {
		end++;
	}*/
	_targets.erase(window);
}

void ToolTipX::refresh() {
	if (!this->_currentTarget || !this->_currentTarget->getTip(this) || !this->_attached) return;
	bool setBackVisible = false;
/*	if (!this->_active)
		return;*/
	this->_dontReset ++;
	if (this->visible()) {
		setBackVisible = true;
		ShowWindow(this->_hwnd, SW_HIDE);
	}

	HDC hdc = GetDC(this->_hwnd);
	RECT rc = {0,0,250,18};
	this->_currentTarget->getTip(this)->setSize(hdc, rc);
	this->adjuctTipRect(rc);
	ReleaseDC(this->_hwnd, hdc);
	this->_size.cx = rc.right;
	this->_size.cy = rc.bottom;
	//if (!this->_attached) return;

	SetWindowPos(this->_hwnd, 0, 0, 0, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
	if (this->_automated)
		this->_placement = pNone; // ¿eby zrobi³ refresha regionu i pozycji...
	setPos(getScreenPoint(), true, positionRefresh);
	this->refreshRegion();
	//RedrawWindow(this->_hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	if (setBackVisible)
		ShowWindow(this->_hwnd, SW_SHOWNOACTIVATE);
	this->_dontReset --;
}
void ToolTipX::refreshRegion() {
	if (this->_region) {
		DeleteObject(this->_region);
	}
	this->_region = CreateRoundRectRgn(0,0,this->_size.cx, this->_size.cy, roundness, roundness);
	HRGN rgn1;
	RECT corner;
	corner.left = (this->_placement & pLeft) ? this->_size.cx - roundness : 0;
	corner.right = (this->_placement & pLeft) ? this->_size.cx - 1 : roundness;
	corner.top = (this->_placement & pTop) ? this->_size.cy - roundness : 0;
	corner.bottom = (this->_placement & pTop) ? this->_size.cy - 1 : roundness;
	rgn1 = CreateRectRgn(corner.left,corner.top,corner.right,corner.bottom);
	CombineRgn(this->_region, this->_region, rgn1, RGN_OR);
	CombineRgn(rgn1, this->_region, 0, RGN_COPY);
	SetWindowRgn(this->_hwnd, rgn1, true);
	RepaintWindow(this->_hwnd);

}

void ToolTipX::setTip(HWND hwnd, TargetPtr & target, bool treatAsAuto) {
	if (!target->getTip(this)) return;
	this->reset(); // od³¹czamy aktualny...
	//SetParent(this->_hwnd, hwnd);
	this->_active = true;
	this->_currentWindow = hwnd;
	this->_currentTarget = target;
	this->_automated = treatAsAuto;
	

//	this->unlockPos();
//	this->refresh();
}
void ToolTipX::setTip(iTip * tip, bool treatAsAuto) {
	if (!tip) {
		if (!this->_currentTarget) return;
		this->_dontReset ++;
		OutputDebugString("Reset\n");
		this->hide();
		if (this->_attached && this->_currentTarget && this->_currentTarget->getTip(this)) {
			this->_currentTarget->detach(this);
		}
		this->_attached = false;
		this->_active = false;
		this->_currentTarget.reset();
		this->_currentWindow = 0;
		this->_dontReset --;
		return;
	}
	setTip(0, TargetPtr(new Target(TipPtr(tip))), treatAsAuto);
}
void ToolTipX::reset() {
	if (this->_dontReset) return;
	this->_lastShown = 0;
	this->setTip(0, false); 
}


void ToolTipX::adjuctClientRect(RECT & rc) {
	rc.left += marginLeft + margin;
	rc.top += margin;
	rc.bottom -= 2*margin;
	rc.right -= 2*margin;
}
void ToolTipX::adjuctTipRect(RECT & rc) {
	rc.right += marginLeft + margin;
	rc.bottom += margin;
	rc.bottom += 2*margin;
	rc.right += 2*margin;
}
HWND ToolTipX::getTipWindow() {
	return this->_hwnd;
}


void ToolTipX::registerClass(HINSTANCE hInst) {
	static int once = true;
	if (!once) return;
	once = true;
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= GetSysColorBrush(COLOR_WINDOW);
	wcex.lpszMenuName	= "";
    wcex.lpfnWndProc        = (WNDPROC)windowProc;
    wcex.lpszClassName      = "ToolTipX_Class";
#define CS_DROPSHADOW 0x00020000
    wcex.style              = isComctl(6 , 0) * CS_DROPSHADOW;
    wcex.hIcon              = 0;
    wcex.hIconSm            = 0;
	RegisterClassEx(&wcex);

}
int CALLBACK ToolTipX::windowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_PAINT:{
			RECT rc;
			GetClientRect(hwnd, &rc);
			PAINTSTRUCT ps;
			HDC hdc;
   			hdc = BeginPaint(hwnd, &ps);
			//FillRect(hdc, &rc, GetSysColorBrush(COLOR_INFOBK));
			SetTextColor(hdc, GetSysColor(COLOR_INFOTEXT));
			ToolTipX * tip = ToolTipX::fromHWND(hwnd);
			if (tip && tip->_currentTarget && tip->_currentTarget->getTip(tip)) {
				tip->adjuctClientRect(rc);
				tip->_currentTarget->getTip(tip)->paint(hdc, rc);	
			}
			if (tip->_region) {
				HBRUSH brush = GetSysColorBrush(COLOR_INFOTEXT);
				FrameRgn(hdc, tip->_region, brush, 1,1);
			}
			EndPaint(hwnd, &ps);
			return 1;}
		case WM_ERASEBKGND:{
			HDC hdc = (HDC)wParam;
			RECT rc;
			ToolTipX * tip = ToolTipX::fromHWND(hwnd);
			GetClientRect(hwnd, &rc);
			FillRect(hdc, &rc, GetSysColorBrush(COLOR_INFOBK));

			int ca = GetSysColor(COLOR_INFOBK);
			int cb = GetSysColor(COLOR_INFOTEXT);
			//HBRUSH brush = CreateSolidBrush(RGB_ALPHA(ca, cb, 50));
			HBRUSH brush = CreateSolidBrush(RGB_ALPHA(ca, cb, 90));
			SetBkMode(hdc, TRANSPARENT);
			HRGN rgn1 = CreateRectRgn(0,0,0,0);
			CombineRgn(rgn1, tip->_region, 0, RGN_COPY);
			OffsetRgn(rgn1, marginLeft, 0);
			CombineRgn(rgn1, tip->_region, rgn1, RGN_DIFF);
			FillRgn(hdc, rgn1, brush);
			DeleteObject(brush);
			DeleteObject(rgn1);
			return 1;
			}
		case WM_MOUSEMOVE:
			//fromHWND(hwnd)->hide();
			break;
		case WM_LBUTTONUP:
			fromHWND(hwnd)->reset();
			break;
		case WM_ACTIVATE: {
			//OutputDebugString(_sprintf("\nWM_ACT w=%d %x != %x", LOWORD(wParam), lParam, hwnd));
			if (LOWORD(wParam) == WA_INACTIVE) {
				//fromHWND(hwnd)->reset();
				HWND wnd = (HWND)GetProp(hwnd, "reactivate");
				if (!wnd) return 0;
				if (!lParam || !WindowBelongsTo((HWND)lParam, wnd)) {
					fromHWND(hwnd)->reset();
				}
				return 0;
				fromHWND(hwnd)->reset();
                SetProp(hwnd, "reactivate", 0);
				char debug [51];
				GetWindowText(wnd, debug, 50);
				OutputDebugString(_sprintf("Activating %s\n", debug));
				//SetProp(hwnd, "reactivate", 0);
				fromHWND(hwnd)->_dontReset ++;
				SetActiveWindow((HWND) wnd);
				SetForegroundWindow((HWND) wnd);
				fromHWND(hwnd)->_dontReset --;
			} else { // zapamietujemy okno deaktywowane...
				char debug [51];
				GetWindowText((HWND)lParam, debug, 50);
				OutputDebugString(_sprintf("Storing %s\n", debug));
                SetProp(hwnd, "reactivate", (HANDLE)lParam);
			}
			return 0;}
	}

	ToolTipX * tip = ToolTipX::fromHWND(hwnd);
	try {
		if (tip && tip->_currentTarget && tip->_currentTarget->getTip(tip)) {
			bool returned = false;
			int r = tip->_currentTarget->getTip(tip)->parentMessageProc(hwnd, message, wParam, lParam, returned);
			if (returned) return r;
		}	
	} catch (...) {}

    return DefWindowProc(hwnd, message, wParam, lParam);
}
