#include "Brian.h"

#include <cmath>

Brian::Brian() {
	for (int i = 0; i < Cipher::BLOCK_SIZE; i++)
		key[i] = '\0';
}

Brian::Brian(char key[]) {
	for (int i = 0; i < Cipher::BLOCK_SIZE; i++)
		this->key[i] = key[i];
}

void Brian::encrypt(char data[]) {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		data[i] = data[i] + key[i];
	}

	char buff[BLOCK_SIZE / 4][BLOCK_SIZE / 4];
	for (int i = 0; i < BLOCK_SIZE; i++)
		buff[i / 4][i % 4] = data[i];

	for (int i = 0; i < BLOCK_SIZE / 4; i++) {
		for (int o = 0; o < i; o++) {
			char first = buff[i][0];
			for (int p = 0; p < (BLOCK_SIZE / 4) - 1; p++) {
				buff[i][p] = buff[i][p + 1];
			}
			buff[i][(BLOCK_SIZE / 4) - 1] = first;
		}
	}

	for (int i = 0; i < BLOCK_SIZE / 4; i++) {
		for (int o = 0; o < i; o++) {
			char first = buff[0][i];
			for (int p = 0; p < (BLOCK_SIZE / 4) - 1; p++) {
				buff[p][i] = buff[p + 1][i];
			}
			buff[(BLOCK_SIZE / 4) - 1][i] = first;
		}
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
		data[i] = buff[i / 4][i % 4];

	for (int i = 0; i < BLOCK_SIZE; i++)
		data[i] = data[i] + pow((double) BASENUM, (double) (i % (BLOCK_SIZE / 4)));
}

void Brian::decrypt(char data[])
{
	for (int i = 0; i < BLOCK_SIZE; i++)
		data[i] = data[i] - pow((double) BASENUM, (double) (i % (BLOCK_SIZE / 4)));

	char buff[BLOCK_SIZE / 4][BLOCK_SIZE / 4];
	for (int i = 0; i < BLOCK_SIZE; i++)
		buff[i / 4][i % 4] = data[i];

	for (int i = (BLOCK_SIZE / 4) - 1; i >= 0; i--) {
		for (int o = 0; o < i; o++) {
			char last = buff[(BLOCK_SIZE / 4) - 1][i];
			for (int p = (BLOCK_SIZE / 4) - 1; p >= 0; p--) {
				buff[p][i] = buff[p - 1][i];
			}
			buff[0][i] = last;
		}
	}

	for (int i = (BLOCK_SIZE / 4) - 1; i >= 0; i--) {
		for (int o = 0; o < i; o++) {
			char last = buff[i][(BLOCK_SIZE / 4) - 1];
			for (int p = (BLOCK_SIZE / 4) - 1; p >= 0; p--) {
				buff[i][p] = buff[i][p - 1];
			}
			buff[i][0] = last;
		}
	}

	for (int i = 0; i < BLOCK_SIZE; i++)
		data[i] = buff[i / 4][i % 4];

	for (int i = 0; i < BLOCK_SIZE; i++) {
		data[i] = data[i] - key[i];
	}
}