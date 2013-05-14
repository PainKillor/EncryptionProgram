#ifndef SUNNY_H
#define SUNNY_H

#include "Cipher.h"

class Sunny : public Cipher
{
public :
	Sunny();
	Sunny(char key[]);
	void encrypt (char data[]);
	void decrypt (char data[]);
};

#endif