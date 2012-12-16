//---------------------------------------------------------------------------
//#pragma link "MEMMGR.LIB"
#include "windows.h"
#include "preg.h"
#include "pcre/pcre.h"


/*
 *  PREG (Perl-style Regular Expressions C++ wrapper for pcre)
 *  (c)Copyright 2002-2003  Rafa³ Lindemann | Stamina
 *  http://www.stamina.eu.org/
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */  


//#pragma link "pcre"


void cPreg::init() {
   _blank = "";
   useCache=true;
   useDelimiters=true;
   keepSubject=true;
   matched = 0;
   start = 0;
   _current.pCompiled = 0;
   _current.pExtra = 0;
   _current_pat = "";
   _tableptr=0;
   _pattern = _pattern_orig = _subject = "";
   _localeOneTime = false;
}

cPreg::cPreg() {
  init();
}

cPreg::cPreg(bool cache) {
  init();
  useCache = cache;
}

void cPreg::deinit() {
    if (!useCache) {
		if (_current.pCompiled) /*pcre_*/free(_current.pCompiled);
		if (_current.pExtra) /*pcre_*/free(_current.pExtra);
		_current.pCompiled = 0;
		_current.pExtra = 0;
    }
}

cPreg::~cPreg() {
  for (_cacheit = _cache.begin() ; _cacheit != _cache.end() ; _cacheit++) {
    if (_cacheit->second.pCompiled) /*pcre_*/free(_cacheit->second.pCompiled);
    if (_cacheit->second.pExtra) /*pcre_*/free(_cacheit->second.pExtra);
  }
  deinit();
  resetLocale();
}


// ------------------------   Internal
int cPreg::preparePattern() {
	if (_localeOneTime) {
		resetLocale();
	}
   if (useDelimiters) {
     flags = 0;
     char de = _pattern[0];
     int pos = _pattern.find_last_of(de);
     if (pos>=(int)_pattern.length()) return 0;
     for (int i = pos+1 ; i < (int)_pattern.length() ; i++) {
       switch (_pattern[i]) {
         case 'i': flags |= PCRE_CASELESS;break;
         case 'm': flags |= PCRE_MULTILINE;break;
         case 's': flags |= PCRE_DOTALL;break;
         case 'x': flags |= PCRE_EXTENDED;break;
         case 'A': flags |= PCRE_ANCHORED;break;
         case 'D': flags |= PCRE_DOLLAR_ENDONLY;break;
         case 'X': flags |= PCRE_EXTRA;break;
         case 'U': flags |= PCRE_UNGREEDY;break;
         case '8': flags |= PCRE_UTF8;break;

         case 'B': flags |= PCRE_NOTBOL;break;
         case 'N': flags |= PCRE_NOTEOL;break;
         case 'E': flags |= PCRE_NOTEMPTY;break;
		 case 'L': 
		 {
			 setLocale(_pattern.substr(i+1).c_str());
			 _localeOneTime = true;
			 i = _pattern.length(); // konczymy szukanie...
		 }
       }
     }
     _pattern.erase(pos , _pattern.length() - pos + 1);
     _pattern.erase(0,1);
   }
   return 1;
}

string cPreg::cacheKey() {
   return _pattern + '/' +(char)((flags & ~(PCRE_NOTBOL | PCRE_NOTEOL | PCRE_NOTEMPTY)) + 'A');
}

string   cPreg::operator [] (int i) {
	if (_result <= i || _vector[i*2]<0) return "";
	return _subject.substr(_vector[i*2] , _vector[i*2+1] - _vector[i*2]);
}


int cPreg::process(bool extract) {
   string key = cacheKey();
//   printf ("  Processing %s with %x [%s]\n" , _pattern.c_str() , flags , key.c_str());
   if (_current.pCompiled && _current_pat != key) _current.pCompiled = 0;
//   if (_current.pCompiled) printf("   Using quick cache\n");
   _current_pat = key;
   if (useCache && !_current.pCompiled) {
     _cacheit = _cache.find(key);
     if (_cacheit != _cache.end()) {
       _current = _cacheit->second;
//       printf("   Using cache!\n");
     }
//     else printf ("   Not in cache!\n");
   }
   lastError = 0;
   lastErrorPos = 0;
   _result = result = -10;
   int execFlags;
   if (!_current.pCompiled) {
     execFlags = flags & ~(PCRE_NOTBOL | PCRE_NOTEOL | PCRE_NOTEMPTY);
     _current.pCompiled = pcre_compile(_pattern.c_str() , execFlags , &lastError , &lastErrorPos , _tableptr);
     _current.pExtra = 0;
     if (lastError) return 0;
     if (useCache) {
       _cache[key]=_current;
//       printf("   Stored in cache\n");
     }
   }

   int cz = _subject.length();
   const char * yo = _subject.c_str();
   pcre_free(_current.pExtra);
   execFlags = flags & (PCRE_NOTBOL | PCRE_NOTEOL | PCRE_NOTEMPTY | PCRE_ANCHORED);
   _result = pcre_exec(_current.pCompiled , _current.pExtra , _subject.c_str() , _subject.length() , start , execFlags , _vector , PREG_VECTORSIZE);
   result  = _result;
   matched = (_result > 0);

   if (!keepSubject) setSubject("");
   return _result;
}
const char * cPreg::getByVector(int i) {
	if (_vector[i]<0) return 0;
	const char * c = _subject.c_str();
	return c + _vector[i];
}


  string cPreg::insertVars(string sub) {
     string re = "";
     re.reserve(sub.length());
     for (unsigned int i = 0 ; i < sub.length() ; i++) {
       if ((sub[i]=='$' || sub[i]=='\\') && i+1<sub.length() &&
           ((sub[i+1]>='0' && sub[i+1]<='9') || sub[i+1]=='&') ) {
         if (sub[i+1]=='&' && sub[i]=='$') sub[i+1]='0';
         re+=(*this)[sub[i+1] - '0'];
         i++;
       } else re+=sub[i];

     }
     return re;
  }


// ------------------------   API


int cPreg::match(const char * pat , const char * sub) {
   setPattern(pat);
   setSubject(sub);
   return match();
}

string cPreg::replace(const char * chg , int limit) {
   string re = "";
   int i = 0;
   keepSubject = true;
   if (start) re=_subject.substr(0 , start);
   while (limit==0 || i<limit) {
     if (process(true)<=0) break;

     re += _subject.substr(start,_vector[0] - start);

     re += insertVars(chg);
     start = _vector[1];
     flags|=PCRE_NOTBOL;
     i++;
   }
   re += _subject.substr(start , _subject.length() - start);
   start = 0;
   return re;
}



string cPreg::replace(fPregReplace callback , int limit , void * param) {
   string re = "";
   int i = 0;
   keepSubject = true;
   if (start) re=_subject.substr(0 , start);
   while (limit==0 || i<limit) {
     if (process(true)<=0) break;
     re += _subject.substr(start,_vector[0] - start);
     re += callback(this , param);
     start = _vector[1];
     flags|=PCRE_NOTBOL;
     i++;
   }
   re += _subject.substr(start , _subject.length() - start);
   return re;
}



int cPreg::match_global() {
     int re;
     re = process(true);
     if (re>0) start = _vector[1];
     return re;
}
void cPreg::setLocale(const char * locale) {
	if (!locale)
		return;
	resetLocale();
	string coll (setlocale(LC_COLLATE, 0));
	string ctyp (setlocale(LC_CTYPE, 0));
	string mone (setlocale(LC_MONETARY, 0));
	string nume (setlocale(LC_NUMERIC, 0));
	string time (setlocale(LC_TIME, 0));
	setlocale(LC_ALL, locale);
	this->_tableptr = pcre_maketables();
	setlocale(LC_COLLATE, coll.c_str());
	setlocale(LC_CTYPE, ctyp.c_str());
	setlocale(LC_MONETARY, mone.c_str());
	setlocale(LC_NUMERIC, nume.c_str());
	setlocale(LC_TIME, time.c_str());
}

void cPreg::resetLocale() {
	if (this->_tableptr) {
		pcre_free((void*)this->_tableptr);
	}
}
 