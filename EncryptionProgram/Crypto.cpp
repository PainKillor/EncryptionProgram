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
	// Set this->cipher = cipher
}

const Cipher * Crypto::getCipher() {
	// Return reference to cipher
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
}

const std::vector<char> * Crypto::getOutputData() {
	// Return a reference to outData
}

void Crypto::encrypt() {
	// Leave this function for me, I need to do something special with it
}

void Crypto::decrypt() {
	// Leave this function for me, I need to do something special with it
}