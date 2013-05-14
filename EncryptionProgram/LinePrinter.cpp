#include "LinePrinter.h"

LinePrinter::LinePrinter() {
	stringPrinters = std::vector<StringPrinter>();
	lineWidth = 0;
	leftMargin = 0;
	rightMargin = 0;
	alignment = 0;
	attribute = -1;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

LinePrinter::LinePrinter(const LinePrinter &linePrinter) {
	stringPrinters = linePrinter.stringPrinters;
	lineWidth = linePrinter.lineWidth;
	alignment = linePrinter.alignment;
	leftMargin = linePrinter.leftMargin;
	rightMargin = linePrinter.rightMargin;
	attribute = linePrinter.attribute;
	hOutput = linePrinter.hOutput;
}

LinePrinter::LinePrinter(int lineWidth, StringPrinter stringPrinter, WORD attribute, HANDLE hOutput) {
	if (lineWidth < 0)
		lineWidth = 0;

	stringPrinters = std::vector<StringPrinter>();
	stringPrinters.push_back(stringPrinter);
	this->lineWidth = lineWidth;
	this->alignment = ALIGN_CENTER;
	this->leftMargin = 0;
	this->rightMargin = 0;
	this->attribute = attribute;
	this->hOutput = hOutput;
}


LinePrinter::LinePrinter(int lineWidth, int alignment,  int leftMargin, int rightMargin, WORD attribute, HANDLE hOutput) {
	if (lineWidth < 0) 
		lineWidth = 0;

	if ((alignment != ALIGN_LEFT) && (alignment != ALIGN_CENTER) && (alignment != ALIGN_RIGHT))
		alignment = ALIGN_CENTER;

	if (leftMargin < 0) {
		leftMargin = 0;
		rightMargin = 0;
	} else if (rightMargin < 0) {
		rightMargin = leftMargin;
	}

	if (leftMargin + rightMargin > lineWidth)
		lineWidth = 0;

	stringPrinters = std::vector<StringPrinter>();
	this->lineWidth = lineWidth;
	this->alignment = alignment;
	this->leftMargin = leftMargin;
	this->rightMargin = rightMargin;
	this->attribute = attribute;
	this->hOutput = hOutput;
}

void LinePrinter::printLine() {
	if (lineWidth > 0) {
		const static char blank = ' ';
		DWORD cWritten;

		// Save screen buffer info for later, so we can restore the text attributes
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hOutput, &csbi);

		// If the user's defined a text attribute to use
		if (attribute > 0)
			SetConsoleTextAttribute(hOutput, attribute);

		// If there's at least 1 stringprinter
		if (stringPrinters.size() > 0) {
			int totalStringSize = 0;

			// Calculate the combined length of all the strings in the stringprinters
			for (std::vector<StringPrinter>::iterator it = stringPrinters.begin(); it != stringPrinters.end(); it++)
				totalStringSize += it->size();

			// If there's at least 1 character in all the combined stringprinters
			if (totalStringSize > 0) {
				// Print blanks to fill the left margin
				for (int i = 0; i < leftMargin; i++)
					WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
	
				// Set the width of the printable area
				int printWidth = lineWidth - leftMargin - rightMargin;

				// If there's exactly 1 string to print
				if (stringPrinters.size() == 1) {
					// If the size of the string is less than the printable area
					if (stringPrinters[0].size() < printWidth) {
						// If line is aligned to the left print the string and then the remaining blanks
						if (alignment == LinePrinter::ALIGN_LEFT) {
							stringPrinters[0].printString();
							for (int i = 0; i < lineWidth - (stringPrinters[0].size() + leftMargin); i++)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
						// Else if the string is aligned to the right print blanks and then the string and then the right margin
						} else if (alignment == LinePrinter::ALIGN_RIGHT) {
							for (int i = 0; i < printWidth - stringPrinters[0].size(); i++)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
							stringPrinters[0].printString();
							for (int i = 0; i < rightMargin; i++)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
						// Else if the string is aligned in the center find the space size on either side of the string between the margins and print that with the string in between
						} else {
							int spacer = printWidth - stringPrinters[0].size();
							spacer /= 2;

							for (int i = 0; i < spacer; i++)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
							stringPrinters[0].printString();

							for (int i = 0; i < lineWidth - (stringPrinters[0].size() + leftMargin + spacer); i++)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
						}
					// Otherwise print the string with the width of the entire printable area
					} else {
						stringPrinters[0].printString(printWidth);
						for (int i = 0; i < rightMargin; i++)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
					}
				// If we're printing multiple strings
				} else {
					// If the total size of all the strings + a 1 char spacer in between them is less than or equal to the size of the printable area
					if (totalStringSize + (stringPrinters.size() - 1) <= printWidth) {
						int spacer = printWidth - totalStringSize;
						spacer /= stringPrinters.size() - 1;
						// If the spacer size * the number of strings is less than the printable area - the total size of all the strings it indicates a loss of precision when rounding, we'll have to add a space in later
						bool offByOne = ((spacer * (stringPrinters.size() - 1)) < (printWidth - totalStringSize)) ? true : false;

						int index = 0;
						// Iterate through stringprinters
						for (std::vector<StringPrinter>::iterator it = stringPrinters.begin(); it != stringPrinters.end(); it++) {
							it->printString();
							// If we're not on the last stringprinter then print a space afterwards
							if (index != stringPrinters.size() - 1) {
								for (int i = 0; i < spacer; i++)
									WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
							}
							// If we need to insert an extra space do it in the middle
							if (offByOne) {
								if (index == stringPrinters.size() / 2)
									WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
							}

							index++;
						}
					} else {
						// Set the print length of each string equal to the size of the printable area - the number of spacers
						int length = printWidth - (stringPrinters.size() - 1);
						length /= stringPrinters.size();

						// If the length of each string * the number of strings is less than the size of the printable area - the spaces then we have to compensate later by adding an extra char to a string
						bool offByOne = ((length * stringPrinters.size()) < (printWidth - (stringPrinters.size() - 1))) ? true : false;

						int index = 0;
						// Iterate through stringPrinters
						for (std::vector<StringPrinter>::iterator it = stringPrinters.begin(); it != stringPrinters.end(); it++) {
							// If we need to add a char
							if (offByOne) {
								// If we're in the middle of the line
								if (index == stringPrinters.size() / 2)
									it->printString(length + 1);
								else
									it->printString(length);
							} else {
								it->printString(length);
							}

							// If we just printed the last string don't add a spacer
							if (index != stringPrinters.size() - 1)
								WriteConsole(hOutput, &blank, 1, &cWritten, NULL);

							index++;
						}
					}

					// Print the right margin
					for (int i = 0; i < rightMargin; i++)
						WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
				}
			} else {
				for (int i = 0; i < lineWidth; i++)
					WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
			}
		} else {
			for (int i = 0; i < lineWidth; i++)
				WriteConsole(hOutput, &blank, 1, &cWritten, NULL);
		}

		// Restore original text attributes
		SetConsoleTextAttribute(hOutput, csbi.wAttributes);
	}
}

void LinePrinter::addStringPrinter(StringPrinter stringPrinter) {
	stringPrinters.push_back(stringPrinter);
}

void LinePrinter::setLineWidth(int lineWidth) {
	this->lineWidth = lineWidth;
}

int LinePrinter::getLineWidth() {
	return lineWidth;
}

void LinePrinter::setLeftMargin(int leftMargin) {
	this->leftMargin = leftMargin;
}

int LinePrinter::getLeftMargin() {
	return leftMargin;
}

void LinePrinter::setRightMargin(int rightMargin) {
	this->rightMargin = rightMargin;
}

int LinePrinter::getRightMargin() {
	return rightMargin;
}

void LinePrinter::setAlignment(int alignment) {
	this->alignment = alignment;
}

int LinePrinter::getAlignment() {
	return alignment;
}

void LinePrinter::setAttribute(WORD attribute) {
	this->attribute = attribute;
}

WORD LinePrinter::getAttribute() {
	return attribute;
}

void LinePrinter::setStringPrinters(std::vector<StringPrinter> stringPrinters) {
	this->stringPrinters = stringPrinters;
}

const std::vector<StringPrinter> * LinePrinter::getStringPrinters() {
	return &stringPrinters;
}

void LinePrinter::setOutputHandle(HANDLE hOutput) {
	this->hOutput = hOutput;
}

HANDLE LinePrinter::getOutputHandle() {
	return hOutput;
}