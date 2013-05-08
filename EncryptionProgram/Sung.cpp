#include "Sung.h"

Sung::Sung()
{
	strcpy(key, "");
}

Sung::Sung(char key[])
{
	strcpy(this->key, key);
}

void Sung::encrypt (char data[])
{	
	for (int i = 0,j=(BLOCK_SIZE-1); i < BLOCK_SIZE; i++, j--) 
	{
		data[i] = (data[i]+'~') + key[j];
	}
	
}

void Sung::decrypt (char data[])
{
	for (int i = 0,j=(BLOCK_SIZE-1); i < BLOCK_SIZE; i++, j--) 
	{
		data[i] = (data[i]-('~')) - key[j];
	}
			
}