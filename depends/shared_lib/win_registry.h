#ifndef WIN_REGISTRY_H
#define WIN_REGISTRY_H

std::string RegQuerySZ(HKEY & hKey , const char * name);
DWORD RegQueryDW(HKEY & hKey , const char * name , DWORD def = 0);
long RegSetSZ(HKEY & hKey , const char * name , std::string val);
long RegSetDW(HKEY & hKey , const char * name , DWORD val);

#endif
 