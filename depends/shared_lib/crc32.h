#pragma once

#ifndef __CRC32_H__
#define __CRC32_H__

/*

Based on code from
http://www.createwindow.com/programming/crc32/

*/

class CRC32 {
private:
	unsigned long lookup [256];
	unsigned long state;

	void BuildLookup();  // Builds lookup table array 
    unsigned long Reflect(unsigned long ref, char ch);  // Reflects CRC bits in the lookup table 

public:
	CRC32();
	unsigned long Calculate(const char * text);  // Creates a CRC from a text string 
    unsigned long Add(const char * buffer , unsigned int size);  // Continues a CRC from a buffer 
	unsigned long GetState();
	void Reset();


};


#endif