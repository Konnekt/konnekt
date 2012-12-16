#ifndef DATEFUNC_H
#define DATEFUNC_H

/*
 *  cTime64 / cDate64
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


#include <time.h>

// Wrapper dla __time64_t

class cTime64 {

  public:                 
  __time64_t sec;

// Constructors
  cTime64(struct tm * timer);
  cTime64(int timer);
  cTime64(bool current=false);
  cTime64(__int64 timer);
  cTime64(SYSTEMTIME &timer);
  cTime64(class cDate64 &timer);
// Copy Contructors
  cTime64 & operator=(__int64 & timer);
  cTime64 & operator=(cDate64 & timer);
//  cTime64 & operator=(__time64_t timer);
#ifdef _WINDOWS_
  cTime64 & operator=( SYSTEMTIME & timer);
#endif
  // Cast operators
  operator __int64();
  operator tm();
#ifdef _WINDOWS_
  operator SYSTEMTIME();
#endif
// Functions
  void strftime(char *strDest,size_t maxsize,const char *format);
#ifdef _STRING_
  std::string strftime(const char *format);
#endif
  bool empty();
  void clear();
  void now();

  private:
  void from_time_t(int timer);
  void from_tm(struct tm * timer);
  void from_int64(__int64 timer);
  void from_date64(cDate64 &timer);
  tm to_tm();
#ifdef _WINDOWS_
  void from_systemtime(SYSTEMTIME * st);
  SYSTEMTIME to_systemtime();
#endif
  friend class cDate64;
};


// Klasa z wydzielonymi czesciami czasu...

class cDate64 {
  public:                 // Maska do porownan to #FFFFFFFFFFFF
  unsigned msec  : 10;   // 0..999                        0x3FF
  unsigned sec   : 6;    // 0..59                        0xFC00
  unsigned min   : 6;    // 0..59                      0x3F0000
  unsigned hour  : 5;    // 0..23                       7C00000
  unsigned day   : 5;    // 1..31                      F8000000
  unsigned month : 4;    // 1..12                     F00000000
  unsigned year  : 12;   // 0..4095                FFF000000000
  unsigned wday  : 3;    // 0..7 (1/7 - sunday    7000000000000
  unsigned yday  : 9;    // 1..366              FF8000000000000
  unsigned isdst : 1;    // 0.1
  unsigned : 2;
  unsigned desc  : 1;    // Zawsze = 1

//  int operator=(int n) {return 3;}
//  bool operator > (sTime64 & t) {return 1;}
  cDate64(struct tm * timer);
  cDate64(int timer);
  cDate64(bool current=false);
  cDate64(__int64 timer);
  cDate64(SYSTEMTIME &timer);
//  cDate64(__time64_t timer);
  cDate64(cTime64 & timer);
// Copy Contructors
  cDate64 & operator=( __int64 & timer);
#ifdef _WINDOWS_
  cDate64 & operator=( SYSTEMTIME & timer);
#endif
  // Cast operators
  operator __int64();
  operator tm();
#ifdef _WINDOWS_
  operator SYSTEMTIME();
#endif
// Functions
  void strftime(char *strDest,size_t maxsize,const char *format);
#ifdef _STRING_
  std::string strftime(const char *format);
#endif
  bool empty();
  void clear();
  void now();

  private:
  void from_time_t(int timer);
  void from_time64_t(__time64_t timer);
  void from_tm(struct tm * timer);
  void from_int64(__int64 timer);
  tm to_tm();
#ifdef _WINDOWS_
  void from_systemtime(SYSTEMTIME * st);
  SYSTEMTIME to_systemtime();
#endif
  friend class cTime64;
};

/*
  sTime64:
   0    4    8     12   16   20    24    28   32   36   40   44   48    52   56   60     
  |.... .... ..|.. ....|.... ..|.. ...|. ....|....|.... .... ....|...|. .... ....|.|xx|.|
     msec-10     sec-6   min-6  hour-5 day-5  m-4     year-12     wd3    yday9    d

*/


#endif
