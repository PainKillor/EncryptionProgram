#ifndef Brian_H
#define Brian_H

#include "Cipher.h"

class Brian : public Cipher
{
private:
	const static int BASENUM = 7;

public :
	Brian();
	Brian(char key[]);
	void encrypt (char data[]);
	void decrypt (char data[]);
};

#endif