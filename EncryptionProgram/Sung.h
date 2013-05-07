#ifndef SUNG_H
#define SUNG_H

#include "Cipher.h"

class Sung : public Cipher
{
public :
	Sung();
	Sung(char key[]);
	void encrypt (char data[]);
	void decrypt (char data[]);
};

#endif