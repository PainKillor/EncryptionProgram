#ifndef RYAN_H
#define RYAN_H

#include "Cipher.h"

class Ryan : public Cipher
{
public :
	Ryan();
	Ryan(char key[]);
	void encrypt (char data[]);
	void decrypt (char data[]);
};

#endif