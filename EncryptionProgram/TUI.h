#ifndef TUI_H
#define TUI_H

#include <Windows.h>

#include "Crypto.h"
#include "PagePrinter.h"

class TUI {
	private:
		Crypto *crypto;
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
		bool encrypting;
		bool decrypting;
		bool loading;
		bool error;
		bool altPressed;

	public:
		const static int EXIT = 0;
		const static int CONTINUE_REFRESH = 1;
		const static int CONTINUE_NOREFRESH = 2;

		const static int ALT_RELEASED = -1;

		const static int SCREEN_HEIGHT = 48;
		const static int SCREEN_WIDTH = 120;
		const static int RIGHT_MARGIN = 10;
		const static int LEFT_MARGIN = 10;
		const static int NUM_DATA_LINES = 20;
		const static int DATA_WIDTH = 48;
		const static int NUM_COMMAND_HISTORY_LINES = 4;

		const static int BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		const static int FG_BLACK = 0x00;
		const static int FG_GREY = FOREGROUND_INTENSITY;
		const static int FG_BROWN = FOREGROUND_GREEN | FOREGROUND_RED;

		TUI(Crypto *crypto = new Crypto(), HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE), HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE));
		~TUI();

		int processNext();
		int getLine();
		void printPage();
		void printHeader(PagePrinter &pagePrinter, HANDLE hScreenBuffer);
		void printKey(PagePrinter &pagePrinter, HANDLE hScreenBuffer);
		void printDataHeaders(PagePrinter &pagePrinter, HANDLE hScreenBuffer);
		void printData(PagePrinter &pagePrinter, HANDLE hScreenBuffer);
		void printAlgorithm(PagePrinter &pagePrinter, HANDLE hScreenBuffer);
		void printCommands(PagePrinter &pagePrinter, HANDLE hScreenBuffer);
};

#endif