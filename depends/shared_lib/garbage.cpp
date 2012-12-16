#include <windows.h>
#include <wininet.h>
#include "garbage.h"

#pragma comment(lib , "Wininet.lib");
//#pragma hdrstop
typedef std::vector <cGarbage *>::iterator items_it_t;

cGarbage * cGarbage::find(void * pointer,char collection) {
  if (collection!=0xFF && (isCollection() != (!collection?false:true))) return 0;
  return (_pointer==pointer)?this:0;
}

size_t cGarbage::count(char collection) {
    return collection==-1?1 : (collection==isCollection()?1:0);
}


cGarbageCollection * cGarbage::findCollection(void * pointer) {return (cGarbageCollection*) find(pointer,true);}

void cGarbage::dispose() {_pointer=0;}

bool cGarbage::isCollection() {
  return false;
}

cGarbage::cGarbage(void * pointer) {
  _pointer=pointer;
}

cGarbage::~cGarbage() {}

// ------------------------


// ------------------------
cGarbageItem * cGarbageCollection::add(cGarbageItem * item) {
    if (!item) return 0;
/*    if (find(item->_pointer,false)) {
        delete item;
        return (cGarbageItem*)find(item->_pointer,false);
    }
*/
    _items.push_back(item);
    return item;
}
cGarbageCollection * cGarbageCollection::addCollection(void * pointer){ // dodaje now¹ kolekcjê
  if (find(pointer,true)) 
	  return (cGarbageCollection*)find(pointer,true);
  cGarbageCollection * gc = new cGarbageCollection(pointer);
  _items.push_back(gc);
  return gc;
}

bool cGarbageCollection::remove(void * pointer) {
 for (items_it_t items_it = _items.begin(); items_it!=_items.end(); items_it++){
   if ((*items_it)->isCollection()) {
	   if (((cGarbageCollection*)*items_it)->remove(pointer)) 
		   return true;
   }
   else if ((*items_it)->_pointer == pointer) {
	   delete *items_it; 
	   _items.erase(items_it);
	   return true;
   }
 }
 return false;
}

cGarbage* cGarbageCollection::find(void * pointer,char collection) {
    // Sprawdza czy aktualny element jest tym szukanym...
    cGarbage * g = cGarbage::find(pointer , collection);
    if (g) return g;
    for (items_it_t items_it = _items.begin(); items_it!=_items.end(); items_it++){
        if ((g=(*items_it)->cGarbage::find(pointer,collection))) 
			return g;
    }
    return 0;
}

size_t cGarbageCollection::count(char collection) {
    size_t sz = cGarbage::count(collection);
    for (items_it_t items_it = _items.begin(); items_it!=_items.end(); items_it++){
        sz+=(*items_it)->count(collection);
    }
    return sz;
}


void cGarbageCollection::dispose() {
 for (items_it_t items_it = _items.begin(); items_it!=_items.end(); items_it++){
   (*items_it)->dispose();
   delete *items_it;
 }
 _items.clear();
 return;
}

void cGarbageCollection::dispose(cGarbage * item) {
 item->dispose();
 for (items_it_t items_it = _items.begin(); items_it!=_items.end(); items_it++){
   if (*items_it == item) {
     _items.erase(items_it);
     return;
   }
 }

}

bool cGarbageCollection::isCollection() {return true;}


void cGarbageCollection::dispose(void *  pointer) {
  cGarbage * g = find(pointer , -1);
  if (!g) return;
  dispose(g);
}

size_t cGarbageCollection::size() {
  return _items.size();
}


cGarbageCollection::cGarbageCollection(void * pointer):cGarbage(pointer) {
}
cGarbageCollection::~cGarbageCollection() {
    dispose();
}


// ------------------------------

void cGarbage_DeleteObject::dispose() {DeleteObject(_pointer);_pointer=0;}

void cGarbage_Internet::dispose() {
    InternetCloseHandle((HINTERNET)_pointer);_pointer=0;
}
