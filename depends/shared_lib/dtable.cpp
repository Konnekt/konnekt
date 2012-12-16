#include "dbtable.h"

#include <windows.h>
#include <vector>
#include <stack>
#include <stdio.h>
#include <string>
//#include <iostream>
#include <io.h>
#include <stdarg.h>
#include <time.h>
using namespace std;




CdtRow::CdtRow (const CdtRow & v) {freeData(); }

CdtColDescItem::CdtColDescItem() {type=0;id=-1;def=0;name="";}

CdtColDesc::CdtColDesc () {deftype=0;table = 0;loader=false;}
int CdtColDesc::getcolcount () {return cols.size();}
void CdtColDesc::clear() {cols.clear();}

unsigned int CdTable::getrowcount() // podaje liczbe wierszy
        {return rows.size();}
int CdTable::getint(unsigned int row , int id)                {return (int)get(row , id);}
int CdTable::setint(unsigned int row , int id , int val)      {
                   #ifdef DT_CHECKTYPE
                     if (!checkcoltype(id , DT_CT_INT))
                        return 0;
                   #endif

                   return set(row,id,(TdEntry)val);}

    void CdTable::setindexes() {
        CS.lock();
        changed = true;
        for (unsigned int i=0; i<rows.size(); i++) {
            rows[i]->index=i;
        }
        CS.unlock();
    }


    bool CdTable::idexists(int id) {
       if (DT_ISROWID(id)) return getrowpos(id)!=-1;
       else return getrowid(id)!=-1;
    }

    unsigned int CdTable::getrowid(unsigned int row) {
       if (DT_ISROWID(row)) return row;
       if (row >= rows.size()) return -1;
       return DT_MASKID(rows[row]->id);
    }
    unsigned int CdTable::getrowpos(unsigned int row) {
       if (!DT_ISROWID(row)) return row;
       row = DT_UNMASKID(row);
       for (unsigned int i=0; i < rows.size(); i++)
         if (rows[i]->id == row) return i;
       return -1;
    }


    int CdTable::findby(TdEntry val , int id , bool csens) {
        if (id==-1) {
            for (int i = 0; i<cols.getcolcount(); i++)
            if (cols.getcolflag(i) & DT_CT_KEY) {id = i;break;}
        } else id = cols.colindex(id);
        if (id==-1) return -1;
        int type = cols.getcoltype(id);
        char * ch;
        CS.lock();
        int ret = -1;
        for (unsigned int i = 0; i<rows.size(); i++) {
            switch (type) {
            case DT_CT_INT: if (rows[i]->getbyindex(id) == val) {ret=i;goto end;} break;
            case DT_CT_64: if (*(__int64*)rows[i]->getbyindex(id) == *(__int64*)val) {ret=i;goto end;} break;
            case DT_CT_PCHAR:
                ch = (char*)rows[i]->getbyindex(id);
                if (!ch) continue;
                if (csens?!strcmp(ch,(char*)val) : !stricmp(ch,(char*)val)) {ret=i;goto end;} break;
            default: goto end;
            }
        }
        end:
        CS.unlock();
        return ret;
    }


    int CdTable::checkcoltype(unsigned int id , int type) {
      if (notypecheck) return 1;
      int i=cols.colindex(id);
      return (cols.getcoltype(i)==type);
    }

    char * CdTable::getch(unsigned int row , int id)
           {char * v = (char *)get(row , id); return v?v:"";}

    int   CdTable::setch(unsigned int row , int id , const char * val) {
//      if (row<0 || row>rows.size()) return 0;
//      return set(row,id,(TdEntry)strcpy((char *)rows[row]->garbage.addArray(new char [strlen(val)+1]) , val));
        return set(row , id , (TdEntry)val);
      }

    CdtctBin CdTable::getbin(unsigned int row , int id){
        void * buff = (void*)get(row , id);
        CdtctBin bin;
        if (!buff) { bin.size = 0; bin.buff = 0;}
        else {bin.size = *(int*)buff; bin.buff = bin.size?(char*)buff+4:0;}
        return bin;
    }
    int CdTable::setbin(unsigned int row , int id , CdtctBin * val){
        return set(row , id , (TdEntry)val);
    }

/*    void * CdTable::getbin(unsigned int row , int id) {
        return (CdtctBin)getbin(row , id).buff;
    }*/
    int CdTable::setbin(unsigned int row , int id , void * val , size_t size){
        CdtctBin b;
        b.size = size;
        b.buff = val;
        return set(row , id , (TdEntry)&b);
    }


    Td64 CdTable::get64(unsigned int row , int id)
           {Td64 * v = (Td64 *)get(row , id); return v?*v:0;}

    int   CdTable::set64(unsigned int row , int id , Td64 val) {
//      if (row<0 || row>rows.size()) return 0;
//      return set(row,id,(TdEntry)strcpy((char *)rows[row]->garbage.addArray(new char [strlen(val)+1]) , val));
        return set(row , id , (TdEntry)&val);
      }

    const string CdTable::getstr(unsigned int row , int id) {
       string * s;
       s=(string *)get(row , id);
       return s?*s:string("");
       }

    int   CdTable::setstr(unsigned int row , int id , string val) {
//      if (row<0 || row>rows.size()) return 0;
//      return set(row,id,(TdEntry)rows[row]->garbage.add(new string (val)));
        return set(row , id , (TdEntry)&val);
      }

/*
  Gdy pobierana jest wartoœæ char to...
    vChar = 0 buffSize = -1  - zwracany jest duplikat
	vChar = 0 buffSize = 0   - b³¹d
	vChar = * buffSize = 0   - zwracana jest aktualna wartoœæ, a w przypadku liczb u¿ywany jest podany wskaŸnik
	vChar = * buffSize = #   - wartoœæ jest kopiowana do *
*/
bool CdTable::getValue(unsigned int row , unsigned int col , sDTValue * value) {
	this->CS.lock();
	row = DTI_GETPOS(row);
	if (value->type == DT_CT_PCHAR && value->vChar)
		value->vChar[0] = 0;
	if (row >= this->rows.size()) row = -1;
	if (row==-1) goto fail;
	this->rows[row]->CS.lock();
	int i=this->cols.colindex(col); // i - pozycja kolumny
	if (i<0) goto fail;
	// W type znajduje siê typ czytanej kolumny
	int type = this->cols.getcoltype(i) & DT_CT_TYPEMASK;
	if (value->type == DT_CT_UNKNOWN) value->type = type;
	if (value->type == DT_CT_PCHAR && type != DT_CT_PCHAR && !value->vChar && value->buffSize==0) goto fail;
	int val = (int)this->get(row,col);
	const char * ch = 0;
	switch (value->type) {
	case DT_CT_INT: switch (type) {
					case DT_CT_INT: value->vInt = val;goto done;
					case DT_CT_64: value->vInt = val ? *(__int64*)val : 0;goto done;
					case DT_CT_PCHAR: value->vInt = val ? atoi((char*)val) : 0;goto done;
					};goto fail;
	case DT_CT_64: switch (type) {
					case DT_CT_INT: value->vInt64 = val;goto done;
					case DT_CT_64: value->vInt64 = val? *(__int64*)val : 0;goto done;
					case DT_CT_PCHAR: value->vInt64 = val ? _atoi64((char*)val) : 0;goto done;
					};goto fail;
	case DT_CT_PCHAR: 
		if (type == DT_CT_PCHAR) {
			ch = val?(char*)val:"";
			if (value->vChar && value->buffSize != -1 && value->buffSize != 0) 
				strncpy(value->vChar , ch , value->buffSize);
			else if (value->buffSize == -1)
				value->vChar = strdup(ch);
			else value->vCChar = ch;
		} else {
			if (!value->vChar && value->buffSize == -1) 
				value->vChar = (char*)malloc(32);
			if (type == DT_CT_INT)
				itoa(val , value->vChar , 10);
			else if (type == DT_CT_64)
				_i64toa(val ? *(__int64*)val : 0 , value->vChar , 10);
			else value->vChar[0] = 0;
		}
		goto done;
	default: goto fail;
	}

done:
	this->rows[row]->CS.unlock();
	this->CS.unlock();
	return true; 
fail:
	if (row != -1) this->rows[row]->CS.unlock();
	this->CS.unlock();
	return false; 
}



bool CdTable::setValue(unsigned int row , unsigned int col , sDTValue * value) {
	this->CS.lock();
	row = DTI_GETPOS(row);
	if (row==-1) goto fail;
	int i=this->cols.colindex(col);
	if (i<0) goto fail;
	int type = this->cols.getcoltype(i) & DT_CT_TYPEMASK;
	if (value->type == DT_CT_UNKNOWN) value->type = type;
	int val = 0;
	__int64 val64;
	char buff [32];
	switch (type) {
	case DT_CT_INT: switch (value->type) {
					case DT_CT_INT: val = value->vInt;goto done;
					case DT_CT_PCHAR: val = atoi(value->vCChar);goto done;
					case DT_CT_64: val = value->vInt64;goto done;
					}; goto fail;
	case DT_CT_64:  switch (value->type) {
					case DT_CT_INT: val64=value->vInt; val = (int)&val64;goto done;
					case DT_CT_PCHAR: val64=_atoi64(value->vCChar); val = (int)&val64;goto done;
					case DT_CT_64: val64=value->vInt64; val = (int)&val64;goto done;
					}; goto fail;
	case DT_CT_PCHAR:  switch (value->type) {
					case DT_CT_INT: itoa(value->vInt , buff , 10);val = (int)buff; goto done;
					case DT_CT_PCHAR: val = (int)value->vCChar;goto done;
					case DT_CT_64: _i64toa(value->vInt64 , buff , 10); val = (int)buff; goto done;
					}; goto fail;
	default: goto fail;
	}
	done:
	this->set(row,col,(TdEntry)val); 
	this->CS.unlock();
	return true;
fail:
	this->CS.unlock();
	return false;
}






// -----------------------------------------------------
// -----------------------------------------------------
 // CdRow


    CdtRow::CdtRow (CdTable * t, int index , char allocate) {
//          garbage.id=1;
//          cout << "C";
          this->index=index;
          id = 0;
          flag = 0;
          size=0;pos=0;
          table=t;
          if (allocate) allocData();
          }

    CdtRow::~CdtRow () {/*cout << "d";  */freeData(); }


    int CdtRow::allocData() {
		CS.lock();
        freeData();
        CdTable * tb=((CdTable *)table);
        size=tb->cols.getcolcount();
		if (!size) {CS.unlock(); return 0;}
//      printf("*CdtRow::alloc(%d)\n" , size);
      data=new TdEntry [size];
//      #ifdef DT_SETBLANKS
      for (int i = 0 ; i < size ; i++) {
        #ifdef DT_SETBLANKSBYTYPE
//        data[i]=((CdTable *)table)->cols.cols[i].def;
//        int chk = (int)tb->cols.cols[i].id;
//        int chk2 = (int)tb->cols.cols[i].def;
        switch (tb->cols.getcoltype(i)) {
          case DT_CT_INT: data[i]=tb->cols.cols[i].def; break;
          case DT_CT_64: data[i]=(void*)new Td64;
               if (tb->cols.cols[i].def)
                  *(Td64*)data[i]=*(Td64*)tb->cols.cols[i].def;
                  else *(Td64*)data[i]=0;
               break;
          case DT_CT_PCHAR:
               if (tb->cols.cols[i].def)
                 {data[i]=(void*)new char [strlen((char *)tb->cols.cols[i].def)+1];
                  data[i]=(void*)strcpy((char *)data[i] , (char *)tb->cols.cols[i].def);
                 }
                 else data[i]=0;
               break;
          case DT_CT_STRING:
               if (tb->cols.cols[i].def)
                 {data[i]=(void*)new string((char *)tb->cols.cols[i].def);
                 }
                 else data[i]=0;
               break;
          case DT_CT_BIN:
               if (tb->cols.cols[i].def)
                 {CdtctBin * bin = (CdtctBin*)tb->cols.cols[i].def;
                  data[i]=(void*)new char [4+bin->size];
                  memcpy((void*)data[i] , &bin->size , 4);
                  if (bin->buff && bin->size) 
                      memcpy((char *)data[i]+4 , bin->buff , bin->size);
                 }
                 else data[i]=0;
               break;
        }
        #else
        data[i]=0;
        #endif
      }
//      #endif

//      printf("\n");
	  CS.unlock();
      return 1;
    }
    int CdtRow::freeData() {
		CS.lock();
      if (size) {/*cout << "f" ;*/
          CdTable * tb=((CdTable *)table);
          for (int i = 0 ; i < size ; i++) {
            switch (tb->cols.getcoltype(i)) {
              case DT_CT_64:
                if (data[i]) delete data[i];
                break;
              case DT_CT_PCHAR:
                if (data[i]) delete [] data[i];
                break;
              case DT_CT_STRING:
               if (data[i]) delete data[i];
               break;
              case DT_CT_BIN:
                if (data[i]) delete [] data[i];
                break;
            }
          }
          delete [] data;size=0;
      }
	  CS.unlock();
      return 1;
    }

    const TdEntry CdtRow::get (int id) {
        CS.lock();
      ((CdTable *)table)->error=0;
      int i=((CdTable *)table)->cols.colindex(id);
	  if (i>=size || i<0) {((CdTable *)table)->error=DT_ERR_NOROW;CS.unlock();return 0;}
      #ifdef DT_CHECKTYPE
      TdEntry r = data[i];
      if (((CdTable *)table)->cols.getcoltype(i)==DT_CT_PCHAR && !r) r = (TdEntry)"";
      CS.unlock();
      return r;
      #else
      CS.unlock();
      return data [i];
      #endif
    }

    const TdEntry CdtRow::getbyindex (int index) {
      if (index>=size || index<0) {((CdTable *)table)->error=DT_ERR_NOROW;return 0;}
      return data [index];
    }

    int CdtRow::set (int id , TdEntry val) {
      int i=((CdTable *)table)->cols.colindex(id);
      if (i>=size || i<0) return 0;
        CS.lock();
      // Alokuje pamiec dla typow

      switch (((CdTable *)table)->cols.getcoltype(i)) {
        case DT_CT_64:
            if (!data[i]) data[i]=(void*)new Td64;
            if (val) *((Td64*)data[i]) = *((Td64*)val);
            break;
        case DT_CT_PCHAR:
            if (data[i] && strlen((char *)data[i])!=strlen((char *)val))
               {
               delete [] data[i];
               data[i]=0;}
            if (!data[i])    data[i]=new char [strlen((char*)val)+1];
            data[i]=(void*)strcpy((char *)data[i] , (char *)val);
            break;
        case DT_CT_STRING:
            if (!data[i]) data[i]=(void*)new string(*((string *)val));
              else *(string *)data[i]=*((string *)val);
            break;
        case DT_CT_BIN: {
            CdtctBin * bin = (CdtctBin *)val;
            if (data[i] && *(int*)data[i]!=bin->size) {
               delete [] data[i];
               data[i]=0;
            }
            if (!data[i]) data[i]=new char [4+bin->size];
            memcpy(data[i] , &bin->size , 4);
            if (bin->size && bin->buff) memcpy((char *)data[i]+4 , bin->buff , bin->size);
            break;}
        default:
            data [i]=val;

      }

      CS.unlock();
      return 0;
    }


 // CdColDesc

    CdtColDesc::CdtColDesc (int count , ...) {
        loader = 0;
        deftype=0;
        table = 0;
	    va_list ap;
	    va_start(ap, count);
            for (int i=0; i<count; i++) {
            int id = va_arg(ap,int);
            int type = va_arg(ap,int);
            void * def = va_arg(ap,void*);
            char * name = va_arg(ap,char*);
            setcol(id , type , def , name);
            }
	    va_end(ap);
    }

    int CdtColDesc::operator = (CdtColDesc & x) {
      cols.resize(x.cols.size());
      for (unsigned int i=0; i<x.cols.size();i++)
        cols[i]=x.cols[i];
      if (table && x.table) {
        ((CdTable*)table)->index_eq_id = ((CdTable*)x.table)->index_eq_id; 
      } else if (table && !x.table) {
        ((CdTable*)table)->index_eq_id = 0;
      }
//       = x.cols;
      return 0;
    }


    int CdtColDesc::setcolcount (int count,int flag) {
        CdtColDescItem bl;
        if (!(flag & DT_CC_RESIZE)) {cols.clear();}
        int resize=cols.size();
        cols.resize(count+resize , bl);
        for (cols_it_t cols_it=cols.begin()+resize; cols_it != cols.end() ; cols_it++ ) {
            cols_it->id=-1;
            cols_it->type=deftype;
            cols_it->def=0;
          }
        if (flag & DT_CC_FILL) {
          for (int i = 0;i<count;i++)
            setcol(i , deftype);
        }
        return cols.size();
      } // ustawia ilosc kolumn

    void CdtColDesc::optimize(void) {
        cols_it_t cols_it=cols.begin();
        while (cols_it!=cols.end()) {
          if (cols_it->id==-1) cols_it=cols.erase(cols_it);
             else cols_it++;
        }
        return;
    }

    int CdtColDesc::setcol (int id , int type , TdEntry def , const char * name) {
        if (!name) name = "";
        if (id!=-1 && ((id & 0xFF000000) == 0xFF000000)) 
            id = -1;
        if (id == -1 && *name) return setuniquecol(name , type , def);
        if (table) ((CdTable *)table)->error=0;
        int index=colindex(id,0);
        try {
          if (index<0) { //jezeli nie moze znalezc istniejacej kolumny szuka wolnej
                        index=colindex(-1,0);
                        if (index<0) { // jezeli nie moze znalezc wolnej
                           index = setcolcount(1 , DT_CC_RESIZE) -1; // dodaje nowa
                        }
                       }
          CdtColDescItem & v=cols[index];
          if (table && id!=index) ((CdTable *)table)->index_eq_id=0;
		  if (this->loader)
			  v.type = DT_CF_LOADED;
		  else
			  v.type = 0;
          v.type |= type;
          v.id=id;
          v.def=def;
          v.name=name;
        } catch (...) {if (table) ((CdTable *)table)->error=DT_ERR_NOCOL;}
        return id;
      }

    int CdtColDesc::setuniquecol (const char * name , int type , TdEntry def) {
        if (!name) name = "";
        if (table) ((CdTable *)table)->error=0;
        int id = getnameid(name);
        if (id == -1) {id=getnewuniqueid();}
        setcol(id , type , def , name);
        return id;
      }

    int CdtColDesc::getcol (int index , int & type , int & id) {
        type=0;id=0;
        try {CdtColDescItem t = cols.at(index);
             type = t.type;
             id = t.id;
             return 1;}
        catch (...) {return 0;}
      }

    const char * CdtColDesc::getcolname (int index) {
        try {
			return cols.at(index).name.c_str();
		} catch (...) {
			return 0;
		}
      }


    int CdtColDesc::getcoltype (int index) {
        int type , id;
        getcol(index , type , id);
        return type & 63;
    }

    int CdtColDesc::getcolflag (int index) {
        int type , id;
        getcol(index , type , id);
        return type;
    }

    int CdtColDesc::getcolid (int index) {
        int type , id;
        getcol(index , type , id);
        return id;
    }

    int CdtColDesc::colindex (int id , bool quick) {
        if (quick && table && ((CdTable *)table)->index_eq_id) return id;
        if (cols.size()==0) return -1;
        int i=0;
        for (cols_it_t cols_it = cols.begin(); cols_it != cols.end(); cols_it++)
          {if (cols_it->id == id) return i; i++;}
        return -1;
      }

    int CdtColDesc::getnewuniqueid(void) {
        int unique = time(0) & 0xFFFF;
        do {
            unique++;
            unique &=0xFFFF;
        } while (this->colindex(unique | DT_COLID_UNIQUE , false)!=-1);
        return unique | DT_COLID_UNIQUE;
    }




    int CdtColDesc::getnameid(const char * name) {
        if (cols.size()==0) return -1;
        for (cols_it_t cols_it = cols.begin(); cols_it != cols.end(); cols_it++)
          {if (cols_it->name == name) return cols_it->id; }
        return -1;
    }

	int CdtColDesc::join(CdtColDesc * other , bool overwrite) { // ³¹czy dwa deskryptory, zwraca liczbê do³¹czonych
		int c = 0;
		for (cols_it_t i = other->cols.begin(); i != other->cols.end(); i++) {
			// Sprawdzamy czy taka ju¿ nie istnieje...
			if (!overwrite) {
				int id = i->id;
				if (id & DT_COLID_UNIQUE)
					id = this->getnameid(i->name.c_str());
				if (id != -1 && this->colindex(id) != -1)
					continue;
			}
			// ustawiamy kolumnê
			this->setcol((i->id & DT_COLID_UNIQUE)? -1 : i->id , i->type , i->def , i->name.c_str());
		}
		return c;
	}


 // CdTable  ***************************************

    CdTable::CdTable() {
      mode=0;
      notypecheck=0;
//      filecols.table=this;
      cols.table=this;
      index_eq_id=1;
      lastid = DT_ROWID_MIN;
      changed = false;
      error=0;
      cxor_key="";
    }
    CdTable::~CdTable() {
//    int start=GetTickCount();
      clearrows();
//    int end=GetTickCount();
//    printf ("Czas czyszczenia: %f s\n" , float(end-start) / 1000);
//    rows.reserve(10);
//    printf ("Wektory: row [%d] \n" , rows.capacity());

    }

    void CdTable::clearrows() {
        CS.lock();
        changed = true;
        for (t_rows_it rows_it=rows.begin() ; rows_it != rows.end() ; rows_it++) {
            delete *rows_it;
            }
        rows.clear();
//      rows.swap(vector<CdtRow *>());
        CS.unlock();
    }

    int CdTable::addrow(int id) {
//      CdtRow * r=new CdtRow(this,1);
        CS.lock();
        changed = true;
        int row = rows.size();
        rows.push_back(new CdtRow(this,row,1));
        if (id < 0) lastid++;
        if (lastid > DT_ROWID_MAX) lastid = DT_ROWID_MIN;
        rows[row]->id = id>0?DT_UNMASKID(id):lastid;
        CS.unlock();
        return rows.size()-1;
    }
    unsigned int CdTable::insertrow(unsigned int row , int id) {
//      CdtRow * r=new CdtRow(this,1);
        CS.lock();
        changed = true;
        row = DTI_GETPOS(row);
        rows.insert(rows.begin()+row , new CdtRow(this,1));
        if (id < 0) lastid++;
        if (lastid > DT_ROWID_MAX) lastid = DT_ROWID_MIN;
        rows[row]->id = id>0?DT_UNMASKID(id):lastid;
        CS.unlock();
        return row;
    }
    int CdTable::deleterow(unsigned int row) {
        CS.lock();
        changed = true;
        row = DTI_GETPOS(row);
		if (row == DT_NOROW || row >= rows.size()) {CS.unlock(); return 0;}
        delete rows[row];
        rows.erase(rows.begin()+row);
        CS.unlock();
        return 1;
    }



    TdEntry CdTable::get(unsigned int row , int id){
        CS.lock();
        row = DTI_GETPOS(row);
        if (row==DT_NOROW || row>=rows.size()) {
            error=DT_ERR_NOROW;
            CS.unlock();
            return 0;}
        try {
            TdEntry ret;
            if (id == DT_C_ID) ret=(TdEntry)rows[row]->id;
            else ret=(rows[row])->get(id);
            CS.unlock();
            return ret;
        } catch (...) {
            error=DT_ERR_NOROW;
            CS.unlock();
            return 0;}
    }
    int CdTable::set(unsigned int row , int id , TdEntry val){
        CS.lock();
        changed = true;
        row = DTI_GETPOS(row);
        if (row==DT_NOROW || row>=rows.size()) {
            error=DT_ERR_NOROW;
            CS.unlock();
            return 0;}
        try {
            rows[row]->set(id,val);
        } catch (...) {}
        CS.unlock();
        return 0;
    }

    unsigned short CdTable::lock(int row) {
        if (row==DT_LOCKALL) {
            this->CS.lock();
            for (size_t i=0; i<rows.size(); i++)
                rows[i]->CS.lock();
            return 0;
        } else {
            CS.lock();
            row = DTI_GETPOS(row);
            if (row==DT_NOROW || row>=rows.size()) {
                error=DT_ERR_NOROW;
                CS.unlock();
                return 0;}
			rows[row]->CS.lock();
            CS.unlock();
            return 0;//rows[row]->CS.getLockCount();
        }
    }
    unsigned short CdTable::unlock(int row){
        if (row==DT_LOCKALL) {
            for (size_t i=0; i<rows.size(); i++)
                rows[i]->CS.unlock();
            this->CS.unlock();
            return 0;
        } else {
            CS.lock();
            row = DTI_GETPOS(row);
            if (row==DT_NOROW || row>=rows.size()) {
                error=DT_ERR_NOROW;
                CS.unlock();
                return 0;}
			rows[row]->CS.unlock();
            CS.unlock();
            return 0;//rows[row]->CS.getLockCount();
        }
    }
    bool CdTable::canaccess(int row){
		bool can = true;
        if (row==DT_LOCKALL) {
			if (!this->CS.canAccess()) return false;
            this->CS.lock();
            for (size_t i=0; i<rows.size(); i++)
				if (!rows[i]->CS.canAccess())
					can = false;
        } else {
			if (!this->CS.canAccess()) return false;
            CS.lock();
            row = DTI_GETPOS(row);
            if (row==DT_NOROW || row>=rows.size()) {
                error=DT_ERR_NOROW;
                CS.unlock();
                return 0;}
			can = rows[row]->CS.canAccess();
        }
        this->CS.unlock();
        return can;
    }


    // ****************************************** FILE

	CdtFile::CdtFile() {
		fcols.loader = true; 
		opened=0;
		write_failed = false;
	}
	CdtFile::CdtFile(CdTable * t) {fcols.loader = true; opened=0;assign(t);}
	void CdtFile::assign(CdTable * t) {table=t;fcols.table=t;}
	CdtFile::~CdtFile() {}


    int CdtFile::load (const char * fn)
    {
      if (!fn) fn=fileName.c_str();
       else fileName = fn;
      table->clearrows();
      fcols.clear();
//      open(fn , DT_READ);
      int r;
      r=open(fn , DT_READ);
      if (r) return r;
      r=freaddesc(); if(r) return r;
      r=readrows(); if(r) return r;
      close();
      return 0;
    }

    int CdtFile::loadAll (const char * fn)
    {
      if (!fn) fn=fileName.c_str();
       else fileName = fn;
      table->clearrows();
      fcols.clear();
//      open(fn , DT_READ);
      int r;
      r=open(fn , DT_READ);
      if (r) return r;
      r=freaddesc(); if(r) return r;
	  table->cols.join(&fcols , false);
      r=readrows(); if(r) return r;
      close();
      return 0;
    }


    int CdtFile::save (const char * fn)
    {
      if (!fn) fn=fileName.c_str();
       else fileName = fn;
      fcols = table->cols;
      int r;
      r=open(fn , DT_WRITE);
      if (r) return r;
      fwritedesc();
      for (unsigned int i=0; i < table->getrowcount() ; i ++)
        {if (!(table->rows[i]->flag & DT_RF_DONTSAVE)) fwriterow(i);}
      close();
      return 0;
    }

    int CdtFile::append (const char * fn) {
      if (!fn) fn=fileName.c_str();
       else fileName = fn;
      fcols.clear();
      int r;
      bool first = _access(fn , 0)!=0;
      r=open(fn , DT_APPEND);
      if (r) return r;
      fset(0 , SEEK_SET);
      if (first) {
        fcols = table->cols;
        table->lastid = DT_ROWID_MIN;
        fwritedesc();
      } else {
        r=freaddesc(); if(r) return r;
      }
/*      close();
      r=open(fn , DT_APPEND);*/
      if (r) return r;
      fset(0 , SEEK_END);
      for (unsigned int i=0; i < table->getrowcount() ; i ++)
        {table->lastid++;
         if (table->lastid > DT_ROWID_MAX) table->lastid = DT_ROWID_MIN;
         table->rows[i]->id=table->lastid;
         fwriterow(i);}

      close();
      return 0;
    }


