/*
 *  SIMXML (SIMple XML parser)
 *  (c)Copyright 2002-2003  Rafa³ Lindemann | Stamina
 *  
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

#ifndef SIMXML_H
#define SIMXML_H

#include <string>
#include "preg.h"

//#define EXPORT __export

using namespace std;
class cXML {
    private:
	cPreg Preg;
	// Pozycje aktualnego elementu

    public:
    cXML();
    ~cXML();
    bool loadFile(const char * fileName);
    bool loadSource(const char * XML);
    string getSource();

    string getContent(const char * path=0); ///<Zwraca cala zawartosc podanej sciezki
    string getNode(const char * path=0); ///<Zwraca cala zawartosc podanej sciezki, razem z elementem
    string getText(const char * path=0); ///<Zwraca zawartosc tekstowa z podanej sciezki

    bool prepareNode(string path , bool full=false); ///<Przygotowuje element do czytania
								/// full - czy ma znaleŸæ równie¿ koniec elementu
    string getAttrib(const char * attrib , const char * attribs = 0); ///<Zwraca wartosc atrybutu w attribs, lub w aktualnie wybranym elemencie...
	string getAttribs(const char * path=0); ///< Zwraca atrybuty dla elementu

    void next();   ///< Nastepne wywolanie zostanie uruchomione z miejsca ostatniego trafienia
    void restart(); ///< Od poczatku

	struct nwInfo {
		string path;
		size_t start;
		size_t end;
		enum eState {
			undefined=0,
			opened=1,
			closed=2, // <a/>
			closing=3 // </a>
		} state;
		nwInfo():state(undefined),start(0),end(0){}
	};
	bool nodeWalk(nwInfo & nfo);

	// Pozycje "obs³ugiwanego" elementu...
	// Najczêœciej u¿ywane wewnêtrznie przez funkcje, ale
	// przydaje siê, ¿eby zapamiêtaæ go na moment podczas odczytywania
	// atrybutów z elementu...
	struct sPos {
		size_t start;     // pocz¹tek
		size_t start_end; // Koniec otwieraj¹cego elementu
		size_t end;     // zamykaj¹cy element
		size_t end_end; // koniec zamykaj¹cego elementu
		sPos();
	};
	sPos pos;
	bool pregPath;  // Czy szukane œcie¿ki do regExp?
	
};


string EncodeEntities(string txt);
string DecodeEntities(string txt);
string __stdcall DecodeCallback(class cPreg * p , void * param);
string __stdcall EncodeCallback(class cPreg * p , void * param);


#endif
 