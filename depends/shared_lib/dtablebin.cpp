//#include <mem.h>
#include "dtablebin.h"
#include <md5/md5.h>
#include <windows.h>
#include "func.h"
#include "crypt_func.h"
#include <io.h>

using namespace std;

void CdtFileBin::init() {
            md5digest[0]=0;
			fcols.loader = true;
            pos_dat = pos_row = flag = 0;opened=0;
            siz_dat=0;
            dflag = DT_BIN_DFID;
            use_temp = true;
            _using_temp = false;
            warn = true;
            mode = 0;
			write_failed = false;

            }
CdtFileBin::CdtFileBin() { init(); }
CdtFileBin::CdtFileBin(CdTable * t) {init();assign(t);}
CdtFileBin::~CdtFileBin() {close();}
int CdtFileBin::fset(int pos , int whence) {return fseek(file , pos , whence);} // ustawia sie na odpowiednia pozycje ...

    void CdtFileBin::setPass(std::string p) {
		MD5(p.c_str() , md5digest);
    }
    bool CdtFileBin::authorize(std::string p) {
       unsigned char d [16];
	   MD5((char*)p.c_str() , d);
       return ((dflag & DT_BIN_DFMD5) && !memcmp(md5digest , d , 16)) || 1; 
    }


    int CdtFileBin::open (const char * fn , int mode) {
      close();
	  this->write_failed = false;
      size=0;
      this->fileName = "";
      if (!mode) mode = DT_READ;
      if (table) table->mode=mode;
      this->mode = mode;
      if (mode & DT_READ) {
        table->lastid = DT_ROWID_MIN;
        md5digest[0]=0;
        table->dbID = -1;
      }
      GetFullPathName(fn , MAX_PATH , _filename.GetBuffer(MAX_PATH) , 0);
      _filename.ReleaseBuffer();
      _temp_filename = "";
      const char * filename = _filename;

      if (use_temp && (mode & DT_WRITE) && !(mode & DT_READ)) {
        _temp_filename = _filename;
        do {
           _temp_filename += "_";
        } while (!_access(_temp_filename+".tmp" , 0));
        _temp_filename += ".tmp";
        filename = _temp_filename;
      }
      file=fopen(filename , 
/*           (mode & DT_APPEND) ? "a+b"
          :*/((mode & DT_APPEND) || (mode & DT_WRITE && mode & DT_READ)) ? (_access(fn,0) ? "w+b" : "r+b")
          :(mode & DT_WRITE) ? "wb"
          :"rb");
      if (file && use_temp && (mode & DT_WRITE)) {
        _using_temp = true;
      } else {
        _using_temp = false;
        _temp_filename = "";
      }

      if (!file) return 1;
      opened=1;
      this->fileName = fn;
      return 0;
    }
    int CdtFileBin::close () {
      if (opened) {
          fwritesize();
          fclose(file);
		  if (this->write_failed) {
		      opened=0;
            #ifdef _WINDOWS_
			  char * msg = _saprintf("Wyst¹pi³ b³¹d podczas zapisywania danych!\r\nUszkodzona kopia znajduje siê w:\r\n\t%s%s" , _using_temp ? _temp_filename.c_str() : _filename.c_str(), _using_temp ? "\r\n\r\nOryginalny plik pozosta³ bez zmian..." : "\r\n\r\nB³¹d wyst¹pi³ podczas zapisywania oryginalnej kopii!");
              int r = MessageBox(0 , msg , "B³¹d zapisu .DTB" , MB_OK | MB_ICONERROR | MB_TASKMODAL);
              free(msg);
            #endif
			  return 1;
		  }

          if (_using_temp) { // trzeba przerzuciæ tempa
            _unlink(_filename);
            bool success = true;
			int retries = 0;
            while (rename(_temp_filename , _filename)) {
                //success = false;
				if (++retries < 8) { // czekamy 2 sekundy
					Sleep(250);
					continue;
				}
            #ifdef _WINDOWS_
                char * msg = _saprintf("Nie mogê zapisaæ danych do pliku\r\n   %s" , _filename.c_str());
                int r = MessageBox(0 , msg , "B³¹d zapisu .DTB" , MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON2 | MB_TASKMODAL);
                free(msg);
                if (r==IDIGNORE) {
					success=true;
					break;
				} else if (r==IDABORT) {
	                success = false;
					break;
				}
                _unlink(_filename);
            #endif
            }
            if (!success) _unlink(_temp_filename);
          }

      }
      
      opened=0;
      return 0;
    }

// WRITE

    int CdtFileBin::fwriterow(int row) {
      row = DT_GETPOS(table , row);
      if (!opened) return 1;
      size++;
      if (fputc('\n' , file) == EOF)
		  goto writefailed;
      int b , ctype , cflag , id;
      char * c , * c2;
      Td64 * d;
      int bsize = 0;
      int pos_size = ftell(file);
      b = 0;
      if (v > '1') {
        fwrite(&b , 4 , 1, file);  //size
        fwrite(&table->rows[row]->flag , 4 , 1 , file); // flag
        bsize+=4;
        b = 8;
        fwrite(&b , 4 , 1 , file); // dsiz , dat
        bsize+=b;
        if (b>0) {
          b = DT_BIN_DFID;
          fwrite(&b , 4 , 1 , file);
          if (fwrite(&table->rows[row]->id , 4 , 1 , file) != 1)
			  goto writefailed;
        }
      }
      for (unsigned int i =0; i<fcols.cols.size(); i++) {
        cflag = fcols.cols[i].type;
        ctype = cflag & 0xFF;
        id = fcols.cols[i].id;
        if (id & DT_COLID_UNIQUE) { 
            id = table->cols.getnameid(fcols.cols[i].name.c_str()); 
        }
        if (cflag & DT_CT_DONTSAVE) continue;
        switch (ctype) {
          case DT_CT_INT: b=(int)table->get(row , id);
            fwrite(&b, 4, 1 , file);
            bsize +=4;
            break;
          case DT_CT_64:
            d = (Td64*)table->get(row , id);
            if (!d) {Td64 db; fwrite(&db , 8 , 1 , file);bsize+=8;break;}
            fwrite(d , 8 , 1 , file);
            bsize+=8;
            break;
          case DT_CT_PCHAR: case DT_CT_STRING:
            c=(ctype==DT_CT_PCHAR)? (char *)table->get(row , id)
                               : (char*)(*(string *)table->get(row , id)).c_str()
                               ;
            if (!c) {b=0;fwrite(&b, 4, 1 , file);bsize+=4;break;}
            b=strlen(c);
            if (cflag & DT_CT_CXOR) {
               c2 = strdup(c);
               sc_encrypt((unsigned char*)table->cxor_key , (unsigned char*)c2 , b);
			} else {c2 = c;}
            fwrite(&b, 4, 1 , file);
            fwrite(c2 , b , 1 , file);
            if (cflag & DT_CT_CXOR) {
               free(c2);
            }
            bsize += 4+b;
            break;
          case DT_CT_BIN: {
              CdtctBin bin = table->getbin(row , id);
              fwrite(&bin.size, 4, 1 , file);
              if (bin.size) fwrite(bin.buff , bin.size , 1 , file);
              bsize+=4+bin.size;
              break;}
        }
		if (fputc('\t' , file) == EOF) // sprawdzamy tylko ostatnie write'y
		    goto writefailed;
        bsize+=1;
      }
      if (v > '1') {
        bsize+=4;
        fwrite(&bsize , 4 , 1, file); // size
        b = ftell(file);
        fseek(file , pos_size , SEEK_SET);
		if (fwrite(&bsize , 4 , 1, file) != 1) // size
		    goto writefailed;

        fseek(file , b , SEEK_SET);
      }
      return 0;
writefailed:
	  this->write_failed = true;
	  return 2;
    }

    int CdtFileBin::fwritesize() {
      if (!opened) return 1;
      if (mode & (DT_WRITE | DT_APPEND) && size != -1) {
        fseek(file , (v>'1')?11:6 , SEEK_SET);
        if (fwrite(&size, 4, 1 , file) != 1)
		    goto writefailed;
        if (table && v>'1' && (dflag & DT_BIN_DFID) && (siz_dat >= 8)) {
          fseek(file , 23 , SEEK_SET);
		  if (fwrite(&table->lastid , 4 , 1 , file) != 1)
		      goto writefailed;
        }
      }
      return 0;
writefailed:
	  this->write_failed = true;
	  return 2;
    }

    int CdtFileBin::fwritedesc() {
      if (!opened) return 1;
      v = DT_BIN_VER;
      subv = DT_BIN_SUBVER;
      if (fputs("DTBIN" , file) == EOF ) // We've to check if we can write...
	      goto writefailed;

      fputc(v , file);    // version
      fputc(subv , file); // subversion
      int b , i;      // temporaries
      fwrite(&flag, 4, 1 , file); // flag

      b = table->getrowcount();
      fwrite(&b, 4, 1 , file);    // rowc

      siz_dat = ((dflag & DT_BIN_DFID)?4:0) + ((dflag & DT_BIN_DFMD5)?16:0) + ((dflag & DT_BIN_DFDBID)?4:0);
      siz_dat += (siz_dat>0)*4;
      fwrite(&siz_dat , 4 , 1 , file); // dsiz , dat
      if (siz_dat>0) {
        fwrite(&dflag , 4 , 1 , file);
        if (dflag & DT_BIN_DFID) fwrite(&table->lastid , 4 , 1 , file);
        if (dflag & DT_BIN_DFMD5) fwrite(md5digest , 16 , 1 , file);
        if (dflag & DT_BIN_DFDBID) fwrite(&table->dbID , 4 , 1 , file);
      }
      // data

      b = fcols.getcolcount();
	  if (fwrite(&b, 4, 1 , file) != 1)    // colc
	      goto writefailed;

      for (i=0; i < fcols.getcolcount() ; i++) {  // Columns definitions
        CdtColDescItem * cdi = &fcols.cols[i];
        fwrite(&cdi->id, 4, 1 , file);   //id
		int type = cdi->type & (~DT_CF_NOSAVEFLAGS); 
        fwrite(&type, 4, 1 , file);   //type
        char c= (cdi->name.size() > 255)?255:cdi->name.size();
        fwrite(&c , 1 , 1 , file); // name length
        if (c) fwrite(cdi->name.c_str() , c , 1 , file); // name
        b = 0;
		if (fwrite(&b , 4 , 1, file) != 1) // 0-length data field. For future use maybe.
	        goto writefailed;

      }
      pos_row = ftell(file);
      return 0;
writefailed:
	  this->write_failed = true;
	  return 2;
    }

    int CdtFileBin::fseterasedrow(bool overwrite , int testIndex) {
        if (feof(file)) return 1;
        if (!opened) return 1;
        if (fgetc(file) != '\n') return 1; // wczytuje '\n'
        if (feof(file)) return 1;
        if (v && v < '3') return 1;
        int siz1 , siz2;
        long pos;
        // Najpierw sprawdza czy zgadza siê rozmiar...
        fread(&siz1 , 4 , 1 , file); // size
        pos = ftell(file); // Pozycja zaraz za pierwszym SIZE'em
        fseek(file , siz1 , SEEK_CUR);
        fread(&siz2 , 4 , 1 , file); // size2
        if (feof(file) || siz1 != siz2) return 1; // Oba rozmiary nie s¹ zgodne!!!
        // Sprawdzamy index jeœli jest taka potrzeba
        if (testIndex) {
            fseek(file , pos + 4 , SEEK_SET); // Czytamy DSize
            int dSize=0;
            fread(&dSize , 4 , 1 , file);
            if (dSize >= 8) { // Potrzebujemy równie¿ flagê...
                int dFlag = 0;
                fread(&dFlag , 4 , 1 , file);
                if (dFlag & DT_BIN_DFID) {
                    int dID = 0;
                    fread(&dID , 4 , 1 , file);
                    if (dID != DT_UNMASKID(testIndex)) return 1; // Z³y index!
                }
            }
        }
        if (this->size != -1 && this->size) { // A mo¿e ju¿ jest ustawione?
            fseek(file , pos , SEEK_SET); // Wracamy do pozycji
            int oldFlag = 0;
            fread(&oldFlag , 4 , 1 , file);
            if (oldFlag == -1) return 0;
            this->size --;
        } else this->size = -1;

        fseek(file , pos , SEEK_SET); // Wracamy do pozycji
        int newFlag = -1;
		if (fwrite(&newFlag  , 4 , 1 , file) != 1) // Zapisujemy flagê
			goto writefailed;
        if (overwrite) {
            // Zapisujemy 
            char * buff = new char [100];
            memset(buff , 0 , 100);
            size_t size = siz1 - 4;
            size_t i = 0;
            while (i < size) {
                if (!fwrite(buff , min(100 , size-i) , 1 , file)) break;
                i+=min(100 , size-i);
            }
            delete [] buff;
        }
        // Zaznaczamy flage w deskryptorze
        fseek(file , 7 , SEEK_SET);
        int flag;
        fread(&flag , 4 , 1 , file);
        flag |= DT_BINTF_FRAGMENTED;
        fseek(file , 7 , SEEK_SET);
		if (fwrite(&flag , 4 , 1, file) != 1)
			goto writefailed;
        fseek(file , pos + siz1 + 4 , SEEK_SET); // Skaczemy dalej
        return 0;
writefailed:
	  this->write_failed = true;
	  return 2;

    }



// READ

    int CdtFileBin::freaddesc() {
      if (!opened) return 1;
      fseek(file , 0 , SEEK_SET);
      char sig [7];
      sig[6]='\0';
      fread(sig , 6 , 1 , file);
      if (strncmp("DTBIN" , sig , 5)) return 2;
      v = sig[5];
      if (v > DT_BIN_VER) return 2;
      if (v>'1') {fread(&subv , 1 , 1 , file);
        fread(&flag , 4 , 1, file);
      }
        else {subv = 0; flag = 0;}
      fread(&size, 4, 1 , file);  // rowc
      table->size = size;
      int csize , a , b;
      table->lastid = DT_ROWID_MIN;
      md5digest[0]=0;
      table->dbID = -1;
      if (v>'1') {
        fread(&siz_dat , 4 , 1 , file); // dsiz
        pos_dat = ftell(file);
        int dat_seek = siz_dat;
        if (siz_dat) {
          fread(&dflag , 4 , 1 , file); // dflag
          dat_seek -=4;
          if (dflag & DT_BIN_DFID)
               {fread(&table->lastid , 4 , 1 , file);dat_seek -=4;}
          if (dflag & DT_BIN_DFMD5)
               {fread(md5digest , 16 , 1 , file);dat_seek -=16;}
          if (dflag & DT_BIN_DFDBID)
               {fread(&table->dbID , 4 , 1 , file);dat_seek -=4;}
        }
        if (dat_seek>0) fseek(file , dat_seek , SEEK_CUR);
      }

      fread(&csize, 4, 1 , file);
      fcols.setcolcount(csize);
      for (int i=0; i<csize; i++) { // columns definitions
        fread(&a, 4, 1 , file);
        fread(&b, 4, 1 , file);
        int id = a;
        int type = b;
        char * name = 0;
        if (v>'2') { // version 3x only
            char c;
            fread(&c , 1 , 1 , file);
            if (c) {
                 name = new char [c+1];
                 fread(name , c , 1 , file);
                 name[c]=0;
            }
            fread(&b , 4 , 1 , file);
            if (b) fseek(file , b , SEEK_CUR); // We have to pass unprocessed data.
        }
        fcols.setcol(id , type , 0 , name?name:"");
        if (name) delete [] name;
      }
      pos_row = ftell(file);
      return 0;
    }
    int CdtFileBin::freadsize() {
      if (!opened) return 1;
      fseek(file , (v>'1')?11:6 , SEEK_SET);
      fread(&size, 4, 1 , file);
//      MessageBox(0 , inttoch(size) , "" , 0);
      return 0;
    }


    int CdtFileBin::freadpartialrow(int row , int * columns) {
        start:
      if (feof(file)) return 1;
      row = DT_GETPOS(table , row);
      if (!opened) return 1;
	  size_t filesize = _filelength(file->_file);
      table->notypecheck=1;  // wylacza sprawdzanie typow ...
      CdtRow * rowObj = table->rows[row];
      rowObj->pos = ftell(file);
      if (fgetc(file) != '\n') return 1;
      if (feof(file)) return 1;
      int b , ctype , cflag , id;
      Td64 d;
      char * c;
      table->lastid++;
      rowObj->id = table->lastid;
      int siz1 , siz2 , p;
      if (v > '1') {
        fread(&siz1 , 4 , 1 , file); // size
        p = ftell(file); // pocz¹tek danych...
		if (p + siz1 + 4 > filesize)
			return 1;
        if (fseek(file , siz1 , SEEK_CUR))
			return 1;
        if (!fread(&siz2 , 4 , 1 , file)) // size2
			return 1;
        fseek(file , p , SEEK_SET); // Wracamy do pozycji
        if (siz1 != siz2) 
			return 1; // Oba rozmiary nie s¹ zgodne!!!
        if (!fread(&rowObj->flag  , 4 , 1 , file))
			return 1; // flag
        // Sprawdzamy czy flaga jest równa -1 czyli czy element
        // nie jest oznaczony jako usuniêty
        if (rowObj->flag == -1) {
            fseek(file , siz1 , SEEK_CUR); // przesuwamy siê o p + size + 4 (rozmiar, wczytany jako flaga)...
            table->lastid --;
            goto start; // Wczytujemy nastêpn¹ pozycjê
        }

        int dsiz;
        fread(&dsiz , 4 , 1 , file); // dsiz
		if ((p + siz1) < ftell(file) + dsiz)
			return 1;
        if (dsiz) {
          int dflag;
          fread(&dflag , 4 , 1 , file); // dflag
          dsiz -=4;
          if (dflag & DT_BIN_DFID)
               {fread(&rowObj->id , 4 , 1 , file);
                if (rowObj->id > table->lastid) table->lastid = rowObj->id+1;
                table->lastid--;
                dsiz -=4;}
        }

        if (dsiz>0) fseek(file , dsiz , SEEK_CUR);

      }

      for (unsigned int i =0; i<fcols.cols.size(); i++) {
        cflag = fcols.cols[i].type;
        ctype = cflag & 0xFF;
        id = fcols.cols[i].id;
        if (id & DT_COLID_UNIQUE) { 
            id = table->cols.getnameid(fcols.cols[i].name.c_str()); 
        }
        if (cflag & DT_CT_DONTSAVE) continue;
		  bool skip = false; // Czy OMIN¥Æ kolumnê?
		  if (columns) { // szukamy, czy ID aktualnej, jest na liœcie, jak nie to omijamy...
			  skip = true;
			  int i = 0;
			  do {
				  if (id == columns[i]) {
					  skip = false;
					  break;
				  }
				  i++;
			  } while (columns[i]);
		  }

		switch (ctype) {
          case DT_CT_INT:
			  if (skip)
				  fseek(file , 4 , SEEK_CUR);
			  else {
				fread(&b, 4, 1 , file);
				table->setint(row , id , b);
			  }
              break;
          case DT_CT_64:
			  if (skip)
				  fseek(file , 8 , SEEK_CUR);
			  else {
				fread(&d , 8 , 1 , file);
				table->set64(row , id , d);
			  }
				break;
          case DT_CT_PCHAR: case DT_CT_STRING:
            fread(&b, 4, 1 , file);
			if ((p + siz1) < ftell(file) + b)
				return 1;
			if (skip)
				fseek(file , b , SEEK_CUR);
			else {
				c=new char [b+1];
				c[b]='\0';
				if (b) fread(c , b , 1 , file);
				if (cflag & DT_CT_CXOR) {
				sc_decrypt((unsigned char*)table->cxor_key , (unsigned char*)c , b);
				}
				if (ctype == DT_CT_PCHAR) table->setch(row , id , c);
				else table->setstr(row , id , c);
				delete [] c;
			}
            break;
          case DT_CT_BIN:{
            CdtctBin bin;
            bin.buff = 0;
            fread(&bin.size, 4, 1 , file); // wczytujemy rozmiar
			if ((p + siz1) < ftell(file) + bin.size)
				return 1;
			if (skip)
				fseek(file , bin.size , SEEK_CUR);
			else {
				if (bin.size) {
					bin.buff = new char [bin.size];
					fread(bin.buff , bin.size , 1 , file);
				}
				table->setbin(row , id , &bin);
				delete [] bin.buff;
			}
            break;}

        }
        if (fgetc(file) != '\t')
			return 1;
      }

	  if (v > '1') {
        fread(&siz2 , 4 , 1 , file); // size
		if (siz1 != siz2) return 1;
	  }
      table->notypecheck=0;
      return 0;
    }

	int CdtFileBin::ffindnextrow() { // przechodzi do nastêpnej linijki (w razie gdy freadrow wywali b³¹d)
		if (v <= '1' || feof(file))
			return 1;
		//size_t filesize = _filelength(file->_file);
		while (!feof(file)) {
			// szukamy '\n'
			if (getc(file) == '\n') {
				fseek(file , -1 , SEEK_CUR);
				return 0; // znalaz³em potencjalny nowy wpis... freadrow zajmie siê sprawdzaniem...
			}
		}
		return 1;
	}


    int CdtFileBin::readrows() {
//      int pos = ftell(file);
//      freadsize();
//      fseek(file , pos , SEEK_SET);
        table->clearrows();
        int i;
        while (1 && !feof(file))
        { 
            i = table->addrow(0);
			while (1) {
				table->rows[i]->pos = ftell(file);
				if (freadrow(i)) {
					if (!ffindnextrow())
						continue; // próbujemy nastêpny...
					else {
						table->deleterow(i);
						return 0;
					}
				} else {
					break; // jest ok, nastêpny...
				}
			}
		
		}
        return 0;
    }

