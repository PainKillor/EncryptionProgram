#include "Crypto.h"

Crypto::Crypto(Cipher *cipher, std::vector<char> inData) {
	// Default/Parametized constructor
this->inData = inData;
this->cipher = cipher;
}

Crypto::~Crypto() {
	// Destructor, should delete cipher
	// Brian, since I couldn't get memory allocated, I cannot delete it either
	delete cipher;// delete the pointer
}

void Crypto::setCipher(Cipher *cipher) {
	// delete this->cipher
	// Set this->cipher = cipher
	delete this->cipher;
	this->cipher = cipher;
}

Cipher * Crypto::getCipher() {
	// Return reference to cipher
return this->cipher;
//	return 0;
}

void Crypto::loadInputFile(std::string inFilePath) throw (int)  {
	// Load the input file into inData
	// If there's an error loading throw -1
	std::ifstream inputFile;
	inputFile.open(inFilePath);
	if (inputFile)
	{

		char ch;
		while((ch = inputFile.get()) != EOF)
		{
			inData.push_back(ch);
		}
		inputFile.close();
	}
	else
	{
		throw (-1);
	}
}

void Crypto::saveOutputFile(std::string outFilePath) throw (int) {
	// Save outData to a file at outFilePath
	// If there's an error saving throw -1
	std::ofstream outputFile;
	outputFile.open(outFilePath);
	if (outputFile)
	{
		for (int i=0; i < outData.size(); i++) 
		{
			outputFile.put(outData[i]);
		}
		outputFile.close();
	}
	else
	{
		throw (-1);
	}
}

void Crypto::clearInput() {
	// Clear the inData vector
	inData.clear();
}

void Crypto::clearOutput() {
	// Clear the outData vector
	outData.clear();
}

void Crypto::setInputData(std::vector<char> data) {
	// Set inData to data
	inData = data;
}

const std::vector<char> * Crypto::getInputData() {
	// Return a reference to inData
	return &inData;
}

const std::vector<char> * Crypto::getOutputData() {
	// Return a reference to outData
	return &outData;
}

void Crypto::encrypt() {
	for (int i = 0; i < (inData.size() % Cipher::BLOCK_SIZE); i++)
		inData.push_back('\0');

	outData.resize(inData.size());

	for (int i = 0; i < inData.size(); i += Cipher::BLOCK_SIZE) {
		char buff[Cipher::BLOCK_SIZE];
		for (int o = 0; o < Cipher::BLOCK_SIZE; o++)
			buff[o] = inData[i + o];

		cipher->encrypt(buff);

		for (int o = 0; o < Cipher::BLOCK_SIZE; o++)
			outData[i + o] = buff[o];
	}
}

void Crypto::decrypt() throw (int) {
	if ((inData.size() % Cipher::BLOCK_SIZE) != 0)
		throw 0;

	outData.resize(inData.size());

	for (int i = 0; i < inData.size(); i += Cipher::BLOCK_SIZE) {
	char buff[Cipher::BLOCK_SIZE];
	for (int o = 0; o < Cipher::BLOCK_SIZE; o++)
		buff[o] = inData[i + o];

	cipher->decrypt(buff);

	for (int o = 0; o < Cipher::BLOCK_SIZE; o++)
		outData[i + o] = buff[o];
	}

	if (outData[outData.size() - 1] == '\0') {
		for (int i = outData.size() - 2; i > 0; i--) {
			if (outData[i] != '\0') {
				outData.resize(i + 1);
				break;
			}
		}
	}
}