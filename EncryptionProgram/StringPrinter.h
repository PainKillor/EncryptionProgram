#ifndef STRINGPRINTER_H
#define STRINGPRINTER_H

#include <Windows.h>
#include <vector>

class StringPrinter {
	private:
		std::string str;
		std::vector<int> attributeIndices;
		std::vector<WORD> attributes;
		HANDLE hOutput;

	public:
		StringPrinter();
		StringPrinter(const StringPrinter &stringPrinter);
		StringPrinter(std::string str, HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE));
		StringPrinter(std::string str, WORD attribute, HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE));

		void printString();
		void printString(int length);
		void setString(std::string str);
		std::string getString();
		void addAttribute(int attributeIndex, WORD attribute);
		const std::vector<WORD> * getAttributes();
		const std::vector<int> * getAttributeIndices();
		void setOutputHandle(HANDLE hOutput);
		HANDLE getOutputHandle();
		int size();
};

#endif