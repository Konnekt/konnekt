#ifndef CRYPTFUNC_H
#define CRYPTFUNC_H


void sc_encrypt(unsigned char * key , unsigned char * data , unsigned int size = 0);
void sc_decrypt(unsigned char * key , unsigned char * data , unsigned int size);

char * sc_key(unsigned char * key , int type) {
  if (!type) return (char*) key;
  char * bck = (char*)key;
  while (*key) {
    switch (type) {
      case 1:
        if (*key < 32) *key = 32;
        if (*key > 127) *key = 127;
        *key+=125;
        break;
      case 2:
        *key-=31;
        break;
    }
//    *key-=58;
//    if (!*key) *key = 53;
    key++;
  }
  return bck;
}

void sc_encrypt(unsigned char * key , unsigned char * data , unsigned int size) {
  unsigned int ki=0;
  if (!size) size = strlen((char *)data);
  unsigned int ksize = strlen((char *)key);
  int j = 0;
  for (unsigned int p=0;p<size;p++) {
    *data = (*data ^ key[ki]) + (unsigned char)((j) &  0xFF);// | (j*2);
//    *data = *data;
    data++;
    ki++;
    if (ki>=ksize) ki=0;
    j++;
  }
}



void sc_decrypt(unsigned char * key , unsigned char * data , unsigned int size) {
  unsigned int ki=0;
  unsigned int ksize = strlen((char *)key);

  int j = 0;
  for (unsigned int p=0;p<size;p++) {
    *data = (*data - (unsigned char)((j) & 0xFF))  ^ key[ki];// | (j*2);
    data++;
    ki++;
    if (ki>=ksize) ki=0;
    j++;
  }

}


#endif
 