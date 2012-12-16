#include "crc32.h"
#include <string.h>

CRC32::CRC32() {
	BuildLookup();
	Reset();
}
void CRC32::BuildLookup() {
	// This is the official polynomial used by CRC-32 
	// in PKZip, WinZip and Ethernet. 
	unsigned long ulPolynomial = 0x04c11db7; 

	// 256 values representing ASCII character codes. 
	for(int i = 0; i <= 0xFF; i++) 
	{ 
		lookup[i]=Reflect(i, 8) << 24; 
		for (int j = 0; j < 8; j++) 
				lookup[i] = (lookup[i] << 1) ^ (lookup[i] & (1 << 31) ? ulPolynomial : 0); 
		lookup[i] = Reflect(lookup[i], 32); 
	} 
}
unsigned long CRC32::Reflect(unsigned long ref, char ch) {
    unsigned long value(0); 

    // Swap bit 0 for bit 7 
    // bit 1 for bit 6, etc. 
    for(int i = 1; i < (ch + 1); i++) 
    { 
        if(ref & 1) 
                value |= 1 << (ch - i); 
        ref >>= 1; 
    } 
    return value; 
}

unsigned long CRC32::Calculate(const char * text) {
	Reset();
	return Add(text , strlen(text));
}
unsigned long CRC32::Add(const char * buffer , unsigned int size) {
    while(size--) 
        state = (state >> 8) ^ lookup[(state & 0xFF) ^ *buffer++]; 
    // Exclusive OR the result with the beginning value. 
	return GetState();
}
void CRC32::Reset() {
	state = 0xffffffff;
}
unsigned long CRC32::GetState() {
    return state ^ 0xffffffff; 
}


