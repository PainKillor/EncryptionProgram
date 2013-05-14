#ifndef Brian_H
#define Brian_H

#include "Cipher.h"

class Brian : public Cipher
{
public :
	Brian();
	Brian(char key[]);
	void encrypt (char data[]);
	void decrypt (char data[]);
};

#endif