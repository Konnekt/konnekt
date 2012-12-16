#include <windows.h>
#include <string>
#include <time.h>
#include "time64.h"

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


cTime64::cTime64(struct tm * timer) {from_tm(timer);}
cTime64::cTime64(int timer) {from_time_t(timer);}
cTime64::cTime64(__int64 timer) {from_int64(timer);}
cTime64::cTime64(SYSTEMTIME &timer) {from_systemtime(&timer);}
cTime64::cTime64(class cDate64 &timer) {from_date64(timer);}
//cTime64::cTime64(__time64_t timer) {sec = timer;}
// Copy Contructors
cTime64 & cTime64::operator=( __int64 & timer) {from_int64(timer);return *this;}
cTime64 & cTime64::operator=( SYSTEMTIME & timer) {from_systemtime(&timer);return *this;}
cTime64 & cTime64::operator=(cDate64 & timer) {from_date64(timer); return *this;}
// Cast operators
cTime64::operator tm() {return to_tm();}
cTime64::operator SYSTEMTIME() {return to_systemtime();}

void cTime64::from_date64(cDate64 &timer) {
	sec = timer.empty()?0 : _mktime64(&timer.to_tm());
}


void cTime64::from_tm(struct tm * timer) {
  if (!timer) return;
  sec = _mktime64(timer);
}

tm cTime64::to_tm() {
  tm timer;
  memset(&timer , 0 , sizeof(tm));
  tm * tmp = _localtime64(&sec);
  if (tmp) {
	  timer = *tmp;
  }
  return timer;
}


void cTime64::from_time_t(int timer) {
	sec = (__time64_t)timer;
}

void cTime64::from_int64(__int64 timer) {
	if (!(timer >> 63)) {
		sec = (__time64_t)timer;
	} else {
		cDate64 cd(timer);
		from_date64(cd);
	}
}


cTime64::cTime64(bool current) {
  if (current)
    sec = _time64(0);
  else
    sec = 0;
}

void cTime64::now() {
    sec = _time64(0);
}

cTime64::operator __int64() {
  return (__int64)sec;
}

void cTime64::strftime(char *strDest,size_t maxsize,const char *format){
  tm t = to_tm();
  ::strftime(strDest , maxsize , format , &t);
}

std::string cTime64::strftime(const char *format) {
  char buff [501];
  strftime(buff , 500 , format);
  return buff;
}

void cTime64::clear() {
  sec = 0;
}

bool cTime64::empty() {
  return sec==0;
}

SYSTEMTIME cTime64::to_systemtime() {
	cDate64 cd(*this);
	return cd.to_systemtime();
}

void cTime64::from_systemtime(SYSTEMTIME * st) {
	cDate64 cd(*st);
	from_date64(cd);
}


// ----------------------------------------------------------

// ----------------------------------------------------------


cDate64::cDate64(struct tm * timer) {from_tm(timer);}
cDate64::cDate64(int timer) {from_time_t(timer);}
cDate64::cDate64(__int64 timer) {from_int64(timer);}
cDate64::cDate64(SYSTEMTIME &timer) {from_systemtime(&timer);}
cDate64::cDate64(cTime64 &timer) {from_time64_t(timer.sec);}
//cDate64::cDate64(__time64_t timer) {from_time64_t(timer);}

// Copy Contructors
cDate64 & cDate64::operator=( __int64 & timer) {from_int64(timer);return *this;}
cDate64 & cDate64::operator=( SYSTEMTIME & timer) {from_systemtime(&timer);return *this;}
// Cast operators
cDate64::operator tm() {return to_tm();}
cDate64::operator SYSTEMTIME() {return to_systemtime();}

void cDate64::from_tm(struct tm * timer) {
  if (!timer) return;
  msec = 0;
  sec = timer->tm_sec;
  min = timer->tm_min;
  hour = timer->tm_hour;
  day = timer->tm_mday;
  month = timer->tm_mon + 1;
  year = timer->tm_year + 1900;
  wday = timer->tm_wday;
  yday = timer->tm_yday + 1;
  isdst = timer->tm_isdst==1;
  desc=1;
}

tm cDate64::to_tm() {
  tm timer;
  timer.tm_sec = sec;
  timer.tm_min = min;
  timer.tm_hour = hour;
  timer.tm_mday = day;
  timer.tm_mon= month - 1;
  timer.tm_year = year - 1900;
  timer.tm_wday = wday;
  timer.tm_yday = yday - 1;
  timer.tm_isdst= isdst;
  return timer;
}


void cDate64::from_time_t(int timer) {
  time_t _timer = timer;
  from_tm(localtime(&_timer));
}
void cDate64::from_time64_t(__time64_t timer) {
	if (!timer) {
	  memcpy(this , &timer , 8);
	  desc = 1;
	} else
      from_tm(_localtime64(&timer));
}

void cDate64::from_int64(__int64 timer) {
  memcpy(this , &timer , 8);
  if (!desc) { // oops! to byl __time64_t
	from_time64_t((__time64_t)timer);	
  }
}


cDate64::cDate64(bool current) {
  if (current)
    from_time_t(time(0));
  else
    from_int64(0);
}

void cDate64::now() {
  from_time_t(time(0));
}

cDate64::operator __int64() {
  __int64 temp;
  memcpy(&temp , this , 8);
  return temp;
}

void cDate64::strftime(char *strDest,size_t maxsize,const char *format){
  tm t = to_tm();
  ::strftime(strDest , maxsize , format , &t);
}

std::string cDate64::strftime(const char *format) {
  char buff [501];
  strftime(buff , 500 , format);
  return buff;
}

void cDate64::clear() {
  from_int64(0);
}

bool cDate64::empty() {
  __int64 temp;
  memcpy(&temp , this , 8);
  return (temp & 0x7FFFFFFFFFFFFFFF)==0;
}

SYSTEMTIME cDate64::to_systemtime() {
    SYSTEMTIME st;
    st.wDay = this->day;
    st.wDayOfWeek = this->wday;
    st.wHour = this->hour;
    st.wMilliseconds = this->msec;
    st.wMinute = this->min;
    st.wMonth = this->month;
    st.wSecond = this->sec;
    st.wYear = this->year;
    return st;
}

void cDate64::from_systemtime(SYSTEMTIME * st) {
    this->desc = 1;
    this->day = st->wDay;
    this->hour = st->wHour;
    this->min = st->wMinute;
    this->month = st->wMonth;
    this->msec = st->wMilliseconds;
    this->sec = st->wSecond;
    this->wday = st->wDayOfWeek;
    this->year = st->wYear;
}


