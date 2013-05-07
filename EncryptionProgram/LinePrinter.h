#ifndef LINEPRINTER_H
#define LINEPRINTER_H

#include <Windows.h>
#include <vector>

#include "StringPrinter.h"

class LinePrinter {
	private:
		std::vector<StringPrinter> stringPrinters;
		int lineWidth;
		int leftMargin;
		int rightMargin;
		int alignment;
		WORD attribute;
		HANDLE hOutput;

	public:
		const static int ALIGN_LEFT = 0;
		const static int ALIGN_CENTER = 1;
		const static int ALIGN_RIGHT = 2;

		LinePrinter();
		LinePrinter(const LinePrinter &linePrinter);
		LinePrinter(int lineWidth, StringPrinter stringPrinter, WORD attribute = -1, HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE));
		LinePrinter(int lineWidth, int alignment = ALIGN_CENTER, int leftMargin = -1, int rightMargin = -1, WORD attribute = -1, HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE));

		void printLine();
		void addStringPrinter(StringPrinter stringPrinter);
		void setLineWidth(int lineWidth);
		int getLineWidth();
		void setLeftMargin(int leftMargin);
		int getLeftMargin();
		void setRightMargin(int rightMargin);
		int getRightMargin();
		void setAlignment(int alignment);
		int getAlignment();
		void setAttribute(WORD attribute);
		WORD getAttribute();
		void setStringPrinters(std::vector<StringPrinter> stringPrinters);
		const std::vector<StringPrinter> * getStringPrinters();
		void setOutputHandle(HANDLE hOutput);
		HANDLE getOutputHandle();
};

#endif