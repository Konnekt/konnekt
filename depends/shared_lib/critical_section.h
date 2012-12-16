#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

/*
  cCriticalSection to bardzo prosta, obiektowa, niekoniecznie szybsza
  wersja obslugi sekcji krytycznych. Dodatkowa opcja, jest mozliwosc
  timeout'owania oczekiwania sekcji wyjatkiem strukturalnym.
*/

#define CRITICAL_SECTION_TIMEOUT 5000
#define CRITICAL_SECTION_TIMEOUT_EXCEPTION STATUS_TIMEOUT | 0x1000


#define CRITICAL_SECTION_TA_BREAK (char)0
#define CRITICAL_SECTION_TA_EXCEPT (char)1
#define CRITICAL_SECTION_TA_EXCEPT2 (char)2 // STATUS_TIMEOUT
#define CRITICAL_SECTION_TA_THROW (char)3



 class cCriticalSection_base {
 public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual bool canAccess() = 0;
	virtual void access() {lock(); unlock();}
	virtual int getLockCount() = 0;
 };

 class cLocker {
 public:
	 __inline cLocker(cCriticalSection_base & section):cs(section) {
		 lock();
	 }
	 __inline ~cLocker(){
		 unlock();
	 }
	 __inline void lock() {
		 cs.lock();
	 }
	 __inline void unlock() {
		 cs.unlock();
	 }
 protected:
	 cCriticalSection_base & cs;
 };

 // =========================================================================

 // struktura do bezposredniej obslugi blokowania dostepu
 class cCriticalSection_w32:public cCriticalSection_base {
 public:
	 CRITICAL_SECTION cs;

     cCriticalSection_w32();
     ~cCriticalSection_w32();
	 __inline void lock() {	EnterCriticalSection(&cs);} 
//	 __inline bool tryLock() {return TryEnterCriticalSection(&cs);} 
	 __inline void unlock() {LeaveCriticalSection(&cs);} 
	 __inline bool canAccess() {
		 return cs.LockCount<=0 || cs.OwningThread == (HANDLE)GetCurrentThreadId();
	 }
	 int getLockCount() {return cs.LockCount;}

 };

 class cCriticalSection_blank:public cCriticalSection_base {
 public:
	 __inline void lock() {} 
	 __inline void unlock() {} 
	 int getLockCount() {return 0;}
	 __inline bool canAccess() {return true;}
 };

 // struktura do bezposredniej obslugi blokowania dostepu
 class cCriticalSection_:public cCriticalSection_base {
 public:
     cCriticalSection_();
     ~cCriticalSection_();
     void lock(); // Zwraca liczbê wczeœniej za³o¿onych blokad
     void lock(DWORD timeout , char action=0); // Z TimeOutem
     void lock(DWORD timeout , char * where); // TA_EXCEPT z parametrem
     void unlock(); // Zwraca liczbê pozosta³ych blokad
     bool canAccess(); // Sprawdza, czy nie ma blokady
     void access(); // czeka tylko a¿ bêdzie móg³ wejœæ, bez blokowania
	 int getLockCount() {return count;}
 protected:
     DWORD currentThread;    // ID aktualnie blokuj¹cego w¹tku
     unsigned short count; // iloœæ za³o¿onych bloków
	 HANDLE event;

 };
 
 
 // Domyœlnie robi timeout...
 class cCriticalSection_to: public cCriticalSection_ {
 public:
     char * place;
     cCriticalSection_to(char * place=""):place(place){}
     void lock() {return cCriticalSection_::lock(5000 , place);}
 };


 // Z zabezpieczeniem na windowsowe komunikaty...
typedef void (*tWMProcess)(void);
class cCriticalSection_WM: public cCriticalSection_ {
private:
public:
    cCriticalSection_WM(tWMProcess WMProcess = 0):_WMProcess(WMProcess) {}
    void lock(void);
protected:
    tWMProcess _WMProcess;

};
   
class cCriticalSection_WM_cond: public cCriticalSection_WM {
private:
public:
    cCriticalSection_WM_cond(int forThread, tWMProcess WMProcess = 0):cCriticalSection_WM(WMProcess), forThread(forThread) {}
    void lock(void);
protected:
	int forThread;
};


 typedef cCriticalSection_w32 cCriticalSection;

#endif