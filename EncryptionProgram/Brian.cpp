#include "Brian.h"

Brian::Brian() {
	for (int i = 0; i < Cipher::BLOCK_SIZE; i++)
		key[i] = '\0';
}

Brian::Brian(char key[]) {
	for (int i = 0; i < Cipher::BLOCK_SIZE; i++)
		this->key[i] = key[i];
}

void Brian::encrypt(char data[]) {
	// Add the value of every index in the key to every index in the data array
	for (int i = 0; i < BLOCK_SIZE; i++) {
		data[i] = data[i] + key[i];
	}

	// Shift every member in the array left by 1
	// So 0123456789012345 -> 1234567890123450
	char first = data[0];
	for (int i = 0; i < BLOCK_SIZE - 1; i++) {
		data[i] = data[i + 1];
	}
	data[BLOCK_SIZE - 1] = first;
}

void Brian::decrypt(char data[])
{
	// Shift every member in the array right by 1
	// So 1234567890123450 -> 0123456789012345
	char last = data[BLOCK_SIZE - 1];
	for (int i = BLOCK_SIZE - 1; i >= 0; i--) {
		data[i] = data[i - 1];
	}
	data[0] = last;

	// Subtract the values of they key from the values of the data array
	for (int i = 0; i < BLOCK_SIZE; i++) {
		data[i] = data[i] - key[i];
	}
}