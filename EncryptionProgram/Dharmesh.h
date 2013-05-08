#ifndef DHARMESH_H
#define DHARMESH_H

#include "Cipher.h"

class Dharmesh : public Cipher
{
public :
	Dharmesh();
	Dharmesh(char key[]);
	void encrypt (char data[]);
	void decrypt (char data[]);
};

#endif