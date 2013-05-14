#include "Sunny.h"

Sunny::Sunny()
{
	strcpy(key, "");
}

Sunny::Sunny(char key[])
{
	strcpy(this->key, key);
}

void Sunny::encrypt (char data[])
{	
// Add the value of every index in the key to every index in the data array 
   for (int i = 0; i < BLOCK_SIZE; i++) 
   { 
   	   data[i] = data[i] + key[i]; 
   } 
   for (int i = 0; i < BLOCK_SIZE; i++) 
   { 
   	 data[i] = data[i] + 5; 
	} 
}

void Sunny::decrypt (char data[])
{
  for (int i = 0; i < BLOCK_SIZE; i++) 
  { 
  	  data[i] = data[i] - 5; 
  } 
// Subtract the values of the key from the values of the data array 
  for (int i = 0; i < BLOCK_SIZE; i++) 
  { 
  	  data[i] = data[i] - key[i]; 
  } 	
}