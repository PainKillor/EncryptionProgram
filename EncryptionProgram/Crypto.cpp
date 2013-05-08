#include "Crypto.h"

Crypto::Crypto(const Crypto &crypto) {
	// Copy constructor
}

Crypto::Crypto(Cipher *cipher, std::vector<char> inData) {
	// Default/Parametized constructor
}

Crypto::~Crypto() {
	// Destructor, should delete cipher
}

void Crypto::setCipher(Cipher *cipher) {
	// delete this->cipher
	// Set this->cipher = cipher
}

const Cipher * Crypto::getCipher() {
	// Return reference to cipher

	return 0;
}

void Crypto::loadInputFile(std::string inFilePath) throw (int) {
	// Load the input file into inData
	// If there's an error loading throw -1
}

void Crypto::saveOutputFile(std::string outFilePath) throw (int) {
	// Save outData to a file at outFilePath
	// If there's an error saving throw -1
}

void Crypto::clearInput() {
	// Clear the inData vector
}

void Crypto::clearOutput() {
	// Clear the outData vector
}

void Crypto::setInputData(std::vector<char> data) {
	// Set inData to data
}

const std::vector<char> * Crypto::getInputData() {
	// Return a reference to inData

	return 0;
}

const std::vector<char> * Crypto::getOutputData() {
	// Return a reference to outData

	return 0;
}

void Crypto::encrypt() {
	// Leave this function for me, I need to do something special with it
}

void Crypto::decrypt() {
	// Leave this function for me, I need to do something special with it
}