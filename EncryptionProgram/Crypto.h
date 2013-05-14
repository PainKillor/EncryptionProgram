#ifndef CRYPTO_H
#define CRYPTO_H

#include "Cipher.h"

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

using namespace std;

#define _BLOCK_SIZE 16

//class Crypto : public Cipher {
class Crypto   {
	private:
		Cipher *cipher;
		std::vector<char> inData;
		std::vector<char> outData;

	public:
		Crypto(const Crypto &crypto);
		Crypto(Cipher *cipher = NULL, std::vector<char> inData = std::vector<char>());
		~Crypto();

		void setCipher(Cipher *cipher);
		Cipher * getCipher();
		void loadInputFile(std::string inFilePath) throw(int);
		void saveOutputFile(std::string outFilePath) throw(int);
		void clearInput();
		void clearOutput();
		void setInputData(std::vector<char> data);
		const std::vector<char> * getInputData();
		const std::vector<char> * getOutputData();
		void encrypt();
		void decrypt() throw (int);
};

#endif