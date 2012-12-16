#ifndef GARBAGE_H
#define GARBAGE_H

#include <vector>


/*
  Kolektor œmieci. Czyli ogromne u³atwienie przy pos³ugiwaniu siê
  czymkolwiek co trzeba zwalniaæ, gdy istnieje ryzyko, ¿e siê tego
  nie zrobi...

  GC dzia³a na zasadzie drzewa sk³adaj¹cego siê ze œmieci i ich
  kolektorów - czyli katalogów ze œmieciami...
  Najlepiej gdy w jednym katalogu s¹ "œmieci" tylko jednego typu,
  nie wyst¹pi¹ wtedy problemy z powtarzaj¹cymi siê wskaŸnikami...

  Dane rozpoznawane s¹ po wskaŸnikach, podobnie kolektory.
  GC mo¿e pracowaæ wielow¹tkowo...
  Np.:
  cGarbageCollection main;
  cGarbageCollection * foo = main.addCollection("FOO");
  foo->add(new cGarbage_Free(strdup("bar")));
  main.dispose("FOO");
*/



class cGarbage {
  friend class cGarbageCollection;
  public:
  virtual cGarbage * find(void * pointer,char collection);
  cGarbageCollection * findCollection(void * pointer);
  virtual bool isCollection();
  cGarbage(void * pointer=0);
  virtual ~cGarbage();
  void * _pointer;
  virtual size_t count(char collection = 0);
  private:
  virtual void dispose(); 

};

class cGarbageItem: public cGarbage {
  public:
  cGarbageItem(void * pointer=0):cGarbage(pointer) {}
  private:
};

class cGarbageCollection: public cGarbage {
  public:
  cGarbageItem * add(cGarbageItem * item);   // dodaje element do kolekcji
  cGarbageCollection * addCollection(void * pointer=0); // dodaje now¹ kolekcjê
  cGarbage * find(void * pointer,char collection);
  bool remove(void * pointer); // usuwa element o podanym wskazniku (nie niszczy!)
  void dispose(cGarbage * item);
  void dispose(void * pointer);
  bool isCollection();
  cGarbageCollection(void * pointer=0);
  ~cGarbageCollection();
  size_t size();
  size_t count(char collection = 0);
  private:
  std::vector <cGarbage *> _items;
  void dispose();
};

// -------------------------
/*
class cGarbage_Free: public cGarbageItem {
  public:
  private:
  void dispose() {free(_pointer);_pointer=0;}
  cGarbage_Free(void * pointer):cGarbageItem(_pointer) {}
};

class cGarbage_Delete: public cGarbageItem {
  public:
  cGarbage_Delete(void * pointer):cGarbageItem(_pointer) {}
  private:
  void dispose() {delete _pointer;_pointer=0;}
};

class cGarbage_DeleteArray: public cGarbageItem {
  public:
  cGarbage_DeleteArray(void * pointer):cGarbageItem(_pointer) {}
  private:
  void dispose() {delete [] _pointer;_pointer=0;}
};
*/
class cGarbage_DeleteObject: public cGarbageItem {
  public:
  cGarbage_DeleteObject(void * pointer):cGarbageItem(pointer) {}
  private:
  virtual void dispose();
};


  class cGarbage_DestroyIcon: public cGarbageItem {
  public:
  cGarbage_DestroyIcon(void * pointer)
	  :cGarbageItem(pointer) {}
  private:
  void dispose() {
	  DestroyIcon((HICON)_pointer);_pointer=0;
  }
};

#ifdef _WININET_
    class cGarbage_Internet: public cGarbageItem {
    public:
    cGarbage_Internet(HINTERNET pointer):cGarbageItem((void *)pointer) {}
    private:
    void dispose();
    };
#endif

#endif
