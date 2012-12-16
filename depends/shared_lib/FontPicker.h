#ifndef FONTPICKER_H
#define FONTPICKER_H

#define CF_NOCOLORSEL 0x40000000L
#define CF_NOBGCOLORSEL 0x80000000L

#define FP_CHECKBOX 0x100000

#define WM_FONTPICKER_CHANGE WM_USER+0x3674


#define FONT_DISABLED_CHAR '~'

class LOGFONTEX {
public:
    LOGFONT lf;
    COLORREF color;
    COLORREF bgColor;
    bool active;

	LOGFONTEX() {
		color = 0;
		bgColor = 0xFFFFFF;
		_font = 0;
	}

	HFONT getFont() {
		if (_font == 0) {
			_font = CreateFontIndirect(&lf);
		}
		return _font;
	}

	bool isBold() {
		return lf.lfWeight > FW_NORMAL;
	}

	void resetFont() {
		if (_font) {
			DeleteObject(_font);
			_font = 0;
		}
	}

private:
	HFONT _font;

};

class CFontPicker {
    public:
        CFontPicker(const char * preview , const char * font , DWORD fdlStyle, DWORD style , int x , int y , int w , int h , HWND parent , HMENU id , HINSTANCE hInst , LPARAM param);
        ~CFontPicker();
        void Refresh();
        void ChooseFont();
        HWND _hwnd;
        std::string _preview;
        LOGFONTEX _font;
        WNDPROC _subclass;
        int _style;
        int style;
    private:
};

/* Zapis: 
face;charset;size;flags;color;bck;
np.
Tahoma;;10;ibu;0;16767623;
*/

LOGFONTEX StrToLogFont(std::string str);

std::string LogFontToStr(LOGFONTEX font);

DWORD PXToHeight(int px);

#endif
 