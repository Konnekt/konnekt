#pragma once
#include <windows.h>
#include <Richedit.h>
#include <stdstring.h>


class cRichEditFormat {
public:
	struct cSetStyle {
		cSetStyle(CHARFORMAT2 & cf , PARAFORMAT2 & pf): cf(cf), pf(pf) {}
		CHARFORMAT2 & cf;
		PARAFORMAT2 & pf;
	};
	typedef void (*fSetStyleCB)(const CStdString & token , const CStdString & styleClass , cSetStyle & ss);

#ifdef _STACK_

	cRichEditFormat(HWND hwnd);

	void InsertHTML(CStdString body , fSetStyleCB styleCB = 0);
	void Clear();
	void InsertText(const CStdString & body);
	void InsertNewLine(int paragraph); /* 1-BR 2-P */

	void operate(int oper);
	void push(bool apply=true);
	void pop(bool apply=true);
	void apply(); // ustawia format

	void setBold(bool bold, bool apply=true);
	void setItalic(bool italic, bool apply=true);
	void setUnderline(bool underline, bool apply=true);
	void setColor(int color, bool apply=true);
	void setSize(int size, bool apply=true);

	template <class TY> 
	class formatStack : public std::stack<TY> {
	public:
		void operate(int oper , TY & v) {
			if (oper > 0) {
				this->push(v);
			} else if (oper < 0) {
				if (this->size() < 2) return; // za ma³o...
				this->pop();
			}
		}
		void reset() {
			while (this->size() > 1)
				this->pop();
		}
	};


	struct cState {
		formatStack <CHARFORMAT2> cf;
		formatStack <PARAFORMAT2> pf;
		formatStack <DWORD> maskcf;
		formatStack <DWORD> maskpf;
	} state;
	CHARFORMAT2 cf;
	PARAFORMAT2 pf;


private:
	HWND hwnd;

#endif
};



class cRichEditHtml {
public:
	static HWND Create(int style , int x , int y , int w , int h , HWND parent , HMENU id);
	static void InsertHTML(HWND hwnd ,  CStdString body , cRichEditFormat::fSetStyleCB styleCB = 0);
	static void SetHTML(HWND hwnd , const CStdString & body , cRichEditFormat::fSetStyleCB styleCB = 0);
	static void Init(HWND hwnd);
	static void Init(HWND hwnd, bool getLinks, COLORREF color);
	static void Clear(HWND hwnd);
	static bool IsReadOnly(HWND hwnd);
	static int CALLBACK ParentWindowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam);
	static int CALLBACK WindowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam);
};
