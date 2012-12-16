#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdarg.h>
//#define SS_NO_REFCOUNT
#include <stdstring.h>

#define C_STR(str) (char*)(str).c_str()

#ifndef MAXDIR
  #define MAXDIR 250
#endif  
#define MAX_STRBUFF 1024
#define MAX_STRING 10000
#define MAX_STRING2 (MAX_STRING/2 - 2)


class cThread {
    public:
    CStdString str; 
    char buff[MAX_STRING+1];
    char * buff2;
    cThread();
};


template <class T> class cTLS {
    private:
    unsigned long tls_index;
    public:
    T & operator ()(void);
    void detach(void);
    cTLS();
    ~cTLS();
};

#ifndef FUNC_NOTHREAD

    template <class T> T & cTLS<T>::operator ()(void) {
        T * t = reinterpret_cast<T*>(TlsGetValue(tls_index));
        if (!t) {t=new T;TlsSetValue(tls_index , t);}
        return *t;
    }

    template <class T> void cTLS<T>::detach(void) {
        T * t = reinterpret_cast<T*>(TlsGetValue(tls_index));
        if (t) {delete t; TlsSetValue(tls_index , 0);}
    }

    template <class T> cTLS<T>::cTLS() {    
        tls_index = TlsAlloc();
    }

    template <class T> cTLS<T>::~cTLS() {
        detach();
        TlsFree(tls_index);
    }


    #ifdef IN_FUNC_CPP
        cTLS<cThread> TLS;
    #else 
        extern cTLS<cThread> TLS;
    #endif

#endif

const char * _sprintf(const char * fmt , ...);

const char * inttoch(int v , int radix=10 , int max=-1 , bool upper=1);
const char * inttoch(int v , char * buff , int radix=10 , int max=-1 , bool upper=1);
CStdString inttostr(int v , int radix=10 , int max=-1 , bool upper=1);

unsigned char chval(unsigned char ch);

int chtoint(const char * str , char base=0xFF);

char * safeChar(void * v);

char * safeChar(int v);

char * removeChar(char * str , char ch);
char * removeChars(char * str , const char * ch);
char * clearChar(char * str);
int strcount(const char * str , char ch);


char *stristr(const char *String, const char *Pattern);


int rmdirtree(const char * path);
int mkdirs(const char * path);

char * filepath(char * path);


#if defined(MD5C_H)
  unsigned char * MD5(const char * string , unsigned char * digest);
  char * MD5Hex(const char * string , char * digest);
  __int64 MD5_64(const char * string);

  #ifdef _STRING_
  std::string MD5Hex(const char * string);
  std::string MD5FileHex(const std::string fileName);
  #endif
#endif

char * _vsaprintf(const char *format, va_list ap);

char * _saprintf(const char *format, ...);
//char * _saprintf(const char *format, ...);

#ifdef _STRING_
std::string stringf(const char *format, ...);
std::string stringf(std::string & str , const char *format, ...);
#endif
//void throwf(const char *format, ...) {
//}

void cp2iso(char *buf);
void iso2cp(char *buf);
void hidePL(char *c);

char * str_tr(char * str , const char * chIn , const char * chOut);

const char * getArgV(const char * find , bool getValue=false , const char * def = 0);
const char * getArgV(const char * const * argList , int argCount , const char * find , bool getValue=false , const char * def=0);
const char * searchArray(const char * find , const char ** ar  , size_t count , bool getNext=false);

#if defined(_VECTOR_) && defined(_STRING_)
typedef std::vector<std::string> tStringVector;
void Split(const std::string & txt, const std::string & splitter, tStringVector & list, bool all = false);
void SplitCommand(const std::string & txt , char splitter ,  tStringVector & list);
#endif


#ifdef _WIN32
	#define WINFUNCH

	#define STR(s) s 
    #define STRING(s) std::string(s).c_str()
//std::string(s).c_str()


	extern HINSTANCE hInst;

	struct FILEVERSIONINFO {
	    VS_FIXEDFILEINFO * ffi;
	    WORD major , minor , release , build;
        bool readInfo;
	    std::string Comments , InternalName , ProductName ,
		    CompanyName , LegalCopyright , ProductVersion ,
		    FileDescription , LegalTrademarks , PrivateBuild ,
		    FileVersion , OriginalFilename , SpecialBuild ,
		    URL;
        FILEVERSIONINFO(bool readInfo=false) {major=minor=release=build=0;ffi=0;this->readInfo=readInfo;}
	};

	// DEFINES

	bool FileVersionInfo(const char * fn , char * fmt=0 , FILEVERSIONINFO * res = 0);

	void * LoadBinaryData(HMODULE inst, const char * name, const char * type, int * size=0);


	std::string latin2(std::string str);
	char * resStr(UINT id , HINSTANCE inst=0);
	char * resStr(HINSTANCE inst , UINT id);
	char * resStri(UINT id , int inst=0);
	bool ProcessMessages(HWND hwnd);
	void ShowLastError();
	const char * GetLastErrorMsg(int err=0);
	int ErrorMsg(int err=0);
	void DrawBitmap (HDC hDC , HBITMAP hBmp , int x , int y , bool trans = 1);
	void DrawBitmapBlended (HDC hDC , HBITMAP hBmp , int x , int y, int alpha = 255);
	void BlendBitmapWithColor (HBITMAP hBmp, COLORREF color, int alpha = 255);

	int GetImageType(HANDLE image);
	SIZE GetImageSize(HANDLE image, int type=-1);


	HFONT GetLogFont(char * face , int height , char style);
	POINT Point (int x , int y);
	void GetChildRect (HWND hWnd , RECT * prc);
	bool InsideRect(RECT rc1 , RECT rc2);
	RECT Rect(int l , int t , int r , int b , bool ref = false);
	SIZE Size(int w , int h);
	int SetWindowRect(HWND hwnd , RECT rc);
	DWORD GetDllVersion(LPCTSTR lpszDllName);
	bool isComctl(unsigned int maj , unsigned int min);
	HICON LoadIconEx(HINSTANCE hInst , LPCTSTR id , int size , int bits=0);
	const char * longToIp(long adr);
	int ipToLong(const char * ip);
	void * memrev(void * buff , int count);
	SIZE ToolBar_GetSize(HWND hwnd);
	void clipSize(int edge , RECT * rc , int minW , int minH, int maxW=0x7FFF, int maxH=0x7FFF);
	void setRectSizeByEdge(int edge , RECT * rc , int w, int h);
	void expandRect(RECT & rc, int x, int y);
	void fitInRect(RECT & rc, RECT fitIn);
	SIZE GetTextSize(HWND hwnd, HFONT font, const char * text, int width = 0);
	SIZE GetTextSize(HDC hdc, const char * text, int width = 0);

	int GetFontHeight(HWND hwnd, HFONT font);
	int GetFontHeight(HDC hdc);

    std::string urlEncode(std::string str, char special = '%' , char * noChange = 0);
    std::string urlDecode(std::string str, char special = '%');
    std::string resString(UINT id , HINSTANCE inst=0);
	int GetLastInputTime();
	SIZE ToolBar_GetSize(HWND hwnd);
     
    #define RGB_ALPHA(color1 , color2 , alpha) RGB( \
        (GetRValue(color1)*alpha+GetRValue(color2)*(100-alpha))/100 \
        , (GetGValue(color1)*alpha+GetGValue(color2)*(100-alpha))/100 \
        , (GetBValue(color1)*alpha+GetBValue(color2)*(100-alpha))/100)

	COLORREF getUniqueColor(int i, int stepping, int range, bool bright, bool reversed = false);


	bool _SetDllDirectory(const char * dir);

    bool SetAlpha(HWND hwnd, unsigned char Alpha);
    bool IsWindowUncovered(HWND hwnd);
    void RepaintWindow(HWND hwnd);
	bool WindowBelongsTo(HWND window, HWND parent);

POINT getScreenPoint();
POINT getLocalPoint(HWND window, POINT screen, int scrollX=-1, int scrollY=-1);
	
	void SetKeyboardLed(BYTE key , BOOL bState );
std::string ExpandEnvironmentStrings(const char * src, unsigned int size = MAX_PATH);


#endif


#endif 