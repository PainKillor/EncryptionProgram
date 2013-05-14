#include "Brian.h"

Brian::Brian()
{
	strcpy(key, "");
}

Brian::Brian(char key[])
{
	strcpy(this->key, key);
}

void Brian::encrypt(char data[]) {
    //Add modified version of key
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        data[i] = data[i] + (key[i] * (i + 10));
    }
    //Use algorithm to scramble placement
    char dcopy[BLOCK_SIZE];
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
	    if (i < 8)
        {
            dcopy[i] = data[i*2];
        }
        else
        {
            dcopy[i] = data[2*(i-8)+1];
        }
	}
	//Subtract by another modified key
	for (int i = 0; i < BLOCK_SIZE; i++)
	{
	    data[i] = dcopy[i] - key[i*2];
	}
}


void Brian::decrypt(char data[])
{
    //Add second modified key
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        data[i] = data[i] + key[i*2];
    }
    //Reverse the scramble algorithm
    char dcopy[BLOCK_SIZE];
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        if (i % 2)
        {
            dcopy[i] = data[(i-1)/2 + 8];
        }
        else
        {
            dcopy[i] = data[i/2];
        }
    }
    //Subtract the modified key
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        data[i] = dcopy[i] - (key[i] * (i + 10));
    }
}