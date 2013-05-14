#include "StringPrinter.h"

StringPrinter::StringPrinter() {
	str = std::string();
	attributeIndices = std::vector<int>();
	attributes = std::vector<WORD>();
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

StringPrinter::StringPrinter(const StringPrinter &stringPrinter) {
	str = stringPrinter.str;
	attributeIndices = stringPrinter.attributeIndices;
	attributes = stringPrinter.attributes;
	hOutput = stringPrinter.hOutput;
}

StringPrinter::StringPrinter(std::string str, HANDLE hOutput) {
	attributeIndices = std::vector<int>();
	attributes = std::vector<WORD>();
	this->str = str;
	this->hOutput = hOutput;
}

StringPrinter::StringPrinter(std::string str, WORD attribute, HANDLE hOutput) {
	std::vector<int> attributeIndices = std::vector<int>();
	attributeIndices.push_back(0);
	std::vector<WORD> attributes = std::vector<WORD>();
	attributes.push_back(attribute);
	this->str = str;
	this->attributes = attributes;
	this->attributeIndices = attributeIndices;
	this->hOutput = hOutput;
}

void StringPrinter::printString() {
	printString(str.size());
}

void StringPrinter::printString(int length) {
	const static char subChar = '.';

	DWORD cWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hOutput, &csbi);

	char *buff = new char[str.length() + 1];
	for (int i = 0; i < str.length(); i++) {
		if (!iscntrl((unsigned char) str.at(i)))
			buff[i] = str.at(i);
		else
			buff[i] = subChar;
	}
	buff[str.length()] = '\0';

	str = std::string(buff);

	int index = 0;
	for (int i = 0; i < attributeIndices.size(); i++) {
		if (attributeIndices[i] < length) {
			if (index <= attributeIndices[i]) {
				int len = attributeIndices[i] - index;
				WriteConsole(hOutput, str.substr(index, len).c_str(), len, &cWritten, NULL);
				index = attributeIndices[i];
				SetConsoleTextAttribute(hOutput, attributes[i]);
			}
		}
	}

	int len = length - index;
	WriteConsole(hOutput, str.substr(index, len).c_str(), len, &cWritten, NULL);

	SetConsoleTextAttribute(hOutput, csbi.wAttributes);
}

void StringPrinter::setString(std::string str) {
	this->str = str;
}

std::string StringPrinter::getString() {
	return str;
}

void StringPrinter::addAttribute(int attributeIndex, WORD attribute) {
	attributeIndices.push_back(attributeIndex);
	attributes.push_back(attribute);
}

const std::vector<int> * StringPrinter::getAttributeIndices() {
	return &attributeIndices;
}

const std::vector<WORD> * StringPrinter::getAttributes() {
	return &attributes;
}

void StringPrinter::setOutputHandle(HANDLE hOutput) {
	this->hOutput = hOutput;
}

HANDLE StringPrinter::getOutputHandle() {
	return hOutput;
}

int StringPrinter::size() {
	return str.size();
}