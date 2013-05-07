#ifndef TUI_H
#define TUI_H

#include <Windows.h>

#include "Crypto.h"
#include "PagePrinter.h"

class TUI {
	private:
		Crypto crypto;
		HANDLE hOutput;
		HANDLE hInput;
		int dataIndex;
		int algorithmIndex;
		std::vector<std::string> commandLines;
		std::string curLine;
		std::string key;
		std::string inFilePath;
		std::string outFilePath;
		std::string errorMessage;
		bool encrypt;
		bool decrypt;
		bool loading;
		bool error;
		bool altPressed;

	public:
		const static int SCREEN_HEIGHT = 40;
		const static int SCREEN_WIDTH = 120;
		const static int RIGHT_MARGIN = 10;
		const static int LEFT_MARGIN = 10;
		const static int NUM_DATA_LINES = 12;
		const static int NUM_COMMAND_HISTORY_LINES = 4;

		TUI();
		TUI(Crypto crypto = Crypto(), HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE), HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE));

		int processNext();
		int getLine();
		void printPage();
		void printHeader(PagePrinter &pagePrinter);
		void printKey(PagePrinter &pagePrinter);
		void printDataHeaders(PagePrinter &pagePrinter);
		void printData(PagePrinter &pagePrinter);
		void printAlgorithm(PagePrinter &pagePrinter);
		void printCommands(PagePrinter &pagePrinter);
};

#endif