
/*
TEST:

bleeeee<b>bold<b><font color="#FF0000">gnie¿<u>d¿ony</u></font></b>i <i>jesz</i>cze</b>koniec<br/>
SPANY:<br/>
piiiipii<span class="mark">MARKO-<span class="mark">WAN</span>IE</span><br/>
<p align="center>Œrodek</p>W nowej linii<div align="right">po prawo</div>

*/

#include <stack>

#include "richhtml.h"


#define FUNC_NOTHREAD
#include "func.h"

#include "simxml.h"
#include "fontpicker.h"


HWND cRichEditHtml::Create(int style , int x , int y , int w , int h , HWND parent , HMENU id) {
	HWND hwnd = CreateWindowEx(0 , RICHEDIT_CLASS  , "" , style | WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | 
                    ES_READONLY | WS_VSCROLL/* | WS_TABSTOP*/
                    ,x,y,w,h,parent,id , 0 , 0);
	Init(hwnd);
	return hwnd;
}
void cRichEditHtml::Init(HWND hwnd) {
	Init(hwnd, true, GetSysColor(COLOR_3DFACE));
}
void cRichEditHtml::Init(HWND hwnd, bool getLinks, COLORREF color) {
	if (GetProp(hwnd, "cRichEditHtml::SubProc")) return;
	SendMessage(hwnd , EM_SETLANGOPTIONS , 0 , 0);
	SendMessage(hwnd , EM_AUTOURLDETECT , getLinks ? 1 : 0 , 0);
	SendMessage(hwnd , EM_SETEVENTMASK  , 0 , ENM_SELCHANGE | (getLinks ? ENM_LINK : 0));
	SendMessage(hwnd , EM_SETBKGNDCOLOR , 0 , color);
	SetProp(hwnd, "cRichEditHtml::SubProc", (HANDLE)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WindowProc));
	if (GetWindowLong(hwnd, GWL_STYLE) & ES_READONLY) {
		HideCaret(hwnd);
	}
}
bool cRichEditHtml::IsReadOnly(HWND hwnd) {
	return (GetWindowLong(hwnd, GWL_STYLE) & ES_READONLY);
}


void cRichEditHtml::SetHTML(HWND hwnd , const CStdString & body , cRichEditFormat::fSetStyleCB styleCB) {
	SendMessage(hwnd , WM_SETREDRAW , 0,0);
	SetWindowText(hwnd , "");
	SendMessage(hwnd , EM_SETSEL , 0 , 0);
	InsertHTML(hwnd , body , styleCB);
	SendMessage(hwnd , EM_SETSEL , 0 , 0);
	if (GetWindowLong(hwnd, GWL_STYLE) & ES_READONLY) {
		HideCaret(hwnd);
	}
	POINT pt;
	pt.x = pt.y = 0;
	SendMessage(hwnd , EM_SETSCROLLPOS , 0 , (int)&pt);
	SendMessage(hwnd , EM_HIDESELECTION , 1 , 0);
	SendMessage(hwnd , WM_SETREDRAW , 1,0);
	RepaintWindow(hwnd);
}


DWORD CALLBACK StreamInNewLine(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb) {
	int & lines = *(int*)dwCookie;
	if (lines == 0 || cb < 2) {
		*pcb = 0;
	} else {
/*		char * buff; 
		if (lines == 2) {
			buff = "{\\rtf\\par}";
		} else {
			buff = "{\\rtf\\line}";
		}
		*pcb = strlen(buff);
		memcpy(pbBuff , buff , *pcb);
*/
		if (lines == 2) {
			pbBuff[0] = '\r';
		}
		pbBuff[lines - 1] = '\n';
		*pcb = lines;
		lines = 0;
	}
	return 0;
}

#define COLOR_SWAP(c) ((c & 0x00FF0000)>>16) | (c&0xFF00) | ((c & 0x00FF)<<16)

void cRichEditHtml::InsertHTML(HWND hwnd ,  CStdString body , cRichEditFormat::fSetStyleCB styleCB) {
	cRichEditFormat f(hwnd);
	f.InsertHTML(body, styleCB);
}
void cRichEditHtml::Clear(HWND hwnd) {
	SetWindowText(hwnd, "");
}

void cRichEditFormat::InsertHTML(CStdString body , fSetStyleCB styleCB) {
	cXML XML;
	cPreg preg(0);
	body = preg.replace("#\\r|\\n|\\&\\#1[30];#" , "" , body);
//	body = preg.replace("#<br/?>#is" , "" , body);
/* HACK */
	body = preg.replace("#(?<!\\>)<br/?>(?!\\<)#is" , "\n" , body);
	body = preg.replace("#<br>#is" , "<br/>" , body);
	XML.loadSource(body);
	cXML::nwInfo ni;
	size_t last = 0 , lastApply = -1 , current = 0;

	CStdString prefix = "";
	CStdString suffix = "";
	while (XML.nodeWalk(ni)) {
		XML.pos.start = XML.pos.end;
		XML.pos.start_end = XML.pos.end_end;
		if (last < ni.start) {
			SETTEXTEX ste;
			ste.codepage = CP_ACP;
			ste.flags = ST_SELECTION | ST_KEEPUNDO;
			SendMessage(hwnd , EM_SETTEXTEX , (int)&ste , (LPARAM)(prefix + CStdString(DecodeEntities(body.substr(last , ni.start - last))) + suffix).c_str());
		}
		current += ni.start - last;
		last = ni.end;
		CStdString token = ni.path.substr(ni.path.find_last_of('/')+1);
		prefix = "";
		suffix = "";
		token.MakeLower();
		int newParagraph = 0;
		int oper = (ni.state == cXML::nwInfo::opened)? 1 : -1;
		if (ni.state == cXML::nwInfo::closed)
			oper = 0;
		if (oper < 0) {
			/* generalnie... nic nie trzeba robiæ... */
		} else if (oper == 0) {
			if (token == "br")
				newParagraph = 1;
			cf.dwMask = pf.dwMask = 0;
		} else if (token == "b") {
			cf.dwMask |= CFM_BOLD;
			cf.dwEffects |= CFE_BOLD;
		} else if (token == "i") {
			cf.dwMask |= CFM_ITALIC;
			cf.dwEffects |= CFM_ITALIC;
		} else if (token == "u") {
			cf.dwMask |= CFM_UNDERLINE;
			cf.dwEffects |= CFM_UNDERLINE;
		} else if (token == "sub") {
			cf.dwMask |= CFM_SUBSCRIPT;
			cf.dwEffects |= CFE_SUBSCRIPT;
		} else if (token == "sup") {
			cf.dwMask |= CFM_SUPERSCRIPT;
			cf.dwEffects |= CFE_SUPERSCRIPT;
		} else if (token == "sup") {
		} else if (token == "font") {
			cf.dwMask |= CFM_COLOR | CFM_FACE | CFM_SIZE;
			CStdString color = XML.getAttrib("color");
			CStdString face = XML.getAttrib("face");
			CStdString size = XML.getAttrib("size");
			/* W aktualnym CF ju¿ siedzi ostatnio ustawiony kolor, czy font, nie trzeba wiêc nic kombinowaæ :) */
			if (!color.empty())
				cf.crTextColor = COLOR_SWAP(chtoint(color));
			if (!face.empty())
				strcpy(cf.szFaceName , face);
			/* parametr Size to ogólnie liczby dodatnie.. * 20 daj¹ dobr¹ wysokoœæ... */
			if (!size.empty())
				cf.yHeight = chtoint(size) * 20;
		} else if (token == "div" || token == "p") {
			CStdString align = XML.getAttrib("align");
			if (!align.empty()) {
				pf.dwMask |= PFM_ALIGNMENT;
				int alignment = 0;
				if (align == "center") alignment = PFA_CENTER;
				else if (align == "right") alignment = PFA_RIGHT;
				else if (align == "justify") alignment = PFA_JUSTIFY;
				else alignment = PFA_LEFT;
				pf.wAlignment = alignment;
			}
		}
		if (oper > 0 && styleCB) {
			// Nak³adamy styl...
			CStdString styleClass = XML.getAttrib("class");
			if (!styleClass.empty()) {
				cSetStyle ss (cf , pf);
				styleCB(token , styleClass , ss);
			}
		}

		/* Nak³adamy zmiany */
		if (oper < 0) {
			if (token == "p" || token == "div")
				newParagraph = 2;
		}
		this->operate(oper);

		if (newParagraph && ni.start/* && ni.end < preg.getSubjectRef().size()-2*/) {
			InsertNewLine(newParagraph);
		}
		/* Nie przywracamy poprzedniego paragrafu ostatniej linijce... 
		¯eby przywróciæ t¹ funkcjonalnoœæ, trzeba tekst zakoñczyæ dodatkowo <br/> */
		this->apply();
		if (cf.dwMask || pf.dwMask)
			lastApply = current;

//		}
	}
	if (ni.end != -1) {
		InsertText(DecodeEntities(body.substr(ni.end)));
	}
}



int CALLBACK cRichEditHtml::ParentWindowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam) {
	if (message == WM_NOTIFY) {
        ENLINK* enl;
        enl = (ENLINK*)lParam;
		if (enl->nmhdr.code == EN_LINK) {
	        if (enl->msg == WM_LBUTTONUP) {
				TEXTRANGE tr;
				tr.chrg = enl->chrg;
				if ((tr.chrg.cpMax - tr.chrg.cpMin) > 500) return 0;
				tr.lpstrText = new char [tr.chrg.cpMax - tr.chrg.cpMin + 1];
				tr.lpstrText[0] = 0;
				if (SendMessage(enl->nmhdr.hwndFrom , EM_GETTEXTRANGE  , 0 , (LPARAM)&tr) && tr.lpstrText[0])
					ShellExecute(0 , "open" , tr.lpstrText , "" , "" , SW_SHOW);
				delete [] tr.lpstrText;
			}
		} else if (enl->nmhdr.code == EN_SELCHANGE) {
			if (IsReadOnly(enl->nmhdr.hwndFrom))
				HideCaret(enl->nmhdr.hwndFrom);
		}
	}
	return 0;
}
int CALLBACK cRichEditHtml::WindowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_SYSKEYDOWN: case WM_KEYDOWN:
			if (!IsReadOnly(hwnd)) break;
			if (wParam != VK_UP 
				&& wParam != VK_DOWN 
				&& wParam != VK_LEFT
				&& wParam != VK_RIGHT
				&& wParam != VK_LEFT
				&& wParam != 'C'
				) {
				return 1;
			}
			break;
	}
	return CallWindowProc((WNDPROC)GetProp(hwnd, "cRichEditHtml::SubProc"), hwnd, message, wParam, lParam);

}

cRichEditFormat::cRichEditFormat(HWND hwnd) {
	this->hwnd = hwnd; 
	memset(&cf , 0 , sizeof(cf));
	memset(&pf , 0 , sizeof(pf));
	cf.cbSize = sizeof(cf);
	pf.cbSize = sizeof(pf);

	cf.dwMask = -1; 
	SendMessage(hwnd , EM_GETCHARFORMAT , SCF_SELECTION , (LPARAM)&cf);
	pf.dwMask = -1;
	SendMessage(hwnd , EM_GETPARAFORMAT , 0 , (LPARAM)&pf);

	state.cf.push(cf);
	state.pf.push(pf);
	state.maskcf.push(0);
	state.maskpf.push(0);

	cf.dwMask = 0;
	pf.dwMask = 0;
}
void cRichEditFormat::InsertText(const CStdString & body) {
	SETTEXTEX ste;
	ste.codepage = CP_ACP;
	ste.flags = ST_SELECTION | ST_KEEPUNDO;
	SendMessage(hwnd , EM_SETTEXTEX , (int)&ste , (LPARAM)(body.c_str()));
}
void cRichEditFormat::InsertNewLine(int paragraph) {
	EDITSTREAM es;
	es.pfnCallback = StreamInNewLine;
	int lines = paragraph;
	es.dwCookie = (DWORD) &lines;
	SendMessage(hwnd , EM_STREAMIN , SF_TEXT | SFF_SELECTION , (LPARAM)&es);
}

void cRichEditFormat::push(bool apply) {
	operate(1);
	if (apply) this->apply();
}
void cRichEditFormat::pop(bool apply) {
	operate(-1);
	if (apply) this->apply();
}
void cRichEditFormat::operate(int oper) {
	if (oper) {
		if (oper < 0) {
			/* Przy "schodzeniu" nak³adamy poprzedni (po operate) format z aktualn¹ mask¹... */
			cf.dwMask = state.maskcf.top();
			pf.dwMask = state.maskpf.top();
		}
		if (cf.dwMask)
			state.cf.operate(oper , cf);
		if (pf.dwMask)
			state.pf.operate(oper , pf);
		state.maskcf.operate(oper , cf.dwMask);
		state.maskpf.operate(oper , pf.dwMask);
		if (oper < 0) {
			DWORD cfmask = cf.dwMask;
			DWORD pfmask = pf.dwMask;
			cf = state.cf.top();
			pf = state.pf.top();
			cf.dwMask = cfmask;
			pf.dwMask = pfmask;
		}
	} else
		cf.dwMask = pf.dwMask = 0;
}

void cRichEditFormat::apply() {
	if (pf.dwMask/* && ni.end < preg.getSubjectRef().size()-2*/) {
		SendMessage(hwnd , EM_SETPARAFORMAT , 0 , (LPARAM)&pf);
		pf.dwMask = 0;
	}
	if (cf.dwMask) {
		SendMessage(hwnd , EM_SETCHARFORMAT , SCF_SELECTION , (LPARAM)&cf);
		cf.dwMask = 0;
	}
}
void cRichEditFormat::Clear() {
	cRichEditHtml::Clear(this->hwnd);

	state.cf.reset();
	state.pf.reset();
	state.maskcf.reset();
	state.maskpf.reset();
	apply();
}

void cRichEditFormat::setBold(bool bold, bool apply) {
	cf.dwMask |= CFM_BOLD;
	cf.dwEffects |= CFE_BOLD;
	if (apply) this->push();
}
void cRichEditFormat::setItalic(bool italic, bool apply) {
	cf.dwMask |= CFM_ITALIC;
	cf.dwEffects |= CFE_ITALIC;
	if (apply) this->push();
}
void cRichEditFormat::setUnderline(bool underline, bool apply) {
	cf.dwMask |= CFM_UNDERLINE;
	cf.dwEffects |= CFE_UNDERLINE;
	if (apply) this->push();
}
void cRichEditFormat::setColor(int color, bool apply) {
	cf.dwMask |= CFM_COLOR;
	cf.crTextColor = color;
	if (apply) this->push();
}
void cRichEditFormat::setSize(int size, bool apply) {
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = size * 20;
	if (apply) this->push();
}
