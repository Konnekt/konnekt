/*
 *  SIMXML (SIMple XML parser)
 *  (c)Copyright 2002-2003  Rafa≥ Lindemann | Stamina
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


#include <windows.h>
#define BUILD_SIMXML_DLL 1
#include "simxml.h"
#include <stdstring.h>
#include <stdlib.h>
#include <stack>
#pragma link "preg.obj"


cXML::cXML() {
	pregPath = false;
}

cXML::sPos::sPos() {
	  end = end_end = start = start_end = -1;
}
cXML::~cXML() {
}

bool cXML::loadFile(const char *  fileName) {
    string _src="";
    FILE * f = fopen (fileName , "rt");
    if (!f) return false;
    char temp [251];
    size_t r;
    while ((r = fread(temp , 1 , 250 , f))) {
      temp[r]=0;
      _src+=temp;
    }
    fclose(f);
    return loadSource(_src.c_str());
}

bool cXML::loadSource(const char *  XML) {
	pos = sPos();
	Preg.setSubject(XML);
	return *XML != 0;
}

string cXML::getSource() {
	  return Preg.getSubject();
}

string cXML::getContent(const char * path){
    if (path) prepareNode(path , true);
	if (pos.start_end == -1 || pos.end==-1) return "";
	return Preg.getSubject(pos.start_end , pos.end);
}
string cXML::getNode(const char * path){
    if (path) prepareNode(path , true);
	if (pos.start == -1 || pos.end_end==-1) return "";
	return Preg.getSubject(pos.start , pos.end_end);
}

string __stdcall EncodeCallback(class cPreg * p , void * param) {
	char v [7] = "&#\0\0\0\0";
	itoa(*p->getByVector(0) , v+2 , 10);
	v[strlen(v)]=';';
	return v;
}
string __stdcall DecodeCallback(class cPreg * p , void * param) {
	string fnd = (*p)[1];
	if (fnd=="amp") return "&";
	else if (fnd=="lt") return "<";
	else if (fnd=="gt") return ">";
	else if (fnd=="quot") return "\"";
	else if (fnd=="apos") return "'";
	char v [2] = " ";
	v[0] = char(strtol(fnd.c_str() , 0 , *(p->getByVector(0)+2) == 'x'?16:10));
	return v;
}

string EncodeEntities(string txt) {
	cPreg pr(0);
	return pr.replace("/[^ \\t\\r\\na-z0-9\\!\\@\\#\\$\\%\\*\\(\\)\\-_=+\\.\\,\\;':\\\\[\\]\\{\\}πÊÍ≥ÒÛúüø•∆ £—”åèØ]/i" , EncodeCallback , txt.c_str());
}
string DecodeEntities(string txt) {
	cPreg pr(0);
	return pr.replace("/&#?x?([0-9a-z]+);/i" , DecodeCallback , txt.c_str());
}

string cXML::getText(const char * path){
    if (path) prepareNode(path , true);
	if (pos.start_end == -1 || pos.end==-1) return "";
	string node = Preg.getSubject(pos.start_end , pos.end);
    cPreg pr(0);
    pr.match("/[ \n]*<![CDATA[(.*?)]]>/m",node.c_str());
    CStdString str;
    if (pr.matched) {str = Preg[1];}
    else str = pr.replace("/<.+>/m","",node.c_str());
//    str = pr.replace("");
    str = DecodeEntities(str);
    return str;
}

bool cXML::nodeWalk(cXML::nwInfo & nfo) {
	if (nfo.state == nfo.closed || nfo.state == nfo.closing) {
		// Obniøamy "element"
		size_t top = nfo.path.find_last_of("/");
		if (top == -1) nfo.path = "";
		else nfo.path.erase(top);
	}
	// 1 - <(/)...   2 - (nazwa)  3 - ...(/)>
	Preg.setPattern("#<(?!\\?|\\!)[ \\t]*(/?)([^>/ \\t\\n\\r]+)(?=\\t| |\\n|>|\\r|/).*?(/?)>#is");
	Preg.match();
	if (!Preg.matched) {
		nfo.state = nfo.undefined;
		return false;
	}
	nfo.start = Preg.getVector(0);
	nfo.end = Preg.getVector(1);
	Preg.start = nfo.end; // Przesuwamy siÍ...
	if (Preg[1]=="/" && nfo.path.substr(nfo.path.find_last_of("/")+1) == Preg[2]) {//Zamykamy wczytany wczeúniej element
		nfo.state = nfo.closing;
	} else {
		if (!nfo.path.empty()) nfo.path+='/';
		nfo.path+=Preg[2];
		if (Preg[3]=="/") nfo.state = nfo.closed;
		else nfo.state = nfo.opened;
	}
	pos.end = Preg.getVector(0);
	pos.end_end = Preg.getVector(1);
	return true;
}

bool cXML::prepareNode(string path , bool full){
	if (path.empty()) return false;
	size_t lastStart = Preg.start;
	nwInfo nfo;
	pos = sPos();
	while (nodeWalk(nfo)) { // dopÛki wczytujemy kolejne elementy
		bool ok;
		if (path[0]=='*')
			ok = (nfo.path.size() >= path.size()-1) 
				&& (nfo.path.substr(nfo.path.size() - path.size() + 1) == path.c_str()+1);
		else ok = (nfo.path == path);
		if (ok && nfo.state != nfo.closing) {
			pos.start = nfo.start;
			pos.start_end = nfo.end;
			if (nfo.state == nfo.closed) {
				pos.end = pos.end_end = nfo.end;
			}
			break;
		}
	}
	if (full && nfo.state != nfo.closed && nfo.state != nfo.undefined) {
		// Szukamy do koÒca
		string toClose = nfo.path;
		while (nodeWalk(nfo)) {
			if (nfo.path == toClose && nfo.state == nfo.closing) {
				pos.end = nfo.start;
				pos.end_end = nfo.end;
				break;
			}
		}
	}
	if (nfo.state == nfo.undefined) {
		pos = sPos();
	}
	Preg.start = lastStart;
	return nfo.state != nfo.undefined;

/*	
	string start("") , end("");
    size_t fpos = 0 , bpos = 0;
    do {
      fpos = path.find('/' , bpos);   // Wyluskujemy kolejne podkatalogi
      string found = path.substr(bpos , fpos==path.npos?path.npos : fpos - bpos);
      if (found == "") continue;
      if (start != "") start+="[^>]*>.*";  // Jeøeli juø coú znaleüliúmy, rozgraniczamy pola poprzez .*
      if (fpos == -1) // ostatni element
		  start += "(<) *("+found+")";
	  else
		  start += "< *"+found;
      bpos = fpos + 1;
    } while (fpos != path.npos);
	// Znajdujemy pierwszy element...
    Preg.setPattern("/" + start + "[^>]*?(\\/?)>/isU");
    Preg.match();
    if (Preg.matched) {
		pos.start = Preg.getVector(2); // poczπtek pierwszego "(<)"
		pos.start_end = Preg.getVector(1); // koniec ca≥oúci
	}
    else {
		pos = sPos(); // resetujemy pozycje...
		return false;
	}
	if (Preg[3]=="/") { // Element zamkniÍty przez />
		pos.end = pos.start_end;
		pos.end_end = pos.start_end;
		return true;
	}
	// parsujemy aø znajdziemy koniec
	if (full) {
	} else {
		pos.end = pos.end_end = -1;
	}
*/
}

string cXML::getAttrib(const char * attrib , const char * attribs){
	if (!attribs && pos.start==-1) return "";
    cPreg PregA(false);
    PregA.setPattern("/[ \\t]"+string(attrib)+"[ \\t]*=[ \\t]*([\"'])(.*?)\\1/is");
	PregA.setSubject(attribs ? attribs : Preg.getSubject(pos.start , pos.start_end));
    PregA.match();
	return DecodeEntities(PregA[2]);
}
string cXML::getAttribs(const char * path){
    if (path) prepareNode(path , true);
	if (pos.start == -1 || pos.start_end==-1) return "";
	if (pos.start==-1) return "";
	return Preg.getSubject(pos.start , pos.start_end);
}


void cXML::next() {
	if (pos.start==-1) return;
	Preg.start = pos.end_end!=-1 ? pos.end_end  :  pos.start_end; 
}
void cXML::restart() {Preg.reset();} 






