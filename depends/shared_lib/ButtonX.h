#ifndef BUTTONX_H
#define BUTTONX_H

class ButtonX {
    public:
        ButtonX(const char * title , int style, int x , int y , int w , int h , HWND parent , HMENU id , HINSTANCE hInst , LPARAM param);
        ButtonX(HWND hwnd);
        ~ButtonX();
		static ButtonX * fromHWND(HWND wnd);
		void setIcon(HICON icon, bool shared=false);
		void setIcon(HIMAGELIST list, int pos);
		HWND getHWND() {return _hwnd;}
    private:
		void freeIcon();
		void initWindow();
		static int CALLBACK windowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam);
		void drawCaption(HDC hdc, RECT updRect);
		static WNDPROC buttonClassProc;
        bool _sharedIcon;
		HICON _icon;
		int _iconW, _iconH, _iconMargin;
        HWND _hwnd;
		CStdString _text;
		bool _duringPaint;
		bool _flat;
};


#endif
 