#include <windows.h>
#include <commctrl.h>
#include <stdstring.h>
#include <map>
#include "func.h"

using namespace std;
#include "ToolTipX.h"


bool ToolTipX::iTarget::operator==(const iTarget & b) const {
	return this == &b || !strcmp(this->getClass(), b.getClass());
}

void ToolTipX::iTarget::attach(ToolTipX * ttx) {
	this->getTip(ttx)->attach(ttx->_hwnd);
}
void ToolTipX::iTarget::detach(ToolTipX * ttx) {
	this->getTip(ttx)->detach(ttx->_hwnd);
}



void ToolTipX::Target::setTip(ToolTipX * ttx, TipPtr tip) {
	bool majorChange = _tip.get() != tip.get();
	TipPtr oldTip = _tip;
	_tip = tip; 
	if (ttx && ttx->isCurrentTarget(this)) {
		//OutputDebugString("\nRefresh");
		if (majorChange && ttx->isAttached()) {
			if (oldTip)
				oldTip->detach(ttx->getTipWindow());
			if (tip)
				tip->attach(ttx->getTipWindow());
		}
		ttx->refresh();
	}
}


bool ToolTipX::TargetWindow::mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local) {
	return WindowFromPoint(screen) == this->_hwnd;
}

bool ToolTipX::TargetId::mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local) {
	HWND item = WindowFromPoint(screen);
	return item ? ( GetWindowLong(item, GWL_ID)==this->_id) : false;
}
bool ToolTipX::TargetRect::mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local) {
	return PtInRect(&this->_rect, local) != 0;
}

bool ToolTipX::TargetRect::operator==(const iTarget & b) const {
	return iTarget::operator==(b) 
		&& !memcmp(&(static_cast<const TargetRect&>(b)._rect) , &_rect, sizeof(RECT));
}



// --------------------------------

void ToolTipX::iTipCallBack::setSize(HDC hdc, RECT &rc) {
	if (checkCache()) _cachedTip->setSize(hdc, rc);
}
void ToolTipX::iTipCallBack::paint(HDC hdc, RECT rc) {
	if (checkCache()) _cachedTip->paint(hdc, rc);
}
void ToolTipX::iTipCallBack::attach(HWND hwnd) {
	if (checkCache()) _cachedTip->attach(hwnd);
}
void ToolTipX::iTipCallBack::detach(HWND hwnd) {
	if (_cachedTip) _cachedTip->detach(hwnd);
	this->resetCache();
}
bool ToolTipX::iTipCallBack::checkCache() {
	if (_cachedTip)
		return true;
	fillCache();
	return _cachedTip;
}

int ToolTipX::iTipCallBack::parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned) {
	if (_cachedTip) return _cachedTip->parentMessageProc(hwnd, message, wParam, lParam, returned);
	return 0;
}

// --------------------------------


ToolTipX::TipText::TipText(const CStdString & txt, HFONT font, int width) {
	this->_text = txt;
	this->_width = width;
	if (font) {
		this->_font = font;
	} else {
		this->_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}
}
void ToolTipX::TipText::setSize(HDC hdc, RECT &rc) {
	HFONT oldFont = (HFONT) SelectObject(hdc, (HGDIOBJ) this->_font);
	rc.bottom = 0;
	if (this->_width) rc.right = rc.left + this->_width;
	DrawText(hdc, this->_text, -1, &rc, DT_LEFT | DT_NOPREFIX | DT_CALCRECT | DT_WORDBREAK);
	SelectObject(hdc, (HGDIOBJ)oldFont);
	rc.right+=2*margin;
	rc.bottom+=2*margin;
}
void ToolTipX::TipText::paint(HDC hdc, RECT rc) {
	HFONT oldFont = (HFONT) SelectObject(hdc, (HGDIOBJ)this->_font);
	rc.left+=margin;
	rc.top+=margin;
	SetBkMode(hdc , TRANSPARENT);
	DrawText(hdc, this->_text, -1, &rc, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
	SelectObject(hdc, (HGDIOBJ)oldFont);
}

// --------------------------------------------------------------------

ToolTipX::TipTextRich::TipTextRich(const CStdString & txt, HFONT font, cRichEditFormat::fSetStyleCB styleCB, int width) {
	this->_text = txt;
	this->_rich = 0;
	this->_styleCB = styleCB;
	this->_width = width ? width : 200;
	RECT rc = {0,0,0,0};
	this->_rect = rc;
	this->_positioned = false;
	if (font) {
		this->_font = font;
	} else {
		this->_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}

}
ToolTipX::TipTextRich::~TipTextRich() {
	if (this->_rich)
		DestroyWindow(this->_rich);
}
void ToolTipX::TipTextRich::setSize(HDC hdc, RECT &rc) {
	this->_positioned = false;
	cRichEditHtml::SetHTML(this->_rich , this->_text , this->_styleCB);
	SendMessage(this->_rich, EM_REQUESTRESIZE , 0, 0);
	rc = this->_rect;
	//OutputDebugString("\nsetSize");
}
void ToolTipX::TipTextRich::paint(HDC hdc, RECT rc) {
	if (!this->_positioned) {
		this->_positioned = true;
		SetWindowPos(this->_rich, 0, rc.left, rc.top, rc.right-rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}
void ToolTipX::TipTextRich::setText(const CStdString & text) {
	this->_text = text;
}
void ToolTipX::TipTextRich::attach(HWND hwnd) {
	this->_rich = CreateWindowEx(0 , RICHEDIT_CLASS  , "" , WS_CHILD|ES_MULTILINE |ES_READONLY
		,0,0,this->_width,10,hwnd,0 , 0 , 0);
	cRichEditHtml::Init (this->_rich, true, GetSysColor(COLOR_INFOBK));
	SendMessage(this->_rich , WM_SETFONT , (WPARAM)this->_font , 1);
	SendMessage(this->_rich , EM_SETEVENTMASK, 0, SendMessage(this->_rich , EM_GETEVENTMASK, 0,0) | ENM_REQUESTRESIZE);
	SetProp(this->_rich, "TipTextRich*", this);
	//OutputDebugString("\nattach");
}
void ToolTipX::TipTextRich::detach(HWND hwnd) {
	DestroyWindow(this->_rich);
	this->_rich = 0;
	//OutputDebugString("\ndetach");
}
ToolTipX::TipTextRich * ToolTipX::TipTextRich::fromHWND(HWND window) {
	return (TipTextRich*) GetProp(window, "TipTextRich*");
}

int ToolTipX::TipTextRich::parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned) {
	switch (message) {
		case WM_NOTIFY: {
			NMHDR * nmhdr = (NMHDR*) lParam;
			switch (nmhdr->code) {
				case EN_REQUESTRESIZE : {
					REQRESIZE * req = (REQRESIZE*)lParam;
					fromHWND(req->nmhdr.hwndFrom)->_rect = req->rc;
					break;}
			}
			break;}
	}
	return cRichEditHtml::ParentWindowProc(hwnd, message, wParam, lParam);
}
CStdString ToolTipX::TipTextRich::getText() {
	return this->_text;
}

// --------------------------------------------------------------------


ToolTipX::TipOnlyTitle::TipOnlyTitle(const CStdString & title, HIMAGELIST il, int pos, HFONT font) {
	this->_title = title;
	setIcon(il, pos);
	setFont(font);
}

ToolTipX::TipOnlyTitle::TipOnlyTitle(const CStdString & title, HICON icon, bool iconShared, HFONT font) {
	this->_title = title;
	setIcon(icon, iconShared);
	setFont(font);
}
void ToolTipX::TipOnlyTitle::setFont(HFONT font) {
	if (font) {
		this->_font = font;
	} else {
		this->_font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	}
}
ToolTipX::TipOnlyTitle::~TipOnlyTitle() {
	resetIcon();
}
void ToolTipX::TipOnlyTitle::setIcon(HICON icon, bool shared) {
	this->resetIcon();
	this->_iconShared = shared;
	this->_icon = icon;
    ICONINFO ii;
	BITMAP b;
	if (icon && GetIconInfo(icon , &ii) && GetObject(ii.hbmColor , sizeof(BITMAP),&b)) {
		this->_iconW = b.bmWidth;
		this->_iconH = b.bmHeight;
		DeleteObject(ii.hbmColor);
		DeleteObject(ii.hbmMask);
	} else {
		this->_iconW = 0;
		this->_iconH = 0;
	}

}
void ToolTipX::TipOnlyTitle::setIcon(HIMAGELIST il, int pos) {
	this->setIcon(ImageList_ExtractIcon(0, il, pos), false);
}
void ToolTipX::TipOnlyTitle::resetIcon() {
	if (this->_icon && !this->_iconShared) {
		DestroyIcon(this->_icon);
	}
	this->_icon = 0;
}
void ToolTipX::TipOnlyTitle::setSize(HDC hdc, RECT &rc) {
	HFONT oldFont = (HFONT) SelectObject(hdc, (HGDIOBJ) this->_font);
	rc.bottom = 0;
	if (!this->_title.empty()) {
		DrawText(hdc, this->_title, -1, &rc, DT_LEFT | DT_NOPREFIX | DT_CALCRECT | DT_SINGLELINE);
	} else {
		rc.right = rc.bottom = 0;
	}
	SelectObject(hdc, (HGDIOBJ)oldFont);
	rc.bottom = max(rc.bottom, this->_iconH);
	rc.right += this->_iconW ? this->_iconW + (this->_title.empty() ? 0 : 2*margin) : 0; 
	rc.right+=2*margin;
	rc.bottom+=2*margin;

}
void ToolTipX::TipOnlyTitle::paint(HDC hdc, RECT rc) {
	HFONT oldFont = (HFONT) SelectObject(hdc, (HGDIOBJ)this->_font);
	rc.left += margin;
	rc.top += margin;
	//rc.right -= margin;
	rc.bottom -= margin;
	SetBkMode(hdc , TRANSPARENT);

	if (this->_icon) {
		DrawIconEx(hdc , rc.left , (rc.bottom - rc.top - this->_iconH)/2 + rc.top , this->_icon , 0,0,0,0,DI_NORMAL);
		rc.left += this->_iconW + 2*margin;
	}

	DrawText(hdc, this->_title, -1, &rc, DT_LEFT | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);
	SelectObject(hdc, (HGDIOBJ)oldFont);
}




ToolTipX::TipOnlyImage::TipOnlyImage(HBITMAP image, bool shared) {
	setImage(image, shared);
}
ToolTipX::TipOnlyImage::~TipOnlyImage() {
	resetImage();
}
void ToolTipX::TipOnlyImage::setSize(HDC hdc, RECT &rc) {
	if (!this->_image) return;
	rc.bottom = this->_imageH;
	rc.right = this->_imageW;
}
void ToolTipX::TipOnlyImage::paint(HDC hdc, RECT rc) {
	if (!this->_image) return;
	//DrawBitmap(hdc , this->_image , rc.left , rc.top, false);
	DrawBitmapBlended(hdc , this->_image , rc.left , rc.top, 255);

}
void ToolTipX::TipOnlyImage::setImage(HBITMAP image, bool shared) {
	this->resetImage();
	this->_imageShared = shared;
	this->_image = image;
	BITMAP b;
	if (image && GetObject(image , sizeof(BITMAP),&b)) {
		this->_imageW = b.bmWidth;
		this->_imageH = b.bmHeight;
	} else {
		this->_imageW = 0;
		this->_imageH = 0;
	}
}
void ToolTipX::TipOnlyImage::resetImage() {
	if (this->_image && !this->_imageShared) {
		DeleteObject(this->_image);
	}
	this->_image = 0;
}


ToolTipX::TipPtr ToolTipX::Tip::titleAndText(HICON icon, const CStdString & title, const CStdString & text, bool textRich, bool iconShared) {
	Tip * tip = new Tip();
	tip->add(TipPtr(new TipOnlyTitle(title, icon, iconShared)));
	if (textRich) {
		tip->add(TipPtr(new TipTextRich(text)));
	} else {
		tip->add(TipPtr(new TipText(text)));
	}
	return TipPtr(tip);
}
ToolTipX::TipPtr ToolTipX::Tip::titleTextAndImage(HICON icon, const CStdString & title, const CStdString & text, bool textRich, HBITMAP image, bool iconShared,bool imageShared) {
	Tip * tip = new Tip();
	tip->add(TipPtr(new TipOnlyTitle(title, icon, iconShared)));
	if (textRich) {
		tip->add(TipPtr(new TipTextRich(text)));
	} else {
		tip->add(TipPtr(new TipText(text)));
	}
	tip->add(TipPtr(new TipOnlyImage(image, imageShared)));
	return TipPtr(tip);
}
ToolTipX::TipPtr ToolTipX::Tip::textAndImage(const CStdString & text, bool textRich, HBITMAP image, bool imageShared) {
	Tip * tip = new Tip();
	if (textRich) {
		tip->add(TipPtr(new TipTextRich(text)));
	} else {
		tip->add(TipPtr(new TipText(text)));
	}
	tip->add(TipPtr(new TipOnlyImage(image, imageShared)));
	return TipPtr(tip);
}
void ToolTipX::Tip::add(ToolTipX::TipPtr tip, enAlign align) {
	RECT rc = {0,0,0,0};
	this->_elements.push_back(Element(tip, rc, align));
}
void ToolTipX::Tip::setSize(HDC hdc, RECT &rc) {
	int x = margin;
	int y = margin;
	int startX = x;
	rc.right = rc.bottom = 0;
	enAlign previousAlign = alignAuto;
	for (tElements::iterator it = this->_elements.begin(); it != this->_elements.end(); ++it) {
		RECT elRc = {0,0,250,18};
		(*it)->setSize(hdc, elRc);
		int w = elRc.right - elRc.left;
		int h = elRc.bottom - elRc.top;
		/* Wrapujemy gdy:
		nowa szerokosc przekroczy wrapWidth
		lub
		szerokosc > wysokosc
		lub poprzedni ma alignLeft
		lub aktualny ma alignRight
		*/
		if (x != startX && (x+w > wrapWidth || (w > h))
			&& !(it->align & alignRight)
			&& !(previousAlign & alignLeft)
			|| (it->align & alignWrap)
			) 
		{
			x = startX; // wrapujemy...
			y = rc.bottom + 2*margin;
		}
		OffsetRect(&elRc, x, y);
		x += 2*margin + w;
		rc.right = max(rc.right, elRc.right);
		rc.bottom = max(rc.bottom, elRc.bottom);
		it->rc = elRc;
		previousAlign = it->align;
	}
	rc.right+=2*margin;
	rc.bottom+=2*margin;
}
void ToolTipX::Tip::paint(HDC hdc, RECT rc) {
	for (tElements::iterator it = this->_elements.begin(); it != this->_elements.end(); ++it) {
		RECT elRc = it->rc;
		OffsetRect(&elRc, rc.left, rc.top);
		(*it)->paint(hdc, elRc);
		//FrameRect(hdc, &elRc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	}
}
void ToolTipX::Tip::attach(HWND hwnd) {
	for (tElements::iterator it = this->_elements.begin(); it != this->_elements.end(); ++it) {
		(*it)->attach(hwnd);
	}
}
void ToolTipX::Tip::detach(HWND hwnd) {
	for (tElements::iterator it = this->_elements.begin(); it != this->_elements.end(); ++it) {
		(*it)->detach(hwnd);
	}
}
int ToolTipX::Tip::parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned) {
	for (tElements::iterator it = this->_elements.begin(); it != this->_elements.end(); ++it) {
		int r = (*it)->parentMessageProc(hwnd, message, wParam, lParam, returned);
		if (returned) return r;
	}
	return 0;
}
