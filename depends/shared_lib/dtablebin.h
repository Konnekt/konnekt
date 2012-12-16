#ifndef DTABLEBIN
#define DTABLEBIN

//#include <mem.h>
#include "dbtable.h"
#define DT_BIN_ROW_SEP = '\n'
#define DT_BIN_COL_SEP = '\t'
#define DT_BIN_VER '3'
#define DT_BIN_SUBVER '4'

#define DT_BIN_DFID   1
#define DT_BIN_DFMD5  2
#define DT_BIN_DFDBID 4

#define DT_BINTF_FRAGMENTED 0x10000 // Flaga tablicy oznaczaj¹ca, ¿e w tablicy znajduj¹ siê puste wiersze



class CdtFileBin : public CdtFile {

   public:
   FILE * file;
   int pos_dat;
   int pos_row;
   int size;
   int mode;
   char v , subv;
   int flag;
   int siz_dat;
   int dflag;
   bool use_temp;
   bool warn;
   unsigned char md5digest [16];
//   string pass;
//   virtual

    void setPass(std::string p);
    bool authorize(std::string p);

    void init();
    CdtFileBin();
    CdtFileBin(CdTable * t);
    ~CdtFileBin();
    int open (const char * fn , int mode); // otwiera plik (fn - nazwa , ft - typ)
    int close (); // zamyka plik
//    virtual examine(); // Zapisuje lokalizacje wierszy
//    virtual examinepos(int row); // zapisuje lokalizacje wiersza
//    virtual readrow(int row); // wczytuje wiersz
//    virtual freerow(int row); // zwalnia wiersz
//    virtual int load (char * fn); // wgrywa caly plik
//    virtual int save (char * fn); // zapisuje caly plik
//    virtual append (char * fn); // dopisuje elementy
    // Obsluga pliku
    int readrows(); // wczytuje wiersz


    int fwriterow(int row);
    int fwritesize();  // zapisuje ilosc elementow
    int fwritedesc();
    int fseterasedrow(bool overwrite=true , int testIndex=0); // Ustawia rz¹d na aktualnej pozycji jako usuniêty
    int freaddesc();
    int freadsize();
	int freadrow(int row) {return freadpartialrow(row , 0);} // wczytuje wiersz na aktualnej pozycji do row
	int freadpartialrow(int row , int * columns);
	int ffindnextrow(); // przechodzi do nastêpnej linijki (w razie gdy freadrow wywali b³¹d)
    int fset(int pos , int whence); // ustawia sie na odpowiednia pozycje ...
private:
    bool _using_temp;
    CStdString _filename; // Podczas uzywania tempa plik do ktorego zostanie zapisany wynik
    CStdString _temp_filename; // Podczas uzywania tempa plik z ... tempem :)
 };


#endif
