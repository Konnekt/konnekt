
#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdstring.h>
#include <Shlwapi.h>     
#include <md5/md5.h>
#include <commctrl.h>  
#include <math.h>
#include <vector>
using namespace std;
#ifdef __BORLANDC__
  #include <dir.h>
#else
  #include <direct.h>
#endif

#define IN_FUNC_CPP

#include "func.h"

//static __declspec(thread) char str_buff [MAX_STRBUFF]="";
//static __declspec(thread) char str_buff2 [MAX_STRBUFF]="";

#ifndef MAXDIR
  #define MAXDIR 250
#endif  


HINSTANCE hInst = 0;

cThread::cThread(){buff2 = buff + MAX_STRING2+1; 
    }


const char * _sprintf(const char * fmt , ...) {
  va_list ap;
  va_start(ap , fmt);
  CStdString str;
  _vsnprintf(TLS().str.GetBuffer(MAX_STRING) ,MAX_STRING , fmt , ap);
  TLS().str.ReleaseBuffer();
  return TLS().str.c_str();
}

const char * inttoch(int v , char * s , int radix , int max , bool upper) {
  _itoa(v , s , radix);
  if (!*s) strcpy(s , "0");
  if (max>0 && strlen(s)!=(unsigned)max) {
    int sz=strlen(s);
    if (sz>max) {
      for (int i=0;i<max;i++) {
        s[i]=s[sz-max+i];
      }
    } else {
      for (int i=max-1;i>=0;i--) {
//      s[i]='E';
        if (i<max-sz) s[i]='0';
          else s[i]=s[i-max+sz];
      }
    }
    s[max]='\0';

  }
  if (radix>10 && upper) strupr(s);
  return s;
}
const char * inttoch(int v , int radix , int max , bool upper) {
	return inttoch(v , TLS().buff , radix , max , upper);
}
CStdString inttostr(int v , int radix , int max , bool upper) {
	CStdString s;
	inttoch(v, s.GetBuffer(10) , radix,max,upper);
	s.ReleaseBuffer();
   return s;
}


unsigned char chval(unsigned char ch) { // zamienia znak na liczbe i odwrotnie
  /* Zamieniamy liczbe na znak */
  if (ch < 10) return '0'+ch; // 0 - 10
  if (ch < '0') return 'A'+(ch-10); // A - ...
  /* Zamieniamy znak na liczbe */
  if (ch < 'A') return ch-'0'; // 0-10
  if (ch < 'a') return ch-'A'+10; // A-...
  return ch-'a'+10; // a-...
}

int chtoint(const char * str , char base) {
  if (!str) return 0;
  bool sign=false; // ujemna
  // sprawdzamy znak ujemnoœci, który zawsze jest PRZED ew. definicj¹ bazy...
  if (*str == '-') {sign = true; str++;}
  // Sprawdzamy base..
  if (base == 0xFF) {
	  if (str[0] == '#' || (str[0]=='0' && str[1]=='x')) {
		  str+=(str[0] == '#')?1 : 2;
		  base = 16;
	  } else base = 10;
  }
  const char * last = str;
  // zliczamy od koñca, który najpierw trzeba znaleŸæ...
  while ((*last>='0' && *last<='9') || (*last>='a' && *last<='f') || (*last>='A' && *last<='F')) last++;
  if (last == str) return 0;
  last--; // Cofamy do pierwszego znaku...
  unsigned int l=0; // liczba wyjsciowa
  int b=1;  // base do potegi 0
  while (last >= str) { // wczytuje znaki od konca
     l+=chval(*last)*b; // dodaje do l wartosc znaku * podstawa podniesiona do potegi
     b*=base; // "podnosi" base o potege
	 last --;
  }
  return sign? (signed)-l : (signed)l;
}



char * safeChar(void * v) {
 return v?(char*)v:(char*)"";
}

char * safeChar(int v) {
 return v?(char*)v:(char*)"";
}

char * removeChars(char * str , const char * ch) {
  char * buff = str;
  int offset = 0;
  while (*buff) {
    if (strchr(ch , *buff)) offset++;
      else
        if (offset)
          *(buff - offset) = *buff;

    buff++;
  }
  *(buff - offset) = *buff;
  return str;
}
char * removeChar(char * str , char ch) {
	char _ch[2];
	_ch[0] = ch;
	_ch[1] = 0;
	return removeChars(str , _ch);
}

char * clearChar(char * str) {
  return str;
}

int strcount(const char * str , char ch) {
  int c = 0;
  while (*str) {
    if (*str == ch) c++;
    str++;
  }
  return c;
}

/*
char toupperl(char) {
  return toupper(char);
}

char tolowerl(char) {
  return tolower(char);
}

char *stristr(const char *String, const char *Pattern)
{
      char *pptr, *sptr, *start;
      uint  slen, plen;

      for (start = (char *)String,
           pptr  = (char *)Pattern,
           slen  = strlen(String),
           plen  = strlen(Pattern);

           // while string length not shorter than pattern length

           slen >= plen;

           start++, slen--)
      {
            // find start of pattern in string
            while (toupperl(*start) != toupperl(*Pattern))
            {
                  start++;
                  slen--;

                  // if pattern longer than string

                  if (slen < plen)
                        return(NULL);
            }

            sptr = start;
            pptr = (char *)Pattern;

            while (toupperl(*sptr) == toupperl(*pptr))
            {
                  sptr++;
                  pptr++;

                  // if end of pattern then pattern was found

                  if ('\0' == *pptr)
                        return (start);
            }
      }
      return(NULL);
}
*/

char *stristr(const char *String, const char *Pattern)
{
  char * str = strdup(String);
  char * pat = strdup(Pattern);
  str = strlwr(str);
  pat = strlwr(pat);
  char * r = strstr(str,pat);
  r = r ? (char*)String + (r-str) : 0;
  free(str); free(pat);
  return r;
}


int rmdirtree(const char * path) {
   char * ch = (char*)malloc(strlen(path) + 255);
   strcpy(ch , path);
   strcat(ch , "\\*.*");
   WIN32_FIND_DATA fd;
   HANDLE hFile;
   BOOL found;
   found = ((hFile = FindFirstFile(ch,&fd))!=INVALID_HANDLE_VALUE);

//   int i = 0;
   while (found)
   {
      if (*fd.cFileName != '.') {
        ch[strlen(path)+1] = 0;
        strcat(ch , fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          rmdirtree(ch);
        } else {
          unlink(ch);
        }
      }
      if (!FindNextFile(hFile , &fd)) break; 
  }
  FindClose(hFile);
  ch[strlen(path)] = 0;
/*
  char * ch2 = strrchr(ch , '\\');
  if (!ch2) ch2 = path;
  else {*ch2 = 0;
    ch2++;
  }
*/
  _rmdir(path);
  //MessageBox(0 , _sprintf("%s %s %d %d [%d]" , ch , ch2 , rmdir(path) , errno , ENOENT) , "" , 0);
  free(ch);
  return 0;
}

char * filepath(char * path) {
  char * p = strrchr(path , '\\');
  if (p) p[1] = 0;
  return path;
}

int mkdirs(const char * path) {
  if (!*path) return 1;
  int r;
  r = 0;
  if (access(path , 0)) {
    char * st = strrchr((char*)path , '\\');
    if (!st) return 1;
    *st=0;
    r = mkdirs(path);
    *st='\\';
    if (_mkdir(path)) {
      return 1;
    }
  }
  return r;
}
/*
int mkdirs(const char * path , bool isFile = false) {
	char * st;
	int r;
	if (isFile) {
		st = strrchr(path , '\\');
		*st=0;
		r = mkdirs(path);
		*st='\\';
		return r;
	}
	if (!*path) return 1;
	r = 0;
	if (_access(path , 0)) {
		if (errno!=ENOENT) return 0;
		st = strrchr((char*)path , '\\');
		if (st!=path) { // nie ma co siê siliæ na ostatni bekslesz
			if (st) {
				*st=0;
				r = mkdirs(path);
				*st='\\';
			}
			if (_mkdir(path)) {
				return 1;
			}
		}
	}
	return r;
}
*/

#ifdef MD5C_H

	unsigned char * MD5(const char * string , unsigned char * digest) {
        MD5_CTX context;
        unsigned int len = strlen (string);
        MD5Init (&context);
        MD5Update (&context, (unsigned char*)string, len);
        MD5Final (digest, &context);
        return digest;
    }

	__int64 MD5_64(const char * string) {
		unsigned char c [16];
        MD5(string , c);

		unsigned char d [8];

		for (int i=0; i < 8; i++) {
			d[i] = c[i*2] + c[i*2+1];
		}

        return *((__int64*)d);
    }

	char * MD5Hex(const char * string , char * digest) {
		unsigned char c [16];
        MD5(string , c);
        for (int i = 0; i<16; i++)
			inttoch(c[i] , &digest[i*2] , 16 , 2 , 0);
        return digest;
    }
    std::string MD5Hex(const char * string) {
        char digest[33];
		return MD5Hex(string , digest);
	}

    string MD5FileHex(const string fileName) {
        FILE *file;
        MD5_CTX context;
        int len;
        unsigned char buffer[1024], digest[16+32+1];

        if ((file = fopen (fileName.c_str(), "rb")) == NULL) {
            return "";
        } else {
            MD5Init (&context);
            while (len = fread (buffer, 1, 1024, file))
                MD5Update (&context, buffer, len);
            MD5Final (digest, &context);
            fclose (file);
        }
        for (int i = 0; i<16; i++)
            strcpy((char*)&digest[i*2 + 16] , inttoch(digest[i] , 16 , 2 , 0));
        digest[16+32] = 0;
        return (char*)(digest + 16);
    }
#endif

#ifdef __BORLANDC__
  #define VSNPRINTF vsnprintf
#else
  #define VSNPRINTF _vsnprintf
#endif

char * _vsaprintf(const char *format, va_list ap)
{
        va_list temp_ap = ap;
        char *buf = NULL, *tmp;
        int size = 0, res;

        if ((size = VSNPRINTF(buf, 0, format, ap)) < 1) {
                size = 128;
                do {
                        size *= 2;
                        if ((tmp = (char*)realloc(buf, size))==0) {
                                free(buf);
                                return NULL;
                        }
                        buf = tmp;
                        res = VSNPRINTF(buf, size, format, ap);
                } while (res == size - 1 || res==-1);
        } else {
                if ((buf = (char*)malloc(size + 1))==0)
                        return NULL;
        }                                    

        ap = temp_ap;

        VSNPRINTF(buf, size + 1, format, ap);

        return buf;
}

char * _saprintf(const char *format, ...);
char * _saprintf(const char *format, ...) {
	va_list ap;

	va_start(ap, format);
	char * ret = _vsaprintf(format, ap);
	va_end(ap);
        return ret;
}

std::string stringf(const char *format, ...) {
	va_list ap;

	va_start(ap, format);
	char * msg = _vsaprintf(format, ap);
        std::string _msg = msg;
        free(msg);
        va_end(ap);
        return _msg;
}


std::string stringf(std::string & str , const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	char * msg = _vsaprintf(format, ap);
        str = msg;
        free(msg);
        va_end(ap);
        return str;
}

//void throwf(const char *format, ...) {
//}






//- -------------------------------------------------------------------
// ====================================================================


bool FileVersionInfo(const char * fn , char * fmt , FILEVERSIONINFO * res) {
  VS_FIXEDFILEINFO * vi;
  int sz;
  sz = GetFileVersionInfoSize((char*)fn,0);
  CStdString block;
  char * buff;
  unsigned int b;
  if (sz>0) {
     buff=new char [sz];
     if (GetFileVersionInfo((char*)fn , 0, sz , buff)) {
        if (VerQueryValue(buff , "\\" , (void **)&vi , &b)) {
          if (fmt) {
            sprintf(fmt , "%d.%d.%d.%d%s%s" , HIWORD(vi->dwFileVersionMS) , LOWORD(vi->dwFileVersionMS) , HIWORD(vi->dwFileVersionLS) , LOWORD(vi->dwFileVersionLS)
                    , vi->dwFileFlags & VS_FF_PRERELEASE?"P":"" , vi->dwFileFlags & VS_FF_DEBUG?"D":"" );
          }
          if (res) {
            res->major = HIWORD(vi->dwFileVersionMS);
            res->minor = LOWORD(vi->dwFileVersionMS);
            res->release = HIWORD(vi->dwFileVersionLS);
            res->build   = LOWORD(vi->dwFileVersionLS);
          }
        }
     }
     // Wczytywanie descriptions
     if (res) {
        struct LANGANDCODEPAGE {
        WORD wLanguage;
        WORD wCodePage;
        } *lpTranslate;
        VerQueryValue(buff,TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&b);
        sprintf( block.GetBuffer(32),"\\StringFileInfo\\%04X%04X\\",lpTranslate[0].wLanguage,lpTranslate[0].wCodePage);
        block.ReleaseBuffer();
     }
  } else {
      if (res) {res->major = res->minor = res->release = res->build = 0;}
  }

  if (res) {
      if (res->readInfo) {
            char * buff2;
            #define FVI_GET(NAME) if (sz) {string subBlock=block + #NAME; res->NAME = VerQueryValue(buff,(char*)(subBlock).c_str(),(void**)&buff2,&b) ? buff2 : "";} else res->NAME="" 
            FVI_GET(Comments);
            FVI_GET(InternalName);
            FVI_GET(ProductName);
            FVI_GET(CompanyName);
            FVI_GET(LegalCopyright);
            FVI_GET(ProductVersion);
            FVI_GET(FileDescription);
            FVI_GET(LegalTrademarks);
            FVI_GET(PrivateBuild);
            FVI_GET(FileVersion);
            FVI_GET(OriginalFilename);
            FVI_GET(SpecialBuild);
            FVI_GET(URL);
            #undef FVI_GET
      }
     if (res->ffi) {
        if (sz) { 
            *res->ffi = *vi;
        } else {
            memset(res->ffi , 0 , sizeof(VS_FIXEDFILEINFO));	
        }
     }
  }
  if (sz>0) 
	  delete [] buff;
 return sz>0;
}

void * LoadBinaryData(HMODULE inst, const char * name, const char * type, int * size) {
	HRSRC found = FindResource(inst, name, type);
	if (!found) return 0;
	HGLOBAL rsrc = LoadResource(inst, found);
	if (!rsrc) return 0;
	if (size)
		*size = SizeofResource(inst, found);
	return LockResource(rsrc);
}


// ------------------------------------------------------
string urlEncode(string str , char special , char * noChange) {
  string res;
  string::iterator str_it;
  res.reserve(str.size()+30);
  char buff [4];
  for (str_it=str.begin(); str_it != str.end(); str_it++) {
    if ((!noChange || !strchr(noChange , *str_it))&&(*str_it<'0'||*str_it>'9')&&(*str_it<'a'||*str_it>'z')&&(*str_it<'A'||*str_it>'Z')) {
      res.append(1 , special);
      itoa((unsigned char)*str_it , buff , 16);
      if (!buff[1]) {buff[2]=0;buff[1]=buff[0];buff[0]='0';}
      res.append(buff);
    } else {res.append(1, *str_it);}
  }
  return res;
}
            
string urlDecode(string str , char special) {
  for (unsigned int i = 0; i<str.size(); i++) {
    if (str[i]==special) {
      str[i]=chtoint(str.substr(i+1,2).c_str() , 16);
      str.erase(i+1 , 2);
    }
  }
  return str;
}

// -----------------------------------------------------
string latin2(string str) {
  string::iterator str_it;
  char * from = "¹œŸ¥Œ";
  char * to = "±¶¼¡¦¬";
  int sz=strlen(from);
  int i;
  for (str_it=str.begin(); str_it != str.end(); str_it++) {
     for (i=0;i<sz;i++) if (*str_it==from[i]) {*str_it=to[i];break;}
  }
  return str;
}
// -----------------------------------------------------

char * resStr(UINT id , HINSTANCE inst) {
  LoadString(inst?inst:hInst , id , TLS().buff2 , MAX_STRING2);
  return TLS().buff2;
}

string resString(UINT id , HINSTANCE inst) {
  return resStr(id , inst);
}

char * resStr(HINSTANCE inst , UINT id) {
  return resStr(id , inst);
}

char * resStri(UINT id , int inst) {
  return resStr(id , (HINSTANCE) inst );

}
//------------------------------------------------------
bool ProcessMessages(HWND hwnd) {
//  if (!hwnd) Beep(0x80 , 1);
  MSG msg;
  while (PeekMessage(&msg , hwnd ,0,0,PM_REMOVE)) {
     TranslateMessage(&msg);
     DispatchMessage(&msg);
  }
  return true;
}

void ShowLastError() {
	int err = GetLastError();
	if (err) 
		MessageBox(NULL , _sprintf("%s\r\ncode: %d",GetLastErrorMsg(err),err),"ERROR" , 0);
}

const char * GetLastErrorMsg(int err) {
  FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    err?err:GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    TLS().buff2,
    MAX_STRING2,
    NULL
  );
  return TLS().buff2;
}

int ErrorMsg(int err) {
    MessageBox(0,GetLastErrorMsg(err) , "" , MB_ICONWARNING);
    return 0;
}

void DrawBitmap (HDC hDC , HBITMAP hBmp , int x , int y , bool trans) {
   BITMAP bmp;
   GetObject(hBmp , sizeof(BITMAP),&bmp);
   HDC hdcmem = CreateCompatibleDC(hDC);
   /*HBITMAP oldBmp = */
   SelectObject(hdcmem , hBmp);
   if (!trans)
      BitBlt(hDC, x,
        y,
        bmp.bmWidth, bmp.bmHeight,
        hdcmem, 0, 0,
        SRCCOPY);
   else
        TransparentBlt(hDC, x,
        y,
        bmp.bmWidth, bmp.bmHeight,
        hdcmem, 0, 0, bmp.bmWidth, bmp.bmHeight,
        GetPixel(hdcmem,0,0));
   DeleteDC (hdcmem);

}

void BitmapPremultiply(void * pixels, unsigned int pixelsCount)
{
	BYTE * ptrByte = (BYTE *) pixels;
	unsigned long i;
	for ( i = 0; i < pixelsCount; i++ )
	{
		ptrByte[0] = ( ptrByte[0] * ptrByte[3] ) / 0xFF;
		ptrByte[1] = ( ptrByte[1] * ptrByte[3] ) / 0xFF;
		ptrByte[2] = ( ptrByte[2] * ptrByte[3] ) / 0xFF;
		ptrByte += 4;
	}
}


void DrawBitmapBlended (HDC hDC , HBITMAP hBmp , int x , int y, int alpha) {
   BITMAP bmp;
   GetObject(hBmp , sizeof(BITMAP),&bmp);
   bool hasAlpha = (bmp.bmBitsPixel == 32);

   HDC hdcmem = CreateCompatibleDC(hDC);
   /*HBITMAP oldBmp = */
   SelectObject(hdcmem , hBmp);
   BLENDFUNCTION bf;
   bf.BlendOp = AC_SRC_OVER;
   bf.BlendFlags = 0;
   bf.SourceConstantAlpha = alpha;
   bf.AlphaFormat = hasAlpha ? AC_SRC_ALPHA : 0;
	void * buff;
	int buffSize;
   if (hasAlpha) { // premultiply...
	   buffSize = bmp.bmWidth * bmp.bmHeight * 4;
	   buff = malloc(buffSize);
	   memcpy(buff, bmp.bmBits, buffSize);
	   BitmapPremultiply(bmp.bmBits, buffSize / 4);
   }
   AlphaBlend(hDC, x,
        y,
        bmp.bmWidth, bmp.bmHeight,
        hdcmem, 0, 0,bmp.bmWidth, bmp.bmHeight,
        bf);
   if (hasAlpha) {
	   memcpy(bmp.bmBits, buff, buffSize);
	   free(buff);
   }
   DeleteDC (hdcmem);
}

COLORREF getUniqueColor(int i, int stepping, int range, bool bright, bool reversed) {
	int loss = range / stepping;
	int r = (i % stepping) * loss;
	i = i / stepping;
	int g = (i % stepping) * loss;
	i = i / stepping;
	int b = (i % stepping) * loss;
	if (bright) {
		r = 0xff - r;
		g = 0xff - g;
		b = 0xff - b;
	}
	return reversed ? RGB(b,g,r) : RGB(r,g,b);
}


void BlendBitmapWithColor (HBITMAP hBmp, COLORREF color, int alpha) {
	BITMAP bmp;
	GetObject(hBmp , sizeof(BITMAP),&bmp);
	if (bmp.bmBitsPixel != 32) return; // nie ma sensu...
	HDC hdcmem = CreateCompatibleDC(0);
	/*HBITMAP oldBmp = */
	SelectObject(hdcmem , hBmp);
	BYTE * bytes = (BYTE *) bmp.bmBits;
	double _alpha = double(alpha) / 0xFF;
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	for (unsigned long i = 0; i < bmp.bmWidth * bmp.bmHeight; i++ ) {
		double useAlpha = _alpha * (double(bytes[3]) / 0xFF);
		bytes[0] = useAlpha * bytes[0] + (1-useAlpha) * b;
		bytes[1] = useAlpha * bytes[1] + (1-useAlpha) * g;
		bytes[2] = useAlpha * bytes[2] + (1-useAlpha) * r;
		bytes[3] = 0xFF;
		bytes += 4;
	}
	DeleteDC (hdcmem);
}




int GetImageType(HANDLE image) {
	int type = GetObjectType(image);
	switch (type) {
		case OBJ_BITMAP: return IMAGE_BITMAP;
		case 0: return IMAGE_ICON;
	}
	return -1;
}

SIZE GetImageSize(HANDLE image, int type) {
	SIZE sz = {0,0};
	if (!image) return sz;
	if (type == -1) type = GetImageType(image);
	if (type == IMAGE_ICON) {
        ICONINFO ii;
        if (!GetIconInfo((HICON)image , &ii)) return sz;
        BITMAP b;
        GetObject(ii.hbmColor , sizeof(BITMAP),&b);
		sz.cx=b.bmWidth;
        sz.cy=b.bmHeight;
		DeleteObject(ii.hbmColor);
		DeleteObject(ii.hbmMask);
	} else if (type == IMAGE_BITMAP) {
          BITMAP cbmp;
          GetObject((HBITMAP)image , sizeof(BITMAP),&cbmp);
          sz.cx=cbmp.bmWidth;
          sz.cy=cbmp.bmHeight;
    }
	return sz;
}


// ----------------------------------------- FONT

HFONT GetLogFont(char * face , int height , char style)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    if (!(*face))
      GetObject(GetStockObject(ANSI_VAR_FONT), sizeof(LOGFONT),&lf);
      else
        lstrcpy(lf.lfFaceName, face);
    lf.lfHeight = height;
    if (style & 1)
      lf.lfWeight = FW_BOLD;
    lf.lfItalic = style & 2;
    lf.lfUnderline = style & 4;
    lf.lfCharSet = 0xEE;
    return CreateFontIndirect(&lf);
}

POINT Point (int x , int y) {
  POINT a = {x , y};
  return a;
}

void GetChildRect (HWND hWnd , RECT * prc) {
  POINT pt;
//  HWND parent = GetParent(hWnd);
  GetWindowRect(hWnd , prc);
  pt.x = prc->left;
  pt.y = prc->top;
  int w = prc->right - prc->left , h = prc->bottom - prc->top;
  ScreenToClient(GetParent(hWnd) , &pt);
  prc->left = pt.x;
  prc->top = pt.y;
  prc->right = pt.x + w;
  prc->bottom = pt.y + h;
  /*  GetWindowRect(GetParent(hWnd) , &rc);
  prc->left-=rc.left;
  prc->top-=rc.top;
  prc->right-=rc.left;
  prc->bottom-=rc.top;
*/
}

bool InsideRect (RECT rc1 , RECT rc2) {
  return    rc2.top    >= rc1.top
         && rc2.left   >= rc1.left
         && rc2.right  <= rc1.right
         && rc2.bottom <= rc1.bottom
         ;
}
void fitInRect(RECT & rc, RECT fitIn) {
	int x = 0;
	int y = 0;
	if (rc.left < fitIn.left) x = fitIn.top - rc.left;
	if (rc.top < fitIn.top) y = fitIn.top - rc.top;
	if (rc.right > fitIn.right) x += fitIn.right - rc.right;
	if (rc.bottom > fitIn.bottom) y += fitIn.bottom - rc.bottom;
	OffsetRect(&rc, x, y);
}
void setRectSizeByEdge(int edge , RECT * rc , int w, int h){
	if (edge == 0) edge = WMSZ_BOTTOMRIGHT;
    if (edge>=WMSZ_BOTTOM) 
		rc->bottom=rc->top+h;
    else 
		rc->top=rc->bottom-h;
    if (edge==WMSZ_RIGHT || edge==WMSZ_TOPRIGHT || edge==WMSZ_BOTTOMRIGHT)
	    rc->right=rc->left+w;
    else 
		rc->left=rc->right-w;
}
void clipSize(int edge , RECT * rc , int minW , int minH, int maxW, int maxH){
	int w = rc->right-rc->left;
	int h = rc->bottom-rc->top;
	w = max(w, minW);
	h = max(h, minH);
	w = min(w, maxW);
	h = min(h, maxH);
	setRectSizeByEdge(edge, rc, w, h);
}                       

RECT Rect(int l , int t , int r , int b , bool ref) {
  RECT rc;
  rc.left=l;
  rc.top=t;
  rc.right=ref?l+r:r;
  rc.bottom=ref?t+b:b;
  return rc;
}
SIZE Size(int w , int h) {
	SIZE sz = {w, h};
	return sz;
}
void expandRect(RECT & rc, int x, int y) {
	rc.left -=x;
	rc.top -= y;
	rc.right += x;
	rc.bottom +=y;
}

int SetWindowRect(HWND hwnd , RECT rc) {
  MoveWindow(hwnd , rc.left , rc.top , rc.right-rc.left , rc.bottom - rc.top , true);
  return 0;
}
int GetFontHeight(HDC hdc) {
	return GetTextSize(hdc, "0Ajg^_|").cy;
}

int GetFontHeight(HWND hwnd, HFONT font) {
	return GetTextSize(hwnd, font, "0Ajg^_|").cy;
}
SIZE GetTextSize(HDC hdc, const char * text, int width) {
	if (!text || !*text) return Size(0,0);
    RECT rt = Rect(0,0,width,0);
	DrawText(hdc, text, -1, &rt, DT_LEFT | DT_NOPREFIX | DT_CALCRECT | (!width ? 0 : DT_WORDBREAK));
	return Size(rt.right, rt.bottom);

}
SIZE GetTextSize(HWND hwnd, HFONT font, const char * text, int width) {
	HDC hdc = GetDC(hwnd);
	if (font) 
		font = (HFONT)SelectObject(hdc , font);
	SIZE sz = GetTextSize(hdc, text, width);
	if (font) 
		SelectObject(hdc , font);
	ReleaseDC(hwnd, hdc);
	return sz;
}


#define PACKVERSION(major,minor) MAKELONG(minor,major)

DWORD GetDllVersion(LPCTSTR lpszDllName)
{

    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    hinstDll = LoadLibrary(lpszDllName);

    if(hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;

        pDllGetVersion = (DLLGETVERSIONPROC) GetProcAddress(hinstDll, "DllGetVersion");

/*Because some DLLs might not implement this function, you
  must test for it explicitly. Depending on the particular
  DLL, the lack of a DllGetVersion function can be a useful
  indicator of the version.
*/
        if(pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr))
            {
                dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }

        FreeLibrary(hinstDll);
    }
    return dwVersion;
}


bool isComctl(unsigned int maj , unsigned int min) {
 static unsigned int ver = 0;
 if (!ver) ver = GetDllVersion(TEXT("comctl32.dll"));
 return ver >= (unsigned int)PACKVERSION(maj,min);
}



// -------------------------
#pragma pack(push, 1)
	struct NEWHEADER {
	WORD Reserved;
	WORD ResType;
	WORD ResCount;
	};
	#define SIZEOF_NEWHEADER 6
	struct ICONRESDIR {
	BYTE Width;
	BYTE Height;
	BYTE ColorCount;
	BYTE reserved;
	};
	struct CURSORDIR {
	WORD Width;
	WORD Height;
	};
	typedef struct tagRESDIR {
		union {
			ICONRESDIR   Icon;
			CURSORDIR    Cursor;
		} ResInfo;
		WORD    Planes;
		WORD    BitCount;
		DWORD   BytesInRes;
		DWORD  IconCursorId; // Jedyna ró¿nica pomiêdzy czytaniem z DLLa a z ICO
	} RESDIR;
	#define SIZEOF_RESDIR 14
#pragma pack(pop)
HICON LoadIconEx(HINSTANCE hInst , LPCTSTR id , int size , int bits) {

   HDC hDc = GetDC(GetDesktopWindow());
   int screenBits = GetDeviceCaps(hDc , BITSPIXEL);
   if (!bits) bits = screenBits;
   if (bits>=32 && !isComctl(6,0)) bits = 24;
   HICON icon = 0;
   HRSRC hr;
   HGLOBAL hg;
   void * p = 0;
   if (!hInst) {
	   FILE * file = fopen(id , "rb");
	   if (!file) goto end;
	   fseek(file , 0 , SEEK_END);
	   int size = ftell(file);
	   rewind(file);
	   if (size < SIZEOF_NEWHEADER) {fclose(file); return 0;}
	   p = malloc(size);
	   fread(p , 1 , size , file);
	   fclose(file);
   } else {
		hr = FindResource(hInst , id , RT_GROUP_ICON);
		if (!hr) goto end;
		hg = LoadResource(hInst , hr);
		if (!hg) goto end;
		p = LockResource(hg);
		if (!p) goto end;
   }
   NEWHEADER * nh = (NEWHEADER *)p;
   RESDIR * rd;
   rd = (RESDIR *)((long)p+SIZEOF_NEWHEADER);
   int found = 0;
   int rsize = 0;
   int sizev = size; // rozmiar pionowy
   for (int i = 0; i < nh->ResCount; i++ , rd++) {
		if (rd->ResInfo.Icon.Width == size || !found) {
			if (!size) size = rd->ResInfo.Icon.Width;
			if (rd->BitCount <= bits || !found) {
				found = hInst?rd->IconCursorId & 0xFFFF : rd->IconCursorId;
				rsize = rd->BytesInRes;

				sizev = rd->ResInfo.Icon.Width == rd->ResInfo.Icon.Height 
					?size : ceilf(float(size / rd->ResInfo.Icon.Width) * rd->ResInfo.Icon.Height);
			}
			if (rd->ResInfo.Icon.Width == size && rd->BitCount == bits) break;
		}
		if (hInst) rd = (RESDIR*)((char*)rd - 2); // Korygujemy pozycjê...
   }
   if (!found) goto end;
   void * p2 = 0;
   if (!hInst) {
	   p2 = (char*)p+found;
   } else {
		hr = FindResource(hInst , MAKEINTRESOURCE(found) , RT_ICON);
		if (!hr) goto end;
		hg = LoadResource(hInst , hr);
		//    = SizeofResource(hInst , hr);
		if (!hg || !rsize) goto end;
		p2 = LockResource(hg);
		if (!p2) goto end;
   }
   icon = CreateIconFromResourceEx((PBYTE)p2 , rsize , true , 0x00030000 , size , sizev , 0);
end:
   if (!hInst && p) 
	   free(p);
   ReleaseDC(GetDesktopWindow() , hDc);
   return icon;
}

// ------------------

void * memrev(void * buff , int count) {
  char t;
  char * buf = (char*)buff;
  for (int i = 0 ; i < count / 2 ; i++) {
    t = buf[i];
    buf[i] = buf[count - i - 1];
    buf[count - i - 1] = t;
  }
  return buff;
}

const char * longToIp(long adr) {
  memrev(&adr , sizeof(adr));
  return _sprintf("%u.%u.%u.%u" , (adr&0xFF000000)>>24 , (adr&0xFF0000)>>16 , (adr&0xFF00)>>8 , adr&0xFF);
}

int ipToLong(const char * ip) {
  int a , b , c , d;
  sscanf(ip , "%u.%u.%u.%u" , &a , &b , &c , &d);
  return ((BYTE)d << 24) | ((BYTE)c<<16) | ((BYTE)b<<8) | (BYTE)a;
}
SIZE ToolBar_GetSize(HWND hwnd) {
   SIZE sz = {0,0};
   int c = SendMessage(hwnd , TB_BUTTONCOUNT , 0 , 0);
   RECT rc;
   RECT tbrc={0,0,0,0};
   for (int i=0; i < c; i++) {
      if (SendMessage(hwnd , TB_GETITEMRECT , i , (LPARAM)&rc)) {
         UnionRect(&tbrc , &rc , &tbrc);
      }
   }
   sz.cx=tbrc.right - tbrc.left;
   sz.cy=tbrc.bottom - tbrc.top;
   return sz;
}

int GetLastInputTime() {
   static FARPROC proc = GetProcAddress(GetModuleHandle("user32.dll") , "GetLastInputInfo");
   if (proc) {
     LASTINPUTINFO lii;
     lii.cbSize = sizeof(lii);
     lii.dwTime = 0;
     ((bool (__stdcall*)(LASTINPUTINFO*))proc)(&lii);
     return lii.dwTime;
   } else {return 0;}  
}

bool _SetDllDirectory(const char * dir) {
#ifdef UNICODE
   static FARPROC proc = GetProcAddress(GetModuleHandle("kernel32.dll") ,  "SetDllDirectoryW");
#else
   static FARPROC proc = GetProcAddress(GetModuleHandle("kernel32.dll") ,  "SetDllDirectoryA");
#endif
   if (proc) {		 
	   return ((bool (__stdcall*)(IN LPCSTR lpPathName))proc)(dir);
   } else {return false;}  
}



bool SetAlpha(HWND hwnd, unsigned char Alpha)
{
    static FARPROC proc = GetProcAddress(GetModuleHandle("user32.dll") , "SetLayeredWindowAttributes");
    if (!hwnd) return proc!=0;
    if(Alpha && Alpha<255)
	{
        if (Alpha == 1) Alpha = 0;
        if (Alpha == 254) Alpha = 255;
		SetWindowLong( hwnd, GWL_EXSTYLE, GetWindowLong( hwnd, GWL_EXSTYLE ) | WS_EX_LAYERED );

        if (proc) {
            ((BOOL (__stdcall*)(HWND,COLORREF,BYTE,DWORD))proc)( hwnd, 0, Alpha, LWA_ALPHA );
        }
	}
	else
	{
		SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
    return true;
}

void RepaintWindow(HWND hwnd) {
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}


bool IsWindowUncovered(HWND hwnd) {
    POINT pt;
    RECT rc;
    GetWindowRect(hwnd , &rc);
    pt.x = (rc.left + rc.right) / 2;
    pt.y = (rc.top + rc.bottom) / 2;
    return ChildWindowFromPointEx(GetDesktopWindow() , pt , CWP_SKIPINVISIBLE | CWP_SKIPDISABLED)==hwnd;
}


bool WindowBelongsTo(HWND window, HWND parent) {
	while (window) {
		if (window == parent) return true;
		window = GetParent(window);
	}
	return false;
}


POINT getScreenPoint() {
	POINT pt;
	GetCursorPos(&pt);
	return pt;
}
POINT getLocalPoint(HWND window, POINT screen, int scrollX, int scrollY) {
	POINT local = screen;
	ScreenToClient(window, &local);
	if (scrollX == -1 || scrollY == -1) {
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		if (scrollX == -1)
			scrollX = GetScrollInfo(window , SB_HORZ , &si) ? si.nPos : 0;
		if (scrollY == -1)
			scrollY = GetScrollInfo(window , SB_VERT , &si) ? si.nPos : 0;
	}

	local.x += scrollX;
	local.y += scrollY;
	return local;
}




void SetKeyboardLed(BYTE key , BOOL bState ) { 

   BYTE keyState[256];

   GetKeyboardState((LPBYTE)&keyState);
   if( (bState && !(keyState[key] & 1)) ||
       (!bState && (keyState[key] & 1)) )
   {
   // Simulate a key press
      keybd_event( key,
                   0x45,
                   KEYEVENTF_EXTENDEDKEY | 0,
                   0 );

   // Simulate a key release
      keybd_event( key,
                   0x45,
                   KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                   0);
   }
} 


/*
 * cp_to_iso()
 *
 * zamienia krzaczki pisane w cp1250 na iso-8859-2, przy okazji maskuj-c
 * znaki, kt¡rych nie da sià wyÂwietliŠ, za wyj-tkiem \r i \n.
 *
 *  - buf.
 */
void cp2iso(char *buf)
{
	if (!buf)
		return;

	while (*buf) {
		if (*buf == '¹') *buf = '±';
		if (*buf == '¥') *buf = '¡';
		if (*buf == 'œ') *buf = '¶';
		if (*buf == 'Œ') *buf = '¦';
		if (*buf == 'Ÿ') *buf = '¼';
		if (*buf == '') *buf = '¬';

//                if (*buf != 13 && *buf != 10 && (*buf < 32 || (*buf > 127 && *buf < 160)))
//                        *buf = '?';

		buf++;
	}
}

/*
 * iso_to_cp()
 *
 * zamienia sensowny format kodowania polskich znaczk¡w na bezsensowny.
 *
 *  - buf.
 */
void iso2cp(char *buf)
{
	if (!buf)
		return;

	while (*buf) {
		if (*buf == '±') *buf = '¹';
		if (*buf == '¡') *buf = '¥';
		if (*buf == '¶') *buf = 'œ';
		if (*buf == '¦') *buf = 'Œ';
		if (*buf == '¼') *buf = 'Ÿ';
		if (*buf == '¬') *buf = '';
		buf++;
	}
}

/*
 * hide_pl()
 *
 * maskuje polsk- literkà w iso.
 *
 *  - c.
 */
void hidePL(char *c)
{
	if (!c)
		return;

	while (*c) {
	    if (*c == '¹') *c='a';
	    if (*c == 'ê') *c='e';
	    if (*c == 'æ') *c='c';
	    if (*c == '³') *c='l';
	    if (*c == 'ñ') *c='n';
	    if (*c == 'ó') *c='o';
	    if (*c == 'œ') *c='s';
	    if (*c == '¿') *c='z';
	    if (*c == 'Ÿ') *c= 'z';

	    if (*c == '¥') *c= 'A';
	    if (*c == 'Ê') *c= 'E';
	    if (*c == 'Æ') *c='C';
	    if (*c == '£') *c= 'L';
	    if (*c == 'Ñ') *c= 'N';
	    if (*c == 'Ó') *c= 'O';
	    if (*c == 'Œ') *c= 'S';
	    if (*c == '') *c= 'Z';
	    if (*c == '¯') *c= 'Z';

	    if (*c == '±') *c = 'a';
	    if (*c == '¡') *c = 'A';
	    if (*c== '¶') *c = 's';
	    if (*c == '¦') *c = 'S';
	    if (*c == '¼') *c = 'z';
	    if (*c == '¬') *c = 'Z';
        c++;    
    }

}
 
char * str_tr(char * str , const char * chIn , const char * chOut) {
	if (!str || !chIn || !chOut || !*chIn || !*chOut || strlen(chIn)!=strlen(chOut)) return str;
	char * c = str;
	while (*c) {
		const char * pos = strchr(chIn , *c);
		if (pos) {
			*c = chOut[pos - chIn];
		}
		c++;
	}
	return str;
}


const char * searchArray(const char * find , const char ** ar  , size_t count , bool getValue) {
	size_t findLen = strlen(find);
	for (size_t i=0; i<count; i++)
		
		if (strlen(ar[i]) >= findLen && !_strnicmp(find , ar[i] , findLen)) {
			if (ar[i][findLen] != 0 && ar[i][findLen] != '=')
				continue;
			if (getValue) {
				char const * value = strchr(ar[i] , '=');
				if (!value) 
					return 0;
				return value + 1;
			} else return ar[i];
		}
    return 0;
}
const char * getArgV(const char * const * argList , int argCount , const char * find , bool getValue , const char * def) {
	const char * r = searchArray(find , (const char**)argList , argCount , getValue);
	return r ? r : def;
}
const char * getArgV(const char * find , bool getValue , const char * def) {
	return getArgV((const char**)__argv+1 , __argc-1 , find , getValue , def);
}

void Split(const std::string & txt, const std::string & splitter, tStringVector & list, bool all) {
	std::string::size_type start = 0, end;
	list.clear();
	while (start < txt.length()) { // dopóki jest co kopiowaæ
		end = txt.find(splitter, start);
		if (all || start != end)
			list.push_back(txt.substr(start, (end == txt.npos? end : end - start)) );
		if (end == txt.npos)
			break;
		start = end + splitter.length();
	}
}
void SplitCommand(const string & txt , char splitter ,  tStringVector & list) {
    if (txt.empty()) return;
    char end;
	size_t pos = 0;
	do {
	    // Ustalamy jakim znakiem ma sià zakonczyc parametr
	    if (splitter && txt[pos]=='"') {pos++; end = '"';}
        else end = splitter;
	    size_t fnd = txt.find(end , pos);
	    list.push_back(txt.substr(pos , (fnd != txt.npos)?fnd-pos:fnd));
	    pos = fnd;
	    if (pos != txt.npos) {
		    pos += (end=='"')?2:1;
            if (pos>=txt.size()) break;
	    } else break;
	} while (1);
}

std::string ExpandEnvironmentStrings(const char * src, unsigned int size) {
	CStdString buff;
	ExpandEnvironmentStrings(src, buff.GetBuffer(500), size);
	buff.ReleaseBuffer();
	return buff;
}
