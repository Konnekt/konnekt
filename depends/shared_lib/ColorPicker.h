#ifndef COLORPICKER_H
#define COLORPICKER_H

#define WM_COLORPICKER_CHANGE WM_USER+0x3673

#define CP_CHECKBOX 0x1000000

#define COLOR_DISABLED_MASK 0xF0000000

class CColorPicker {
    public:
        CColorPicker(DWORD color , DWORD style , int x , int y , int w , int h , HWND parent , HMENU id , HINSTANCE hInst , LPARAM param);
        ~CColorPicker();
        void Refresh();
        void ChooseColor();

        int style;
        HWND _hwnd;
        DWORD _color;
        WNDPROC _subclass;
        HFONT _font;
        bool _disabled;
    private:
};


#endif
 