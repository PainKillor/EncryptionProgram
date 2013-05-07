#ifndef CIPHER_H
#define CIPHER_H

#include <cstring>

#define _BLOCK_SIZE 16

class Cipher
{
protected: 
	char key[_BLOCK_SIZE];

public: 
	const static int BLOCK_SIZE = _BLOCK_SIZE;

	Cipher();
	Cipher(char Key[]);
	Cipher(const Cipher &cipher);
	void setKey(char Key[]);
	const char* getKey();
	virtual void encrypt(char data[]) = 0;
	virtual void decrypt(char data[]) = 0;
};

#endif