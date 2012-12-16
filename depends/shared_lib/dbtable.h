#ifndef DTABLEH
#define DTABLEH

#define _DTABLE_


#include <vector>
#include <Stdstring.h>
#include "critical_section.h"

#define DT_READ 1
#define DT_WRITE 2
#define DT_APPEND 4

#define DT_C_ID 0x7FFFFF

#define DT_ROWID_MASK   0x40000000  // ta maska nalozona na numer wiersza oznacza ze numer jest identyfikatorem
#define DT_ROWID_MAX    0x3FFFFFFF  // maxymalna wartosc identyfikatora
#define DT_ROWID_MIN    0x1

#define DT_COLID_UNIQUE 0x00800000 // Identyfikator kolumny jest unikatowy

#define DT_CT_INT 0
#define DT_CT_PCHAR 1
#define DT_CT_STR DT_CT_PCHAR
#define DT_CT_STRING 2
#define DT_CT_64     3 // __int64 lub double , lub cokolwiek 64-bitowego
#define DT_CT_BIN    4 // binarka. 4 bajty rozmiaru + bufor
#define DT_CT_UNKNOWN -1  ///< dla sDTValue, typ nieznany (zostanie ustawiony).

#define DT_CT_TYPEMASK 0xF


#define DT_CF_NOSAVE   0x100       // ta wartosc nie zostanie zapisana
#define DT_CF_KEY      0x200       // ta kolumna jest "kluczem"
#define DT_CF_SECRET   0x400       // ta kolumna mo¿e byæ has³em i powinna byæ odpowiednio strze¿ona...

#define DT_CT_DONTSAVE DT_CF_NOSAVE
#define DT_CT_KEY      DT_CF_KEY


#define DT_CF_CRYPTMASK 0xFF000
#define DT_CT_CRYPTMASK DT_CF_CRYPTMASK
#define DT_CF_CXOR      0x01000
#define DT_CT_CXOR      DT_CF_CXOR

#define DT_CF_NOSAVEFLAGS 0x000000 // Flagi które nie zostan¹ zapisane do pliku
#define DT_CF_LOADED      0x100000 // Kolumna wczytana z pliku...

#define DT_CC_RESIZE 1
#define DT_CC_FILL 2

// RowFlags
#define DT_RF_DONTSAVE  0x1

#define DT_FT_TEXT 0
#define DT_FT_BINARY 1
//#define DT_FT_AUTO -1

#define DT_ERR_NOCOL 100
#define DT_ERR_NOROW 200
#define DT_ERR_BADTYPE 300
#define DT_NOROW 0xFFFFFFFF

#define DT_SETBLANKSBYTYPE

#define DT_CHECKTYPE

#define DT_LOCKALL -1
 // Klasy

 typedef void * TdEntry;
 typedef __int64 Td64;

#pragma pack(push, 1)
struct sDTValue {
    short type; ///< Typ przekazywanej wartoœci
    union {
        struct {
            union {
                char * vChar;
                const char * vCChar;
            };
            unsigned int buffSize;
        };
        int vInt;
        __int64 vInt64;
    };
    sDTValue(short type=DT_CT_UNKNOWN):type(type) {vInt64 = 0;buffSize=0;}
};
#pragma pack(pop)

 // strukturka do przekazywania DT_CT_BIN
 struct CdtctBin {
     int size;
     void * buff;
 };

 // class CdTable;
 class CdtRow {  // Wiersz w tabeli ... Zawiera tylko wartosci kolumn
  int size;  // ilosc zaalokowanych kolumn
  class CdTable * table; // Tablica do ktorej nalezy wiersz
  TdEntry * data;
  public:
  unsigned int pos;
  int index;
  unsigned int id;    // identyfikator wiersza
  unsigned int flag;
  cCriticalSection_blank CS; // blokada
//  CGarbage_Collector garbage;

  int allocData();
  int freeData();
  const TdEntry get (int id); // Pobiera wartosc kolumny
  int set (int id , TdEntry val); // ustawia wartosc kolumny

  const TdEntry getbyindex (int index); // Pobiera wartosc kolumny

  CdtRow (const CdtRow & v);

  CdtRow (CdTable * t, int index , char allocate=1);
  ~CdtRow ();

  unsigned short lock(int row);
  unsigned short unlock(int row);
//  unsigned short access(int row);

 };

 struct CdtColDescItem {
   int type;
   int id;
   TdEntry def; // default
   std::string name;  // tekstowy identyfikator
   CdtColDescItem();
 };

 class CdtColDesc {  // Opis typow kolumn
  public:
    typedef std::vector <CdtColDescItem>::iterator cols_it_t;
    std::vector <CdtColDescItem> cols;
    void * table;
    int deftype;
	bool loader;
    CdtColDesc ();
    CdtColDesc (int count , ...); // (...,id,type,def,...)
    int operator = (CdtColDesc & x);

    int setcolcount (int count, int=0); // ustawia ilosc kolumn
    int setcol (int id , int type , TdEntry def=0 , const char * name="");  // ustawia rodzaj danych w kolumnie
    int setuniquecol (const char * name , int type , TdEntry def=0);  // ustawia rodzaj danych w kolumnie o podanej nazwie
    int getcolcount ();
    int getcol (int index , int & type , int & id);
    int getcoltype (int index);
    int getcolflag (int index);
    int getcolid (int index);
	const char * getcolname (int index);
	int colindex (int id , bool quick = 1); // zwraca index kolumny
    void clear();
    void optimize();
	int join(CdtColDesc * other , bool overwrite); // ³¹czy dwa deskryptory, zwraca liczbê do³¹czonych

    int getnewuniqueid(void);
    int getnameid(const char * name);
 };




 typedef std::vector <CdtRow *>::iterator t_rows_it;

 class CdTable {
  public:
    std::vector <CdtRow *> rows;
    int size;
    unsigned int lastid; // ostatni identyfikator wiersza
    CdtColDesc cols;
    int error;
    int index_eq_id; // ustala czy indexKolumny == idKolumny
    int mode;
    int notypecheck;
    char * cxor_key;
    unsigned int dbID; // Identyfikator bazy
    cCriticalSection CS; // mechanizm blokuj¹cy
    bool changed;
    CdTable ();
    ~CdTable ();



    // rows
    unsigned int getrowid(unsigned int row);  // Uzyskuje identyfikator
    unsigned int getrowpos(unsigned int row); // Uzyskuje pozycje


    #define DT_ISROWID(row) (((row)!=0xFFFFFFFF)&&((row)&DT_ROWID_MASK)) // Sprawdza czy row jest identyfikatorem
	#define DT_MASKID(row) ((row)| DT_ROWID_MASK) // Dodaje maske do identyfiaktora
    #define DT_MAKEID(row) DT_MAKEID(row)
    #define DT_UNMASKID(row) (row&(~DT_ROWID_MASK)) // Zdejumuje maske z identyfikatora
    // f-cje wewnetrzne
    #define DTI_GETID(row) (DT_ISROWID(row)?row:getrowid(row))
    #define DTI_GETPOS(row) (DT_ISROWID(row)?getrowpos(row):row)
    // f-cje zewnetrzne
    #define DT_GETID(dt , row) (DT_ISROWID(row)?row:dt->getrowid(row))
    #define DT_GETPOS(dt , row) (DT_ISROWID(row)?dt->getrowpos(row):row)

    int addrow(int id = -1); // Dodaje wiersz , mozna podac ID
    unsigned int insertrow(unsigned int row , int id = -1); // wstawia wiersz , mozna podac ID
    int deleterow(unsigned
 int row);
    unsigned int getrowcount(); // podaje liczbe wierszy
    void clearrows();
    int findby(TdEntry val , int id=-1 , bool csens=0);

    TdEntry get(unsigned int row , int id); // zwraca wartosc w wierszu/kolumnie
    int set(unsigned int row , int id , TdEntry val);
    // inne
    int getint(unsigned int row , int id);
    int setint(unsigned int row , int id , int val);
    char * getch(unsigned int row , int id);
    int setch(unsigned int row , int id , const char * val);

//    void * getbin(unsigned int row , int id);
    int setbin(unsigned int row , int id , void * val , size_t size);
    CdtctBin getbin(unsigned int row , int id);
    int setbin(unsigned int row , int id , CdtctBin * val);

    Td64 get64(unsigned int row , int id);
    int set64(unsigned int row , int id , Td64 val);

    const std::string getstr(unsigned int row , int id);
    int setstr(unsigned int row , int id , std::string val);

    int checkcoltype(unsigned int id , int type);

    void setindexes();

    bool idexists(int id);

    unsigned short lock(int row);
    unsigned short unlock(int row);
    bool canaccess(int row);

	bool getValue(unsigned int row , unsigned int col , sDTValue * value);
	bool setValue(unsigned int row , unsigned int col , sDTValue * value);


 };


 class CdtFile {
   public:
   CdTable * table;
   int opened;
   CdtColDesc fcols;
   std::string fileName;
   bool write_failed;
//   virtual

    CdtFile();
    CdtFile(CdTable * t);
    void assign(CdTable * t);
    ~CdtFile();
    // Defined
    virtual int load (const char * fn); // wgrywa caly plik
    virtual int loadAll (const char * fn); // wgrywa caly plik , razem z deskryptorem
    virtual int save (const char * fn); // zapisuje caly plik
    virtual int append (const char * fn); // dopisuje elementy

    // placeHolders
    virtual int open (const char * fn , int mode) {return 0;}; // otwiera plik (fn - nazwa , ft - typ)
    virtual int close () {return 0;}; // zamyka plik
    virtual int examine() {return 0;}; // Zapisuje lokalizacje wierszy
    virtual int examinepos(int row) {return 0;}; // zapisuje lokalizacje wiersza
    virtual int readrow(int row) {return 0;}; // wczytuje wiersz
    virtual int freerow(int row) {return 0;}; // zwalnia wiersz
    virtual int readrows() {return 0;}; // wczytuje wiersze
    virtual int next() {return 0;} // przesuwa sie na nastepny
    virtual int prev() {return 0;} // przesuwa sie na poprzedni
    // Obsluga pliku
    virtual int fwriterow(int row) {return 0;};
    virtual int fwritedesc() {return 0;};
    virtual int freaddesc() {return 0;};
    virtual int freadrow(int row) {return 0;}; // wczytuje wiersz na aktualnej pozycji do row
    virtual int freadpartialrow(int row , int * columns) {return 0;}; // wczytuje tylko podane kolumny na aktualnej pozycji do row. Lista kolumn musi byæ zakoñczone ID 0, a d³ugoœæ listy minimum 2, lub pusty wskaŸnik
    virtual int ffindnextrow() {return 0;}; // przechodzi do nastêpnej linijki (w razie gdy freadrow wywali b³¹d)

    virtual int fset(int pos , int whence) {return 0;} // ustawia sie na odpowiednia pozycje ...
 };

#endif


