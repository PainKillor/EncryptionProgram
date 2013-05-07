#include "Cipher.h"

Cipher::Cipher( )
{
	strcpy(key,"");
}
Cipher::Cipher(char Key[])
{
	strcpy(key,Key);
}
Cipher::Cipher(const Cipher &cipher)
{
	strcpy(key,cipher.key);
}
void Cipher::setKey(char Key[])
{
	strcpy(key,Key);
}
const char* Cipher::getKey()
{
	return key;
}