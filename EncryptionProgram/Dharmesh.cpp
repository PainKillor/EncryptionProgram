#include "Dharmesh.h"

Dharmesh::Dharmesh()
{
	strcpy(key, "");
}

Dharmesh::Dharmesh(char key[])
{
	strcpy(this->key, key);
}

void Dharmesh::encrypt(char data[]) {
	//modify the key itself to make encryption/decryption harder if key is compromised
	char keymod[BLOCK_SIZE];
	for (int i=0; i < BLOCK_SIZE; i++){
		keymod[i] = key[i]*key[i];
	}
	//reverse the data
	char newdata[BLOCK_SIZE];
	for (int j=0; j < BLOCK_SIZE; j++) {
		newdata[j] = data[BLOCK_SIZE-1-j];
	}
	//get the reverse data into the data array
	for (int k=0; k < BLOCK_SIZE; k++) {
		data[k] = newdata[k];
	}
	//encrypt the data
	for (int p=0; p < BLOCK_SIZE; p++) {
		data[p] = data[p] + keymod[p];
	}
}

void Dharmesh::decrypt(char data[]) {
	//modify the key itself to make encryption/decryption harder if key is compromised
	char keymod[BLOCK_SIZE];
	for (int i=0; i < BLOCK_SIZE; i++){
		keymod[i] = key[i]*key[i];
	}
	//decrypt the data
	for (int p=0; p < BLOCK_SIZE; p++) {
		data[p] = data[p] - keymod[p];
	}
	//reverse the data into a temp array
	char newdata[BLOCK_SIZE];
	for (int j=0; j < BLOCK_SIZE; j++) {
		newdata[j] = data[BLOCK_SIZE-1-j];
	}
	//get the reverse data into the data array
	for (int k=0; k < BLOCK_SIZE; k++) {
		data[k] = newdata[k];
	}
}