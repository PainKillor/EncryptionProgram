#include "StringPrinter.h"

StringPrinter::StringPrinter() {
	str = std::string();
	attributes = std::vector<WORD>();
	attributeIndices = std::vector<int>();
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

StringPrinter::StringPrinter(StringPrinter &stringPrinter) {
	str = stringPrinter.getString();
	attributes = *stringPrinter.getAttributes();
	attributeIndices = *stringPrinter.getAttributeIndices();
	hOutput = stringPrinter.getOutputHandle();
}

StringPrinter::StringPrinter(std::string str, std::vector<WORD> attributes, std::vector<int> attributeIndices, HANDLE hOutput) {
	this->str = str;
	this->attributes = attributes;
	this->attributeIndices = attributeIndices;
	this->hOutput = hOutput;
}

void StringPrinter::printString() {
	DWORD cWritten;
	SetConsoleTextAttribute(hOutput, StringPrinter::DEFAULT_ATTRIBUTES);

	int index = 0;
	for (int i = 0; i < attributeIndices.size(); i++) {
		if (attributeIndices[i] < str.size()) {
			if (index <= attributeIndices[i]) {
				int len = attributeIndices[i] - index;
				WriteConsole(hOutput, str.substr(index, len).c_str(), len, &cWritten, NULL);
				index = attributeIndices[i];
				SetConsoleTextAttribute(hOutput, attributes[i]);
			}
		}
	}

	int len = str.size() - index;
	WriteConsole(hOutput, str.substr(index, len).c_str(), len, &cWritten, NULL);
}

void StringPrinter::setString(std::string str) {
	this->str = str;
}

std::string StringPrinter::getString() {
	return str;
}

void StringPrinter::addAttribute(WORD attribute, int attributeIndex) {
	attributes.push_back(attribute);
	attributeIndices.push_back(attributeIndex);
}

const std::vector<WORD> * StringPrinter::getAttributes() {
	return &attributes;
}

const std::vector<int> * StringPrinter::getAttributeIndices() {
	return &attributeIndices;
}

void StringPrinter::setOutputHandle(HANDLE hOutput) {
	this->hOutput = hOutput;
}

HANDLE StringPrinter::getOutputHandle() {
	return hOutput;
}