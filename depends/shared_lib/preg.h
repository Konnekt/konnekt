#ifndef FILEPREGH
#define FILEPREGH

/*
 *  PREG (Perl-style Regular Expressions C++ wrapper for pcre)
 *  (c)Copyright 2002-2003  Rafa³ Lindemann | Stamina
 *  
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

# ifdef BUILD_PREG_DLL
#     define EXPORT __export
# else
#     define EXPORT __import
# endif


#include <deque>
#include <string>
#include <map>
#include "pcre/pcre.h"


//#define PREG_DEBUG(s) printf(s);

#define PREG_VECTORSIZE 1000

using namespace std;

typedef std::string (__stdcall*fPregReplace)(class cPreg * , void * param);

class cPreg {
  struct cacheItem {
    pcre * pCompiled;
    pcre_extra * pExtra;
  };
  public:

  const char * lastError;
  int lastErrorPos;
  bool useCache;
  bool useDelimiters;
  bool keepSubject;
  bool matched;
  int  result;
  int  flags;
  int  start;


  cPreg ();
  cPreg (bool cache);
  ~cPreg();


  string   operator [] (int i);
  string   operator () (int i) {return (*this)[i];}
  string getSub(int i) {return (*this)[i];}

  const char * getByVector(int i);

  void setPattern (string v) {_pattern = v;_pattern_orig = v; deinit(); preparePattern();}
  void setSubject (string v) {_subject = v;start=0;}

  string getPattern() {return _pattern_orig;}
  string getSubject() {return _subject;}
  string getSubject(size_t start , size_t end) {return _subject.substr(start , end<start?end:end - start);}
  const string & getSubjectRef() {return _subject;}

  void reset() {start=0;}
  int getVector(unsigned int num) {if (result > 0 && num<(unsigned int)result*2) return _vector[num]; else return 0;}

  int match(const char * pat) {setPattern(pat); return match();}
  int match(const char * pat , const char * sub);
  int match() {process(true); return this->_result>0?this->_result:0;}
  int match_global();
  int match_global(const char * pat , const char * sub)
           {setPattern(pat);setSubject(sub);return match_global();}
  string replace(const char * chg , int limit=0);
  string replace(const char * pat , const char * chg , const char * sub , int limit=0)
            {start=0;setPattern(pat);setSubject(sub);return replace(chg , limit);}
  string replace(fPregReplace callback , int limit=0 , void * param=0);
  string replace(const char * pat , fPregReplace callback , const char * sub , int limit=0, void * param=0)
            {start=0;setPattern(pat);setSubject(sub);return replace(callback , limit,param);}
  void setLocale(const char * locale);
  void resetLocale();
  // ----------------------------------------------------
  private:
  map <string , cacheItem> _cache;
  map <string , cacheItem>::iterator _cacheit;

  cacheItem  _current;
  string     _current_pat ;
  const unsigned char * _tableptr;
  bool _localeOneTime;

  string _pattern  , _pattern_orig;
  string _subject;
  bool _patternReady;
  int _result;
  string _blank;
  int _vector [PREG_VECTORSIZE];

  void init();
  void deinit();

  int process(bool extract);
  int preparePattern();
  string cacheKey();
  string insertVars(string sub);

};




#endif