#include "Cipher.h"

Cipher::Cipher( )
{
	strcpy(key,"");
}
Cipher::Cipher(char Key[])
{
	setKey(Key);
}
Cipher::Cipher(const Cipher &cipher)
{
	strcpy(key,cipher.key);
}
void Cipher::setKey(char Key[])
{
	for (int i = 0; i < BLOCK_SIZE; i++)
		key[i] = Key[i];
}
const char* Cipher::getKey()
{
	return key;
}