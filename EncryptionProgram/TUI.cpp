#include "TUI.h"

#include <string>

TUI::TUI(Crypto *crypto, HANDLE hOutput, HANDLE hInput) {
	this->crypto = crypto;
	this->hOutput = hOutput;
	this->hInput = hInput;
	dataIndex = 0;
	algorithmIndex = 0;
	commandLines = std::vector<std::string>();
	encrypt = false;
	decrypt = false;
	loading = false;
	error = false;
	altPressed = false;

	_COORD coord;
    coord.X = SCREEN_WIDTH;
    coord.Y = SCREEN_HEIGHT;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = SCREEN_HEIGHT - 1;
    Rect.Right = SCREEN_WIDTH - 1;

    SetConsoleScreenBufferSize(hOutput, coord);            // Set Buffer Size
    SetConsoleWindowInfo(hOutput, TRUE, &Rect);            // Set Window Size
	SetConsoleTextAttribute(hOutput, FG_BLACK | BG_WHITE);

	printPage();
}

TUI::~TUI() {
	delete crypto;
}

int TUI::processNext() {
	curLine = std::string();
	int keyPressed = getLine();

	//if (keyPressed != 0) {
	//	if (keyPressed == VK_UP) {
	//		if (index > NUM_LINES)
	//			index--;
	//	} else if (keyPressed = VK_DOWN) {
	//		if (index < lines.size()) 
	//			index++;
	//	}
	//} else {
	//	if (strcmp(chBuffer, "exit") != 0) {
	//		lines.push_back(string(chBuffer));
	//		chBuffer[0] = '\0';
	//		index++;
	//	}
	//}

	DWORD lpNumWritten;
	const static char prompt[] = "Alt Pressed!";

	if (altPressed)
		WriteConsole(hOutput, prompt, 12, &lpNumWritten, NULL);
	
	if (curLine.compare("exit") == 0)
		return EXIT;
	else
		return CONTINUE;
}

int TUI::getLine() {
	INPUT_RECORD prInput;
	DWORD lpNumRead, lpNumWritten;

	const static int length = 256;
	char chBuffer[length];
	int index = curLine.copy(chBuffer, curLine.length(), 0);

	while (true) {
		ReadConsoleInput(hInput, &prInput, 1, &lpNumRead);
		if (prInput.EventType == KEY_EVENT) {
			if (prInput.Event.KeyEvent.bKeyDown == true) {
				if (isprint(prInput.Event.KeyEvent.uChar.AsciiChar)) {
					chBuffer[index++ % length] = prInput.Event.KeyEvent.uChar.AsciiChar;
					WriteConsole(hOutput, &prInput.Event.KeyEvent.uChar.AsciiChar, 1, &lpNumWritten, NULL);
				} else if (iscntrl(prInput.Event.KeyEvent.uChar.AsciiChar)) {
					switch (prInput.Event.KeyEvent.wVirtualKeyCode) {
						case VK_MENU:
							altPressed = true;
						case VK_UP:
						case VK_DOWN:
						case VK_LEFT:
						case VK_RIGHT:
							curLine = std::string(chBuffer);
							return prInput.Event.KeyEvent.wVirtualKeyCode;
					}

					if (index > 0) {
						const static char chBackup[] = "\b \b";
						const static char chNextLine[] = "\r\n";

						switch (prInput.Event.KeyEvent.uChar.AsciiChar) {
							case '\b':
								WriteConsole(hOutput, chBackup, 3, &lpNumWritten, NULL);
								index--;
								break;
							case '\r':
								WriteConsole(hOutput, chNextLine, 2, &lpNumWritten, NULL);
								curLine = std::string(chBuffer);
								return 0;
						}
					}
				}

				if (index % length == 0) {
					if (index == length)
						chBuffer[length - 1] = '\0';
					else
						chBuffer[0] = '\0';
				} else {
					chBuffer[index % length] = '\0';
				}
			} else {
				if (prInput.Event.KeyEvent.wVirtualKeyCode == VK_MENU)
					altPressed = false;
			}
		}
	}
}

void TUI::printPage() {
	HANDLE hScreenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleTextAttribute(hScreenBuffer, FG_BLACK | BG_WHITE);

	PagePrinter pagePrinter = PagePrinter();

	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpBlank);

	StringPrinter spHeader1 = StringPrinter("Alpha Team", hScreenBuffer);
	LinePrinter lpHeader1 = LinePrinter(SCREEN_WIDTH, spHeader1, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader1);

	StringPrinter spHeader2 = StringPrinter("Encryption Program", hScreenBuffer);
	LinePrinter lpHeader2 = LinePrinter(SCREEN_WIDTH, spHeader2, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader2);

	StringPrinter spHeader3 = StringPrinter("CISP 400", hScreenBuffer);
	LinePrinter lpHeader3 = LinePrinter(SCREEN_WIDTH, spHeader3, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpHeader3);

	pagePrinter.addLinePrinter(lpBlank);
	pagePrinter.addLinePrinter(lpBlank);

	if (!key.empty()) {
		StringPrinter spKey = StringPrinter("Key: " + key, FG_BLACK | BG_WHITE, hScreenBuffer);
		spKey.addAttribute(5, FG_GREY | BG_WHITE);
		LinePrinter lpKey = LinePrinter(SCREEN_WIDTH, spKey, FG_BLACK | BG_WHITE, hScreenBuffer);
		pagePrinter.addLinePrinter(lpKey);
	} else {
		pagePrinter.addLinePrinter(lpBlank);
	}

	pagePrinter.addLinePrinter(lpBlank);

	printData(pagePrinter, hScreenBuffer);

	pagePrinter.printPage();

	SetConsoleActiveScreenBuffer(hScreenBuffer);
	CloseHandle(hOutput);
	hOutput = hScreenBuffer;
}

void TUI::printData(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {
	LinePrinter lpBlank = LinePrinter(SCREEN_WIDTH, NULL, NULL, NULL, FG_BLACK | BG_WHITE, hScreenBuffer);

	StringPrinter spDataInHeader = StringPrinter("Data In", FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpDataInHeader = LinePrinter(SCREEN_WIDTH / 2, spDataInHeader, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpDataInHeader);

	StringPrinter spDataOutHeader = StringPrinter("Data Out", FG_BLACK | BG_WHITE, hScreenBuffer);
	LinePrinter lpDataOutHeader = LinePrinter(SCREEN_WIDTH - (SCREEN_WIDTH / 2), spDataOutHeader, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpDataOutHeader);

	pagePrinter.addLinePrinter(lpBlank);

	StringPrinter spPathIn;
	if (!inFilePath.empty()) {
		spPathIn = StringPrinter("Mode: File   Path: " + inFilePath, FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathIn.addAttribute(5, FG_GREY | BG_WHITE);
		spPathIn.addAttribute(10, FG_BLACK | BG_WHITE);
		spPathIn.addAttribute(18, FG_GREY | BG_WHITE);
	} else {
		spPathIn = StringPrinter("Mode: Text", FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathIn.addAttribute(5, FG_GREY | BG_WHITE);
	}

	LinePrinter lpPathIn(SCREEN_WIDTH / 2, spPathIn, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpPathIn);

	StringPrinter spPathOut;
	if (!outFilePath.empty()) {
		spPathOut = StringPrinter("Mode: File   Path: " + outFilePath, FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathOut.addAttribute(5, FG_GREY | BG_WHITE);
		spPathOut.addAttribute(10, FG_BLACK | BG_WHITE);
		spPathOut.addAttribute(18, FG_GREY | BG_WHITE);
	} else {
		spPathOut = StringPrinter("Mode: Text", FG_BLACK | BG_WHITE, hScreenBuffer);
		spPathOut.addAttribute(5, FG_GREY | BG_WHITE);
	}

	LinePrinter lpPathOut(SCREEN_WIDTH - (SCREEN_WIDTH / 2), spPathOut, FG_BLACK | BG_WHITE, hScreenBuffer);
	pagePrinter.addLinePrinter(lpPathOut);
}

void TUI::printAlgorithm(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {

}

void printCommands(PagePrinter &pagePrinter, HANDLE hScreenBuffer) {

}