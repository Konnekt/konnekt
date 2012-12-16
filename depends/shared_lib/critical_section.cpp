#include <windows.h>
#include "critical_section.h"

cCriticalSection_w32::cCriticalSection_w32(){
	InitializeCriticalSection(&cs);
}
cCriticalSection_w32::~cCriticalSection_w32(){
	DeleteCriticalSection(&cs);
}


cCriticalSection_::cCriticalSection_() {
	this->currentThread = 0;
	this->count = 0;
	this->event = CreateEvent(0, FALSE, TRUE, 0);
}
cCriticalSection_::~cCriticalSection_() {
	CloseHandle(event);
}



void cCriticalSection_::lock(){
    DWORD current = GetCurrentThreadId();
    // czekamy a¿ lock siê zwolni...
    while (this->count && this->currentThread!=current) {
		if (WaitForSingleObject(event, INFINITE) == WAIT_OBJECT_0) // czekamy a¿ siê zwolni...
			break;
    }
    // ustawiamy siebie
    this->currentThread = current;
    return;
}

void cCriticalSection_::lock(DWORD timeout , char action){
    DWORD current = GetCurrentThreadId();
    // czekamy a¿ lock siê zwolni...
    int test=0;
    while (this->count && this->currentThread && this->currentThread!=current) {
		if (WaitForSingleObject(event, 100) == WAIT_OBJECT_0)
			break;
        test+=100;
        if (test == timeout) {
            switch (action) {
                case 0: return;
                case CRITICAL_SECTION_TA_EXCEPT: RaiseException(CRITICAL_SECTION_TIMEOUT_EXCEPTION , 0 , 0 , 0); 
                case CRITICAL_SECTION_TA_EXCEPT2: RaiseException(STATUS_TIMEOUT , 0 , 0 , 0); 
                case CRITICAL_SECTION_TA_THROW: throw 0;
            }
        }
    }
    // ustawiamy siebie
    this->currentThread = current;
    return;
}


void cCriticalSection_::lock(DWORD timeout , char * where){
    DWORD current = GetCurrentThreadId();
    // czekamy a¿ lock siê zwolni...
    int test=0;
    while (this->count && this->currentThread && this->currentThread!=current) {
		if (WaitForSingleObject(event, 100) == WAIT_OBJECT_0)
			break;
        test+=100;
        if (test == timeout) {
//            static ULONG_PTR tab[1];
//            tab[0] = (ULONG_PTR) where;
            RaiseException(CRITICAL_SECTION_TIMEOUT_EXCEPTION , 0 , where?1:0 , (ULONG_PTR *)&where); 
        }
    }
    // ustawiamy siebie
    this->currentThread = current;
    return;
}



void cCriticalSection_::unlock(){
    DWORD current = GetCurrentThreadId();
    // Locka mo¿e zdejmowaæ tylko aktualny w³aœciciel
    if (this->count == 0 || this->currentThread!=current) return;
	if ( (--this->count) == 0 ) {
        this->currentThread=0;
		SetEvent(this->event); // zwolniony!
    }
    return;
}

bool cCriticalSection_::canAccess(){
    return this->count==0 || this->currentThread==0 || this->currentThread == GetCurrentThreadId();
}
void cCriticalSection_::access(){
    while (!canAccess()) {
        SleepEx(1,false);
    }
}



// ------------------------------------------------------------------------

void cCriticalSection_WM::lock(void) {
	DWORD current = GetCurrentThreadId();
    // czekamy a¿ lock siê zwolni...
    while (this->count && this->currentThread!=current) {
		if (_WMProcess) {
			if (MsgWaitForMultipleObjectsEx(1 , &event , INFINITE , QS_ALLINPUT | QS_ALLPOSTMESSAGE , MWMO_ALERTABLE | MWMO_INPUTAVAILABLE) != WAIT_OBJECT_0)
				_WMProcess();
		} else {
			if (WaitForSingleObjectEx(event, INFINITE, TRUE) == WAIT_OBJECT_0)
				break;
		}
    }
    // ustawiamy siebie
    this->currentThread = current;
    return;
}
void cCriticalSection_WM_cond::lock() {
	DWORD current = GetCurrentThreadId();
	if (current == forThread)
		cCriticalSection_WM::lock();
	else
		cCriticalSection_::lock();
}